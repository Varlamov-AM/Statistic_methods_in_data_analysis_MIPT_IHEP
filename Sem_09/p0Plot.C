using namespace RooStats;
using namespace RooFit;

void p0Plot() { 

  const char* infile =  "GausExpModel.root"; 
  const char* workspaceName = "w";
  const char* modelConfigName = "ModelConfig";
  const char* dataName = "data";

  /////////////////////////////////////////////////////////////
  // First part is just to access the workspace file 
  ////////////////////////////////////////////////////////////

  // Check if example input file exists
  TFile *file = TFile::Open(infile);

  // get the workspace out of the file
  RooWorkspace* w = (RooWorkspace*) file->Get(workspaceName);


  // get the modelConfig out of the file
  RooStats::ModelConfig* mc = (RooStats::ModelConfig*) w->obj(modelConfigName);

  // get the modelConfig out of the file
  RooAbsData* data = w->data(dataName);

  // get the modelConfig (S+B) out of the file
  // and create the B model from the S+B model
  ModelConfig*  sbModel = (RooStats::ModelConfig*) w->obj(modelConfigName);
  sbModel->SetName("S+B Model");      
  RooRealVar* poi = (RooRealVar*) sbModel->GetParametersOfInterest()->first();
  poi->setVal(50);  // set POI snapshot in S+B model for expected significance
  sbModel->SetSnapshot(*poi);
  ModelConfig * bModel = (ModelConfig*) sbModel->Clone();
  bModel->SetName("B Model");      
  poi->setVal(0);
  bModel->SetSnapshot( *poi  );

  vector<double> masses;
  vector<double> p0values;
  vector<double> p0valuesExpected;

  double massMin = 0.5;
  double massMax = 8.5;

  // loop on the mass values 
 
  for( double mass=massMin; mass<=massMax; mass += (massMax-massMin)/40.0 )   {                    cout << endl << endl << "Running for mass: " << mass << endl << endl;    
    w->var("mass")->setVal( mass );

     AsymptoticCalculator *  ac  = new AsymptoticCalculator(*data, *sbModel, *bModel);
     ac->SetOneSidedDiscovery(true);  // for one-side discovery test                                      
     AsymptoticCalculator::SetPrintLevel(-1);


     HypoTestResult* asymCalcResult = ac->GetHypoTest();
 
     asymCalcResult->Print();
     
     masses.push_back( mass );
     p0values.push_back( asymCalcResult->NullPValue() );
  
     double expectedP0 = AsymptoticCalculator::GetExpectedPValues(  asymCalcResult->NullPValue(),  asymCalcResult->AlternatePValue(), 0, false);
     p0valuesExpected.push_back( expectedP0 );
     std::cout << "expected p0 = " << expectedP0 << std::endl;   
  }   
  TGraph * graph1  = new TGraph(masses.size(),&masses[0],&p0values[0]);   
  TGraph * graph2  = new TGraph(masses.size(),&masses[0],&p0valuesExpected[0]);   graph1->SetMarkerStyle(20);
  graph1->Draw("APC");
  graph2->SetLineStyle(2);
  graph2->Draw("C");
  graph1->GetXaxis()->SetTitle("mass");
  graph1->GetYaxis()->SetTitle("p0 value");
  graph1->SetTitle("Significance vs Mass");
  graph1->SetMinimum(graph2->GetMinimum());
  graph1->SetLineColor(kBlue);
  graph2->SetLineColor(kRed);
  gPad->SetLogy(true);
  
     
}
