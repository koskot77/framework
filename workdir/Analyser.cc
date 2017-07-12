using namespace std;
#include "Analyser.h"
#include "AnObjects/Particle.h"
#include "AnObjects/EMTFTrack.h"
#include "PtLutVarCalc.cc"
#include <fstream>
namespace {

int sat(int x, unsigned int n){ int m = (1<<n)-1; if(x>m) return m; if(x<-m) return -m; return x; }
unsigned int lsb(unsigned int x, unsigned int n){ return (x&((1<<n)-1)); }
unsigned int msb(unsigned int x, unsigned int n, unsigned int m){ return (x>>m)&((1<<(n+1-m))-1); }

unsigned int predictors2address15(int dPhi12, int dPhi23, int dPhi34, int dTheta14, unsigned int theta, unsigned int ring1, int clct1, int fr1){
  unsigned int address = 0;
  // set highest bit [29:29] to indicate this was mode_inv=15
  // address |= 0x20000000;
  // ignore all of the signs
  address |= (msb(sat(abs(dPhi12),9),9,2) & 0x7F) << 0;
  address |= (msb(sat(abs(dPhi23),7),7,2) & 0x1F) << (0+7);
  address |= (msb(sat(abs(dPhi34),7),7,2) & 0x1F) << (7+5);
  address |= (dPhi23*dPhi12>=0?0:1) << (7+5+5);
  address |= (dPhi34*dPhi12>=0?0:1) << (7+5+5+1);
  address |= (sat(abs(dTheta14),2) & 0x3) << (7+5+5+1+1);
  address |= ((const int[]){0,0,0,0,1,1,2,2,3,3,3,0,0,0,0,0}[(clct1&0xF)]) << (7+5+5+1+1+2);
  address |= fr1 << (0+7+5+5+1+1+2+2);
  address |= (msb(theta + (const int[]){0,0,6,6,0}[ring1],7,2)&0x1F) << (0+7+5+5+1+1+2+2+1);
  return address;
}

}

AppResult Analyser::beginJob(AppEvent& event){
//    ptLUT.reset(new float[0x20000000]);
//    std::ifstream in("lut15.txt");
//    for(int i=0; i<0x20000000; i++)
//        in>>ptLUT[i];
//    in.close();
    return AppResult();
}

AppResult Analyser::endJob(AppEvent& event){
    return AppResult();
}

AppResult Analyser::event(AppEvent& event){
    numberOfGenMuons = 0;
    muPtGen[0] = -1;
    muPtGen[1] = -1;
    muPtGen[2] = -1;
    muPtGen[3] = -1;
    bzero(theta_,   sizeof(theta_));
    bzero(st1_ring2,sizeof(st1_ring2));
    bzero(mode_,    sizeof(mode_));
    bzero(muEtaGen, sizeof(muEtaGen));
    bzero(muPhiGen, sizeof(muPhiGen));
    bzero(dPhi12,   sizeof(dPhi12));
    bzero(dPhi13,   sizeof(dPhi13));
    bzero(dPhi14,   sizeof(dPhi14));
    bzero(dPhi23,   sizeof(dPhi23));
    bzero(dPhi24,   sizeof(dPhi24));
    bzero(dPhi34,   sizeof(dPhi34));
    bzero(dTheta12, sizeof(dTheta12));
    bzero(dTheta13, sizeof(dTheta13));
    bzero(dTheta14, sizeof(dTheta14));
    bzero(dTheta23, sizeof(dTheta23));
    bzero(dTheta24, sizeof(dTheta24));
    bzero(dTheta34, sizeof(dTheta34));
    bzero(clct1,    sizeof(clct1));
    bzero(clct2,    sizeof(clct2));
    bzero(clct3,    sizeof(clct3));
    bzero(clct4,    sizeof(clct4));
    bzero(fr1,      sizeof(fr1));
    bzero(fr2,      sizeof(fr2));
    bzero(fr3,      sizeof(fr3));
    bzero(fr4,      sizeof(fr4));

    bzero(dPhiS4,   sizeof(dPhiS4));
    bzero(dPhiS4A,  sizeof(dPhiS4A));
    bzero(dPhiS3,   sizeof(dPhiS3));
    bzero(dPhiS3A,  sizeof(dPhiS3A));

    const char *path = 0;

    if( event.get("runNumber",       run) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No runNumber found");
    if( event.get("eventNumber",     evt) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No eventNumber found");
    if( event.get("chainEntryNumber",etr) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No chainEntryNumber found");
    if( event.get("inputFileName",  path) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No inputFileName found");

    event.put("run",   (const int*)&run);
    event.put("event", (const int*)&evt);
    event.put("entry", (const int*)&etr);

    cout<<"Event #"<<evt<<endl;
/*
    const ParticleCollection *genParticles;
    if( event.get("genParticles",genParticles) || !genParticles ) return AppResult(AppResult::STOP|AppResult::ERROR,"No Gen Info found");
    cout<<" #genParticles = "<<genParticles->size()<<std::endl;

    numberOfGenMuons = genParticles->size();
    for(int j=0; j<numberOfGenMuons && j<2; j++){
        cout<<" pTGenmuon["<<j<<"] = "<<genParticles->at(j)->pt()<<" eta="<<genParticles->at(j)->eta()<<" phi="<<genParticles->at(j)->phi()<<std::endl;
        muPtGen [j] = genParticles->at(j)->pt();
        muEtaGen[j] = genParticles->at(j)->eta();
        muPhiGen[j] = genParticles->at(j)->phi();
    }

    event.put("numberOfGenMuons", (const int*)&numberOfGenMuons);
    event.put("muPtGen",  (const double*)muPtGen);
    event.put("muEtaGen", (const double*)muEtaGen);
    event.put("muPhiGen", (const double*)muPhiGen);
*/
    const EMTFTrackCollection *tracks;
    if( event.get("emtfTracks",tracks) || !tracks ) return AppResult(AppResult::STOP|AppResult::ERROR,"No EMTF tracks found");
    cout<<" #tracks = "<<tracks->size()<<std::endl;

    numberOfEMTFTracks = tracks->size();
    for(int j=0; j<numberOfEMTFTracks && j<4; j++){
/*
        mode[j]     = tracks->at(j)->Mode();
        theta[j]    = tracks->at(j)->Theta_fp();
        dPhi12[j]   = tracks->at(j)->PtLUT().delta_ph[0];
        dPhi13[j]   = tracks->at(j)->PtLUT().delta_ph[1];
        dPhi14[j]   = tracks->at(j)->PtLUT().delta_ph[2];
        dPhi23[j]   = tracks->at(j)->PtLUT().delta_ph[3];
        dPhi24[j]   = tracks->at(j)->PtLUT().delta_ph[4];
        dPhi34[j]   = tracks->at(j)->PtLUT().delta_ph[5];
        dTheta12[j] = tracks->at(j)->PtLUT().delta_th[0];
        dTheta13[j] = tracks->at(j)->PtLUT().delta_th[1];
        dTheta14[j] = tracks->at(j)->PtLUT().delta_th[2];
        dTheta23[j] = tracks->at(j)->PtLUT().delta_th[3];
        dTheta24[j] = tracks->at(j)->PtLUT().delta_th[4];
        dTheta34[j] = tracks->at(j)->PtLUT().delta_th[5];
        sPhi12[j]   = tracks->at(j)->PtLUT().sign_ph[0];
        sPhi13[j]   = tracks->at(j)->PtLUT().sign_ph[1];
        sPhi14[j]   = tracks->at(j)->PtLUT().sign_ph[2];
        sPhi23[j]   = tracks->at(j)->PtLUT().sign_ph[3];
        sPhi24[j]   = tracks->at(j)->PtLUT().sign_ph[4];
        sPhi34[j]   = tracks->at(j)->PtLUT().sign_ph[5];
        sTheta12[j] = tracks->at(j)->PtLUT().sign_th[0];
        sTheta13[j] = tracks->at(j)->PtLUT().sign_th[1];
        sTheta14[j] = tracks->at(j)->PtLUT().sign_th[2];
        sTheta23[j] = tracks->at(j)->PtLUT().sign_th[3];
        sTheta24[j] = tracks->at(j)->PtLUT().sign_th[4];
        sTheta34[j] = tracks->at(j)->PtLUT().sing_th[5];
        clct1[j]    = tracks->at(j)->PtLUT().cpattern[0];
        clct2[j]    = tracks->at(j)->PtLUT().cpattern[1];
        clct3[j]    = tracks->at(j)->PtLUT().cpattern[2];
        clct4[j]    = tracks->at(j)->PtLUT().cpattern[3];
        fr1[j]      = tracks->at(j)->PtLUT().fr[0];
        fr2[j]      = tracks->at(j)->PtLUT().fr[1];
        fr3[j]      = tracks->at(j)->PtLUT().fr[2];
        fr4[j]      = tracks->at(j)->PtLUT().fr[3];
*/
        // https://github.com/cms-l1t-offline/cmssw/blob/l1t-integration-CMSSW_9_2_5_patch2/L1Trigger/L1TMuonEndCap/src/PtAssignmentEngine2017.cc#L424-L505

  int endcap = tracks->at(j)->Endcap();
  int mode   = tracks->at(j)->Mode();
  int theta  = tracks->at(j)->Theta_fp();
  int phi    = tracks->at(j)->Phi_fp();
  
  EMTFPtLUT data = tracks->at(j)->PtLUT();

  // Which stations have hits
  int st1 = (mode >= 8);
  int st2 = ((mode % 8) >= 4);
  int st3 = ((mode % 4) >= 2);
  int st4 = ((mode % 2) == 1);
  
  // Variables for input to XMLs
  int dPhi_12, dPhi_13, dPhi_14, dPhi_23, dPhi_24, dPhi_34, dPhiSign;
  int dPhiSum4, dPhiSum4A, dPhiSum3, dPhiSum3A, outStPhi;
  int dTh_12, dTh_13, dTh_14, dTh_23, dTh_24, dTh_34;
  int FR_1, FR_2, FR_3, FR_4;
  int bend_1, bend_2, bend_3, bend_4;
  int RPC_1, RPC_2, RPC_3, RPC_4;
  int St1_ring2 = data.st1_ring2;
  
  int  ph1 = -99,  ph2 = -99,  ph3 = -99,  ph4 = -99;
  int  th1 = -99,  th2 = -99,  th3 = -99,  th4 = -99;
  int pat1 = -99, pat2 = -99, pat3 = -99, pat4 = -99;
  
  // Compute the original phi and theta coordinates
  if        (st2) {
    ph2 = phi;   // Track phi is from station 2 (if it exists), otherwise 3 or 4
    th2 = theta; // Likewise for track theta
    if (st1) ph1 = ph2 - data.delta_ph[0]*(data.sign_ph[0] ? 1 : -1);
    if (st1) th1 = th2 - data.delta_th[0]*(data.sign_th[0] ? 1 : -1);
    if (st3) ph3 = ph2 + data.delta_ph[3]*(data.sign_ph[3] ? 1 : -1);
    // Important that phi be from adjacent station pairs (see note below)
    if (st3 && st4) ph4 = ph3 + data.delta_ph[5]*(data.sign_ph[5] ? 1 : -1);
    else if   (st4) ph4 = ph2 + data.delta_ph[4]*(data.sign_ph[4] ? 1 : -1);
    // Important that theta be from first-last station pair, not adjacent pairs: delta_th values are "best" for each pair, but 
    // thanks to duplicated CSC LCTs, are not necessarily consistent (or physical) between pairs or between delta_th and delta_ph.
    // This is an artifact of the firmware implementation of deltas: see src/AngleCalculation.cc.
    if (st1 && st3) th3 = th1 + data.delta_th[1]*(data.sign_th[1] ? 1 : -1);
    else if   (st3) th3 = th2 + data.delta_th[3]*(data.sign_th[3] ? 1 : -1);
    if (st1 && st4) th4 = th1 + data.delta_th[2]*(data.sign_th[2] ? 1 : -1); 
    else if   (st4) th4 = th2 + data.delta_th[4]*(data.sign_th[4] ? 1 : -1); 
  } else if (st3) {
    ph3 = phi;
    th3 = theta;
    if (st1) ph1 = ph3 - data.delta_ph[1]*(data.sign_ph[1] ? 1 : -1);
    if (st1) th1 = th3 - data.delta_th[1]*(data.sign_th[1] ? 1 : -1);
    if (st4) ph4 = ph3 + data.delta_ph[5]*(data.sign_ph[5] ? 1 : -1);
    if (st1 && st4) th4 = th1 + data.delta_th[2]*(data.sign_th[2] ? 1 : -1);
    else if   (st4) th4 = th3 + data.delta_th[5]*(data.sign_th[5] ? 1 : -1);
  } else if (st4) {
    ph4 = phi;
    th4 = theta;
    if (st1) ph1 = ph4 - data.delta_ph[2]*(data.sign_ph[2] ? 1 : -1);
    if (st1) th1 = th4 - data.delta_th[2]*(data.sign_th[2] ? 1 : -1);
  }

  if (st1) pat1 = data.cpattern[0];
  if (st2) pat2 = data.cpattern[1];
  if (st3) pat3 = data.cpattern[2];
  if (st4) pat4 = data.cpattern[3];

  // BEGIN: Identical (almost) to BDT training code in EMTFPtAssign2017/PtRegression_Apr_2017.C

  theta = CalcTrackTheta( th1, th2, th3, th4, St1_ring2, mode, true );
  
  CalcDeltaPhis( dPhi_12, dPhi_13, dPhi_14, dPhi_23, dPhi_24, dPhi_34, dPhiSign,
		 dPhiSum4, dPhiSum4A, dPhiSum3, dPhiSum3A, outStPhi,
		 ph1, ph2, ph3, ph4, mode, true );
  
  CalcDeltaThetas( dTh_12, dTh_13, dTh_14, dTh_23, dTh_24, dTh_34,
		   th1, th2, th3, th4, mode, true );
  
  FR_1 = (st1 ? data.fr[0] : -99);
  FR_2 = (st2 ? data.fr[1] : -99);
  FR_3 = (st3 ? data.fr[2] : -99);
  FR_4 = (st4 ? data.fr[3] : -99);
  
  CalcBends( bend_1, bend_2, bend_3, bend_4,
	     pat1, pat2, pat3, pat4,
	     dPhiSign, endcap, mode, true );
  
  RPC_1 = (st1 ? (pat1 == 0) : -99);
  RPC_2 = (st2 ? (pat2 == 0) : -99);
  RPC_3 = (st3 ? (pat3 == 0) : -99);
  RPC_4 = (st4 ? (pat4 == 0) : -99);

  CalcRPCs( RPC_1, RPC_2, RPC_3, RPC_4, mode, St1_ring2, theta, true );


        address[j]  = data.address;
        mode_[j]    = mode;
        theta_[j]   = theta;
        st1_ring2[j]= St1_ring2;
        dPhi12[j]   = dPhi_12;
        dPhi13[j]   = dPhi_13;
        dPhi14[j]   = dPhi_14;
        dPhi23[j]   = dPhi_23;
        dPhi24[j]   = dPhi_24;
        dPhi34[j]   = dPhi_34;
        dTheta12[j] = dTh_12;
        dTheta13[j] = dTh_13;
        dTheta14[j] = dTh_14;
        dTheta23[j] = dTh_23;
        dTheta24[j] = dTh_24;
        dTheta34[j] = dTh_34;
        clct1[j]= bend_1;
        clct2[j]= bend_2;
        clct3[j]= bend_3;
        clct4[j]= bend_4;
        fr1[j]  = FR_1;
        fr2[j]  = FR_2;
        fr3[j]  = FR_3;
        fr4[j]  = FR_4;
        rpc1[j] = RPC_1; 
        rpc2[j] = RPC_2; 
        rpc3[j] = RPC_3; 
        rpc4[j] = RPC_4; 

        dPhiS4[j]   = dPhiSum4;
        dPhiS4A[j]  = dPhiSum4A;
        dPhiS3[j]   = dPhiSum3;
        dPhiS3A[j]  = dPhiSum3A;
    }


    event.put("numberOfEMTFTracks", (const int*)&numberOfEMTFTracks);
    event.put("address[2]",(const int*)address);
    event.put("mode[2]",   (const int*)mode_);
    event.put("theta[2]",  (const int*)theta_);
    event.put("st1_ring2[2]", (const int*)st1_ring2);
    event.put("dPhi12[2]", (const int*)dPhi12);
    event.put("dPhi13[2]", (const int*)dPhi13);
    event.put("dPhi14[2]", (const int*)dPhi14);
    event.put("dPhi23[2]", (const int*)dPhi23);
    event.put("dPhi24[2]", (const int*)dPhi24);
    event.put("dPhi34[2]", (const int*)dPhi34);
    event.put("dTheta12[2]", (const int*)dTheta12);
    event.put("dTheta13[2]", (const int*)dTheta13);
    event.put("dTheta14[2]", (const int*)dTheta14);
    event.put("dTheta23[2]", (const int*)dTheta23);
    event.put("dTheta24[2]", (const int*)dTheta24);
    event.put("dTheta34[2]", (const int*)dTheta34);
    event.put("clct1[2]", (const int*)clct1);
    event.put("clct2[2]", (const int*)clct2);
    event.put("clct3[2]", (const int*)clct3);
    event.put("clct4[2]", (const int*)clct4);
    event.put("fr1[2]", (const int*)fr1);
    event.put("fr2[2]", (const int*)fr2);
    event.put("fr3[2]", (const int*)fr3);
    event.put("fr4[2]", (const int*)fr4);
    event.put("rpc1[2]", (const int*)rpc1);
    event.put("rpc2[2]", (const int*)rpc2);
    event.put("rpc3[2]", (const int*)rpc3);
    event.put("rpc4[2]", (const int*)rpc4);
    event.put("dPhiS4[2]",  (const int*)dPhiS4);
    event.put("dPhiS4A[2]", (const int*)dPhiS4A);
    event.put("dPhiS3[2]",  (const int*)dPhiS3);
    event.put("dPhiS3A[2]", (const int*)dPhiS3A);

    return AppResult();
}
