using namespace std;
#include "Analyser.h"
#include "AnObjects/EMTFTrack.h"
#include "AnObjects/Particle.h"
#include "AnObjects/EMTFTrack.h"

AppResult Analyser::beginJob(AppEvent& event){
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

    const EMTFTrackCollection *tracks;
    if( event.get("emtfTracks",tracks) || !tracks ) return AppResult(AppResult::STOP|AppResult::ERROR,"No EMTF tracks found");
    cout<<" #tracks = "<<tracks->size()<<std::endl;

    numberOfEMTFTracks = tracks->size();
    for(int j=0; j<numberOfEMTFTracks && j<4; j++){
        mode[j]     = tracks->at(j)->Mode();
        dPhi12[j]   = tracks->at(j)->DPhi_12();
        dPhi23[j]   = tracks->at(j)->DPhi_23();
        dPhi34[j]   = tracks->at(j)->DPhi_34();
        dTheta12[j] = tracks->at(j)->DTheta_12();
        dTheta23[j] = tracks->at(j)->DTheta_23();
        dTheta34[j] = tracks->at(j)->DTheta_34();
        clct1[j]    = tracks->at(j)->CLCT_1();
        clct2[j]    = tracks->at(j)->CLCT_2();
        clct3[j]    = tracks->at(j)->CLCT_3();
        clct4[j]    = tracks->at(j)->CLCT_4();
        fr1[j]      = tracks->at(j)->FR_1();
        fr2[j]      = tracks->at(j)->FR_2();
        fr3[j]      = tracks->at(j)->FR_3();
        fr4[j]      = tracks->at(j)->FR_4();
    }

    event.put("numberOfEMTFTracks", (const int*)&numberOfEMTFTracks);
    event.put("mode[2]",   (const int*)mode);
    event.put("dPhi12[2]", (const int*)dPhi12);
    event.put("dPhi23[2]", (const int*)dPhi23);
    event.put("dPhi34[2]", (const int*)dPhi34);
    event.put("dTheta12[2]", (const int*)dTheta12);
    event.put("dTheta23[2]", (const int*)dTheta23);
    event.put("dTheta34[2]", (const int*)dTheta34);
    event.put("clct1[2]", (const int*)clct1);
    event.put("clct2[2]", (const int*)clct2);
    event.put("clct3[2]", (const int*)clct3);
    event.put("clct4[2]", (const int*)clct4);
    event.put("fr", (const int*)fr1);
    event.put("fr2[2]", (const int*)fr2);
    event.put("fr3[2]", (const int*)fr3);
    event.put("fr4[2]", (const int*)fr4);

    return AppResult();
}
