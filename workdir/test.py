from ctypes import cdll
lib = cdll.LoadLibrary('./AppFramework.so')

class AppFramework(object):
    def __init__(self):
        self.obj = lib.AppFramework_new()

    def __init__(self,seq):
        self.obj = lib.AppFramework_new(seq)

    def modList(self):
        lib.AppFramework_modList(self.obj)

    def verbose(self,target,action):
        lib.AppFramework_verbose(self.obj,target,action)

    def getParameter(self,modparname):
        lib.AppFramework_getParameter(self.obj,modparname)

    def modify(self,modparname,parvalue):
        lib.AppFramework_modify(self.obj,modparname,parvalue)

    def process(self,number):
        lib.AppFramework_process(self.obj,number)

    def beginJob(self):
        lib.AppFramework_beginJob(self.obj)

    def endJob(self):
        lib.AppFramework_endJob(self.obj)

if __name__ == '__main__':

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

