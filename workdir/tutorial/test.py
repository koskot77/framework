from init import *

sequence = AppFramework('MyModule1->MyModule2->MyModule3->MyModule4->MyModule5')
sequence.modList()
sequence.verbose("AppFramework","cout on")
sequence.modList();
sequence.modify("MyModule4::mystring","Simple string");
sequence.modify("MyModule4::mydouble","999");
sequence.modify("MyModule5::formula","( val1+ val2) *val3");
sequence.beginJob();
sequence.process(10);
sequence.endJob();
