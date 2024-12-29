#include "createNtuple.hh"

CreateNtuple::CreateNtuple() {

	G4AnalysisManager *manager = G4AnalysisManager::Instance();

	manager->CreateNtuple("generator", "generator");
	manager->CreateNtupleDColumn("muonMomentum");
	manager->FinishNtuple(0);

	manager->CreateNtuple("events", "events");
	manager->CreateNtupleIColumn("eventNumber");
	manager->CreateNtupleDColumn("muonMomentum");  // GeV
	manager->CreateNtupleDColumn("hitTime");  // ns
	manager->CreateNtupleDColumn("hitPositionX_truth");  // mm
	manager->CreateNtupleDColumn("hitPositionY_truth");
	manager->CreateNtupleDColumn("hitPositionZ_truth");
	manager->CreateNtupleDColumn("hitPixelX");
	manager->CreateNtupleDColumn("hitPixelY");
	manager->CreateNtupleDColumn("hitPixelZ");
	manager->FinishNtuple(1);

}
CreateNtuple::~CreateNtuple() {}

void CreateNtuple::BeginOfRunAction(const G4Run *run) {

	G4AnalysisManager *manager = G4AnalysisManager::Instance();

	G4int runID = run->GetRunID();

	std::stringstream strRunID;
	strRunID << runID;

	G4String fileName = "output_"+strRunID.str()+".root";
	manager->OpenFile(fileName);
}

void CreateNtuple::EndOfRunAction(const G4Run *) {

	G4AnalysisManager *manager = G4AnalysisManager::Instance();
	manager->Write();
	manager->CloseFile();

}