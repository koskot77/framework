from init import *
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-s", "--sampleSection", dest="sampleSection", help="XofY")
parser.add_option("-i", "--input",  dest="input",  help="W4J W3J")
parser.add_option("-o", "--output", dest="output", help="/data/users/kkotov/")
options, arguments = parser.parse_args()

#fw = AppFramework('InputModule->BasicReader->TriggerReader->GeneratorReader->ElectronReader->MuonReader->JetReader->METReader->Analyser->OutputModule')
fw = AppFramework('InputModule->BasicReader->TriggerReader->ElectronReader->MuonReader->JetReader->METReader->GeneratorReader->Analyser->OutputModule')
fw.verbose("AppFramework","cout on") 
fw.verbose("Analyser","cout off");
fw.modList();
fw.modify("SampleHelper::sample",options.input); #"S3m300");
fw.modify("SampleHelper::sampleSection",options.sampleSection); #"1of1");
#fw.modify("InputModule::path","/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
fw.modify("InputModule::showProgressPeriod","1000");
fw.modify("InputModule::firstEntry","0");
fw.modify("OutputModule::filter","numberOfJets==3 && jetPtRec[0]>60 && jetPtRec[1]>60 && jetPtRec[2]>40 && m3jets<250 && met>200");
fw.modify("OutputModule::output",options.output + options.input + "_" + options.sampleSection);
fw.modify("OutputModule::leaflist","event/I:entry/I:pfmet170/I:numberOfJets/I:m3jets/D:m2jets/D:m1jet/D:met/D:metNoMu/D:jetPtRec[4]/D:jetEtaRec[4]/D:jetPhiRec[4]/D:jetMass[4]/D:nSubJets[4]/I:numberOfRecMuons/I:numberOfRecElectrons/I:numberOfGenElectrons/I:numberOfGenMuons/I:numberOfGenTaus/I:wPtGen/D:wEtaGen/D:wPhiGen/D:tPtGen/D:tEtaGen/D:tPhiGen/D");
fw.beginJob();
fw.process(0);
fw.endJob();
