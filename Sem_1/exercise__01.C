
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TMath.h"
using namespace RooFit;

Double_t DoubleGauss(Double_t x, Double_t mu1, Double_t sigma1){//,
   //Double_t mu2, Double_t sigma2){
      Double_t result = TMath::Gaus(x, mu1, sigma1) + TMath::Gaus(x, mu1, sigma1);
      return result;
   }

void exercise__01()
{
   // S e t u p   m o d e l
   // ---------------------

   // Declare variables x,mean,sigma with associated name, title, initial value and allowed range
   RooRealVar x("x", "x", -10, 10);
   RooRealVar mean1("mean", "mean of gaussian", -3, -10, 10);
   RooRealVar sigma1("sigma", "width of gaussian", 2, 0.1, 10);
   RooRealVar mean2("mean", "mean of gaussian", 3, -10, 10);
   RooRealVar sigma2("sigma", "width of gaussian", 2, 0.1, 10);

   // Build gaussian pdf in terms of x,mean and sigma
   RooGaussian gauss1("gauss1", "gaussian PDF", x, mean1, sigma1);
   RooGaussian gauss2("gauss2", "gaussian PDF", x, mean2, sigma2);


   // Construct plot frame in 'x'
   RooPlot *xframe = x.frame(Title("Gaussian pdf."));

   // P l o t   m o d e l   a n d   c h a n g e   p a r a m e t e r   v a l u e s
   // ---------------------------------------------------------------------------

   // Plot gauss in frame (i.e. in x)
   gauss1.plotOn(xframe);
   gauss2.plotOn(xframe);  
   // Change the value of sigma to 3
   //sigma1.setVal(3);

   // Plot gauss in frame (i.e. in x) and draw frame on canvas
   //gauss1.plotOn(xframe, LineColor(kRed));

   // G e n e r a t e   e v e n t s
   // -----------------------------

   // Generate a dataset of 1000 events in x from gauss
   RooDataSet *data1 = gauss1.generate(x, 100000);
   RooDataSet *data2 = gauss2.generate(x, 100000);
   
   data2->append((*data1));

   // Make a second plot frame in x and draw both the
   // data and the pdf in the frame
   RooPlot *xframe2 = x.frame(Title("Gaussian pdf with data"));
   data2->plotOn(xframe2);

   TF1 *fa1 = new TF1("fa1","sin(x)/x",0,10);

   RooRealVar y("y","y",0.01,20) ;
   RooAbsReal* rfa1 = bindFunction(fa1,y) ;

   RooPlot* frame = y.frame() ;
   rfa1->plotOn(frame) ; 
   //gauss1.plotOn(xframe2);

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
   frame->GetYaxis()->SetTitleOffset(1.6);
   frame->Draw();
   c->cd(2);
   gPad->SetLeftMargin(0.15);
   xframe2->GetYaxis()->SetTitleOffset(1.6);
   xframe2->Draw();
}
