#include "EMTFTrackSep2016.h"
bool operator<(EMTFTrackSep2016Pointer i, EMTFTrackSep2016Pointer j) { return ((bool)i && (bool)j ? (*i)<(*j) : false); }
