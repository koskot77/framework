from init import *

fw = AppFramework('InputModule->BasicReader->EMTFReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.verbose("Analyser","cout off");
fw.modList();
fw.modify("InputModule::path","/tmp/kkotov/EMTF_Tree_Collisions_298783_emul17_noGT_100k.root");
fw.modify("InputModule::showProgressPeriod","1000");
fw.modify("InputModule::firstEntry","0");
fw.modify("OutputModule::filter","1 && 1");
fw.modify("OutputModule::output","qwe.csv");
fw.modify("OutputModule::leaflist",
"entry/I:\
numberOfEMTFTracks/I:address[2]/I:mode[2]/I:theta[2]/I:st1_ring2[2]/I:\
dPhi12[2]/I:dPhi13[2]/I:dPhi14[2]/I:\
dPhi23[2]/I:dPhi24[2]/I:dPhi34[2]/I:\
dTheta12[2]/I:dTheta13[2]/I:dTheta14[2]/I:\
dTheta23[2]/I:dTheta24[2]/I:dTheta34[2]/I:\
dPhiS4[2]/I:dPhiS4A[2]/I:dPhiS3[2]/I:dPhiS3A[2]/I:\
clct1[2]/I:clct2[2]/I:clct3[2]/I:clct4[2]/I:\
fr1[2]/I:fr2[2]/I:fr3[2]/I:fr4[2]/I:\
rpc1[2]/I:rpc2[2]/I:rpc3[2]/I:rpc4[2]/I");
fw.beginJob();
fw.process(0);
fw.endJob();
