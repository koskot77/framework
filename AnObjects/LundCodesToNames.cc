using namespace std;
#include "LundCodesToNames.h"

LundCodesToNames __makeInitialization__;

LundCodesToNames::LundCodesToNames(void){
	names[0] = "unknown";

	names[-521] = "B-";         names[521] = "B+";
	names[-511] = "B0bar";      names[511] = "B0";
	names[-423] = "antiDstar0"; names[423] = "Dstar0";
	names[-421] = "D0bar"; names[421] = "D0";
	names[-411] = "D+";    names[411] = "D-";
	names[-413] = "D*-";   names[413] = "D*+";

	names[ 22] = "g";      names[111] = "pi0";
	names[-11] = "e+";     names[ 11] = "e-";
	names[-12] = "nue+";   names[ 12] = "nue-";
	names[-13] = "mu+";    names[ 13] = "mu-";
	names[-14] = "numu+";  names[ 14] = "numu-";
	names[-15] = "tau+";   names[ 15] = "tau-";
	names[-16] = "nutay+"; names[ 16] = "nutau-";

	names[-211] = "pi-";   names[211] = "pi+";
	names[-321] = "K-";    names[321] = "K+";
	names[ 130] = "Kl";    names[310] = "Ks";
	names[-311] = "K0bar"; names[311] = "K0";

	names[ 221] = "eta";
	names[-323] = "K*-";   names[ 323] = "K*+";
	names[ 313] = "K*0";   names[-313] = "antiK*0";

	names[-213] = "RHO-";  names[ 213] = "RHO+";
	names[ 221] = "Eta";   names[ 223] = "OMEG";
	names[ 113] = "RHO0";

	names[10411] = "D_0*+";  names[-10411] = "D_0*-";
	names[10421] = "D_0*0";  names[-10421] = "antiD_0*0";
	names[10413] = "D_1+";   names[-10413] = "D_1-";
	names[10423] = "D_10";   names[-10423] = "antiD_10";
	names[  415] = "D_2*+";  names[  -415] = "D_2*-";
	names[  425] = "D_2*0";  names[  -425] = "antiD_2*0";
	names[20413] = "D'_1+";  names[-20413] = "D'_1-";
	names[20423] = "D'_10";  names[-20423] = "antiD'_10";
	names[  431] = "D_s+";   names[  -431] = "D_s-";
	names[  433] = "D_s*+";  names[  -433] = "D_s*-";
	names[10431] = "D_s0*+"; names[-10431] = "D_s0*-";
	names[10433] = "D_s1+";  names[-10433] = "D_s1-";
	names[  435] = "D_s2*+"; names[  -435] = "D_s2*-";
	names[20433] = "D'_s1+"; names[-20433] = "D'_s1-";
	names[30411] = "D(2S)+"; names[-30411] = "D(2S)-";
	names[30421] = "D(2S)0"; names[-30421] = "antiD(2S)0";
	names[30413] = "D*(2S)+";names[-30413] = "D*(2S)-";
	names[30423] = "D*(2S)0";names[-30423] = "antiD*(2S)0";

	names[2212] = "P";      names[-2212] = "antiP";
	names[2112] = "N";      names[-2112] = "antiN";
	names[3122] = "Lambda"; names[-3122] = "LambdaBar";
	names[3222] = "Sigma+"; names[-3222] = "SigmaBar-";
	names[3212] = "Sigma0"; names[-3212] = "SigmaBar0";
	names[3112] = "Sigma-"; names[-3112] = "SigmaBar+";
	names[3322] = "Xi0";    names[-3322] = "XiBar0";
	names[3312] = "Xi-";    names[-3312] = "XiBar+";

	names[331] = "etaP"; names[333] = "phi";
	names[441] = "etaC"; names[443] = "JPsi";
}

const char* LundCodesToNames::convert(int code) {
	unordered_map<int,const char*>::const_iterator name = names.find(code);
	if( name != names.end() ) return name->second;
	else return "";
}
