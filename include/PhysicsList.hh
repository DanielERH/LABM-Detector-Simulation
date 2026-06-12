#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PhysicsListMessenger;
class G4SynchrotronRadiation;

class PhysicsList: public G4VUserPhysicsList
{
public:
	PhysicsList();
	virtual ~PhysicsList();

	virtual void ConstructParticle();

	virtual void ConstructProcess();

	virtual void SetCuts();

	inline void SetAnalyticSR(G4bool val) {fSRType = val;};

	void ConstructDecay();
	void ConstructEM();
	void ConstructOp();

	void SetVerbose(G4int);
	void SetNbOfPhotonsCerenkov(G4int);

private:

	void ConstructBosons();
	void ConstructLeptons();

	void ConstructGeneral();

	G4int                	fVerboseLebel;
	G4bool                  fSRType;
	PhysicsListMessenger*   fMess;
	G4int			fMaxNumPhotonStep;
};
#endif
