#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstructor.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
:G4UserSteppingAction(),
fEventAction(eventAction),
fScoringVolume(0)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
if (!fScoringVolume) { 
	const DetectorConstructor* detectorConstructor
	= static_cast<const DetectorConstructor*>
	(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	fScoringVolume = detectorConstructor->GetScoringVolume();   
}

G4LogicalVolume* volume 
= step->GetPreStepPoint()->GetTouchableHandle()
->GetVolume()->GetLogicalVolume();

if (volume != fScoringVolume) return;

G4double edepStep = step->GetTotalEnergyDeposit();
fEventAction->AddEdep(edepStep);  
}
