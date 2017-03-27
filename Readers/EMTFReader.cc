using namespace std;
#include "EMTFReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"
edm::Wrapper<vector<l1t::EMTFTrackExtra>> *__emtfTracks = new edm::Wrapper<vector<l1t::EMTFTrackExtra>>();

AppResult EMTFReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputEMTFTrackBranch = Events->GetBranch("l1tEMTFTrackExtras_simEmtfDigis__L1TMuonEmulation.");
    if( !inputEMTFTrackBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'l1tEMTFTrackExtras_simEmtfDigis__L1TMuonEmulation.' branch found");
    inputEMTFTrackBranch->SetAddress(&__emtfTracks);

    return AppResult();
}

AppResult EMTFReader::event(AppEvent& event) {
    tracks.clear();
    if( __emtfTracks->isPresent() )
      for(vector<l1t::EMTFTrackExtra>::const_iterator trk = __emtfTracks->product()->begin(); trk != __emtfTracks->product()->end(); ++trk){
          EMTFTrackPointer track(new EMTFTrack());
          track->set_endcap        (trk->Endcap());
          track->set_sector        (trk->Sector());
          track->set_sector_GMT    (trk->Sector_GMT());
          track->set_sector_index  (trk->Sector_index());
          track->set_mode          (trk->Mode());
          track->set_mode_LUT      (trk->Mode_LUT());
          track->set_quality       (trk->Quality());
          track->set_bx            (trk->BX());
          track->set_pt            (trk->Pt());
          track->set_pt_GMT        (trk->Pt_GMT());
          track->set_pt_LUT_addr   (trk->Pt_LUT_addr());
          track->set_eta           (trk->Eta());
          track->set_eta_GMT       (trk->Eta_GMT());
          track->set_eta_LUT       (trk->Eta_LUT());
          track->set_phi_loc_int   (trk->Phi_loc_int());
          track->set_phi_loc_deg   (trk->Phi_loc_deg());
          track->set_phi_loc_rad   (trk->Phi_loc_rad());
          track->set_phi_GMT       (trk->Phi_GMT());
          track->set_phi_glob_deg  (trk->Phi_glob_deg());
          track->set_phi_glob_rad  (trk->Phi_glob_rad());
          track->set_charge        (trk->Charge());
          track->set_charge_GMT    (trk->Charge_GMT());
          track->set_charge_valid  (trk->Charge_valid());
          track->set_dPhi_12       (trk->DPhi_12());
          track->set_dPhi_13       (trk->DPhi_13());
          track->set_dPhi_14       (trk->DPhi_14());
          track->set_dPhi_23       (trk->DPhi_23());
          track->set_dPhi_24       (trk->DPhi_24());
          track->set_dPhi_34       (trk->DPhi_34());
          track->set_dTheta_12     (trk->DTheta_12());
          track->set_dTheta_13     (trk->DTheta_13());
          track->set_dTheta_14     (trk->DTheta_14());
          track->set_dTheta_23     (trk->DTheta_23());
          track->set_dTheta_24     (trk->DTheta_24());
          track->set_dTheta_34     (trk->DTheta_34());
          track->set_clct_1        (trk->CLCT_1());
          track->set_clct_2        (trk->CLCT_2());
          track->set_clct_3        (trk->CLCT_3());
          track->set_clct_4        (trk->CLCT_4());
          track->set_fr_1          (trk->FR_1());
          track->set_fr_2          (trk->FR_2());
          track->set_fr_3          (trk->FR_3());
          track->set_fr_4          (trk->FR_4());
          track->set_track_num     (trk->Track_num());
          track->set_has_neighbor  (trk->Has_neighbor());
          track->set_all_neighbor  (trk->All_neighbor());

          l1t::EMTFHitExtraCollection hits = const_cast<l1t::EMTFTrackExtra&>(*trk).HitsExtra();
          l1t::EMTFHitExtra stub[5];
          for(auto &h : hits){
              if( h.Station()<1 && h.Station()>4 ){
                  cerr << "Invalid station: " << h.Station() << endl;
                  continue;
              }
              stub[ h.Station() ] = h;
          }

          track->set_dTheta_12( (stub[1].Station()==1 && stub[2].Station()==2 ? stub[1].Theta_int() - stub[2].Theta_int() : -999) );
          track->set_dTheta_23( (stub[2].Station()==2 && stub[3].Station()==3 ? stub[2].Theta_int() - stub[3].Theta_int() : -999) );
          track->set_dTheta_34( (stub[3].Station()==3 && stub[4].Station()==4 ? stub[3].Theta_int() - stub[4].Theta_int() : -999) );

//          if( stub[1].Station()==1 && stub[2].Station()==2 && trk->DPhi_12() != stub[1].Phi_loc_int() - stub[2].Phi_loc_int() )
//                  cerr << "Inconsistent dPhi12: " << trk->DPhi_12() << " = " << stub[1].Phi_loc_int() << "-" << stub[2].Phi_loc_int() << endl;
//          if( stub[2].Station()==2 && stub[3].Station()==3 && trk->DPhi_23() != stub[2].Phi_loc_int() - stub[3].Phi_loc_int() )
//                  cerr << "Inconsistent dPhi23: " << trk->DPhi_23() << " = " << stub[2].Phi_loc_int() << "-" << stub[3].Phi_loc_int() << endl;
//          if( stub[3].Station()==3 && stub[4].Station()==4 && trk->DPhi_34() != stub[3].Phi_loc_int() - stub[4].Phi_loc_int() )
//                  cerr << "Inconsistent dPhi34: " << trk->DPhi_34() << " = " << stub[3].Phi_loc_int() << "-" << stub[4].Phi_loc_int() << endl;

          track->set_clct_1( ( stub[1].Station()==1 ? stub[1].Pattern() : -999 ) );
          track->set_clct_2( ( stub[2].Station()==2 ? stub[2].Pattern() : -999 ) );
          track->set_clct_3( ( stub[3].Station()==3 ? stub[3].Pattern() : -999 ) );
          track->set_clct_4( ( stub[4].Station()==4 ? stub[4].Pattern() : -999 ) );

          tracks.push_back(track);
      }

    sort   (tracks.begin(),tracks.end());
    reverse(tracks.begin(),tracks.end());

    event.put("emtfTracks",(const EMTFTrackSep2016Collection*)&tracks);

    return AppResult();
}
