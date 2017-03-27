using namespace std;
#include "EMTFReaderAWB2017.h"
#include "TTree.h"
#include "TBranch.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"

using namespace l1t;

edm::Wrapper<vector<EMTFTrack>> *__emtfTracks = new edm::Wrapper<vector<EMTFTrack>>();

AppResult EMTFReaderAWB2017::beginRun(AppEvent& event) {
    TTree *Events = 0;
    if( event.get("Events",Events) || !Events )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'Events' tree found");

    TBranch *inputEMTFTrackBranch = Events->GetBranch("l1tEMTFTracks_simEmtfDigis__reL1T.");
    if( !inputEMTFTrackBranch )
        return AppResult(AppResult::STOP|AppResult::ERROR,"No 'l1tEMTFTracks_simEmtfDigis__reL1T.' branch found");
    inputEMTFTrackBranch->SetAddress(&__emtfTracks);

    return AppResult();
}

AppResult EMTFReaderAWB2017::event(AppEvent& event) {
    tracks.clear();
    if( __emtfTracks->isPresent() )
      for(unsigned int t=0; t<__emtfTracks->product()->size(); t++){
          const EMTFTrack &trk = __emtfTracks->product()->operator[](t);

          EMTFTrackSep2016Pointer track(new EMTFTrackSep2016());

          track->set_endcap        (trk.Endcap());
          track->set_sector        (trk.Sector());
          track->set_sector_index  (trk.Sector_idx());
          track->set_mode          (trk.Mode());
          track->set_quality       (trk.GMT_quality());
          track->set_bx            (trk.BX());
          track->set_pt            (trk.Pt());
          track->set_pt_GMT        (trk.GMT_pt());

          track->set_theta         (trk.Theta());
          track->set_theta_int     (trk.Theta_fp());
          track->set_eta           (trk.Eta());

          track->set_eta_GMT       (trk.GMT_eta());
          track->set_phi_GMT       (trk.GMT_phi());

          track->set_phi_loc_int   (trk.Phi_fp());
          track->set_phi_loc_rad   (trk.Phi_loc());
          track->set_phi_glob_rad  (trk.Phi_glob());


          track->set_charge        (trk.Charge());
          track->set_charge_GMT    (trk.GMT_charge());
          track->set_charge_valid  (trk.GMT_charge_valid());

          track->set_track_num     (trk.Track_num());
          track->set_has_neighbor  (trk.Has_neighbor());
          track->set_all_neighbor  (trk.All_neighbor());

          EMTFHitCollection hits = trk.Hits();
          EMTFHit stub[5];
          for(unsigned int h=0; h<hits.size(); h++){
              if( hits[h].Station()<1 || hits[h].Station()>4 ){
                  cerr << "Invalid station: " << hits[h].Station() << endl;
                  continue;
              }
              stub[ hits[h].Station() ] = hits[h];
          }

          track->set_dTheta_12( (stub[1].Station()==1 && stub[2].Station()==2 ? -stub[1].Theta_fp() + stub[2].Theta_fp() : -999) );
          track->set_dTheta_13( (stub[1].Station()==1 && stub[3].Station()==3 ? -stub[1].Theta_fp() + stub[3].Theta_fp() : -999) );
          track->set_dTheta_14( (stub[1].Station()==1 && stub[4].Station()==4 ? -stub[1].Theta_fp() + stub[4].Theta_fp() : -999) );
          track->set_dTheta_23( (stub[2].Station()==2 && stub[3].Station()==3 ? -stub[2].Theta_fp() + stub[3].Theta_fp() : -999) );
          track->set_dTheta_24( (stub[2].Station()==2 && stub[4].Station()==4 ? -stub[2].Theta_fp() + stub[4].Theta_fp() : -999) );
          track->set_dTheta_34( (stub[3].Station()==3 && stub[4].Station()==4 ? -stub[3].Theta_fp() + stub[4].Theta_fp() : -999) );
 
          track->set_dPhi_12( (stub[1].Station()==1 && stub[2].Station()==2 ? -stub[1].Phi_fp() + stub[2].Phi_fp() : -999) );
          track->set_dPhi_13( (stub[1].Station()==1 && stub[3].Station()==3 ? -stub[1].Phi_fp() + stub[3].Phi_fp() : -999) );
          track->set_dPhi_14( (stub[1].Station()==1 && stub[4].Station()==4 ? -stub[1].Phi_fp() + stub[4].Phi_fp() : -999) );
          track->set_dPhi_23( (stub[2].Station()==2 && stub[3].Station()==3 ? -stub[2].Phi_fp() + stub[3].Phi_fp() : -999) );
          track->set_dPhi_24( (stub[2].Station()==2 && stub[4].Station()==4 ? -stub[2].Phi_fp() + stub[4].Phi_fp() : -999) );
          track->set_dPhi_34( (stub[3].Station()==3 && stub[4].Station()==4 ? -stub[3].Phi_fp() + stub[4].Phi_fp() : -999) );

/*
          if( stub[1].Station()==1 && stub[2].Station()==2 && trk.PtLUT().delta_ph[0] * (trk.PtLUT().sign_ph[0]?-1:+1) != stub[1].Phi_fp() - stub[2].Phi_fp() )
            cerr << "Inconsistent dPhi12: " << track->DPhi_12() << " = " << trk.PtLUT().delta_ph[0] * (trk.PtLUT().sign_ph[0]?-1:+1) << " = " << stub[1].Phi_fp() << "-" << stub[2].Phi_fp() << endl;
          if( stub[2].Station()==2 && stub[3].Station()==3 && trk.PtLUT().delta_ph[3] * (trk.PtLUT().sign_ph[3]?-1:+1) != stub[2].Phi_fp() - stub[3].Phi_fp() )
            cerr << "Inconsistent dPhi23: " << track->DPhi_23() << " = " << trk.PtLUT().delta_ph[3] * (trk.PtLUT().sign_ph[3]?-1:+1) << " = " << stub[2].Phi_fp() << "-" << stub[3].Phi_fp() << endl;
          if( stub[3].Station()==3 && stub[4].Station()==4 && trk.PtLUT().delta_ph[5] * (trk.PtLUT().sign_ph[5]?-1:+1) != stub[3].Phi_fp() - stub[4].Phi_fp() )
            cerr << "Inconsistent dPhi34: " << track->DPhi_34() << " = " << trk.PtLUT().delta_ph[5] * (trk.PtLUT().sign_ph[5]?-1:+1) << " = " << stub[3].Phi_fp() << "-" << stub[4].Phi_fp() << endl;

          if( stub[1].Station()==1 && stub[2].Station()==2 && trk.PtLUT().delta_th[0] * (trk.PtLUT().sign_th[0]?+1:-1) != stub[1].Theta_fp() - stub[2].Theta_fp() )
            cerr << "Inconsistent dTheta12: " << track->DTheta_12() << " = " << trk.PtLUT().delta_th[0] * (trk.PtLUT().sign_th[0]?+1:-1) << " = " << stub[1].Theta_fp() << "-" << stub[2].Theta_fp() << endl;
          if( stub[2].Station()==2 && stub[3].Station()==3 && trk.PtLUT().delta_th[3] * (trk.PtLUT().sign_th[3]?+1:-1) != stub[2].Theta_fp() - stub[3].Theta_fp() )
            cerr << "Inconsistent dTheta23: " << track->DTheta_23() << " = " << trk.PtLUT().delta_th[3] * (trk.PtLUT().sign_th[3]?+1:-1) << " = " << stub[2].Theta_fp() << "-" << stub[3].Theta_fp() << endl;
          if( stub[3].Station()==3 && stub[4].Station()==4 && trk.PtLUT().delta_th[5] * (trk.PtLUT().sign_th[5]?+1:-1) != stub[3].Theta_fp() - stub[4].Theta_fp() )
            cerr << "Inconsistent dTheta34: " << track->DTheta_34() << " = " << trk.PtLUT().delta_th[5] * (trk.PtLUT().sign_th[5]?+1:-1) << " = " << stub[3].Theta_fp() << "-" << stub[4].Theta_fp() << endl;
*/
          track->set_clct_1( ( stub[1].Station()==1 ? stub[1].Pattern() : -99 ) );
          track->set_clct_2( ( stub[2].Station()==2 ? stub[2].Pattern() : -99 ) );
          track->set_clct_3( ( stub[3].Station()==3 ? stub[3].Pattern() : -99 ) );
          track->set_clct_4( ( stub[4].Station()==4 ? stub[4].Pattern() : -99 ) );

          track->set_fr_1( trk.PtLUT().fr[0] );
          track->set_fr_2( trk.PtLUT().fr[1] );
          track->set_fr_3( trk.PtLUT().fr[2] );
          track->set_fr_4( trk.PtLUT().fr[3] );

          track->set_ring_station1( ( stub[1].Station()==1 ? stub[1].Ring() : -999 ) );
          track->set_ring_station2( ( stub[2].Station()==2 ? stub[2].Ring() : -999 ) );
          track->set_ring_station3( ( stub[3].Station()==3 ? stub[3].Ring() : -999 ) );
          track->set_ring_station4( ( stub[4].Station()==4 ? stub[4].Ring() : -999 ) );

          track->set_rpc_station1( ( stub[1].Station()==1 ? stub[1].Is_RPC() : -99 ) );
          track->set_rpc_station2( ( stub[2].Station()==2 ? stub[2].Is_RPC() : -99 ) );
          track->set_rpc_station3( ( stub[3].Station()==3 ? stub[3].Is_RPC() : -99 ) );
          track->set_rpc_station4( ( stub[4].Station()==4 ? stub[4].Is_RPC() : -99 ) );

          tracks.push_back(track);
      }

    sort   (tracks.begin(),tracks.end());
    reverse(tracks.begin(),tracks.end());

    event.put("emtfTracks",(const EMTFTrackSep2016Collection*)&tracks);

    return AppResult();
}
