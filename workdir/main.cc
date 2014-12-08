using namespace std;
#include <iostream>
#include "AppFramework/AppModule.h"
#include "AppFramework/AppFramework.h"
#include<map>
#include<vector>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<sys/shm.h>
#include<getopt.h>
#include<getopt.h>
#define SIGNAL(s, handler) { \
    sa.sa_handler = handler; \
    if (sigaction(s, &sa, NULL) < 0) { \
        fprintf(stderr, "Couldn't establish signal handler (%d)", s); \
        exit(1); \
    } \
}

AppFramework fw("InputModule->BasicReader->GeneratorReader->MuonReader->ElectronReader->JetReader->METReader->Analyser->OutputModule");

static void term(int qwe){
    printf("terminating with signal #%d\n",qwe);
    fw.endJob();
    exit(2);
} 

int main(int argc, char **argv) {
    struct sigaction sa;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT); // finish the job on Ctrl-C
    sigaddset(&mask, SIGTERM); // condor_rm
    sa.sa_mask = mask;
    sa.sa_flags = 0;
    SIGNAL(SIGINT, term); // Terminate and call the cleanup function
    SIGNAL(SIGTERM, term); // Terminate and call the cleanup function


    // parse the options
    static struct option options[] = {
       {"help",         0, 0, 'h'},
       {"datatype",     1, 0, 'd'},
       {"input",        1, 0, 'i'},
       {"output",       1, 0, 'o'},
       {0, 0, 0, 0}
    };

    cout<<"Running: ";
    for(int arg=0; arg<argc; arg++) cout<<argv[arg]<<" ";
    cout<<endl;

    map<string,int> dataNames;
    map<int, vector<const char*> > MET;//[NUMBER_OF_TYPES+1];
    map<int, vector<const char*> > sMuon;//[NUMBER_OF_TYPES+1];
    map<int, vector<const char*> > sElectron;//[NUMBER_OF_TYPES+1];
    map<int, vector<const char*> > MC;//[NUMBER_OF_TYPES+1];
    #include "BEANs2012.h"
    map<int, vector<const char*> > *BEANtuples = &MC;

    int dataType=NONE;
    string dataName;
    const char *outputDirectory = "";
    const char *inputDirectory = 0;

    while( 1 ){
       int index=0;
       int c = getopt_long(argc, argv, "hd:i:o:",options, &index);
       if( c == -1 ) break;
       switch( tolower(c) ) {
           case 'h':
              printf("Usage:\n");
              printf("-h     ,   --help              show this message\n");
              printf("-dTYPE ,   --datatype=TYPE     dataset to run on (TYPE=\n\
                A,A[1-5(e),6(mu)]\n\
                B,B[1-15(e),20(mu)],\n\
                C,C[1-24(e),34(mu)],\n\
                D,D[1-30(e),29(mu)],\n\
                T,TT[1-12],\n\
                WJ,WJ[1-29]\n\
                ZJ,ZJ[1-16],\n\
                ZN,ZN[1-4],\n\
                WW,WW[1-5],WZ,WZ[1-5],ZZ,ZZ[1-5],\n\
                W1J,W1J[1-12],\n\
                W2J,W2J[1-17],\n\
                W3J,W3J[1-8],\n\
                W4J,W4J[1-7],\n\
                TTll,TThh,TTlh,TTll2,\n\
                W3Jsu,W3Jsu1,W3Jsu2,W3Jsu3,W3Jsu4,\n\
                W3Jsd,W3Jsd1,W3Jsd2,W3Jsd3,W3Jsd4,\n\
                W4Jsu,W4Jsu1,W4Jsu2,W4Jsu3,W4Jsu4,\n\
                W4Jsd,W4Jsd1,W4Jsd2,W4Jsd3,W4Jsd4,\n\
                W3Jmu,W3Jmu1,W3Jmu2,W3Jmu3,W3Jmu4,\n\
                W3Jmd,W3Jmd1,W3Jmd2,W3Jmd3,W3Jmd4,\n\
                W4Jmu,W4Jmu1,W4Jmu2,W4Jmu3,W4Jmu4,\n\
                W4Jmd,W4Jmd1,W4Jmd2,W4Jmd3,W4Jmd4)\n\
                signals,\n\
                s3,s3m0,s3m50,s3m100,s3m150,s3m200,s3m300,s3m500,\n\
                s4,s4m0,s4m50,s4m100,s4m150,s4m200,s4m300,s4m500,\n\
                WJsu,WJsd,WJmu,WJmd,ZJsu,ZJsd,ZJmu,ZJmd,TTsu,TTsd,TTmu,TTmd,\n\
                Signals,\n\
                S3,S3m0,S3m50,S3m100,S3m150,S3m200,S3m300,S3m500,\n\
                S4,S4m0,S4m50,S4m100,S4m150,S4m200,S4m300,S4m500,\n");
              printf("-iPATH ,   --input=PATH        alternative to the -d option - path with input ntuples\n");
              printf("-oPATH ,   --output=PATH       store output in PATH directory\n");
              return 0;
           break;
           case 'd': {
                     std::map<string,int>::const_iterator tp = dataNames.find(optarg);
                     if( tp != dataNames.end() ){
                        dataType = tp->second;
                        dataName = tp->first;
                     }
                  }
           break;
           case 'i': inputDirectory  = optarg; break;
           case 'o': outputDirectory = optarg; break;
           default : printf("Type -h for help\n"); return 0;
       }
    }

    if( dataType==NONE && inputDirectory==0 ){ printf("Missed input, type -h for help\n"); return 0; }
    if( dataType!=NONE && inputDirectory!=0 ){ printf("Ambiguous input, type -h for help\n"); return 0; }

    string outputFileName(outputDirectory);
    outputFileName.append("micro_");

    string inputFiles;

    if( dataType!=NONE ){
        for(vector<const char*>::const_iterator tuple = (*BEANtuples)[dataType].begin(); tuple != (*BEANtuples)[dataType].end(); tuple++){
            inputFiles.append(*tuple);
            inputFiles.append(":");
        }
        inputFiles.erase(inputFiles.size()-1,1);
        outputFileName.append( dataName );
    } else {
        inputFiles = inputDirectory;
        inputFiles.append("/*.root");

        string sampleName(inputDirectory);
        size_t length = sampleName.length();
        size_t startPos = sampleName.rfind('/');
        if( startPos != string::npos ){ // found a '/' symbol
            if( startPos+1 == length ){ // it is a trailing '/' symbol(s)
                // remove all trailing '/' symbols w/o using regular expressions
                for(size_t previous = startPos;
                    startPos != 0 && startPos != string::npos && (previous-startPos) <= 1;
                    previous = startPos, startPos = sampleName.rfind('/',startPos-1) )
                    sampleName.resize(startPos);
            }
        }
        if( startPos == string::npos )
            outputFileName.append(sampleName);
        else
            outputFileName.append(sampleName.substr(startPos+1));
    }
//    outputFileName.append(".csv");

    fw.verbose("AppFramework","cout on");
    fw.verbose("Analyser","cout off");
    fw.modList();
    fw.modify("InputModule::path",inputFiles.c_str());
    fw.modify("InputModule::showProgressPeriod","10000");
    fw.modify("InputModule::firstEntry","0");
    fw.modify("OutputModule::filter","numberOfJets==3 && diMuPtRec>300 && numberOfRecMuons==2"); // && muEtaRec[0]<2.1 && muEtaRec[0]>-2.1 && muPtRec[0]>40 && muEtaRec[1]<2.1 && muEtaRec[1]>-2.1 && muPtRec[1]>40
    fw.modify("OutputModule::output",outputFileName.c_str());
    fw.modify("OutputModule::leaflist","event/I:entry/I:numberOfJets/I:m3jets/D:met/D:jetPtRec[4]/D:ZpT/D:numberOfRecMuons/I:diMuPtRec/D:muPtRec[4]/D:muPfIso[4]/D:numberOfGenMuons/I:diMuPtGen/D");
    fw.beginJob();
    fw.process(-1);
    fw.endJob();

    return 0;
}
