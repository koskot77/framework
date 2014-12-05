from init import *

fw = AppFramework('InputModule->BasicReader->GeneratorReader->MuonReader->ElectronReader->JetReader->METReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.verbose("Analyser","cout off");
fw.modList();
#fw.modify("InputModule::path","/store/user/kkotov/BN_MonotopS3_MSM-500_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/MonotopS3_MSM-500_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_1_3_19w.root");
fw.modify("InputModule::path","/store/user/ahart/BN_W4JetsToLNu_TuneZ2Star_8TeV-madgraph_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_666_*_*.root");
fw.modify("InputModule::showProgressPeriod","100");
fw.modify("InputModule::firstEntry","0");
fw.modify("OutputModule::filter","numberOfJets==3");
fw.modify("OutputModule::output","qwe.csv");
fw.modify("OutputModule::leaflist","event/I:numberOfJets/I:m3jets/D:met/D:jetPtRec[4]/D:numberOfRecMuons/I:muPtRec[4]/D:muEtaRec[4]/D:muPhiRec[4]/D:muPtGen[4]/D:muEtaGen[4]/D:muPhiGen[4]/D");
fw.beginJob();
fw.process(10);
fw.endJob();
