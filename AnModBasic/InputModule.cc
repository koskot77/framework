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

AppResult InputModule::beginJob(AppEvent& event){
    stringstream returnMessage;

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

    returnMessage<<"Total "<<inputFiles.size()<<" files to process"<<endl;
    return AppResult(AppResult::OK|AppResult::LOG, returnMessage.str());
}
