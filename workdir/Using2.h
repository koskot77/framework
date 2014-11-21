#include "AppFramework/AppFramework.h"
#include "AnModBasic/InputModule.h"
#include "Readers/JetReader.h"

AppFramework::AppFramework(const char* sequence){
        addModule( new InputModule("InputModule", "InputModule" ) );
        addModule( new JetReader  ("JetReader",   "JetReader"   ) );
	reorder(sequence);
}

