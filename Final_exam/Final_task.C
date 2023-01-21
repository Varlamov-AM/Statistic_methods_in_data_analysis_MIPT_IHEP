#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TMath.h"
using namespace RooFit;


void Final_task(){

    RooRealVar x("x", "x", 0, 10);
    RooRealVar mean1("mean1", "mean of gaussian", 4, 0, 10);
    RooRealVar sigma1("sigma1", "width of gaussian", 0.4, 0.1, 10);
    RooRealVar mean2("mean2", "mean of gaussian", 5, 0, 10);
    RooRealVar sigma2("sigma2", "width of gaussian", 0.3, 0.1, 10);
    RooRealVar lambda_exp("lambda_exp", "par in exponent", -0.5);

    // Build gaussian pdf in terms of x,mean and sigma
    RooGaussian gauss1("gauss1", "gaussian PDF", x, mean1, sigma1);
    RooGaussian gauss2("gauss2", "gaussian PDF", x, mean2, sigma2);
    RooExponential exp1("exp1", "Backgr", x, lambda_exp);

    RooRealVar sig1frac("sig1frac", "fraction of component 1 in signal", 0.6, 0., 1.);
    RooAddPdf sig("sig", "Signal", RooArgList(gauss1, gauss2), sig1frac);

    RooRealVar bkgfrac("bkgfrac", "fraction of background", 0.8, 0., 1.);
    RooAddPdf model("model", "g1+g2+a", RooArgList(exp1, sig), bkgfrac);

    RooDataSet *data = model.generate(x, 100000);

    RooWorkspace *w = new RooWorkspace("w", "workspace");

   
    w->import(model);
    w->import(*data);
    w->Print();
    w->writeToFile("Final_tasK_workspace.root");
    gDirectory->Add(w);

    model.fitTo(*data);

    RooPlot *xframe = x.frame(Title("Example of composite pdf=(sig1+sig2)+bkg"));
    data->plotOn(xframe);
    model.plotOn(xframe);   
    // Overlay the background component of model with a dashed line
    model.plotOn(xframe, Components("exp1"), LineColor(kRed));

    // Overlay the background+sig2 components of model with a dotted line
    model.plotOn(xframe, Components("gauss1"), LineColor(kOrange));
    model.plotOn(xframe, Components("gauss2"), LineColor(kMagenta));

    model.Print("t");    

    new TCanvas("rf201_composite", "rf201_composite", 600, 600);
    gPad->SetLeftMargin(0.15);
    xframe->GetYaxis()->SetTitleOffset(1.4);
    xframe->Draw();

    return;
}