
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TMath.h"
#include <vector>
using namespace RooFit;



void exercise_07()
{
   // S e t u p   m o d e l
   // ---------------------

   const int num_iter = 10000;
   const int num_even = 1000;



   // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
   RooRealVar x("x", "x", -10, 10);
   RooRealVar mean("mean", "mean of gaussian", 4, -10, 10);
   RooRealVar sigma("sigma", "width of gaussian", 2, 0.1, 10);

   // Build gaussian pdf in terms of x,mean and sigma
   RooGaussian gauss("gauss", "gaussian PDF", x, mean, sigma);

   // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
   // ---------------------------------------------------------------------------


   // G e n e r a t e   e v e n t s
   // -----------------------------

   // Generate a dataset of 1000 events in x from gauss

   vector<RooDataSet*> data;

   for (int i = 0; i < num_iter; i++){
      data.push_back(gauss.generate(x, num_even));
   }

   vector<RooRealVar> fitted_data_par_sigma;
   vector<RooRealVar> fitted_data_par_mean;

   TH1D* hist_sigma = new TH1D("hist_sigma", "sigma distribution", 1000, 1.9, 2.1);
   TH1D* hist_mean  = new TH1D("hist_mean",  "mean distribution",  1000, 3.9, 4.1);

   for (int i = 0; i < num_iter; i++){
      gauss.fitTo(*(data[i]));
      fitted_data_par_mean.push_back(mean);
      fitted_data_par_sigma.push_back(sigma);  
      hist_sigma->Fill(sigma.getVal());    
      hist_mean ->Fill(mean.getVal());    
   }


   // F i t   m o d e l   t o   d a t a
   // -----------------------------

   // Fit pdf to data
   // gauss.fitTo(*data);

   // Print values of mean and sigma (that now reflect fitted values and errors)
   // mean.Print();
   // sigma.Print();

   // Draw all frames on a canvas
   TCanvas *c = new TCanvas("rf101_basics", "rf101_basics", 800, 400);
   c->Divide(2);
   c->cd(1);
   gPad->SetLeftMargin(0.15);
   hist_sigma->Draw();
   c->cd(2);
   gPad->SetLeftMargin(0.15);
   hist_mean->Draw();

   TFile
}
