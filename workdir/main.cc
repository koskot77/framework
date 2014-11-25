using namespace std;
#include <iostream>
#include "AppFramework/AppModule.h"
#include "AppFramework/AppFramework.h"
#include "AppFramework/AppOStream.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include<TSystem.h>
#include<TROOT.h>
#include<TChain.h>
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"

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

AppFramework fw("InputModule->BasicReader->MuonReader->ElectronReader->JetReader->METReader->Analyser");

static void term(int qwe){
    printf("terminating with signal #%d\n",qwe);
    fw.endJob();
    exit(2);
} 

int main(void){
    struct sigaction sa;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT); // finish the job on Ctrl-C
    sigaddset(&mask, SIGTERM); // condor_rm
    sa.sa_mask = mask;
    sa.sa_flags = 0;
    SIGNAL(SIGINT, term); // Terminate and call the cleanup function
    SIGNAL(SIGTERM, term); // Terminate and call the cleanup function

    fw.verbose("AppFramework","cout on");
    fw.modList();
    fw.modify("InputModule::path","/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
    fw.modify("Analyser::output","/tmp/kkotov/qwe.root");
    fw.beginJob();
    fw.process(5000);
    cout<<"5000"<<endl;
    fw.process(5000);
    cout<<"10000"<<endl;
    fw.process(5000);
    cout<<"15000"<<endl;
    fw.process(5000);
    cout<<"20000"<<endl;
    fw.process(5000);
    cout<<"25000"<<endl;
    fw.process(5000);
    cout<<"30000"<<endl;
    fw.process(5000);
    cout<<"35000"<<endl;
    fw.process(5000);
    cout<<"40000"<<endl;
    fw.process(5000);
    cout<<"45000"<<endl;
    fw.process(5000);
    cout<<"50000"<<endl;
    fw.endJob();
    return 0;
}
