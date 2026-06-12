# LABM Detector Simulation

Full GEANT4 Monte Carlo simulation of the **Large Angle Beamstrahlung Monitor (LABM)** — a beam diagnostic detector built for the [Belle II experiment](https://www.belle2.org/) at the SuperKEKB electron-positron collider in Tsukuba, Japan.

This simulation models the complete optical transport chain of the LABM: from beamstrahlung photon generation near the interaction point, through the beam pipe and mirror system, all the way to the photon detectors — with accurate material properties, surface optics, and magnetic field effects included.

---

## What it simulates

SuperKEKB collides electrons and positrons at extremely high luminosity. The beams produce beamstrahlung — synchrotron-like radiation emitted when the bunches interact — which the LABM captures and analyzes to monitor beam quality in real time.

This simulation reproduces the full photon path through the detector system:

1. **Particle generation** — synchrotron radiation photons are generated using the General Particle Source (`G4GPS`), configurable via macro files for energy spectrum, angular distribution, and beam current conditions
2. **Beam pipe geometry** — both the High Energy Ring (HER, electrons) and Low Energy Ring (LER, positrons) beam pipes are modeled in aluminum, including their crossing angle and the interaction region structure at the IP
3. **Primary mirror system** — beryllium thin mirrors positioned near the beam pipe in each of the four optical channels capture photons and redirect them into the optical transport system
4. **Optical transport** — four independent light guides (Oho Up, Oho Down, Nikko Up, Nikko Down) route photons through a series of enhanced aluminum mirrors and aluminum pipes, with realistic surface reflectivity and optical boundary conditions at every interface
5. **Collimation** — aluminum collimators near the detectors filter stray photons and reduce background
6. **Detection** — four copper photon detectors record hits, storing position (global and local), energy, and detector channel ID for each photon that reaches them
7. **Magnetic fields** — local dipole magnetic fields (~10 T and ~5.7 T) at the final focus quadrupole regions are included to correctly model photon emission angles from the beam

---

## Physics processes modeled

The physics list is fully custom-built and includes:

- **Electromagnetic processes**: photoelectric effect, Compton scattering, gamma conversion, Rayleigh scattering, electron/positron ionization, bremsstrahlung, multiple scattering, pair production
- **Synchrotron radiation**: both analytic and in-material variants, switchable at runtime via macro command
- **Optical physics**: Cerenkov radiation, scintillation, optical absorption, Rayleigh scattering of optical photons, Mie scattering, and full optical boundary process at every material interface
- **Decay processes**: particle decays handled for all applicable species

---

## Materials and optical properties

Material optical properties are defined over a 360-point photon energy spectrum from ~1.77 eV to ~3.54 eV (the visible and near-UV range relevant for beamstrahlung):

- **Standard aluminum** — real and imaginary refractive index + absorption length (beam pipe, structural components)
- **Enhanced aluminum** — higher-reflectivity coating for mirrors, with measured optical constants
- **Beryllium** — refractive index and absorption length for the thin primary mirrors near the beam pipe
- **Copper** — 100% photon detection efficiency for the sensitive detector volumes
- **Vacuum** — refractive index defined for proper optical transport in beam pipe regions

Surface models use GLISUR with polished finish (95% polish factor) for mirrors and ground finish for structural aluminum, matching measured surface properties of the real detector.

---

## Output

The simulation writes a ROOT file (`LABM.root`) containing:

- **1D histograms**: photon count per detector channel, photon energy spectrum
- **3D histograms**: global and local photon position distributions at the detector plane
- **Ntuple**: event-by-event record of detector channel, energy, global position (x, y, z), and local position (x, y, z) for each detected photon

This output is used to evaluate detector acceptance, optimize mirror alignment, and model radiation backgrounds in the interaction region.

---

## File structure

```
LABM_simulation/
├── Detector.cc                  ← Main entry point
├── GPSSIMULATION                ← Particle gun macro: dual-beam configuration
├── init_vis.mac                 ← Interactive mode setup and run initialization
├── vis.mac                      ← Visualization settings
├── src/
│   ├── DetectorConstructor.cc   ← Full geometry: beam pipe, mirrors, pipes, detectors
│   ├── PhysicsList.cc           ← Custom EM + optical + synchrotron physics
│   ├── PhysicsListMessenger.cc  ← Runtime physics configuration via macros
│   ├── PrimaryGeneratorAction.cc← Particle source (GPS)
│   ├── ActionInitialization.cc  ← Wires together all user actions
│   ├── RunAction.cc             ← ROOT file management, histogram booking
│   ├── EventAction.cc           ← Per-event processing and logging
│   ├── SteppingAction.cc        ← Step-level energy deposition tracking
│   ├── TrackerSD.cc             ← Sensitive detector: hit recording and analysis output
│   ├── TrackerHit.cc            ← Hit data structure
│   └── Run.cc                   ← Run-level data accumulation
├── include/                     ← Header files for all the above
└── build/                       ← Compile here (excluded from version control)
```

---

## Requirements

- [GEANT4](https://geant4.web.cern.ch/) 10.x or later (developed on 10.x series)
- [ROOT](https://root.cern/) (for output file writing via G4AnalysisManager)
- CMake 3.x or later
- C++11 compatible compiler

---

## How to build and run

```bash
# Create and enter the build directory
mkdir build && cd build

# Configure with CMake (point to your GEANT4 installation)
cmake ..

# Compile
make -j4

# Run in batch mode with the GPS macro
./Detector GPSSIMULATION

# Run in interactive mode with visualization
./Detector
```
The GPSSIMULATION macro configures the two-beam particle source that drives the simulation. It fires a 7 GeV electron beam and a 4 GeV positron beam simultaneously from opposite sides of the interaction point, each with a 0.5 mm ellipsoidal source volume matching the real SuperKEKB beam spot size. Visualization and trajectory storage are disabled for faster batch processing.

To switch between analytic and in-material synchrotron radiation:

```
/testem/phys/analyticSR true    # analytic (default)
/testem/phys/analyticSR false   # in-material
```

To adjust Cerenkov photon production:

```
/testem/phys/cerenkovMaxPhotons 20
```

---

## Context

This simulation was developed as part of the LABM detector R&D program at KEK, supporting the Belle II experiment's beam background monitoring strategy. The four optical channels — named after their geographic locations relative to the interaction point (Oho Up/Down, Nikko Up/Down) — each route photons from a primary beryllium mirror near the beam pipe through a multi-mirror optical system to a shielded detector approximately 5 meters away.

The simulation was used to optimize mirror positions, evaluate photon acceptance as a function of beam conditions, and estimate backgrounds from non-signal photon sources in the interaction region.

---

## Skills demonstrated

`C++` · `GEANT4` · `Monte Carlo simulation` · `Optical transport modeling` · `Detector geometry design` · `Physics list development` · `ROOT data analysis` · `Radiation transport` · `Scientific computing` · `Experimental physics instrumentation`

---

## Author

**Daniel Enrique Ricalde Herrmann**  
PhD in Physics — Wayne State University (2024)  
daniel.ricalde.h@gmail.com
