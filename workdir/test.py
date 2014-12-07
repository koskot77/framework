from init import *

fw = AppFramework('InputModule->BasicReader->GeneratorReader->MuonReader->ElectronReader->JetReader->METReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.verbose("Analyser","cout off");
fw.modList();
fw.modify("InputModule::path","/store/user/ahart/BN_DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_110?_*_*.root");
fw.modify("InputModule::showProgressPeriod","1000");
fw.modify("InputModule::firstEntry","19993");
#fw.modify("OutputModule::filter","event==25501249");
fw.modify("OutputModule::output","qwe");
fw.modify("OutputModule::leaflist","event/I:entry/I:numberOfJets/I:m3jets/D:met/D:jetPtRec[4]/D:ZpT/D:numberOfRecMuons/I:diMuPtRec/D:numberOfGenMuons/I:diMuPtGen/D");
fw.beginJob();
fw.process(20);
fw.endJob();
