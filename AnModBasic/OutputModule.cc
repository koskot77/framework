using namespace std;
#include "OutputModule.h"
#include "AnUtils/Value_t.h"
#include "TFile.h"
#include <list>
#include <sstream>
#include <algorithm>

AppResult OutputModule::beginJob(AppEvent& event){
// open output file
    if( output.length()==0 ){
        if( event.get("identifier", output) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Specify 'output' parameter explicitly or add the 'SampleHelper'");
    }
    // root
    if( ((string&)output).find(".root") != string::npos ){
        //outputFile = TFile::Open(output.c_str(),"RECREATE");
        microTuple = new TTree("micro","micro");
    } else // csv
    if( ((string&)output).find(".csv") != string::npos ){
        //outputFile = 0;
        microTuple = 0;
        if( !output.length() ) return AppResult();
        csvfile.open( output.c_str() ); //, ios::app );
    } // both
    else {
        //outputFile = TFile::Open(output.c_str(),"RECREATE");
        microTuple = new TTree("micro","micro");
        if( !output.length() ) return AppResult();
        csvfile.open( (((string&)output)+".csv").c_str() ); //, ios::app );
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
    for(unsigned int leaf=0; leaf<colnames.size(); leaf++){
        size_t bra = colnames[leaf].find('['), ket = colnames[leaf].find(']'), nElements = 0;
        // an array?
        if( bra != string::npos && ket != string::npos && ket > bra )
            nElements = atoi( colnames[leaf].substr(bra+1,ket-bra-1).c_str() );

        if( nElements > 0 ){
            if( csvfile.is_open() ){
                stringstream elements;
                for(size_t i=0; i<nElements; i++){
                    elements<<colnames[leaf].substr(0,bra)<<"["; 
                    elements<<i<<"]";
                    if( leaf+1 != colnames.size() || i+1 != nElements ) elements<<",";
                    else elements<<endl;
                }
                csvfile<<elements.str();
            }
            colarray.push_back(nElements);
        } else {
            csvfile<<colnames[leaf];
            colarray.push_back(1);
            if( csvfile.is_open() ){
                if( leaf+1 != colnames.size() ) csvfile<<",";
                else csvfile<<endl;
            }
        }
    }

    if( microTuple ){
        for(unsigned int leaf=0; leaf<colnames.size(); leaf++){
            string branchname = colnames[leaf];
            size_t bra = colnames[leaf].find('['), ket = colnames[leaf].find(']');
            if( bra != string::npos && ket != string::npos && ket > bra )
                branchname = colnames[leaf].substr(0,bra);

            microTuple->Branch( branchname.c_str(), (void*)0, (colnames[leaf]+"/"+coltypes[leaf]).c_str() ); 
        }
    }

    return AppResult();
}

AppResult OutputModule::endJob  (AppEvent& event){
    if( csvfile.is_open() ) csvfile.close();
    if( microTuple ){
        if( ((string&)output).find(".root") == string::npos )
            ((string&)output).append(".root");
        TFile *outputFile = TFile::Open(output.c_str(),"RECREATE");
        microTuple->Write();
        outputFile->Close();
    }
//gROOT->GetListOfFiles()->Last()
//   if( microTuple ) delete microTuple;
//   if( outputFile ) delete outputFile;
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
                if( event.get(colnames[leaf].c_str(),val) || !val ){
                    string errorMsg = "No ";
                    errorMsg.append(colnames[leaf].c_str());
                    errorMsg.append("/I found");
                    return AppResult(AppResult::STOP|AppResult::ERROR,errorMsg);
                } else {
                        if( colarray[leaf] == 1 ){
                            if( microTuple ) microTuple->SetBranchAddress(colnames[leaf].c_str(),const_cast<int*>(val));
                            eventContent.push_back( Value_t( colnames[leaf], val[0] ) );
                        } else {
                            string branchname = colnames[leaf].substr(0,colnames[leaf].find('['));
                            if( microTuple ) microTuple->SetBranchAddress(branchname.c_str(),const_cast<int*>(val));
                            char buff[ colarray[leaf] ];
                            for(size_t i=0; i<colarray[leaf]; i++){
                                sprintf(buff,"%s[%ld]",branchname.c_str(),i);
                                eventContent.push_back( Value_t( buff, val[i] ) );
                            }
                        }
                }
            } break;
            case 'D' : {
                const double *val = 0;
                if( event.get(colnames[leaf].c_str(),val) || !val ){
                    string errorMsg = "No ";
                    errorMsg.append(colnames[leaf].c_str());
                    errorMsg.append("/D found");
                    return AppResult(AppResult::STOP|AppResult::ERROR,errorMsg);
                } else {
                        if( colarray[leaf] == 1 ){
                            if( microTuple ) microTuple->SetBranchAddress(colnames[leaf].c_str(),const_cast<double*>(val));
                            eventContent.push_back( Value_t( colnames[leaf], val[0] ) );
                        } else {
                            string branchname = colnames[leaf].substr(0,colnames[leaf].find('['));
                            if( microTuple ) microTuple->SetBranchAddress(branchname.c_str(),const_cast<double*>(val));
                            char buff[ colarray[leaf] ];
                            for(size_t i=0; i<colarray[leaf]; i++){
                                sprintf(buff,"%s[%ld]",branchname.c_str(),i);
                                eventContent.push_back( Value_t( buff, val[i] ) );
                            }
                        }
                }
            } break;
            default : break;
        }
    }

    CompiledFormula &f = filter;

    if( strlen(f.formula()) )
        for(list<Value_t>::const_iterator item = eventContent.begin(); item != eventContent.end(); item++){
            f[ item->getName() ] = *item;
        }

    if( strlen(f.formula())==0 || f.eval() == true ){
        size_t n = eventContent.size();
        for(list<Value_t>::const_iterator item = eventContent.begin(); item != eventContent.end(); item++){
            csvfile<<*item;
            if( --n ) csvfile<<",";
            else      csvfile<<endl;
        }
        if( microTuple ) microTuple->Fill();
    }

    return AppResult();
}
