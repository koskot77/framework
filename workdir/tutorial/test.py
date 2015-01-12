from init import *

fw = AppFramework('MyModule1->MyModule2->MyModule3->MyModule4->MyModule5')
fw.modList()
fw.verbose("AppFramework","cout on")
fw.modList();
fw.modify("MyModule4::mystring","Simple string");
fw.modify("MyModule4::mydouble","999");
fw.modify("MyModule5::formula","( val1+ val2) *val3");
fw.beginJob();
fw.process(10);
fw.endJob();
