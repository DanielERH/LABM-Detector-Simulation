#include "DetectorConstructor.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4SDManager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

int main(int argc,char** argv)
{
// Detect interactive mode (if no arguments) and define UI session
G4UIExecutive* ui = 0;
if ( argc == 1 ) {
ui = new G4UIExecutive(argc, argv);
}

// Choose the Random engine
G4Random::setTheEngine(new CLHEP::RanecuEngine);

// Construct the default run manager
#ifdef G4MULTITHREADED
G4MTRunManager* runManager = new G4MTRunManager;
#else
G4RunManager* runManager = new G4RunManager;
#endif

// Set mandatory initialization classes

// Detector constructor
runManager->SetUserInitialization(new DetectorConstructor());

// Physics list
runManager->SetUserInitialization(new PhysicsList);

// User action initialization
runManager->SetUserInitialization(new ActionInitialization());

// Initialize visualization
G4VisManager* visManager = new G4VisExecutive;
visManager->Initialize();

// Get the pointer to the User Interface manager
G4UImanager* UImanager = G4UImanager::GetUIpointer();

// Process macro or start UI session
if ( ! ui ) {
// batch mode
G4String command = "/control/execute ";
G4String fileName = argv[1];
UImanager->ApplyCommand(command+fileName);
}
else {
// interactive mode
UImanager->ApplyCommand("/control/execute init_vis.mac");
ui->SessionStart();
delete ui;
}

// Job termination
delete visManager;
delete runManager;
}
