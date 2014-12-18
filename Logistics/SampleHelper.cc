using namespace std;
#include "SampleHelper.h"

SampleHelper::SampleHelper(void):AppAgentWrapper("sampleHelper","simple object-counter"),AppAgent<string>(),AppAgent<const char*>(){
     sampleNames["TT"] = "";
}

list<string> SampleHelper::dataList(void) const {
    list<string> retval;
    retval.push_back("TT1");
    return retval;
}

bool SampleHelper::fetch(const char *name,      string &c){
    return false;
}

bool SampleHelper::fetch(const char *name, const char* &c){
    return false;
}

/*
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
*/
