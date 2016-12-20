#include "EMTFTrack.h"
bool operator<(EMTFTrackPointer i, EMTFTrackPointer j) { return ((bool)i && (bool)j ? (*i)<(*j) : false); }
