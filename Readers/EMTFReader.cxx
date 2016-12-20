using namespace std;
#include "EMTFReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"

//#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
edm::Wrapper<vector<l1t::EMTFTrack>> *__emtfTracks = new edm::Wrapper<vector<l1t::EMTFTrack>>();

AppResult EMTFReader::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputEMTFTrackBranch = Events->GetBranch("l1tEMTFTracks_simEmtfDigis__L1TMuonEmulation.");
    if( !inputEMTFTrackBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'l1tEMTFTracks_simEmtfDigis__L1TMuonEmulation.' branch found");
    inputEMTFTrackBranch->SetAddress(&__emtfTracks);

    return AppResult();
}

AppResult EMTFReader::event(AppEvent& event) {
    tracks.clear();
    if( __emtfTracks->isPresent() )
      for(vector<l1t::EMTFTrack>::const_iterator trk = __emtfTracks->product()->begin(); trk != __emtfTracks->product()->end(); ++trk){
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
          tracks.push_back(track);
      }

    sort   (tracks.begin(),tracks.end());
    reverse(tracks.begin(),tracks.end());

    event.put("emtfTracks",(const EMTFTrackCollection*)&tracks);

    return AppResult();
}
