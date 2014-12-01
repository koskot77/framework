using namespace std;
#include "OutputModule.h"
#include "AnUtils/Value_t.h"

#include <list>
#include <sstream>
#include <algorithm>

AppResult OutputModule::beginJob(AppEvent& event){
// open output file
    // root
    if( ((string&)output).find(".root") != string::npos ){
        outputFile = TFile::Open(output.c_str(),"RECREATE");
        microTuple = new TTree("micro","micro");
    } else // csv
    if( ((string&)output).find(".csv") != string::npos ){
        outputFile = 0;
        microTuple = 0;
        if( !output.length() ) return AppResult();
        csvfile.open( output.c_str() ); //, ios::app );
    } // both
    else {
        outputFile = TFile::Open(output.c_str(),"RECREATE");
        microTuple = new TTree("micro","micro");
        if( !output.length() ) return AppResult();
        csvfile.open( output.c_str() ); //, ios::app );
    }

    colnames.clear();

    // parse the leaflist
    char *argument = strdup( leaflist.c_str() ), *token = argument;
    while(1){
        if( (token = strrchr(argument,':')) ) *token++ = '\0'; else token = argument;
        if( !strlen(token) ) continue;
        char *type = "D";
        if( (type = strrchr(token,'/')) ) *type++ = '\0';
        colnames.push_back((string)token);
        coltypes.push_back(*type);
        if( token == argument ) break;
    }
    free(argument);
    reverse(colnames.begin(),colnames.end());
    reverse(coltypes.begin(),coltypes.end());

    // initilize the output
    if( csvfile.is_open() ){
        for(unsigned int leaf=0; leaf<colnames.size(); leaf++){
            size_t bra = colnames[leaf].find('['), ket = colnames[leaf].find(']'), nElements = 0;
            // an array?
            if( bra != string::npos && ket != string::npos && ket > bra )
                nElements = atoi( colnames[leaf].substr(bra,ket-bra).c_str() );

            if( nElements > 0 ){
                stringstream elements;
                for(size_t i=0; i<nElements; i++){
                    elements<<colnames[leaf].substr(bra)<<"["; 
                    elements<<nElements<<"]";
                    if( leaf+1 != colnames.size() ) elements<<",";
                    else elements<<endl;
                }
                csvfile<<elements;
                colarray.push_back(nElements);
            } else {
                csvfile<<colnames[leaf];
                colarray.push_back(1);
                if( leaf+1 != colnames.size() ) csvfile<<",";
                else csvfile<<endl;
            }
        }
    }
    if( microTuple ){
        for(unsigned int leaf=0; leaf<colnames.size(); leaf++){
            microTuple->Branch( colnames[leaf].c_str(), (void*)0, colnames[leaf].c_str() ); 
        }
    }

    return AppResult();
}

AppResult OutputModule::endJob  (AppEvent& event){
//   if( microTuple ) delete microTuple;
//   if( outputFile ) delete outputFile;
    if( csvfile.is_open() ) csvfile.close();
    return AppResult();
}

AppResult OutputModule::beginRun(AppEvent& event){
    return AppResult();
}

AppResult OutputModule::endRun(AppEvent& event){
    return AppResult();
}

AppResult OutputModule::event(AppEvent& event){

    list<Value_t> eventContent;

    for(unsigned int leaf=0; leaf<colnames.size(); leaf++){
        switch( coltypes[leaf] ){
            case 'I' : {
                const int *val = 0;
                if( event.get(colnames[leaf].c_str(),val) ){
                    string errorMsg = "No ";
                    errorMsg.append(colnames[leaf].c_str());
                    errorMsg.append("/I found");
                    return AppResult(AppResult::STOP|AppResult::ERROR,errorMsg);
                } else {
                    for(size_t i=0; i<colarray[leaf]; i++)
                        eventContent.push_back( Value_t(val[i]) );
                }
            } break;
            case 'D' : {
                const double *val = 0;
                if( event.get(colnames[leaf].c_str(),val) ){
                    string errorMsg = "No ";
                    errorMsg.append(colnames[leaf].c_str());
                    errorMsg.append("/I found");
                    return AppResult(AppResult::STOP|AppResult::ERROR,errorMsg);
                } else {
                    for(size_t i=0; i<colarray[leaf]; i++)
                        eventContent.push_back( Value_t(val[i]) );
                }
            } break;
            default : break;
        }
    }

    size_t n = eventContent.size();
    for(list<Value_t>::const_iterator item = eventContent.begin(); item != eventContent.end(); item++){
        csvfile<<*item;
        if( --n ) csvfile<<",";
        else      csvfile<<endl;
    }

    if( microTuple ) microTuple->Fill();

    return AppResult();
}
