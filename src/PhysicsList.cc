#include "PhysicsList.hh"

#include <G4UnitsTable.hh>
#include <iomanip>

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4ParticleWithCuts.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4SystemOfUnits.hh"

namespace B2 {

PhysicsList::PhysicsList() : G4VUserPhysicsList() {
    SetVerboseLevel(1);

    fGammaCut = PhysicsList::GetDefaultCutValue();
    fElectronCut = PhysicsList::GetDefaultCutValue();

    // Default physics
    RegisterPhysics(new G4DecayPhysics());

    // EM physics
    //RegisterPhysics(new G4EmStandardPhysics_option4());
    RegisterPhysics(new G4EmLivermorePhysics());

    // Add step Max
    //AddStepMax();
}

void PhysicsList::SetCuts() {
    if (PhysicsList::GetVerboseLevel() > 0) {
        G4cout << "PhysicsList::SetCuts: defaultCut = " << G4BestUnit(defaultCutValue, "Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second,
    // because some processes for e- need cut values for gamma
    SetCutValue(fGammaCut, "gamma");
    SetCutValue(fElectronCut, "e-");

    DumpCutValuesTable();
}

void PhysicsList::AddStepMax() {
    // Step limitation
    G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics();
    stepLimitPhys->SetApplyToAll(true);  // activates step limit for ALL particles
    RegisterPhysics(stepLimitPhys);
}
}
