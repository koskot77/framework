using namespace std;
#include <unordered_map>
#include "MCParticles.h"
#include "LundCodesToNames.h"

AppResult MCParticles::event(AppEvent &event){
/*
	if( !event.get("McRecord",kedrmc) && kedrmc ){

		hash_map< long, pair<long,long> > decay_sequence; // диапазон second индексов потомков частицы с индексом first
		long ntrk=0;

		if( kedrmc->Header.Dir[2][1] && createMCHistory ){
				Particle &part = part_pool[trk];
				part.setType(type);
				part.setName( LundCodesToNames::convert(type) );
				part.setPxPyPz(px,py,pz);
				part.setE(e);      // particle ehergy together with the previous call sets its mass
				part.setEnergy(e); // energy deposits in calorimeter
				part.setXYZ(x,y,z);
				part.mother_ = ( orig >= 0 ? &part_pool[orig] : 0 );
				part.daughters_.clear();
			}

		} else // <- флаг createMCHistory имеет приоритет над createMCPrimary

		if( kedrmc->Header.Dir[1][1] && createMCPrimary ) {
			long iptr = kedrmc->Header.Dir[1][0]-1;
				 ntrk = kedrmc->Long[ iptr++ ];
			if( ntrk > (long)part_pool.size() ) expand(ntrk); // expanding
				Particle &part = part_pool[trk];
				part.setType(type);
				part.setName( LundCodesToNames::convert(type) );
				part.setPxPyPz(px,py,pz);
				part.setE(e);      // энергия частицы, а вместе с предыдущим вызовом и ее масса
				part.setEnergy(e); // энерговыделение в калориметре
				part.setXYZ(x,y,z);
				part.mother_ = ( orig >= 0 ? &part_pool[orig] : 0 );
				part.daughters_.clear();
			}
		}

		hash_map< long, pair<long,long> >::const_iterator iter = decay_sequence.begin();
		while( iter != decay_sequence.end() ){
			if( iter->second.second - iter->second.first >= ntrk - iter->first ){ cout<<"Too many daughters"<<endl; break; }
			for(int daughter=iter->second.first; daughter<=iter->second.second; daughter++)
				part_pool[ iter->first ].daughters_.push_back( &part_pool[daughter] );
			iter++;
		}

		particles.resize(ntrk);
		for(int trk=0; trk<ntrk; trk++) particles[trk] = part_pool[trk];

		if( placeParticlesAsReal ){
			if( event.put("Particles",  (const vector<Particle>*) &particles ) ) return AppResult( AppResult::STOP|AppResult::ERROR, "Can't put Particles" );
		} else
			if( event.put("McParticles",(const vector<Particle>*) &particles ) ) return AppResult( AppResult::STOP|AppResult::ERROR, "Can't put McParticles" );
	}
*/
	return AppResult();
}
