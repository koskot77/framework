using namespace std;
#include "SampleHelper.h"
#include <vector>
#include <sstream>
#include <dirent.h>
#include <fnmatch.h>
#include <libgen.h>

SampleHelper::SampleHelper(const char* name, const char *descr):AppAgentWrapper(name,descr),AppAgent< vector<string> >(),AppAgent<string>(),
         sample(*this,"sample", ""), sampleSection(*this,"sampleSection", "1of1"){

     allFiles["SingleMuA"].push_back("/mnt/hadoop/se/store/user/abrinke1/SingleMu/SingleMu_Run2012A-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/de3c20242948dd0d39f546037f2fbda9/ttH_pat2bean_53x_*_*_*.root");
     allFiles["SingleMuB"].push_back("/mnt/hadoop/se/store/user/abrinke1/SingleMu/SingleMu_Run2012B-13Jul2012-v1_BEAN_53xOn53x_V02_CV01/36fb01f1aafbede72fa3cecfbe30213a/ttH_pat2bean_53x_*_*_*.root");
     allFiles["SingleMuC"].push_back("/mnt/hadoop/se/store/user/abrinke1/SingleMu/SingleMu_Run2012C-PromptReco-v2_BEAN_53xOn53x_V02_CV01/5966f50c1614e99fadd7a4442d7fe207/ttH_pat2bean_53x_*_*_*.root");
     allFiles["SingleMuD"].push_back("/mnt/hadoop/se/store/user/abrinke1/SingleMu/SingleMu_Run2012D-PromptReco-v1_BEAN_53xOn53x_V02_CV01/4c32d30ed3f823639d81416a264f1466/ttH_pat2bean_53x_*_*_*.root");

     allFiles["META"].push_back("/store/user/biliu/BN_MET_Run2012A-22Jan2013-v1_AOD_0/*_AOD_*_*_*.root");
     allFiles["METB"].push_back("/store/user/biliu/BN_MET_Run2012B-22Jan2013-v1_AOD_0/*_AOD_*_*_*.root");
     allFiles["METC"].push_back("/store/user/jbrinson/MET/BEAN2012-v4/66b7c0b77dff84935489342550a9cb3e/*_AOD_*_*_*.root");
     allFiles["METD"].push_back("/mnt/hadoop/se/store/user/jbrinson/BN_METParked_Run2012D-22Jan2013-v1_AOD_0/*_AOD_*_*_*.root");

     allFiles["T"].push_back("/store/user/ahart/BN_T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_*_*_*.root");
     allFiles["T"].push_back("/store/user/ahart/BN_Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_*_*_*.root");
     allFiles["T"].push_back("/store/user/ahart/BN_T_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_*_*_*.root");
     allFiles["T"].push_back("/store/user/ahart/BN_Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_*_*_*.root");
     allFiles["T"].push_back("/store/user/ahart/BN_T_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_*_*_*.root");
     allFiles["T"].push_back("/store/user/ahart/BN_Tbar_s-channel_TuneZ2star_8TeV-powheg-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_0/*_AODSIM_*_*_*.root");

     allFiles["TT"].push_back("/store/user/wulsin/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/BEANs-v4/0ff8045eb3a4a7ce9562dd332df0072c/ttH_pat2bean_53x_*.root");

     allFiles["S3m600"].push_back("/tmp/kkotov/MonotopToHad_S3_MSM-600_Tune4C_13TeV-madgraph-tauola.root");
     allFiles["S3m300"].push_back("/tmp/kkotov/MonotopToHad_S3_MSM-300_Tune4C_13TeV-madgraph-tauola.root");
     allFiles["S4m1200"].push_back("/tmp/kkotov/MonotopToHad_S4_MVM-1200_Tune4C_13TeV-madgraph-tauola.root");
}

AppResult SampleHelper::beginJob(void){
    stringstream returnMessage;

    string str = (string&)sampleSection; 
    size_t of = str.find("of");
    if( of == string::npos ){
        returnMessage<<"Invalid 'sampleSection' format, check 'of' exists in "<<str;
        return AppResult(AppResult::STOP|AppResult::ERROR, returnMessage.str());
    }

    int part  = atoi(str.substr(0,of).c_str());
    int total = atoi(str.substr(of+2).c_str());

    if( total <= 0 ){
        returnMessage<<"Invalid 'sampleSection' format, 'total' == "<<total;
        return AppResult(AppResult::STOP|AppResult::ERROR, returnMessage.str());
    }
    if( part  <= 0 || part  > total ){
        returnMessage<<"Invalid 'sampleSection' format, 'part' == "<<part;
        return AppResult(AppResult::STOP|AppResult::ERROR, returnMessage.str());
    }

    map<string,list<string> >::const_iterator s = allFiles.find((string&)sample);
    if( s == allFiles.end() ) return AppResult(AppResult::STOP|AppResult::ERROR, "No sample found");

    vector<string> sampleFiles;

    for(list<string>::const_iterator files = s->second.begin(); files != s->second.end(); files++){
        char *path1 = strdup(files->c_str());
        char *path2 = strdup(files->c_str());
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
            sampleFiles.push_back(string(dir)+"/"+dirp->d_name);
        }
        closedir(dp);
        free(path1);
        free(path2);
    }

    int numberOfFilesPerSection = int(sampleFiles.size()/float(total) + 0.5);

    filesToProcess.clear();
    for(int i = (part-1) * numberOfFilesPerSection; i < part * numberOfFilesPerSection && i<int(sampleFiles.size()); i++)
        filesToProcess.push_back(sampleFiles[i]);

    return AppResult();
}

bool SampleHelper::fetch(const char *name, vector<string> &c){
    if( strcmp(name,"path") == 0 ){
        c = filesToProcess;
        return false;
    }
    return true;
}

bool SampleHelper::fetch(const char *name, string &c){
    if( strcmp(name,"identifier") == 0 ) {
        map<string,list<string> >::const_iterator s = allFiles.find((string&)sample);
        if( s == allFiles.end() ) return true;
        c = string(s->first) + string("_") + string(sampleSection);
        return false;
    }
    return true;
}
