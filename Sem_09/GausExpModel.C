#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooRandom.h"

#include "RooStats/ModelConfig.h"

using namespace RooFit; 



void GausExpModel(int nsig = 100,    // number of signal events 
                  int nbkg = 1000 )  // number of background events
{ 

   RooWorkspace w("w"); 
   w.factory("Polynomial:bkg_pdf(x[1,10], a[0.1,0,0.2], b[0.8, 0., 0.9])");
   w.factory("BreitWigner:sig_pdf(x, mass[2], sigma[0.3])");

   w.factory("SUM:model(nsig[0,10000]*sig_pdf, nbkg[0,10000]*bkg_pdf)");  // for extended model

   RooAbsPdf * pdf = w.pdf("model");
   RooRealVar * x = w.var("x");  // the observable

   // set the desired value of signal and background events
   w.var("nsig")->setVal(nsig);
   w.var("nbkg")->setVal(nbkg);

   // generate the data

   // use fixed random numbers for reproducibility (use 0 for changing every time)
   RooRandom::randomGenerator()->SetSeed(111);

   // fix number of bins to 50 to plot or to generate data (default is 100 bins) 
   x->setBins(50);

   RooDataSet * data = pdf->generate( *x);  // will generate accordint to total S+B events
   //RooDataSet * data = pdf->generate( *x, AllBinned());  // will generate accordint to total S+B events
   data->SetName("data");
   w.import(*data);

   data->Print(); 


   RooPlot * plot = x->frame(Title("Gaussian Signal over Exponential Background"));
   data->plotOn(plot);
   plot->Draw();

   RooFitResult * r = pdf->fitTo(*data, RooFit::Save(true), RooFit::Minimizer("Minuit2","Migrad"));
   r->Print();

   pdf->plotOn(plot);
   //draw the two separate pdf's
   pdf->plotOn(plot, RooFit::Components("bkg_pdf"), RooFit::LineStyle(kDashed) );
   pdf->plotOn(plot, RooFit::Components("sig_pdf"), RooFit::LineColor(kRed), RooFit::LineStyle(kDashed) );

   pdf->paramOn(plot,Layout(0.5,0.9,0.85));

   plot->Draw();


   RooStats::ModelConfig mc("ModelConfig",&w);
   mc.SetPdf(*pdf);
   mc.SetParametersOfInterest(*w.var("nsig"));
   mc.SetObservables(*w.var("x"));
   // define set of nuisance parameters
   w.defineSet("nuisParams","a,nbkg");

   mc.SetNuisanceParameters(*w.set("nuisParams"));

   // import model in the workspace 
   w.import(mc);

   // write the workspace in the file
   TString fileName = "GausExpModel.root";
   w.writeToFile(fileName,true);
   cout << "model written to file " << fileName << endl;
}
