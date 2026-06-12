#include "RunAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

RunAction::RunAction()
:G4UserRunAction()
{ 
G4RunManager::GetRunManager()->SetPrintProgress(1000);

G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

analysisManager->SetVerboseLevel(1);
analysisManager->SetFirstHistoId(1);
analysisManager->CreateH1("Detector","Detector Number", 10, 0., 4);
analysisManager->CreateH1("Energy","Photon Energy", 100, 0*eV, 1000*eV);
analysisManager->CreateH3("GlobalPosition","Photon GlobalPosition", 100, -1000*cm, 1000*cm, 100, -1000*cm, 1000*cm, 100, -1000*cm, 1000*cm);
analysisManager->CreateH3("LocalPosition","Photon LocalPosition", 100, -1000*cm, 1000*cm, 100, -1000*cm, 1000*cm, 100, -1000*cm, 1000*cm);
analysisManager->CreateNtuple("Detector, Energy, position", "Detector, Photon Energy, Photon Position");
analysisManager->CreateNtupleDColumn("DetectorNumber");
analysisManager->CreateNtupleDColumn("Energy");
analysisManager->CreateNtupleDColumn("GlobalPositionx");
analysisManager->CreateNtupleDColumn("GlobalPositiony");
analysisManager->CreateNtupleDColumn("GlobalPositionz");
analysisManager->CreateNtupleDColumn("LocalPositionx");
analysisManager->CreateNtupleDColumn("LocalPositiony");
analysisManager->CreateNtupleDColumn("LocalPositionz"); 
analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
delete G4AnalysisManager::Instance();  
}

void RunAction::BeginOfRunAction(const G4Run*)
{ 
G4RunManager::GetRunManager()->SetRandomNumberStore(false);

G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
G4cout << "Using " << analysisManager->GetType() << G4endl;

G4String fileName = "LABM.root";
analysisManager->OpenFile(fileName);
}

void RunAction::EndOfRunAction(const G4Run*)
{
G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
analysisManager->Write();
analysisManager->CloseFile();
}
