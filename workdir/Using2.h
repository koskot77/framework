#include "AppFramework/AppFramework.h"
#include "Logistics/SampleHelper.h"
#include "AnModBasic/InputModule.h"
#include "AnModBasic/OutputModule.h"
#include "Readers/BasicReader.h"
#include "Readers/TriggerReader.h"
#include "Readers/MuonReader.h"
#include "Readers/ElectronReader.h"
#include "Readers/METReader.h"
#include "Readers/JetReader.h"
#include "Readers/GeneratorReader.h"
#include "Analyser.h"

AppFramework::AppFramework(const char* sequence){
        addAgent ( new SampleHelper("SampleHelper", "SampleHelper" ) );
        addModule( new InputModule("InputModule", "InputModule" ) );
        addModule( new GeneratorReader("GeneratorReader", "GeneratorReader" ) );
        addModule( new BasicReader("BasicReader", "BasicReader" ) );
        addModule( new TriggerReader("TriggerReader", "TriggerReader" ) );
        addModule( new MuonReader ("MuonReader",  "MuonReader"  ) );
        addModule( new ElectronReader ("ElectronReader",  "ElectronReader"  ) );
        addModule( new METReader  ("METReader",   "METReader"   ) );
        addModule( new JetReader  ("JetReader",   "JetReader"   ) );
        addModule( new Analyser   ("Analyser",    "Analyser"    ) );
        addModule( new OutputModule("OutputModule", "OutputModule" ) );
	reorder(sequence);
}

