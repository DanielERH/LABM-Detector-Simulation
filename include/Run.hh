#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "globals.hh"

class G4Event;

class Run : public G4Run
{
public:
	Run();
	virtual ~Run();
	virtual void Merge(const G4Run*);
	void AddEdep (G4double edep); 
	G4double GetEdep()  const { return fEdep; }
	G4double GetEdep2() const { return fEdep2; }

private:
	G4double  fEdep;
	G4double  fEdep2;
};
#endif
