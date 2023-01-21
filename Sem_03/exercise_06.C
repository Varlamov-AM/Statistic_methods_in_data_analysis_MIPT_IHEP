/// \file
/// \ingroup tutorial_fit
/// \notebook -js
/// Get in memory an histogram from a root file and fit a user defined function.
/// Note that a user defined function must always be defined
/// as in this example:
///  - first parameter: array of variables (in this example only 1-dimension)
///  - second parameter: array of parameters
/// Note also that in case of user defined functions, one must set
/// an initial value for each parameter.
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \author Rene Brun

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TInterpreter.h>
#include <TROOT.h>

#include <cmath>

double fitf(double *x, double *par)
{
   
   // double argx = 0;
   // double argy = 0;
   // if (par[2] != 0) argx = (x[0]*x[0] - par[1])/par[2];
   // if (par[4] != 0) argy = (x[1]*x[1] - par[3])/par[4];
   
   double arg = 0;
   if (par[2] != 0) arg = (std::sqrt(x[0]*x[0] + x[1]*x[1]) - par[1])/par[2];
   double fitval = par[0]*std::exp(-0.5*arg*arg);
   return fitval;
}
void exercise_06()
{
   TString dir = gROOT->GetTutorialDir();
   dir.Append("/hsimple.C");
   dir.ReplaceAll("/./","/");
   if (!gInterpreter->IsLoaded(dir.Data())) gInterpreter->LoadMacro(dir.Data());
   TFile *hsimpleFile = (TFile*)gROOT->ProcessLineFast("hsimple(1)");
   if (!hsimpleFile) return;

   TCanvas *c1 = new TCanvas("c1","the fit canvas",500,400);

   TH2F *hpxpy = (TH2F*)hsimpleFile->Get("hpxpy");

// Creates a Root function based on function fitf above
   TF2 *func = new TF2("fitf",fitf,-2,2,-2,2,3);

// Sets initial values and parameter names
   func->SetParameters(100,0,1);
   func->SetParNames("Constant","Mean_value","Sigma");

// Fit histogram in range defined by function
   hpxpy->Fit(func,"r");

// Gets integral of function between fit limits
   printf("Integral of function = %g\n",func->Integral(-2,2));
}
