using namespace std;

#include "AppFramework/AppFramework.h"

int main(int argc, char **argv) {

    AppFramework sequence("MyModule1->MyModule2->MyModule3->MyModule4->MyModule5");
    sequence.modList();
    sequence.verbose("AppFramework","cout on");
    sequence.modify("MyModule4::mystring","Simple string");
    sequence.modify("MyModule4::mydouble","999");
    sequence.modify("MyModule5::formula","( val1+ val2) *val3");

    sequence.beginJob();
    sequence.process(10);
    sequence.endJob();

    return 0;
}
