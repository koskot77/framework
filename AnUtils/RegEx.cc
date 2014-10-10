using namespace std;
#include "RegEx.h"
#include <string.h>

map< string, vector<string> > RegExSearch(const char *pattern, const char *target){
//  Example:
//	map< string, vector<string> > result = RegExSearch("(\\w+) (\\w+)", "This is a test");
//  result.begin()->first == "This is";
//  result.begin()->second[0] = "This";
//  result.begin()->second[1] = "is";
//  (--result.end())->first == "a test";
//  (--result.end())->second[0] = "a";
//  (--result.end())->second[1] = "test";

	map< string, vector<string> > retval;
	regex_t regsys;
	regsys.buffer    = 0;
	regsys.allocated = 0;
	regsys.fastmap   = 0;
	regsys.translate = 0;
	regsys.no_sub    = 0;
	regsys.used      = 0;
	re_syntax_options = RE_SYNTAX_AWK;
	re_syntax_options ^= RE_NO_GNU_OPS;

	if(re_compile_pattern(pattern, strlen(pattern), &regsys)){
		cout<<"Can't compile the pattern: "<<pattern<<endl;
		return retval;
	}
	re_registers regs; regs.num_regs = 0;

	while(1){
		if(re_search(&regsys, target, strlen(target), 0, strlen(target), &regs)<0){
			regfree(&regsys); return retval;
		}
#ifdef __DEBUG__
		cout<<regs.num_regs<<" matches found"<<endl;
#endif
		if(regs.num_regs){
#ifdef __DEBUG__
			cout<<"First match range: "<<*regs.start<<" - "<<*regs.end<<endl;
#endif
			char match[*regs.end - *regs.start + 1];
			strncpy(match, &target[*regs.start], *regs.end - *regs.start);
			match[*regs.end - *regs.start] = '\0';
			retval[match] = vector<string>(0);
			vector<string> &submatches = retval.find(match)->second;

			for(unsigned i=1; i<regs.num_regs && regs.end[i]-regs.start[i]; i++){
				char tmp[regs.end[i]-regs.start[i]+1];
				strncpy(tmp,&target[regs.start[i]],regs.end[i]-regs.start[i]);
				tmp[regs.end[i]-regs.start[i]] = '\0';
				submatches.push_back(tmp);
#ifdef __DEBUG__
				cout<<"Match "<<i<<": "<<tmp<<endl;
#endif
			}
			target += *regs.end;
		} else {
			regfree(&regsys); return retval;
		}
	}
	regfree(&regsys); return retval;
}

string RegExSubstitude(const char *pattern_from, const char *pattern_to, const char* target){
//  Expression $var =~ s/(\\w)\\.(\\w)/$2.$1/gx is equivalent to
//  var = RegExSubstitude("(\\w)\\.(\\w)", "$2.$1", "first.second"); (var == "second.first")
	string retval;
	regex_t regsys;
	regsys.buffer    = 0;
	regsys.allocated = 0;
	regsys.fastmap   = 0;
	regsys.translate = 0;
	regsys.no_sub    = 0;
	regsys.used      = 0;
	re_syntax_options = RE_SYNTAX_AWK;
	re_syntax_options ^= RE_NO_GNU_OPS;

	if(re_compile_pattern(pattern_from, strlen(pattern_from), &regsys)){
		cout<<"Can't compilate the pattern: "<<pattern_from<<endl;
		return retval;
	}
	re_registers regs; regs.num_regs = 0;

	while(1){
		if(re_search(&regsys, target, strlen(target), 0, strlen(target), &regs)<0){
			retval.append(target,0,strlen(target));
			regfree(&regsys); return retval;
		}
#ifdef __DEBUG__
		cout<<regs.num_regs<<" matches found"<<endl;
#endif
		if(regs.num_regs){
#ifdef __DEBUG__
			cout<<"First match range: "<<*regs.start<<" - "<<*regs.end<<endl;
#endif
			string subsitude = pattern_to;
			size_t var;
			while( (var = subsitude.find("$")) != string::npos && subsitude[var-1] != '\\'){
				int k = atoi(&subsitude[var+1]);
				subsitude.replace(var,2,&target[regs.start[k]],regs.end[k]-regs.start[k]);
			}

			retval.append(target,0,*regs.start);
			retval.append(subsitude);
			target += *regs.end;
#ifdef __DEBUG__
			cout<<"Match "<<subsitude<<endl;
#endif
		} else {
			retval.append(target,0,strlen(target));
			regfree(&regsys); return retval;
		}
	}
	regfree(&regsys); return retval;
}

