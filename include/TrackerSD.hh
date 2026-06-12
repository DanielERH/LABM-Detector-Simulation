#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "G4VSensitiveDetector.hh"
class DetectorConstruction;
class RunAction;
 
#include "TrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class TrackerSD : public G4VSensitiveDetector
{
public:
	TrackerSD(G4String Detector,
	const G4String& hitsCollection);
	~TrackerSD();
public:
	G4bool	ProcessHits(G4Step* step, G4TouchableHistory* history);
	void	Initialize(G4HCofThisEvent* hce);
	void	EndOfEvent(G4HCofThisEvent* hce);

private:
	TrackerHitsCollection*	fHitsCollection;
};

#endif
