using namespace std;
#include "EMTFReader.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"

//using namespace l1t;

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
      for(unsigned int t=0; t<__emtfTracks->product()->size(); t++){
          const l1t::EMTFTrack &trk = __emtfTracks->product()->operator[](t);

          EMTFTrackPointer track(new EMTFTrack());

          track->set_endcap        (trk.Endcap());
          track->set_sector        (trk.Sector());
          track->set_sector_idx    (trk.Sector_idx());
          track->set_mode          (trk.Mode());
          track->set_mode_inv      (trk.Mode_inv());
          track->set_rank          (trk.Rank());
          track->set_winner        (trk.Winner());
          track->set_charge        (trk.Charge());
          track->set_bx            (trk.BX());
          track->set_first_bx      (trk.First_BX());
          track->set_second_bx     (trk.Second_BX());
          track->set_pt            (trk.Pt());
          track->set_pt_XML        (trk.Pt_XML());
          track->set_zone          (trk.Zone());
          track->set_ph_num        (trk.Ph_num());
          track->set_ph_q          (trk.Ph_q());
          track->set_theta_fp      (trk.Theta_fp());
          track->set_theta         (trk.Theta());
          track->set_eta           (trk.Eta());
          track->set_phi_fp        (trk.Phi_fp());
          track->set_phi_loc       (trk.Phi_loc());
          track->set_phi_glob      (trk.Phi_glob());
          track->set_gmt_pt        (trk.GMT_pt());
          track->set_gmt_phi       (trk.GMT_phi());
          track->set_gmt_eta       (trk.GMT_eta());
          track->set_gmt_quality   (trk.GMT_quality());
          track->set_gmt_charge    (trk.GMT_charge());
          track->set_gmt_charge_valid(trk.GMT_charge_valid());
          track->set_track_num     (trk.Track_num());
          l1t::EMTFPtLUT tmp = trk.PtLUT();
          track->set_PtLUT         (reinterpret_cast<EMTFPtLUT&>(tmp));

          tracks.push_back(track);
      }

    sort   (tracks.begin(),tracks.end());
    reverse(tracks.begin(),tracks.end());

    event.put("emtfTracks",(const EMTFTrackCollection*)&tracks);

    return AppResult();
}
