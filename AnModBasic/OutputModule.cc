using namespace std;
#include "OutputModule.h"
#include <string.h>

AppResult OutputModule::beginRun(AppEvent& event){
	// Check output state
	if( file.is_open() ) return AppResult( AppResult::STOP|AppResult::ERROR, "File is opened!" );
	if( bzfile )         return AppResult( AppResult::STOP|AppResult::ERROR, "File is opened!" );

	string filename = output;
	// Do nothing if output is empty
	if( !filename.length() ) return AppResult();

	// Open output file for append or write
	if( filename.rfind("bz") == string::npos )
		if( append ) file.open( filename.c_str(), ios::app );
		else         file.open( filename.c_str() );
	else
		if( append ) bzfile = BZ2_bzopen(filename.c_str(),"a");
		else         bzfile = BZ2_bzopen(filename.c_str(),"w");

	// Check output state
	if( !file.is_open() && !bzfile ) return AppResult( AppResult::STOP|AppResult::ERROR, "File is not opened!" );

	string retval; //int length; short id, tmp1 = 0, tmp2 = 0;
/*
		const short *csiped = 0;
		if( !event.get("CsiPed",csiped) ) {
			length = 5+1536; // heder length + data length in shorts ( words )
			id     = 321;    // csiped identificator
			if( !bzfile ){
				file.write( &length, sizeof(length) );
				file.write( &id,     sizeof(id)     );
				file.write( &tmp1,   sizeof(tmp1)   );
				file.write( &tmp2,   sizeof(tmp2)   );
				file.write( csiped,  sizeof(short)*1536 );
			} else {
				BZ2_bzwrite( bzfile, &length, sizeof(length) );
				BZ2_bzwrite( bzfile, &id,     sizeof(id)     );
				BZ2_bzwrite( bzfile, &tmp1,   sizeof(tmp1)   );
				BZ2_bzwrite( bzfile, &tmp2,   sizeof(tmp2)   );
				BZ2_bzwrite( bzfile, const_cast<short*>(csiped), sizeof(short)*1536 );
				//if( BZ2_bzerror() ) AppResult( AppResult::STOP|AppResult::ERROR, "bzerror" );
			}
		} else retval += " No CsiPed ";
*/
	if( retval.length() ) return AppResult( AppResult::OK|AppResult::LOG, retval );
	else return AppResult();
}

AppResult OutputModule::endRun(AppEvent& event){
	// Do nothing if output is empty
	if( !((string)output).length() ) return AppResult();

	// Check output state
	if( !file.is_open() && !bzfile ) return AppResult( AppResult::STOP|AppResult::ERROR, "File is not opened!" );

	// Write EndOfRun record if it is necessary
	string retval;

	// Closing file
	if( !bzfile ) file.close(); else { BZ2_bzclose(bzfile); bzfile = 0; }

	if( retval.length() ) return AppResult();
	return AppResult( AppResult::LOG, retval );
}

AppResult OutputModule::event(AppEvent& event){
	// Do nothing if output is empty
	if( !((string)output).length() ) return AppResult();

	// Check output state
	if( !file.is_open() && !bzfile ) return AppResult( AppResult::STOP|AppResult::ERROR, "File is not opened!" );

	return AppResult();
}
