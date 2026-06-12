#include "globals.hh"
#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4RayleighScattering.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4SynchrotronRadiation.hh"
#include "G4SynchrotronRadiationInMat.hh"
#include "G4StepLimiter.hh"
#include "G4SystemOfUnits.hh"
#include "G4AutoDelete.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

PhysicsList::PhysicsList()
:G4VUserPhysicsList(),
	fVerboseLebel(1), fMaxNumPhotonStep(20), fMess(0)
{
SetDefaultCutValue(1.*km);
fSRType = true;
fMess = new PhysicsListMessenger(this);
}

PhysicsList::~PhysicsList()
{
delete fMess;
}

void PhysicsList::ConstructParticle()
{
ConstructBosons();
ConstructLeptons();
G4PionPlus::PionPlusDefinition();
G4PionMinus::PionMinusDefinition();
G4KaonPlus::KaonPlusDefinition();
G4KaonMinus::KaonMinusDefinition();
G4Proton::ProtonDefinition();
G4AntiProton::AntiProtonDefinition();
G4GenericIon::GenericIonDefinition();
}

void PhysicsList::ConstructBosons()
{
G4Geantino::GeantinoDefinition();
G4ChargedGeantino::ChargedGeantinoDefinition();
G4Gamma::GammaDefinition();
G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsList::ConstructLeptons()
{
G4Electron::ElectronDefinition();
G4Positron::PositronDefinition();
G4MuonPlus::MuonPlusDefinition();
G4MuonMinus::MuonMinusDefinition();
G4NeutrinoE::NeutrinoEDefinition();
G4AntiNeutrinoE::AntiNeutrinoEDefinition();
G4NeutrinoMu::NeutrinoMuDefinition();
G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void PhysicsList::ConstructProcess()
{
AddTransportation();
ConstructDecay();
ConstructEM();
ConstructGeneral();
ConstructOp();
}

#include "G4Decay.hh"

void PhysicsList::ConstructDecay()
{
G4Decay* theDecayProcess = new G4Decay();
auto theParticleIterator=GetParticleIterator();
theParticleIterator->reset();
while( (*theParticleIterator)() ){
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	if (theDecayProcess->IsApplicable(*particle)) {
		pmanager ->AddProcess(theDecayProcess);
		pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
		pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
	}
}
}

void PhysicsList::ConstructEM()
{
auto theParticleIterator=GetParticleIterator();
theParticleIterator->reset();
G4SynchrotronRadiation* fSync = new G4SynchrotronRadiation();
G4AutoDelete::Register(fSync);
while( (*theParticleIterator)() ){
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4String particleName = particle->GetParticleName();

	if (particleName == "gamma") {
		pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
		pmanager->AddDiscreteProcess(new G4ComptonScattering);
		pmanager->AddDiscreteProcess(new G4GammaConversion);
		pmanager->AddDiscreteProcess(new G4RayleighScattering);
	} else if (particleName == "e-") {
		pmanager->AddProcess(new G4eMultipleScattering,       -1, 1, 1);
		pmanager->AddProcess(new G4eIonisation,               -1, 2, 2);
		pmanager->AddProcess(new G4eBremsstrahlung,           -1, 3, 3);
		if (fSRType) {
			pmanager->AddProcess(fSync,    -1,-1, 4);
		} else {
			pmanager->AddProcess(new G4SynchrotronRadiationInMat,-1,-1, 4);
		}
		pmanager->AddProcess(new G4StepLimiter,               -1,-1, 5);
	} else if (particleName == "e+") {
	pmanager->AddProcess(new G4eMultipleScattering,       -1, 1, 1);
	pmanager->AddProcess(new G4eIonisation,               -1, 2, 2);
	pmanager->AddProcess(new G4eBremsstrahlung,           -1, 3, 3);
	pmanager->AddProcess(new G4eplusAnnihilation,          0,-1, 4);
		if (fSRType) {
			pmanager->AddProcess(fSync,      -1,-1, 5);
		} else {
			pmanager->AddProcess(new G4SynchrotronRadiationInMat, -1,-1, 5);
		}
		pmanager->AddProcess(new G4StepLimiter,               -1,-1, 6);
	} else if(particleName == "mu+" || particleName == "mu-") {
		pmanager->AddProcess(new G4MuMultipleScattering, -1, 1, 1);
		pmanager->AddProcess(new G4MuIonisation,         -1, 2, 2);
		pmanager->AddProcess(new G4MuBremsstrahlung,     -1, 3, 3);
		pmanager->AddProcess(new G4MuPairProduction,     -1, 4, 4);
		pmanager->AddProcess(fSync, -1,-1, 5);
		pmanager->AddProcess(new G4StepLimiter,               -1,-1, 6);
	} else if( particleName == "proton") {
		pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
		pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
		pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
		pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);
		pmanager->AddProcess(fSync, -1,-1, 5);
		pmanager->AddProcess(new G4StepLimiter,          -1,-1, 6);
	} else {
		if ((particle->GetPDGCharge() != 0.0) &&
		(particle->GetParticleName() != "chargedgeantino") &&
		!particle->IsShortLived()) {
			pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
			pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
			pmanager->AddProcess(fSync,-1,-1, 1);
		}
	}
}
}

#include "G4Decay.hh"

void PhysicsList::ConstructGeneral()
{
G4Decay* theDecayProcess = new G4Decay();
auto theParticleIterator=GetParticleIterator();
theParticleIterator->reset();
while ((*theParticleIterator)()){
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
		if (theDecayProcess->IsApplicable(*particle)) {
			pmanager ->AddProcess(theDecayProcess);
			pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
			pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
		}
}
}

#include "G4Threading.hh"

void PhysicsList::ConstructOp()
{
G4Cerenkov* cerenkovProcess = new G4Cerenkov("Cerenkov");
cerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
cerenkovProcess->SetMaxBetaChangePerStep(10.0);
cerenkovProcess->SetTrackSecondariesFirst(true);
G4Scintillation* scintillationProcess = new G4Scintillation("Scintillation");
//scintillationProcess->SetScintillationYieldFactor(1.);
scintillationProcess->SetTrackSecondariesFirst(true);
G4OpAbsorption* absorptionProcess = new G4OpAbsorption();
G4OpRayleigh* rayleighScatteringProcess = new G4OpRayleigh();
G4OpMieHG* mieHGScatteringProcess = new G4OpMieHG();
G4OpBoundaryProcess* boundaryProcess = new G4OpBoundaryProcess();

cerenkovProcess->SetVerboseLevel(fVerboseLebel);
scintillationProcess->SetVerboseLevel(fVerboseLebel);
absorptionProcess->SetVerboseLevel(fVerboseLebel);
rayleighScatteringProcess->SetVerboseLevel(fVerboseLebel);
mieHGScatteringProcess->SetVerboseLevel(fVerboseLebel);
boundaryProcess->SetVerboseLevel(fVerboseLebel);

if(!G4Threading::IsWorkerThread())
{
	G4EmSaturation* emSaturation =
	G4LossTableManager::Instance()->EmSaturation();
	scintillationProcess->AddSaturation(emSaturation);
}

auto theParticleIterator=GetParticleIterator();
theParticleIterator->reset();
while( (*theParticleIterator)() ){
	G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
	G4String particleName = particle->GetParticleName();
	if (cerenkovProcess->IsApplicable(*particle)) {
		pmanager->AddProcess(cerenkovProcess);
		pmanager->SetProcessOrdering(cerenkovProcess,idxPostStep);
	}
	if (scintillationProcess->IsApplicable(*particle)) {      pmanager->AddProcess(scintillationProcess);
		pmanager->SetProcessOrderingToLast(scintillationProcess, idxAtRest);
		pmanager->SetProcessOrderingToLast(scintillationProcess, idxPostStep);
	}
	if (particleName == "opticalphoton") {
		G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
		pmanager->AddDiscreteProcess(absorptionProcess);
		pmanager->AddDiscreteProcess(rayleighScatteringProcess);
		pmanager->AddDiscreteProcess(mieHGScatteringProcess);
		pmanager->AddDiscreteProcess(boundaryProcess);
	}
}
}

void PhysicsList::SetVerbose(G4int verbose)
{
fVerboseLebel = verbose;
}

void PhysicsList::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{
fMaxNumPhotonStep = MaxNumber;
}

void PhysicsList::SetCuts()
{
SetCutsWithDefault();
if (verboseLevel>0) DumpCutValuesTable();
}
