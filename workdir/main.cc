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

int main(void){

gSystem->Load("libFWCoreFWLite.so");
gROOT->ProcessLine("AutoLibraryLoader::enable()");
gSystem->Load("libDataFormatsFWLite.so");
gSystem->Load("libDataFormatsPatCandidates.so");
std::vector<std::string> samplefiles(1);
samplefiles[0] = "/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root";
fwlite::ChainEvent *evtchain = new fwlite::ChainEvent(samplefiles);
fwlite::Handle<pat::JetCollection> h_jets;
h_jets.getByLabel(*evtchain, "slimmedJetsAK8", "");
for(unsigned int index = 0; index < h_jets->size(); index++) {
const pat::Jet *const jet = &h_jets->at(index);
cout<<"QWE5"<<endl;
cout<<jet->emEnergyFraction()<<endl;
}
/*
TChain *chain = new TChain("Events");
chain->AddFile("/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
chain->GetEntry(0);
TTree *Events = chain;
TBranch *bpj = Events->GetBranch("patJets_slimmedJetsAK8__PAT.");
edm::Wrapper<pat::JetCollection> *patJets = new edm::Wrapper<pat::JetCollection>();
bpj->SetAddress(&patJets);
chain->GetEntry(0);
bpj->SetAddress(&patJets);
chain->GetEntry(0);
cout<<"QWE5"<<endl;

    if( patJets->isPresent() )
      for(vector<pat::Jet>::const_iterator pjet = patJets->product()->begin(); pjet != patJets->product()->end(); pjet++)
        cout<<pjet->emEnergyFraction()<<endl;

*/
/*        AppFramework fw("MyModule1->MyModule2->MyModule3->MyModule4->MyModule5");
        fw.verbose("AppFramework","cout on");
        fw.verbose("AppFramework","clog on");
        fw.verbose("AppFramework","cerr on");
        fw.modList();
        fw.modify("MyModule4::mystring","Simple string");
        fw.modify("MyModule4::mydouble","999");
        fw.modify("MyModule5::formula","( val1+ val2) *val3");
        fw.beginJob();
        fw.process(10);
        fw.endJob();
*/
/*
    AppFramework fw("InputModule->BasicReader->JetReader");
    fw.verbose("AppFramework","cout on");
    fw.modList();
    fw.modify("InputModule::path","/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
    fw.beginJob();
    fw.process(10);
    fw.endJob();
*/
        return 0;
}
