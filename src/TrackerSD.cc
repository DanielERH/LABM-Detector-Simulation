#include "TrackerSD.hh"

#include "G4Step.hh"
#include "Randomize.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleDefinition.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "Analysis.hh"

TrackerSD::TrackerSD(const G4String Detector, const G4String& hitsCollection) 
:G4VSensitiveDetector(Detector),
fHitsCollection(0)
{
collectionName.insert(hitsCollection);
}

TrackerSD::~TrackerSD() 
{}

void TrackerSD::Initialize(G4HCofThisEvent* hce)
{
fHitsCollection = new TrackerHitsCollection(GetName(), collectionName[0]); 
G4int hcID = GetCollectionID(0);
hce->AddHitsCollection( hcID, fHitsCollection ); 
}

G4bool TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{  
G4double pos1 = 0;
G4double pos2 = 0;
G4double pos3 = 0;
G4int detecNum = 0;
G4double localpos1 = 0;
G4double localpos2 = 0;
G4double localpos3 = 0;

G4String DetectorVolume = aStep->GetTrack()->GetVolume()->GetName();

if ((DetectorVolume == "physCubiertaFinalOho") or (DetectorVolume == "physCubiertaFinalNikko")) {
	aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	return false;
}

G4double edep = aStep->GetPreStepPoint()->GetKineticEnergy();
G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
aStep->GetTrack()->SetTrackStatus(fStopAndKill);

TrackerHit* newHit = new TrackerHit();
newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
newHit->SetEdep(edep);
newHit->SetPos (pos);
fHitsCollection->insert( newHit );
pos1 = pos.x();
pos2 = pos.y();
pos3 = pos.z();
G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
G4ThreeVector worldPosition = preStepPoint->GetPosition();
G4ThreeVector localPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
localpos1 = localPosition.x();
localpos2 = localPosition.y();
localpos3 = localPosition.z();

if ((-558*cm<pos1) & (pos1<-544*cm)) detecNum = 1; //OHO UP
if ((-578*cm<pos1) & (pos1<-564*cm)) detecNum = 2; //OHO DOWN
if ((679*cm<pos1) & (pos1<693*cm)) detecNum = 3; //NIKKO UP
if ((698*cm<pos1) & (pos1<712*cm)) detecNum = 4; //NIKKO DOWN

G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

analysisManager->FillH1(1, detecNum);
analysisManager->FillH1(2, edep);
analysisManager->FillH3(1, pos1, pos2, pos3);
analysisManager->FillH3(2, localpos1, localpos2, localpos3);

analysisManager->FillNtupleDColumn(0, detecNum);
analysisManager->FillNtupleDColumn(1, edep);
analysisManager->FillNtupleDColumn(2, pos1);
analysisManager->FillNtupleDColumn(3, pos2);
analysisManager->FillNtupleDColumn(4, pos3);
analysisManager->FillNtupleDColumn(5, localpos1);
analysisManager->FillNtupleDColumn(6, localpos2);
analysisManager->FillNtupleDColumn(7, localpos3);
analysisManager->AddNtupleRow(); 

newHit->Print();

return true;
}

void TrackerSD::EndOfEvent(G4HCofThisEvent*)
{
//if ( verboseLevel>0) { 
	G4int nofHits = fHitsCollection->entries();
	G4cout << G4endl
	<< "-------->Hits Collection LABM: in this event there are " << nofHits
	<< " hits in the tracker chambers: " << G4endl;
	for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
//}
}
