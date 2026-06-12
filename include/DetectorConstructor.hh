#ifndef DetectorConstructor_h
#define DetectorConstructor_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstructor : public G4VUserDetectorConstruction
{
public:
	DetectorConstructor();
	virtual ~DetectorConstructor();

	virtual G4VPhysicalVolume* Construct();

G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

protected:
	G4LogicalVolume*  fScoringVolume;
};

#endif
