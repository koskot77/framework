#include "AppFramework/AppFramework.h"
#include "AnModBasic/InputModule.h"
#include "Readers/BasicReader.h"
#include "Readers/MuonReader.h"
#include "Readers/JetReader.h"

AppFramework::AppFramework(const char* sequence){
        addModule( new InputModule("InputModule", "InputModule" ) );
        addModule( new BasicReader("BasicReader", "BasicReader" ) );
        addModule( new MuonReader ("MuonReader",  "MuonReader"  ) );
        addModule( new JetReader  ("JetReader",   "JetReader"   ) );
	reorder(sequence);
}

