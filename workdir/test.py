from init import *

#fw = AppFramework('InputModule->BasicReader->GeneratorReader->MuonReader->ElectronReader->JetReader->METReader->Analyser->OutputModule')
fw = AppFramework('InputModule->BasicReader->GeneratorReader->JetReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.modList();
fw.modify("InputModule::path","/store/user/kkotov/BN_MonotopS3_MSM-500_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/MonotopS3_MSM-500_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_1_3_19w.root");
fw.modify("InputModule::showProgressPeriod","100");
fw.modify("InputModule::firstEntry","0");
fw.modify("OutputModule::output","qwe.csv");
fw.modify("OutputModule::leaflist","numberOfJets/I:m3jets/D:mmm/D");
fw.modify("Analyser::output","qwe.root");
fw.beginJob();
fw.process(10);
fw.endJob();
