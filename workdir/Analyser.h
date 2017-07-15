#ifndef Analyser_h
#define Analyser_h
#include "AppFramework/AppParameter.h"
#include "AppFramework/AppModule.h"
#include "AppFramework/AppResult.h"
#include "AppFramework/AppEvent.h"
#include <memory>

class Analyser : public AppModule{
private:
    int    run, evt, etr;
    int    numberOfGenMuons;
    double muPtGen[4], muEtaGen[4], muPhiGen[4];
    int    numberOfEMTFTracks;

    // pT LUT fields
    int    address[4],  mode_[4], theta_[4], st1_ring2[4];
    int    dPhi12[4],   dPhi13[4],   dPhi14[4];
    int    dPhi23[4],   dPhi24[4],   dPhi34[4];
    int    dTheta12[4], dTheta13[4], dTheta14[4];
    int    dTheta23[4], dTheta24[4], dTheta34[4];

    int    dPhiS4[4],   dPhiS4A[4],  dPhiS3[4], dPhiS3A[4];

    int    clct1[4],    clct2[4],    clct3[4], clct4[4];
    int    fr1[4],      fr2[4],      fr3[4],   fr4[4];
    int    rpc1[4],     rpc2[4],     rpc3[4],  rpc4[4];

    double pt[4], ptXML[4], theta_glob[4], eta_glob[4], phi_glob[4];

    AppResult beginJob(AppEvent& event);
    AppResult beginRun(AppEvent& event){ return AppResult(); }
    AppResult endRun  (AppEvent& event){ return AppResult(); }
    AppResult endJob  (AppEvent& event);

    AppResult event (AppEvent& event);

    unique_ptr<float[]> ptLUT;

public:
    Analyser(const char *nm, const char *descr):AppModule(nm,descr){}
    virtual ~Analyser(void){}
};

#endif
