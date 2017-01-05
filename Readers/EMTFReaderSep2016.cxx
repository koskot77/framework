using namespace std;
#include "EMTFReaderSep2016.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
edm::Wrapper<vector<L1TMuonEndCap::EMTFTrack>> *__emtfTracks = new edm::Wrapper<vector<L1TMuonEndCap::EMTFTrack>>();
edm::Wrapper<vector<L1TMuonEndCap::EMTFTrackExtra>> *__emtfTracksExtra = new edm::Wrapper<vector<L1TMuonEndCap::EMTFTrackExtra>>();

AppResult EMTFReaderSep2016::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputEMTFTrackBranch = Events->GetBranch("L1TMuonEndCapEMTFTracks_simEmtfDigisSep2016__reL1T.");
    if( !inputEMTFTrackBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'L1TMuonEndCapEMTFTracks_simEmtfDigisSep2016__reL1T.' branch found");
    inputEMTFTrackBranch->SetAddress(&__emtfTracks);

    TBranch *inputEMTFTrackExtraBranch = Events->GetBranch("L1TMuonEndCapEMTFTrackExtras_simEmtfDigisSep2016__reL1T.");
    if( !inputEMTFTrackExtraBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'L1TMuonEndCapEMTFTrackExtras_simEmtfDigisSep2016__reL1T.' branch found");
    inputEMTFTrackExtraBranch->SetAddress(&__emtfTracksExtra);

    return AppResult();
}

AppResult EMTFReaderSep2016::event(AppEvent& event) {
    tracks.clear();
    if( __emtfTracks->isPresent() )
      for(unsigned int t=0; t<__emtfTracks->product()->size(); t++){
          const L1TMuonEndCap::EMTFTrack      &trk  = __emtfTracks->product()->operator[](t);
          const L1TMuonEndCap::EMTFTrackExtra &trkE = __emtfTracksExtra->product()->operator[](t);

          EMTFTrackPointer track(new EMTFTrack());
/*
          track->set_endcap        (trk.endcap);
          track->set_sector        (trk.sector);
          track->set_mode          (trk.mode);
          track->set_bx            (trk.bx);
          track->set_quality       (trk.gmt_quality);
          track->set_pt            (trk.Pt());
          track->set_pt_GMT        (trk.Pt_GMT());
          track->set_pt_LUT_addr   (trk.Pt_LUT_addr());
          track->set_eta_GMT       (trk.gmt_eta);
          track->set_phi_GMT       (trk.gmt_phi);
          track->set_charge_GMT    (trk.gmt_charge);
          track->set_charge_valid  (trk.gmt_charge_valid);
*/
          track->set_endcap        (trk.Endcap());
          track->set_sector        (trk.Sector());
          track->set_sector_GMT    (trk.Sector_GMT());
          track->set_sector_index  (trk.Sector_index());
          track->set_mode          (trk.Mode());
          track->set_mode_LUT      (trk.Mode_LUT());
          track->set_quality       (trk.Quality());
          track->set_bx            (trk.BX());
          track->set_pt            (trk.Pt());
          track->set_pt_GMT        (trk.Pt_GMT());
          track->set_pt_LUT_addr   (trk.Pt_LUT_addr());
          track->set_eta           (trk.Eta());
          track->set_eta_GMT       (trk.Eta_GMT());
          track->set_eta_LUT       (trk.Eta_LUT());
          track->set_phi_loc_int   (trk.Phi_loc_int());
          track->set_phi_loc_deg   (trk.Phi_loc_deg());
          track->set_phi_loc_rad   (trk.Phi_loc_rad());
          track->set_phi_GMT       (trk.Phi_GMT());
          track->set_phi_glob_deg  (trk.Phi_glob_deg());
          track->set_phi_glob_rad  (trk.Phi_glob_rad());
          track->set_charge        (trk.Charge());
          track->set_charge_GMT    (trk.Charge_GMT());
          track->set_charge_valid  (trk.Charge_valid());
          track->set_dPhi_12       (trk.DPhi_12());
          track->set_dPhi_13       (trk.DPhi_13());
          track->set_dPhi_14       (trk.DPhi_14());
          track->set_dPhi_23       (trk.DPhi_23());
          track->set_dPhi_24       (trk.DPhi_24());
          track->set_dPhi_34       (trk.DPhi_34());
          track->set_dTheta_12     (trk.DTheta_12());
          track->set_dTheta_13     (trk.DTheta_13());
          track->set_dTheta_14     (trk.DTheta_14());
          track->set_dTheta_23     (trk.DTheta_23());
          track->set_dTheta_24     (trk.DTheta_24());
          track->set_dTheta_34     (trk.DTheta_34());
          track->set_clct_1        (trk.CLCT_1());
          track->set_clct_2        (trk.CLCT_2());
          track->set_clct_3        (trk.CLCT_3());
          track->set_clct_4        (trk.CLCT_4());
          track->set_fr_1          (trk.FR_1());
          track->set_fr_2          (trk.FR_2());
          track->set_fr_3          (trk.FR_3());
          track->set_fr_4          (trk.FR_4());
          track->set_track_num     (trk.Track_num());
          track->set_has_neighbor  (trk.Has_neighbor());
          track->set_all_neighbor  (trk.All_neighbor());

          L1TMuonEndCap::EMTFHitCollection hits = const_cast<L1TMuonEndCap::EMTFTrack&>(trk).Hits();
          L1TMuonEndCap::EMTFHitExtra stub[5];
          for(unsigned int h=0; h<hits.size(); h++){
              if( hits[h].Station()<1 || hits[h].Station()>4 ){
                  cerr << "Invalid station: " << hits[h].Station() << endl;
                  continue;
              }
              stub[ hits[h].Station() ] = trkE.xhits[h];
          }

          track->set_dTheta_12( (stub[1].station==1 && stub[2].station==2 ? stub[1].theta_fp - stub[2].theta_fp : -99) );
          track->set_dTheta_13( (stub[1].station==1 && stub[3].station==3 ? stub[1].theta_fp - stub[3].theta_fp : -99) );
          track->set_dTheta_14( (stub[1].station==1 && stub[4].station==4 ? stub[1].theta_fp - stub[4].theta_fp : -99) );
          track->set_dTheta_23( (stub[2].station==2 && stub[3].station==3 ? stub[2].theta_fp - stub[3].theta_fp : -99) );
          track->set_dTheta_24( (stub[2].station==2 && stub[4].station==4 ? stub[2].theta_fp - stub[4].theta_fp : -99) );
          track->set_dTheta_34( (stub[3].station==3 && stub[4].station==4 ? stub[3].theta_fp - stub[4].theta_fp : -99) );
 
          track->set_dPhi_12( (stub[1].station==1 && stub[2].station==2 ? stub[1].phi_fp - stub[2].phi_fp : -999) );
          track->set_dPhi_13( (stub[1].station==1 && stub[3].station==3 ? stub[1].phi_fp - stub[3].phi_fp : -999) );
          track->set_dPhi_14( (stub[1].station==1 && stub[4].station==4 ? stub[1].phi_fp - stub[4].phi_fp : -999) );
          track->set_dPhi_23( (stub[2].station==2 && stub[3].station==3 ? stub[2].phi_fp - stub[3].phi_fp : -999) );
          track->set_dPhi_24( (stub[2].station==2 && stub[4].station==4 ? stub[2].phi_fp - stub[4].phi_fp : -999) );
          track->set_dPhi_34( (stub[3].station==3 && stub[4].station==4 ? stub[3].phi_fp - stub[4].phi_fp : -999) );

/*
          if( stub[1].station==1 && stub[2].station==2 && trkE.ptlut_data.delta_ph[0] != stub[1].phi_fp - stub[2].phi_fp )
            cerr << "Inconsistent dPhi12: " << trk.DPhi_12() << " = " << trkE.ptlut_data.delta_ph[0] << " = " << stub[1].phi_fp << "-" << stub[2].phi_fp << endl;
          if( stub[2].station==2 && stub[3].station==3 && trkE.ptlut_data.delta_ph[3] != stub[2].phi_fp - stub[3].phi_fp )
            cerr << "Inconsistent dPhi23: " << trk.DPhi_23() << " = " << trkE.ptlut_data.delta_ph[3] << " = " << stub[2].phi_fp << "-" << stub[3].phi_fp << endl;
          if( stub[3].station==3 && stub[4].station==4 && trkE.ptlut_data.delta_ph[5] != stub[3].phi_fp - stub[4].phi_fp )
            cerr << "Inconsistent dPhi34: " << trk.DPhi_34() << " = " << trkE.ptlut_data.delta_ph[5] << " = " << stub[3].phi_fp << "-" << stub[4].phi_fp << endl;
*/
/*
          if( stub[1].station==1 && stub[2].station==2 && trkE.ptlut_data.delta_th[0] != stub[1].theta_fp - stub[2].theta_fp )
            cerr << "Inconsistent dTheta12: " << trk.DTheta_12() << " = " << trkE.ptlut_data.delta_th[0] << " = " << stub[1].theta_fp << "-" << stub[2].theta_fp << endl;
          if( stub[2].station==2 && stub[3].station==3 && trkE.ptlut_data.delta_th[3] != stub[2].theta_fp - stub[3].theta_fp )
            cerr << "Inconsistent dTheta23: " << trk.DTheta_23() << " = " << trkE.ptlut_data.delta_th[3] << " = " << stub[2].theta_fp << "-" << stub[3].theta_fp << endl;
          if( stub[3].station==3 && stub[4].station==4 && trkE.ptlut_data.delta_th[5] != stub[3].theta_fp - stub[4].theta_fp )
            cerr << "Inconsistent dTheta34: " << trk.DTheta_34() << " = " << trkE.ptlut_data.delta_th[5] << " = " << stub[3].theta_fp << "-" << stub[4].theta_fp << endl;
*/
          track->set_clct_1( ( stub[1].station==1 ? stub[1].pattern : -99 ) );
          track->set_clct_2( ( stub[2].station==2 ? stub[2].pattern : -99 ) );
          track->set_clct_3( ( stub[3].station==3 ? stub[3].pattern : -99 ) );
          track->set_clct_4( ( stub[4].station==4 ? stub[4].pattern : -99 ) );

          tracks.push_back(track);
      }

    sort   (tracks.begin(),tracks.end());
    reverse(tracks.begin(),tracks.end());

    event.put("emtfTracks",(const EMTFTrackCollection*)&tracks);

    return AppResult();
}
