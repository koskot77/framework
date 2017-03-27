from init import *

fw = AppFramework('InputModule->BasicReader->GeneratorReader->EMTFReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.verbose("Analyser","cout off");
fw.modList();
fw.modify("InputModule::path","/tmp/kkotov/CMSSW_9_0_0/src/L1Trigger/L1TMuonEndCap/test/qwe.root");
fw.modify("InputModule::showProgressPeriod","1000");
fw.modify("InputModule::firstEntry","0");
fw.modify("OutputModule::filter","1 && 1");
fw.modify("OutputModule::output","muonGunPt3_100_emtf.csv");
fw.modify("OutputModule::leaflist",
"entry/I:muPtGen/D:muEtaGen/D:muPhiGen/D:\
numberOfEMTFTracks/I:pt[2]/D:mypt[2]/D:ptGMT[2]/I:mode[2]/I:\
dPhi12[2]/I:dPhi13[2]/I:dPhi14[2]/I:\
dPhi23[2]/I:dPhi24[2]/I:dPhi34[2]/I:\
dTheta12[2]/I:dTheta13[2]/I:dTheta14[2]/I:\
dTheta23[2]/I:dTheta24[2]/I:dTheta34[2]/I:\
clct1[2]/I:clct2[2]/I:clct3[2]/I:clct4[2]/I:\
fr1[2]/I:fr2[2]/I:fr3[2]/I:fr4[2]/I:\
ring1[2]/I:ring2[2]/I:ring3[2]/I:ring4[2]/I:\
rpc1[2]/I:rpc2[2]/I:rpc3[2]/I:rpc4[2]/I");
fw.beginJob();
fw.process(100);
fw.endJob();
