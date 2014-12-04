from init import *

fw = AppFramework('InputModule->BasicReader->GeneratorReader->MuonReader->ElectronReader->JetReader->METReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.modList();
#fw.modify("InputModule::path","/store/user/kkotov/BN_MonotopS3_MSM-500_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/MonotopS3_MSM-500_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_1_3_19w.root");
fw.modify("InputModule::path","/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
fw.modify("InputModule::showProgressPeriod","100");
fw.modify("InputModule::firstEntry","1");
fw.modify("OutputModule::output","qwe.root");
fw.modify("OutputModule::leaflist","event/I:numberOfJets/I:m3jets/D:met/D:jetPtRec[4]/D");
fw.beginJob();
fw.process(10);
fw.endJob();
