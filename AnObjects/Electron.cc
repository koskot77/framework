#include "Electron.h"
bool operator<(ElectronPointer i, ElectronPointer j) { return ((bool)i && (bool)j ? (*i)<(*j) : false); }

