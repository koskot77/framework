using namespace std;
#include "InputModule.h"
#include <sstream>
#include <string.h>
#include <dirent.h>
#include <fnmatch.h>
#include <libgen.h>

void InputModule::setParameter(const char* parname, const char* value){
    string val(value);
    size_t recursion = val.find("$path");
    if( recursion == string::npos )
        AppModule::setParameter(parname,value);
    else {
        string origVal = AppModule::getParameter(parname);
        string newVal  = val.substr(0,recursion) + origVal + val.substr(recursion+5,string::npos);
        AppModule::setParameter(parname,newVal.c_str());
    }
}

#include<TSystem.h>
#include<TROOT.h>
AppResult InputModule::beginJob(AppEvent& event){
    stringstream returnMessage;
    inputFiles.clear();
    currentFile = 0;

    if( path.length() ){
        size_t begin = 0, end = string::npos;
        while( true ){
            end = ((string&)path).find(":",begin);
            string fileName = ((string&)path).substr(begin, (end!=string::npos ? end-begin : end) );

            char *path1 = strdup(fileName.c_str());
            char *path2 = strdup(fileName.c_str());
            const char *dir  = dirname (path1);
            const char *file = basename(path2);
            DIR *dp;
            if( (dp = opendir(dir)) == NULL ){
                returnMessage<<"Error while opening directory "<<dir<<endl;
                return AppResult(AppResult::STOP|AppResult::ERROR, returnMessage.str());
            }
            struct dirent *dirp;
            while( (dirp = readdir(dp)) != NULL ){
                if( fnmatch(file, dirp->d_name, 0) ) continue;
                inputFiles.push_back(string(dir)+"/"+dirp->d_name);
            }
            closedir(dp);
            free(path1);
            free(path2);

            if( end == string::npos ) break;
            else begin = end+1;
        }
    } else {
        vector<string> path;
        if( event.get("path", path) ) return AppResult(AppResult::STOP|AppResult::ERROR,"Specify 'path' parameter explicitly or add the 'SampleHelper'");
        inputFiles = path;
    }

    returnMessage<<"Total "<<inputFiles.size()<<" files to process"<<endl;

    if( inputFiles.size()==0 ) return AppResult(AppResult::STOP|AppResult::LOG,"Empty input, nothing to do");

    gSystem->Load("libFWCoreFWLite.so");
    gROOT->ProcessLine("FWLiteEnabler::enable()");
    gSystem->Load("libDataFormatsFWLite.so");
    gSystem->Load("libDataFormatsPatCandidates.so");

    chain = new TChain("Events");
    for(vector<string>::const_iterator file = inputFiles.begin(); file != inputFiles.end(); file++)
        chain->AddFile( file->c_str() );

    event.put("Events",(TTree*)chain);
    chainEntryNumber = firstEntry;

    return AppResult(AppResult::OK|AppResult::LOG, returnMessage.str());
}

AppResult InputModule::beginRun(AppEvent& event){
    if( chain->GetEntry(chainEntryNumber) ){
        return AppResult();
    } else 
        return AppResult(AppResult::STOP|AppResult::LOG, "Cannot move on to the next file");
}

#include<TFile.h>
AppResult InputModule::event(AppEvent& event){

    if( !chain->GetEntry(chainEntryNumber) ){
        // no more events left
        AppEvent tmpEvent;
        if( endRunNotify(tmpEvent) )
            return AppResult(AppResult::STOP|AppResult::LOG, "Cannot finalize the job");
        // exiting
        return AppResult(AppResult::STOP|AppResult::LOG, "End of stream");
    }

    const char *path = chain->GetFile()->GetName();

    if( currentFile != path ){
        psets = (TChain*)chain->GetFile()->Get("ParameterSets");

        // first run? if not, finish the previous
        if( path ){
            AppEvent tmpEvent;
            if( endRunNotify(tmpEvent) )
                return AppResult(AppResult::STOP|AppResult::LOG, "Cannot move on to the next file");
        }

        AppEvent initEvent;
        initEvent.put("Events",(TTree*)chain);
        initEvent.put("PSets", (TTree*)psets);
        if( beginRunNotify(initEvent) )
            return AppResult(AppResult::STOP|AppResult::LOG, "Cannot move on to the next file");

        currentFile = path;

        if( !chain->GetEntry(chainEntryNumber) )
            return AppResult(AppResult::STOP|AppResult::ERROR, "Cannot reread the event");
    }

    if( (chainEntryNumber % showProgressPeriod) == 0 ){
        const char *file = rindex(path,'/');
        clog<<"Processed "<<chainEntryNumber<<" entries ("<<(file!=NULL?file+1:path)<<")"<<endl;
    }

    event.put("chainEntryNumber", chainEntryNumber);
    event.put("inputFileName",    path);
    chainEntryNumber++;

    return AppResult();
}
