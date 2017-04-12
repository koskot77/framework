using namespace std;
#include "Analyser.h"
#include "AnObjects/Particle.h"
#include "AnObjects/EMTFTrackSep2016.h"
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
    ptLUT.reset(new float[0x20000000]);
    std::ifstream in("lut15.txt");
    for(int i=0; i<0x20000000; i++)
        in>>ptLUT[i];
    in.close();
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
    bzero(theta_i,  sizeof(theta_i));
    bzero(mode,     sizeof(mode));
    bzero(muEtaGen, sizeof(muEtaGen));
    bzero(muPhiGen, sizeof(muPhiGen));
    bzero(dPhi12,   sizeof(dPhi12));
    bzero(dPhi23,   sizeof(dPhi23));
    bzero(dPhi34,   sizeof(dPhi34));
    bzero(dTheta12, sizeof(dTheta12));
    bzero(dTheta23, sizeof(dTheta23));
    bzero(dTheta34, sizeof(dTheta34));
    bzero(clct1,    sizeof(clct1));
    bzero(clct2,    sizeof(clct2));
    bzero(clct3,    sizeof(clct3));
    bzero(clct4,    sizeof(clct4));
    bzero(fr1,      sizeof(fr1));
    bzero(fr2,      sizeof(fr2));
    bzero(fr3,      sizeof(fr3));
    bzero(fr4,      sizeof(fr4));
    bzero(ring1,    sizeof(ring1));
    bzero(ring2,    sizeof(ring2));
    bzero(ring3,    sizeof(ring3));
    bzero(ring4,    sizeof(ring4));
    bzero(rpc1,     sizeof(rpc1));
    bzero(rpc2,     sizeof(rpc2));
    bzero(rpc3,     sizeof(rpc3));
    bzero(rpc4,     sizeof(rpc4));

    const char *path = 0;

    if( event.get("runNumber",       run) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No runNumber found");
    if( event.get("eventNumber",     evt) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No eventNumber found");
    if( event.get("chainEntryNumber",etr) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No chainEntryNumber found");
    if( event.get("inputFileName",  path) ) return AppResult(AppResult::STOP|AppResult::ERROR,"No inputFileName found");

    event.put("run",   (const int*)&run);
    event.put("event", (const int*)&evt);
    event.put("entry", (const int*)&etr);

    cout<<"Event #"<<evt<<endl;

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

    const EMTFTrackSep2016Collection *tracks;
    if( event.get("emtfTracks",tracks) || !tracks ) return AppResult(AppResult::STOP|AppResult::ERROR,"No EMTF tracks found");
    cout<<" #tracks = "<<tracks->size()<<std::endl;

    numberOfEMTFTracks = tracks->size();
    for(int j=0; j<numberOfEMTFTracks && j<4; j++){
        mode[j]     = tracks->at(j)->Mode();
        dPhi12[j]   = tracks->at(j)->DPhi_12();
        dPhi13[j]   = tracks->at(j)->DPhi_13();
        dPhi14[j]   = tracks->at(j)->DPhi_14();
        dPhi23[j]   = tracks->at(j)->DPhi_23();
        dPhi24[j]   = tracks->at(j)->DPhi_24();
        dPhi34[j]   = tracks->at(j)->DPhi_34();
        dTheta12[j] = tracks->at(j)->DTheta_12();
        dTheta13[j] = tracks->at(j)->DTheta_13();
        dTheta14[j] = tracks->at(j)->DTheta_14();
        dTheta23[j] = tracks->at(j)->DTheta_23();
        dTheta24[j] = tracks->at(j)->DTheta_24();
        dTheta34[j] = tracks->at(j)->DTheta_34();
        clct1[j]    = tracks->at(j)->CLCT_1();
        clct2[j]    = tracks->at(j)->CLCT_2();
        clct3[j]    = tracks->at(j)->CLCT_3();
        clct4[j]    = tracks->at(j)->CLCT_4();
        fr1[j]      = tracks->at(j)->FR_1();
        fr2[j]      = tracks->at(j)->FR_2();
        fr3[j]      = tracks->at(j)->FR_3();
        fr4[j]      = tracks->at(j)->FR_4();
        ring1[j]    = tracks->at(j)->ring_station1();
        ring2[j]    = tracks->at(j)->ring_station2();
        ring3[j]    = tracks->at(j)->ring_station3();
        ring4[j]    = tracks->at(j)->ring_station4();
        rpc1[j]     = tracks->at(j)->isRPC_station1();
        rpc2[j]     = tracks->at(j)->isRPC_station2();
        rpc3[j]     = tracks->at(j)->isRPC_station3();
        rpc4[j]     = tracks->at(j)->isRPC_station4();
        pt[j]       = tracks->at(j)->Pt();
        ptGMT[j]    = tracks->at(j)->Pt_GMT();
        theta_i[j]  = tracks->at(j)->Theta_int();
//                           ( ifelse( 2*atan(exp(-muEtaGen))<3.1415927/2,
//                                     2*atan(exp(-muEtaGen)),
//                                     3.1415927-2*atan(exp(-muEtaGen))
//                             ) * 180/3.1415927 - 8.5
//                           ) * 128 / (45.0-8.5),
        if( mode[j]==15 ){
        cout << dPhi12[j] << " " << dPhi23[j] << " " << dPhi34[j] << " " << dTheta14[j] << " " << theta_i[j] << " " << ring1[j] << " " << clct1[j] << " " << fr1[j] << endl;
        cout << "#"<<j<<": "<<std::hex<<predictors2address15(dPhi12[j], dPhi23[j], dPhi34[j], dTheta14[j], theta_i[j], ring1[j], clct1[j], fr1[j]) <<std::dec<< endl;
        cout << ptLUT[ predictors2address15(dPhi12[j], dPhi23[j], dPhi34[j], dTheta14[j], theta_i[j], ring1[j], clct1[j], fr1[j]) ] << endl;
}
        mypt[j]     = (mode[j]==15 ?
                       ptLUT[ predictors2address15(dPhi12[j], dPhi23[j], dPhi34[j], dTheta14[j], theta_i[j], ring1[j], clct1[j], fr1[j]) ]
                       :
                       -1
                      );
    }

    event.put("numberOfEMTFTracks", (const int*)&numberOfEMTFTracks);
    event.put("mode[2]",   (const int*)mode);
    event.put("pt[2]",   (const double*)pt);
    event.put("theta_i[2]",(const int*)theta_i);
    event.put("ptGMT[2]",  (const int*)ptGMT);
    event.put("mypt[2]",   (const double*)mypt);
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
    event.put("ring1[2]", (const int*)ring1);
    event.put("ring2[2]", (const int*)ring2);
    event.put("ring3[2]", (const int*)ring3);
    event.put("ring4[2]", (const int*)ring4);
    event.put("rpc1[2]", (const int*)rpc1);
    event.put("rpc2[2]", (const int*)rpc2);
    event.put("rpc3[2]", (const int*)rpc3);
    event.put("rpc4[2]", (const int*)rpc4);

    return AppResult();
}
