#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* physL)
:G4UImessenger(),fPhysList(physL),
fPhysDir(0),
fSRTypeCmd(0)
{
fPhysDir = new G4UIdirectory("/testem/phys/");
fPhysDir->SetGuidance("physics list commands");

fSRTypeCmd = new G4UIcmdWithABool("/testem/phys/analyticSR",this);
fSRTypeCmd->SetGuidance("choose analytic synchrotron radiation");
fSRTypeCmd->SetParameterName("SRType",true);
fSRTypeCmd->SetDefaultValue(true);

fVerboseCmd = new G4UIcmdWithAnInteger("/testem/phys/verbose",this);
fVerboseCmd->SetGuidance("set verbose for physics processes");
fVerboseCmd->SetParameterName("verbose",true);
fVerboseCmd->SetDefaultValue(1);
fVerboseCmd->SetRange("verbose>=0");
fVerboseCmd->AvailableForStates(G4State_PreInit);

fCerenkovCmd = new G4UIcmdWithAnInteger("/testem/phys/cerenkovMaxPhotons",this);
fCerenkovCmd->SetGuidance("set max nb of photons per step");
fCerenkovCmd->SetParameterName("MaxNumber",false);
fCerenkovCmd->SetRange("MaxNumber>=0");
fCerenkovCmd->AvailableForStates(G4State_PreInit);
}

PhysicsListMessenger::~PhysicsListMessenger()
{
delete fSRTypeCmd;
delete fPhysDir;
delete fVerboseCmd;
delete fCerenkovCmd;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
if (command == fSRTypeCmd)
{ fPhysList->SetAnalyticSR(fSRTypeCmd->GetNewBoolValue(newValue));}

if( command == fVerboseCmd )
{fPhysList->SetVerbose(fVerboseCmd->GetNewIntValue(newValue));}

if( command == fCerenkovCmd )
{fPhysList->SetNbOfPhotonsCerenkov(fCerenkovCmd->GetNewIntValue(newValue));}
}
