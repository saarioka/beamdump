#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

namespace B2 {

class PhysicsList : public G4VModularPhysicsList {
   public:
    PhysicsList();

    void SetCuts() override;
    void AddStepMax();

   private:
    G4double fGammaCut;
    G4double fElectronCut;
    G4VPhysicsConstructor* fEmPhysicsList;
    G4String fEmName;
};

}

#endif
