using namespace RooFit;
using namespace RooStats;

void SimpleMCMC( const char* infile =  "GausExpModel.root", 
                 const char* workspaceName = "w",
                 const char* modelConfigName = "ModelConfig",
                 const char* dataName = "data" )
{
  /////////////////////////////////////////////////////////////
  // First part is just to access the workspace file 
  ////////////////////////////////////////////////////////////

  // open input file 
  TFile *file = TFile::Open(infile);
  if (!file) return;

  // get the workspace out of the file
  RooWorkspace* w = (RooWorkspace*) file->Get(workspaceName);


  // get the modelConfig out of the file
  RooStats::ModelConfig* mc = (RooStats::ModelConfig*) w->obj(modelConfigName);

  // get the modelConfig out of the file
  RooAbsData* data = w->data(dataName);

  RooRealVar * nsig = w->var("nsig"); 
  if (nsig) nsig->setRange(0,200);

  MCMCCalculator mcmc(*data,*mc);
  mcmc.SetConfidenceLevel(0.683); // 68.3% interval

  // Define proposal function. This proposal function seems fairly robust
  SequentialProposal sp(0.1);
  mcmc.SetProposalFunction(sp);

  // set number of iterations and initial burning steps 
  mcmc.SetNumIters(100000);         // Metropolis-Hastings algorithm iterations
  mcmc.SetNumBurnInSteps(1000);       // first N steps to be ignored as burn-in

  // default is the shortest interval
  // here we use central interval
  mcmc.SetLeftSideTailFraction(0.5); // for central Bayesian interval
  //mcmc.SetLeftSideTailFraction(0); // for one-sided Bayesian interval

  // run the calculator
  MCMCInterval* interval = mcmc.GetInterval();

  // print out the iterval on the first Parameter of Interest
  RooRealVar* firstPOI = (RooRealVar*) mc->GetParametersOfInterest()->first();
  cout << "\n68% interval on " <<firstPOI->GetName()<<" is : ["<<     interval->LowerLimit(*firstPOI) << ", "<<     interval->UpperLimit(*firstPOI) <<"] "<<endl;


  // make a plot of posterior function
  new TCanvas("IntervalPlot");
  MCMCIntervalPlot plot(*interval);
  plot.Draw();

}
