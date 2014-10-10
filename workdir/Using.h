#include "AppFramework/AppFramework.h"
#include "MyModule1.h"
#include "MyModule2.h"
#include "MyModule3.h"
#include "MyModule4.h"
#include "MyModule5.h"

AppFramework::AppFramework(const char* sequence){
//	addAgent ( new SomeAgent1("SomeAgent1","performing of some operatioms") );
	addModule( new MyModule1("MyModule1", "First example"  ) );
	addModule( new MyModule2("MyModule2", "Second example" ) );
	addModule( new MyModule3("MyModule3", "Third example"  ) );
	addModule( new MyModule4("MyModule4", "Fourth example" ) );
	addModule( new MyModule5("MyModule5", "Fifth example"  ) );
	reorder(sequence);
}

