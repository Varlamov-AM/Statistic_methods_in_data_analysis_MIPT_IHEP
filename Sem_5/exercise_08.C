/// \file
/// \ingroup tutorial_roofit
/// \notebook -js
/// Basic functionality: Calculating chi^2 from histograms and curves in RooPlots, making
/// histogram of residual and pull distributions
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \date July 2008
/// \author Wouter Verkerke

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChiSquarePdf.h"
#include "RooConstVar.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooHist.h"
#include <vector>
#include "TMath.h"
#include "TH1D.h"
using namespace RooFit;

void exercise_08()
{

   // S e t u p   m o d e l
   // ---------------------

   // Create observables
   RooRealVar x("x", "x", -10, 10);

   const int n_iter = 5000;
   const int ndf = 38;
   TH1D* hist_prob = new TH1D("hist_prob", "Probability", ndf, 0., 1);
   TH1D* hist_chis = new TH1D("hist_chis", "Probability_chi_sq", ndf, 0., 1);

   // Create Gaussian
   RooRealVar sigma("sigma", "sigma", 3, 0.1, 10);
   RooRealVar mean("mean", "mean", 0, -10, 10);
   RooRealVar ndf_for_pdf("ndf_", "ndf_", ndf, 0, 40);
   RooGaussian gauss("gauss", "gauss", x, RooConst(0), sigma);
   RooChiSquarePdf ch_square("chi_square", "chi_square", x, ndf_for_pdf);
   

   // Generate a sample of 1000 events with sigma=3
   //RooDataSet *data = gauss.generate(x, 10000);

   // Change sigma to 3.15
   //sigma.setVal(3.15);

   // P l o t   d a t a   a n d   s l i g h t l y   d i s t o r t e d   m o d e l
   // ---------------------------------------------------------------------------

   // Overlay projection of gauss with sigma=3.15 on data with sigma=3.0

   for (int i = 0; i < n_iter; i++){
      RooDataSet* data = gauss.generate(x, 10000);
      RooDataSet* dat = ch_square.generate(x, 1);
      RooPlot *frame1 = x.frame(Title("Data with distorted Gaussian pdf"), Bins(ndf + 2));
      data->plotOn(frame1, DataError(RooAbsData::SumW2));
      gauss.plotOn(frame1);   
      sigma.setVal(3.0);
      hist_prob->Fill(TMath::Prob((frame1->chiSquare())*ndf, ndf));
      dat->Print("V");

   }



   // C a l c u l a t e   c h i ^ 2
   // ------------------------------

   // Show the chi^2 of the curve w.r.t. the histogram
   // If multiple curves or datasets live in the frame you can specify
   // the name of the relevant curve and/or dataset in chiSquare()

   // S h o w   r e s i d u a l   a n d   p u l l   d i s t s
   // -------------------------------------------------------

   // Construct a histogram with the residuals of the data w.r.t. the curve
   //RooHist *hresid = frame1->residHist();

   // Construct a histogram with the pulls of the data w.r.t the curve
   //RooHist *hpull = frame1->pullHist();

   // Create a new frame to draw the residual distribution and add the distribution to the frame
   //RooPlot *frame2 = x.frame(Title("Residual Distribution"));
   //frame2->addPlotable(hresid, "P");

   // Create a new frame to draw the pull distribution and add the distribution to the frame
   //RooPlot *frame3 = x.frame(Title("Pull Distribution"));
   //frame3->addPlotable(hpull, "P");

   TCanvas *c = new TCanvas("rf109_chi2residpull", "rf109_chi2residpull", 900, 300);
   hist_prob->Draw();
}
