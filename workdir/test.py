from init import *

fw = AppFramework('InputModule->BasicReader->TriggerReader->GeneratorReader->ElectronReader->MuonReader->JetReader->METReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.verbose("Analyser","cout off");
fw.modList();
fw.modify("InputModule::path","/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
fw.modify("InputModule::showProgressPeriod","1000");
fw.modify("InputModule::firstEntry","0");
fw.modify("OutputModule::filter","numberOfRecMuons==0 && numberOfRecElectrons==0"); #&& muEtaRec[0]<2.1 && muEtaRec[0]>-2.1 && muPtRec[0]>40 && muEtaRec[1]<2.1 && muEtaRec[1]>-2.1 && muPtRec[1]>40
fw.modify("OutputModule::output","qwe");
fw.modify("OutputModule::leaflist","event/I:entry/I:pfmet170/I:numberOfJets/I:m3jets/D:m2jets/D:met/D:jetPtRec[4]/D:jetEtaRec[4]/D:jetPhiRec[4]/D:numberOfRecMuons/I:numberOfRecElectrons/I:wPtGen/D:wEtaGen/D:wPhiGen/D:tPtGen/D:tEtaGen/D:tPhiGen/D");
fw.beginJob();
fw.process(0);
fw.endJob();
