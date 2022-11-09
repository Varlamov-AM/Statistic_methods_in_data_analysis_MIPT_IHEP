#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"
#include "TString.h"
using namespace RooFit;

RooWorkspace create_workspace(){

    RooWorkspace w("w", kTRUE);
    w.factory("Landau::l1(x[0, 10], 3, 1.)");
    w.factory("Landau::l2(x, 6., 1.)");
    w.factory("Chebychev::bkg(x,{a0[0.5,0.,1],a1[0.2,0.,1.]})");
    w.factory("SUM::sig(sig1frac[0.8,0.,1.]*l1,l2)");
    w.factory("SUM::model(bkgfrac[0.5,0.,1.]*bkg,sig)");

    RooDataSet *data = w.pdf("model")->generate(*w.var("x"), 100000);
    w.import(*data, Rename("data"));

    return w;
}

void write_workspace(RooWorkspace* wspace, TString file_name){

    TFile* output_file = new TFile(file_name.Data(), "Recreate");
    output_file->cd();
    wspace->writeToFile(file_name.Data());

    return;
}

RooWorkspace* read_workspace(TString file_name){

    TFile* import_data = new TFile(file_name.Data(), "Read");
    RooWorkspace* import_ws = (RooWorkspace *)import_data->Get("w");

    return import_ws;
}

int exercise_11(){

    RooWorkspace workspace = create_workspace();
    TString file_name = Form("Output_data_with_my_own_workspace.root");
    
    write_workspace(&workspace, file_name);

    RooWorkspace* readed_ws = read_workspace(file_name);

    readed_ws->Print();

    RooRealVar *x = readed_ws->var("x");
    RooAbsPdf *model = readed_ws->pdf("model");
    RooAbsData *data = readed_ws->data("data");

    // Fit model to data
    model->fitTo(*data);

    // Plot data and PDF overlaid
    RooPlot *xframe = x->frame(Title("Model and data read from workspace"));
    data->plotOn(xframe);
    model->plotOn(xframe);

    // Overlay the background component of model with a dashed line
    model->plotOn(xframe, Components("bkg"), LineStyle(kDashed));

    // Overlay the background+sig2 components of model with a dotted line
    model->plotOn(xframe, Components("l1"), LineColor(kRed));
    model->plotOn(xframe, Components("l2"), LineColor(kGreen));

    // Draw the frame on the canvas
    new TCanvas("rf503_wspaceread", "rf503_wspaceread", 600, 600);
    gPad->SetLeftMargin(0.15);
    xframe->GetYaxis()->SetTitleOffset(1.4);
    xframe->Draw();

    return 0;
}