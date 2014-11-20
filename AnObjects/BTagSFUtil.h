/*************************************************************

Class Usage:

This class should only be used for upgrading and downgrading 
if a single operating point is used in an analysis. 

bool isBTagged = b-tag flag for jet
int pdgIdPart = parton id
float Btag_SF = MC/data scale factor for b/c-tagging efficiency
float Btag_eff = b/c-tagging efficiency in data
float Bmistag_SF = MC/data scale factor for mistag efficiency
float Bmistag_eff = mistag efficiency in data

Author: Michael Segala
Contact: michael.segala@gmail.com
Updated: Ulrich Heintz 12/23/2011

v 1.1

*************************************************************/


#ifndef BTagSFUtil_lite_h
#define BTagSFUtil_lite_h

#include <stdlib.h> // rand, srand

class BTagSFUtil{

 public:
    
  BTagSFUtil( int seed=0 ){ srand(seed); }
  ~BTagSFUtil(void){}

  enum {LOOSE=1, MEDIUM=2, TIGHT=3};    

  void modifyBTagsWithSF( bool& isBTagged, int pdgIdPart, float Btag_SF = 0.98, float Btag_eff = 1.0, float Bmistag_SF = 1.0, float  Bmistag_eff = 1.0);
  float GetLightJeteff (float pt = 40, float eta = 1.3, int meanminmax = 0, int workingPoint = MEDIUM);
  float GetBTagSF (float pt = 40, float eta = 1.3, int meanminmax=0, int workingPoint = MEDIUM);
  float GetBTageff (int workingPoint = MEDIUM);
  float GetLightJetSF (float pt = 40, float eta = 1.3, int meanminmax = 0, int workingPoint = MEDIUM);

 private:
  
  bool applySF(bool& isBTagged, float Btag_SF = 0.98, float Btag_eff = 1.0);

};

void BTagSFUtil::modifyBTagsWithSF(bool& isBTagged, int pdgIdPart, float Btag_SF, float Btag_eff, float Bmistag_SF, float Bmistag_eff){

  bool newBTag = isBTagged;

  // b quarks and c quarks:
  if( abs( pdgIdPart ) == 5 ||  abs( pdgIdPart ) == 4) { 

    double bctag_eff = Btag_eff;
    if ( abs(pdgIdPart)==4 )  bctag_eff = Btag_eff/5.0; // take ctag eff as one 5th of Btag eff
    newBTag = applySF(isBTagged, Btag_SF, bctag_eff);

    // light quarks:
  } else if( abs( pdgIdPart )>0 ) { //in data it is 0 (save computing time)

    newBTag = applySF(isBTagged, Bmistag_SF, Bmistag_eff);
    
  }

  isBTagged = newBTag;

}


bool BTagSFUtil::applySF(bool& isBTagged, float Btag_SF, float Btag_eff){
  
  bool newBTag = isBTagged;

  if (Btag_SF == 1) return newBTag; //no correction needed 

  //throw die
  float coin = (rand()%100000)/100000.;    

  if(Btag_SF > 1){  // use this if SF>1

    if( !isBTagged ) {

      //fraction of jets that need to be upgraded
///      float mistagPercent = (1.0 - Btag_SF) / (1.0 - (Btag_SF/Btag_eff) );
      float mistagPercent = (1.0 - Btag_SF) / (1.0 - (1.0/Btag_eff) ); //// https://hypernews.cern.ch/HyperNews/CMS/get/B2G-12-022/21/1/1/1/1.html

      //upgrade to tagged
      if( coin < mistagPercent ) {newBTag = true;}
    }

  }else{  // use this if SF<1
      
    //downgrade tagged to untagged
    if( isBTagged && coin > Btag_SF ) {newBTag = false;}

  }

  return newBTag;
}
float BTagSFUtil::GetBTagSF (float pt, float eta, int meanminmax, int workingPoint){
  float x=pt;
// old SSV:
//  float SFb = 0.896462*((1.+(0.00957275*x))/(1.+(0.00837582*x)));
// new tagger: CSVM within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
// from https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-pt_payload_Moriond13.txt

//  float ptmin[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
//  float ptmax[] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};
// above is equivalent to:
  int bin = -1;
  if( pt<20 ) bin = 0;
  if( pt>=20  && pt<80  ) bin = (pt-20)/10;
  if( pt>=80  && pt<120 ) bin = (pt-80)/20 + 6;
  if( pt>=120 && pt<160 ) bin = 8;
  if( pt>=160 && pt<210 ) bin = 9;
  if( pt>=210 && pt<260 ) bin = 10;
  if( pt>=260 && pt<320 ) bin = 11;
  if( pt>=320 && pt<400 ) bin = 12;
  if( pt>=400 && pt<500 ) bin = 13;
  if( pt>=500 && pt<600 ) bin = 14;
  if( pt>=600 && pt<800 ) bin = 15;
  if( pt>800 ) bin = 15;

  double uncertFactor = 1.;
  if( x< 20 ){ x =  20; uncertFactor = 2.; }
  if( x>800 ){ x = 800; uncertFactor = 2.; }

  float SFb_errorLoose[]  = { 0.033299,  0.0146768, 0.013803,  0.0170145, 0.0166976, 0.0137879, 0.0149072, 0.0153068,
                              0.0133077, 0.0123737, 0.0157152, 0.0175161, 0.0209241, 0.0278605, 0.0346928, 0.0350099 };
  float SFb_errorMedium[] = { 0.0415707, 0.0204209, 0.0223227, 0.0206655, 0.0199325, 0.0174121, 0.0202332, 0.0182446,
                              0.0159777, 0.0218531, 0.0204688, 0.0265191, 0.0313175, 0.0415417, 0.0740446, 0.0596716 };
  float SFb_errorTight[] =  { 0.0515703, 0.0264008, 0.0272757, 0.0275565, 0.0248745, 0.0218456, 0.0253845, 0.0239588,
                              0.0271791, 0.0273912, 0.0379822, 0.0411624, 0.0786307, 0.0866832, 0.0942053, 0.102403 };
  float SFb = 1;
  switch( workingPoint ){
     case LOOSE  : SFb = 0.997942*((1.+(0.00923753*x))/(1.+(0.0096119*x)));
        if( meanminmax == 1 ) SFb += SFb_errorLoose[bin] * uncertFactor;
        if( meanminmax ==-1 ) SFb -= SFb_errorLoose[bin] * uncertFactor;
     break;
     case MEDIUM : SFb = (0.938887+(0.00017124*x))+(-2.76366e-07*(x*x));
        if( meanminmax == 1 ) SFb += SFb_errorMedium[bin] * uncertFactor;
        if( meanminmax ==-1 ) SFb -= SFb_errorMedium[bin] * uncertFactor;
     break;
     case TIGHT  : SFb = (0.927563+(1.55479e-05*x))+(-1.90666e-07*(x*x));
        if( meanminmax == 1 ) SFb += SFb_errorTight[bin] * uncertFactor;
        if( meanminmax ==-1 ) SFb -= SFb_errorTight[bin] * uncertFactor;
     break;
     default : break;
  }
// those are always <1 -> we won't need to promote random jets to b-jets
  return SFb;
}

float BTagSFUtil::GetBTageff (int workingPoint){
//  float x=1.74;
  float effb=1;

// this function is not important if SFb<1
//  effb =  -0.00592795459517*x*x*x*x +  0.100276502585*x*x*x +  -0.562858452018*x*x +  1.02676379935*x +  0.0810613917651;
 
//https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/eff_b_c-ttbar_payload.txt :
//  double y=0; // discriminator value for CSV
//  switch( workingPoint ){
//     case LOOSE:  y = 0.244; break;
//     case MEDIUM: y = 0.679; break;
//     case TIGHT:  y = 0.898; break;
//  }
//  effb = -4.46932526854*y*y*y*y+7.32781975653*y*y*y-3.78459588569*y*y+0.221027515486*y+0.970299300468;
//  effb_err_max = -2.28719954194*y*y*y*y+1.80361455348*y*y*y+1.18308812929*y*y-1.58926423122*y+1.22173072594;
  effb = 0.696407; // root.exe 'bEff.C++(4,250,250)'
  return effb;
}

float BTagSFUtil::GetLightJetSF (float pt, float eta, int meanminmax, int workingPoint){
  float x=pt; 
  float tmpSFl=1;
// old SSV:
/*
  if(fabs(eta)>=0.0 &&  fabs(eta) <=0.8)
      {
	if( meanminmax == 0 ) tmpSFl = ((0.86318+(0.000801639*x))+(-1.64119e-06*(x*x)))+(2.59121e-10*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.790364+(0.000463086*x))+(-4.35934e-07*(x*x)))+(-9.08296e-10*(x*(x*x)));
	if( meanminmax == 1 ) tmpSFl = ((0.935969+(0.0011402*x))+(-2.84645e-06*(x*x)))+(1.42654e-09*(x*(x*x)));
      }
  if(fabs(eta)>0.8 &&  fabs(eta) <=1.6)
    {
      if( meanminmax == 0 ) tmpSFl = ((0.958973+(-0.000269555*x))+(1.381e-06*(x*x)))+(-1.87744e-09*(x*(x*x)));
      if( meanminmax == -1 ) tmpSFl = ((0.865771+(-0.000279908*x))+(1.34144e-06*(x*x)))+(-1.75588e-09*(x*(x*x)));
      if( meanminmax == 1 ) tmpSFl = ((1.0522+(-0.000259296*x))+(1.42056e-06*(x*x)))+(-1.999e-09*(x*(x*x)));
    }
  if(fabs(eta)>1.6 &&  fabs(eta) <=2.4)
    {
      if( meanminmax == 0 ) tmpSFl = ((0.923033+(-0.000898227*x))+(4.74565e-06*(x*x)))+(-6.11053e-09*(x*(x*x)));
      if( meanminmax == -1 ) tmpSFl =((0.828021+(-0.000731926*x))+(4.19613e-06*(x*x)))+(-5.81379e-09*(x*(x*x)));
      if( meanminmax == 1 ) tmpSFl =((1.01812+(-0.00106483*x))+(5.29518e-06*(x*x)))+(-6.40728e-09*(x*(x*x)));
    }
*/

  // using https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_Moriond2013.C and
  // assuming we are running on the whole A+B+C+D 2012 dataset, the SFs are:
switch( workingPoint ){
case LOOSE:
    if( fabs(eta)>= 0.0 && fabs(eta)<0.5 )
      {
	if( meanminmax == 0  ) tmpSFl = ((1.01177+(0.0023066*x))+(-4.56052e-06*(x*x)))+(2.57917e-09*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.977761+(0.00170704*x))+(-3.2197e-06*(x*x)))+(1.78139e-09*(x*(x*x)));
	if( meanminmax == 1  ) tmpSFl = ((1.04582+(0.00290226*x))+(-5.89124e-06*(x*x)))+(3.37128e-09*(x*(x*x)));
      }
    if( fabs(eta)>=0.5 && fabs(eta)<1.0 )
      {
	if( meanminmax == 0  ) tmpSFl = ((0.975966+(0.00196354*x))+(-3.83768e-06*(x*x)))+(2.17466e-09*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.945135+(0.00146006*x))+(-2.70048e-06*(x*x)))+(1.4883e-09*(x*(x*x)));
	if( meanminmax == 1  ) tmpSFl = ((1.00683+(0.00246404*x))+(-4.96729e-06*(x*x)))+(2.85697e-09*(x*(x*x)));
      }
    if( fabs(eta)>=1.0 && fabs(eta)<1.5 )
      {
	if( meanminmax == 0  ) tmpSFl = ((0.93821+(0.00180935*x))+(-3.86937e-06*(x*x)))+(2.43222e-09*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.911657+(0.00142008*x))+(-2.87569e-06*(x*x)))+(1.76619e-09*(x*(x*x)));
	if( meanminmax == 1  ) tmpSFl = ((0.964787+(0.00219574*x))+(-4.85552e-06*(x*x)))+(3.09457e-09*(x*(x*x)));
      }
    if( fabs(eta)>=1.5 && fabs(eta)<2.4 )
      {
	if( meanminmax == 0  ) tmpSFl = ((1.00022+(0.0010998*x))+(-3.10672e-06*(x*x)))+(2.35006e-09*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.970045+(0.000862284*x))+(-2.31714e-06*(x*x)))+(1.68866e-09*(x*(x*x)));
	if( meanminmax == 1  ) tmpSFl = ((1.03039+(0.0013358*x))+(-3.89284e-06*(x*x)))+(3.01155e-09*(x*(x*x)));
      }
break;
case MEDIUM:
  if(fabs(eta)>=0.0 &&  fabs(eta) <=0.8)
      {
	if( meanminmax == 0  ) tmpSFl = ((1.07541+(0.00231827*x))+(-4.74249e-06*(x*x)))+(2.70862e-09*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.964527+(0.00149055*x))+(-2.78338e-06*(x*x)))+(1.51771e-09*(x*(x*x)));
	if( meanminmax == 1  ) tmpSFl = ((1.18638+(0.00314148*x))+(-6.68993e-06*(x*x)))+(3.89288e-09*(x*(x*x)));
      }
  if(fabs(eta)>0.8 &&  fabs(eta) <=1.6)
    {
      if( meanminmax == 0  ) tmpSFl = ((1.05613+(0.00114031*x))+(-2.56066e-06*(x*x)))+(1.67792e-09*(x*(x*x)));
      if( meanminmax == -1 ) tmpSFl = ((0.946051+(0.000759584*x))+(-1.52491e-06*(x*x)))+(9.65822e-10*(x*(x*x)));
      if( meanminmax == 1  ) tmpSFl = ((1.16624+(0.00151884*x))+(-3.59041e-06*(x*x)))+(2.38681e-09*(x*(x*x)));
    }
  if(fabs(eta)>1.6 &&  fabs(eta) <=2.4)
    {
      if( meanminmax == 0  ) tmpSFl = ((1.05625+(0.000487231*x))+(-2.22792e-06*(x*x)))+(1.70262e-09*(x*(x*x)));
      if( meanminmax == -1 ) tmpSFl = ((0.956736+(0.000280197*x))+(-1.42739e-06*(x*x)))+(1.0085e-09*(x*(x*x)));
      if( meanminmax == 1  ) tmpSFl = ((1.15575+(0.000693344*x))+(-3.02661e-06*(x*x)))+(2.39752e-09*(x*(x*x)));
    }
break;
case TIGHT:
    if( fabs(eta)>=0.0 && fabs(eta)<2.4 )
      {
	if( meanminmax == 0  ) tmpSFl = ((1.00462+(0.00325971*x))+(-7.79184e-06*(x*x)))+(5.22506e-09*(x*(x*x)));
	if( meanminmax == -1 ) tmpSFl = ((0.845757+(0.00186422*x))+(-4.6133e-06*(x*x)))+(3.21723e-09*(x*(x*x)));
	if( meanminmax == 1  ) tmpSFl = ((1.16361+(0.00464695*x))+(-1.09467e-05*(x*x)))+(7.21896e-09*(x*(x*x)));
      }
break;
default: break;
}
    return tmpSFl;

}

float BTagSFUtil::GetLightJeteff (float pt, float eta, int meanminmax, int workingPoint){
  float x=pt;
  float tmpMistag=1;
// old SSV:
/*
  if(fabs(eta)>=0.0 &&  fabs(eta) <=0.8)
    {
      if( meanminmax == 0 ) tmpMistag = (((0.000547883+(0.00023023*x))+(-7.31792e-07*(x*x)))+(1.15659e-09*(x*(x*x))))+(-7.00641e-13*(x*(x*(x*x))));
    }
  if(fabs(eta)>0.8 &&  fabs(eta) <=1.6)
    {
      if( meanminmax == 0 ) tmpMistag = (((0.000615562+(0.000240254*x))+(-7.00237e-07*(x*x)))+(1.2566e-09*(x*(x*x))))+(-8.59011e-13*(x*(x*(x*x))));
    }
  if(fabs(eta)>1.6 &&  fabs(eta) <=2.4)
      {
	if( meanminmax == 0 ) tmpMistag = (((0.000372388+(0.000309735*x))+(-4.35952e-07*(x*x)))+(3.63763e-10*(x*(x*x))))+(-2.11993e-13*(x*(x*(x*x))));
      }
*/
// official interpolations are nowhere to be found, using a homemade linear approximation of top plot in Fig.18 of
// http://cms.cern.ch/iCMS/jsp/openfile.jsp?tp=draft&files=AN2012_195_v2.pdf
// i.e. MC: pt=45, eff=1%, pt=775, eff=2.6%
//    tmpMistag = (0.026-0.01)*(x-45.)/(775.-45.) + 0.01;

// https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/MistagFuncs.C:
///if( x<20 || x>670 ) return tmpMistag;

meanminmax=0; 

switch( workingPoint ){
case LOOSE:
  if( fabs(eta)>=0.0 && fabs(eta)<0.5 )
    {
       if( meanminmax == 0 ) tmpMistag = 242534*(((1+(0.0182863*x))+(4.50105e-05*(x*x)))/(1+(108569*x)));
    }
  if( fabs(eta)>=0.5 && fabs(eta)<1.0 )
    {
       if( meanminmax == 0 ) tmpMistag = 129.938*(((1+(0.0197657*x))+(4.73472e-05*(x*x)))/(1+(55.2415*x)));
    }
  if( fabs(eta)>=1.0 && fabs(eta)<1.5 )
    {
       if( meanminmax == 0 ) tmpMistag = 592.214*(((1+(0.00671207*x))+(6.46109e-05*(x*x)))/(1+(134.318*x)));
    }
  if( fabs(eta)>=1.5 && fabs(eta)<2.4 )
    {
       if( meanminmax == 0 ) tmpMistag = 93329*(((1+(0.0219705*x))+(3.76566e-05*(x*x)))/(1+(18245.1*x)));
    }
//if( fabs(eta)>=0.0 && fabs(eta)<2.4 )
//{
//if( meanminmax == 0 ) tmpMistag = 18168.8*(((1+(0.020356*x))+(2.73475e-05*(x*x)))/(1+(5239.42*x)));
//}
break;
case MEDIUM:
  if( fabs(eta)>=0.0 &&  fabs(eta) <0.8 )
    {
      if( meanminmax == 0 ) tmpMistag = 0.0164342 + (-2.16288e-05*x) + (3.01435e-08*x*x); //(0.00967751+(2.54564e-05*x))+(-6.92256e-10*(x*x));
    }
  if( fabs(eta)>=0.8 &&  fabs(eta) <1.6 )
    {
      if( meanminmax == 0 ) tmpMistag = 0.0133283 + (-6.65263e-06*x) + (3.2886e-08*x*x); //(0.00974141+(5.09503e-05*x))+(2.0641e-08*(x*x));
    }
  if( fabs(eta)>=1.6 &&  fabs(eta) <2.4 )
    {
      if( meanminmax == 0 ) tmpMistag = 0.01527 + (3.91721e-05*x) + (-1.25045e-08*x*x); //(0.013595+(0.000104538*x))+(-1.36087e-08*(x*x));
    }
//  if( fabs(eta)>0.0 &&  fabs(eta) <=2.4 )
//      {
//	if( meanminmax == 0 ) tmpMistag = (0.0113428+(5.18983e-05*x))+(-2.59881e-08*(x*x));
//      }
break;
case TIGHT:
  if( fabs(eta)>=0.0 && fabs(eta) <2.4 )
    {
      if( meanminmax == 0 ) tmpMistag = 0.00315116*(((1+(-0.00769281*x))+(2.58066e-05*(x*x)))+(-2.02149e-08*(x*(x*x))));
    }
break;
default: break;
}
    return tmpMistag;

}

#endif

