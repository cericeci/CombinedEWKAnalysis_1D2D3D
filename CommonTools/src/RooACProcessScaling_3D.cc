/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "HiggsAnalysis/CombinedLimit/interface/RooACProcessScaling_3D.h" 

#include <math.h> 
#include "TMath.h" 
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include "RooFit.h"

#include "TFile.h"

ClassImp(RooACProcessScaling_3D) 

RooACProcessScaling_3D::RooACProcessScaling_3D() : 
  type_(notype),
  P_par1par2par3_histo(0),P_par1par2par3_TF(0)
{
  initializeProfiles();
}

RooACProcessScaling_3D::RooACProcessScaling_3D(const char *name, 
					     const char *title,
					     RooAbsReal& _x,
					     RooAbsReal& _par1,
					     RooAbsReal& _par2,
     					     RooAbsReal& _par3,
					     RooAbsReal& _SM_shape,
								 const char * parFilename,LimitType typeInit) :
   RooAbsReal(name,title),
   par1("par1","par1",this,_par1),
   par2("par2","par2",this,_par2),
   par3("par3","par3",this,_par3),
   type_(typeInit),
   profileFilename(parFilename),
   P_par1par2par3_histo(0),
   P_par1par2par3_TF(0)
{ 
  initializeProfiles();
  initializeNormalization(_x,_SM_shape);
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(parFilename,"READ");  
  gDirectory->cd(pwd);
  std::vector<double> bins_;

  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(_x);
  int N_bins=rdep.getBinning("").numBins();

  for (int i=0;i<N_bins;i++){
    if (i==0) bins_.push_back(rdep.getBinning("").binLow(i));
    bins_.push_back(rdep.getBinning("").binHigh(i));
  }
  readProfiles(bins_,*f,type_);
  f->Close();
} 

RooACProcessScaling_3D::
RooACProcessScaling_3D(const RooACProcessScaling_3D& other, 
		      const char* name) :  
  RooAbsReal(other,name),
  par1("par1",this,other.par1),
  par2("par2",this,other.par2),
  par3("par3",this,other.par3),
  type_(other.type_), 
  SM_integral(other.SM_integral),
  integral_basis(other.integral_basis),
  bins(other.bins),
  profileFilename(other.profileFilename),
  P_par1par2par3_histo(0),
  P_par1par2par3_TF(0)
{ 
  initializeProfiles();
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(profileFilename,"READ");  
  gDirectory->cd(pwd);  
  readProfiles(bins,*f,type_);
  f->Close();
} 

void RooACProcessScaling_3D::initializeProfiles() {
  int const N_bins=100;

  P_par1par2par3_histo = new TH3D*[N_bins]();
  P_par1par2par3_TF = new TF3*[N_bins]();
}

void RooACProcessScaling_3D::initializeNormalization(const RooAbsReal& dep,
						    const RooAbsReal& shape) {
  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(dep);
  RooRealVar& b = const_cast<RooRealVar&>(rdep);
  
  bool bin_true=rdep.hasBinning("");
  if (!bin_true)
    cout <<"\t @@@@@@@@@@@ can't find biining! (RooATGCProcessScaling file)" << endl;

  int N_bins=rdep.getBinning("").numBins();

  
  RooAbsReal* integral = shape.createIntegral(RooArgSet(dep),RooArgSet());
  SM_integral = integral->getVal();
  delete integral;

  for(int i=0; i<N_bins; ++i) {
    int bin_low=rdep.getBinning("").binLow(i);
    int bin_high=rdep.getBinning("").binHigh(i);
    const string intRange="integRange";
    b.setRange((const char*)intRange.c_str(),bin_low,bin_high);
    RooAbsReal* integral = shape.createIntegral(RooArgSet(rdep),RooArgSet(),(const char*)intRange.c_str());
    integral_basis.push_back(integral->getVal());
    if (i==0){
      bins.push_back(bin_low);
    }
    bins.push_back(bin_high);

    delete integral;

  }



}

void RooACProcessScaling_3D::readProfiles(std::vector<double> bins,TDirectory& dir,LimitType type_) const {

  int N_bins;
  int i;
  
  switch(type_) {
  case par1par2par3_TH3:
  case par1par2par3_TF3:
    N_bins=bins.size()-1;
    break;
  case EFTtoATGC:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }
  
  for(i=0; i<N_bins; ++i) {
    
    if (P_par1par2par3_histo[i]) delete P_par1par2par3_histo[i];
    if (P_par1par2par3_TF[i]) delete P_par1par2par3_TF[i];
    TString par2name = TString::Format("bin_content_par1_par2_par3_%i",i+1);

    switch(type_) {
    case par1par2par3_TH3:
      P_par1par2par3_histo[i] = dynamic_cast<TH3D *>(dir.Get(par2name)->Clone(par2name+"new"));
      P_par1par2par3_histo[i]->SetDirectory(0);
      break;
    case par1par2par3_TF3:
      P_par1par2par3_TF[i] = dynamic_cast<TF3 *>(dir.Get(par2name)->Clone(par2name+"new"));
      break;
    case EFTtoATGC:
      P_par1par2par3_TF[i] = dynamic_cast<TF3 *>(dir.Get(par2name)->Clone(par2name+"new"));
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }
  
  }

}

void RooACProcessScaling_3D::readProfiles(RooACProcessScaling_3D const& other) {

  int N_bins;
  switch(type_) {
  case par1par2par3_TH3:
  case par1par2par3_TF3:
    N_bins=bins.size()-1;
    break;
  case EFTtoATGC:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }


  for (int i = 0; i<N_bins; ++i) {    
    TString par2name = TString::Format("bin_content_par1_par2_par3_%i",i+1);

    switch(type_) {
    case par1par2par3_TH3:
      P_par1par2par3_histo[i] = dynamic_cast<TH3D *>(other.P_par1par2par3_histo[i]->Clone(par2name+"new"));
      P_par1par2par3_histo[i]->SetDirectory(0);
      break;
    case par1par2par3_TF3:
      P_par1par2par3_TF[i] = dynamic_cast<TF3 *>(other.P_par1par2par3_histo[i]->Clone(par2name+"new"));
      break;
    case EFTtoATGC:
      P_par1par2par3_TF[i] = dynamic_cast<TF3 *>(other.P_par1par2par3_histo[i]->Clone(par2name+"new"));
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }

  }
}

RooACProcessScaling_3D::~RooACProcessScaling_3D() {
  int N_bins=100;

  for(int i = 0; i<N_bins; ++i) {
    if (P_par1par2par3_histo[i])
      delete P_par1par2par3_histo[i];
    if (P_par1par2par3_TF[i])
      delete P_par1par2par3_TF[i];
  }
  delete[] P_par1par2par3_histo;
  delete[] P_par1par2par3_TF;
}

Double_t RooACProcessScaling_3D::evaluate() const 
{ 


  double MZ=0.0911876;
  double MW=0.080385;
  //  double s=MZ*MZ/(MW*MW);
  double sin2thw=0.23126;
  double tan2thw=-1000.;
  tan2thw=sin2thw/(1-sin2thw);
  double alpha=0.0072973525698;
  double e=sqrt(4.*3.141592*alpha);
  double g=e/sqrt(sin2thw);


  TH3D ** P_histo = NULL;
  TF3 ** P_TF = NULL;
  double v1(0.0), v2(0.0), v3(0.0);
  
    P_histo = P_par1par2par3_histo;
    P_TF = P_par1par2par3_TF;
    v1 = par1;
    v2 = par2;
    v3 = par3;
  
  if (not P_histo[0] and not P_TF[0]) {
    TFile f(profileFilename);
    readProfiles(bins,f,type_);
    f.Close();
  }

  switch(type_) {
  case par1par2par3_TH3:
    if (v1 < P_histo[0]->GetXaxis()->GetXmin())
      v1 = P_histo[0]->GetXaxis()->GetXmin();
    if (v1 > P_histo[0]->GetXaxis()->GetXmax())
      v1 = P_histo[0]->GetXaxis()->GetXmax();
    if (v2 < P_histo[0]->GetYaxis()->GetXmin())
      v2 = P_histo[0]->GetYaxis()->GetXmin();
    if (v2 > P_histo[0]->GetYaxis()->GetXmax())
      v2 = P_histo[0]->GetYaxis()->GetXmax();
    if (v3 < P_histo[0]->GetZaxis()->GetXmin())
      v3 = P_histo[0]->GetZaxis()->GetXmin();
    if (v3 > P_histo[0]->GetZaxis()->GetXmax())
      v3 = P_histo[0]->GetZaxis()->GetXmax();
    break;
  case par1par2par3_TF3:
/*
    if (v1 < P_TF[0]->GetXmin())
      v1 = P_TF[0]->GetXmin();
    if (v1 > P_TF[0]->GetXmax())
      v1 = P_TF[0]->GetXmax();
    if (v2 < P_TF[0]->GetYmin())
      v2 = P_TF[0]->GetYmin();
    if (v2 > P_TF[0]->GetYmax())
      v2 = P_TF[0]->GetYmax();
    if (v3 < P_TF[0]->GetZmin())
      v3 = P_TF[0]->GetZmin();
    if (v3 > P_TF[0]->GetZmax())
      v3 = P_TF[0]->GetZmax();
*/
    break;
  case EFTtoATGC:
    break;
  case notype:
    assert(NULL && "invalid limit type!");
    break;

  }

  
  double ret(0.);
  int N_bins=bins.size()-1;;

  double cwww=v1*2./(3.*g*g*MW*MW);
  double cw=v2*2./(MZ*MZ);
  double cb=v3*(-2.)/(sin2thw*MZ*MZ)+cw/tan2thw;


  for(int i = 0; i<N_bins; i++) {
 
    switch(type_) {
      cout <<"place D: case type= "<<type_ << endl;
    case par1par2par3_TH3:
      ret += P_histo[i]->Interpolate(v1,v2,v3)*integral_basis[i];
      break;
    case par1par2par3_TF3:
      ret += P_TF[i]->Eval(v1,v2,v3)*integral_basis[i];
      break;
    case EFTtoATGC:
      /*
      double cwww=v1*2./(3.*g*g*MW*MW);
      double cw=v2*2./(MZ*MZ);
      double cb=v3*(-2.)/(sin2thw*MZ*MZ)+cw/tan2thw;
      */
      ret += P_TF[i]->Eval(cwww,cw,cb)*integral_basis[i];
      break;
      /*
    case notype:
      assert(NULL && "invalid limit type!");
      break;
      */
    default:
      assert(NULL && "invalid limit type!");
      break;
    }
  }
  cout << "ret/SM_integral= "<< ret<<"/"<<SM_integral<<" = "<< ret/SM_integral << endl;
  return ret/SM_integral;

}

