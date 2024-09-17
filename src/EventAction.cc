//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B2/B2a/src/EventAction.cc
/// \brief Implementation of the B2::EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4AnalysisManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "TrackerHit.hh"
namespace B2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  auto analysisManager = G4AnalysisManager::Instance();

  // get number of stored trajectories

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  std::size_t n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing

  /*
  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "
           << hc->GetSize() << " hits stored in this event" << G4endl;
  }
  */

  // iterate over hits collections

  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  G4int n_collections = HCE->GetNumberOfCollections();
  for (G4int i=0; i<n_collections; i++) {
    G4VHitsCollection* hc = HCE->GetHC(i);
    G4String hcName = hc->GetName();
    G4int n_hit = hc->GetSize();
    if ( n_hit > 0 ) {
      //G4cout << "    Hits Collection " << i <<": " << hcName << " with " << n_hit << " hits" << G4endl;
      for (G4int j=0; j<n_hit; j++) {
        auto hit = static_cast<TrackerHit*>(hc->GetHit(j));
        auto E = hit->GetE();
        G4int evt = event->GetEventID();
        G4int det = hit->GetChamberNb();

        analysisManager->FillH1(det, E / keV);

        analysisManager->FillNtupleDColumn(0, E / keV);
        analysisManager->FillNtupleIColumn(1, det);
        analysisManager->FillNtupleIColumn(2, evt);
        analysisManager->AddNtupleRow();

      }
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

