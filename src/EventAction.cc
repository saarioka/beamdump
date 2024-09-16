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
/// \file B2/B2b/src/EventAction.cc
/// \brief Implementation of the B2::EventAction class

#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
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
  // get number of stored trajectories

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing

  G4int eventID = event->GetEventID();
  if ( eventID % 10000000 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(2);
    G4cout << "    " << hc->GetSize() << " hits stored in this event" << G4endl;
  }

  // Moderator
  G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
  G4int nHit = hc->GetSize();
  bool firstNeutron = true;
  if (nHit > 0) {
    auto analysisManager = G4AnalysisManager::Instance();

    for (G4int i=0; i<nHit; i++){
      auto hit = dynamic_cast<TrackerHit*>(hc->GetHit(i));
      G4String particleName = hit->GetParticleName();
      G4double E = hit->GetE();
      G4double Edep = hit->GetEdep();
      G4ThreeVector pos = hit->GetPos();
      G4int chamberNb = hit->GetChamberNb();

      if (firstNeutron && particleName == "neutron") {
        analysisManager->FillH1(5, E / keV);
        analysisManager->FillNtupleDColumn(0, E / keV);
        firstNeutron = false;
      } 

      analysisManager->FillH1(1, Edep / keV);
      analysisManager->FillH1(2, pos.x() / cm);
      analysisManager->FillH1(3, pos.y() / cm);
      analysisManager->FillH1(4, pos.z() / cm);

      analysisManager->FillNtupleDColumn(1, Edep / keV);
      analysisManager->FillNtupleDColumn(2, pos.x() / cm);
      analysisManager->FillNtupleDColumn(3, pos.y() / cm);
      analysisManager->FillNtupleDColumn(4, pos.x() / cm);
      analysisManager->FillNtupleIColumn(5, eventID);
      analysisManager->FillNtupleIColumn(6, chamberNb);
      analysisManager->AddNtupleRow();
    }
  }

  // Berthold
  hc = event->GetHCofThisEvent()->GetHC(2);
  nHit = hc->GetSize();
  firstNeutron = true;
  if (nHit > 0) {
    auto analysisManager = G4AnalysisManager::Instance();

    for (G4int i=0; i<nHit; i++){
      auto hit = dynamic_cast<TrackerHit*>(hc->GetHit(i));
      G4String particleName = hit->GetParticleName();
      G4double E = hit->GetE();
      G4double Edep = hit->GetEdep();
      G4ThreeVector pos = hit->GetPos();
      G4int chamberNb = hit->GetChamberNb();

      if (firstNeutron && particleName == "neutron") {
        analysisManager->FillH1(0, E / keV);
        analysisManager->FillNtupleDColumn(0, E / keV);
        firstNeutron = false;
      } 

      analysisManager->FillH1(1, Edep / keV);
      analysisManager->FillH1(2, pos.x() / cm);
      analysisManager->FillH1(3, pos.y() / cm);
      analysisManager->FillH1(4, pos.z() / cm);

      analysisManager->FillNtupleDColumn(1, Edep / keV);
      analysisManager->FillNtupleDColumn(2, pos.x() / cm);
      analysisManager->FillNtupleDColumn(3, pos.y() / cm);
      analysisManager->FillNtupleDColumn(4, pos.x() / cm);
      analysisManager->FillNtupleIColumn(5, eventID);
      analysisManager->FillNtupleIColumn(6, chamberNb);
      analysisManager->AddNtupleRow();
    }
  }

  // Scorer1
  hc = event->GetHCofThisEvent()->GetHC(3);
  nHit = hc->GetSize();
  firstNeutron = true;
  if (nHit > 0) {
    auto analysisManager = G4AnalysisManager::Instance();

    for (G4int i=0; i<nHit; i++){
      auto hit = dynamic_cast<TrackerHit*>(hc->GetHit(i));
      G4String particleName = hit->GetParticleName();
      G4double E = hit->GetE();
      G4double Edep = hit->GetEdep();
      G4ThreeVector pos = hit->GetPos();
      G4int chamberNb = hit->GetChamberNb();

      if (firstNeutron && particleName == "neutron") {
        analysisManager->FillH1(6, E / keV);
        analysisManager->FillNtupleDColumn(0, E / keV);
        firstNeutron = false;
      } 

      analysisManager->FillH1(1, Edep / keV);
      analysisManager->FillH1(2, pos.x() / cm);
      analysisManager->FillH1(3, pos.y() / cm);
      analysisManager->FillH1(4, pos.z() / cm);

      analysisManager->FillNtupleDColumn(1, Edep / keV);
      analysisManager->FillNtupleDColumn(2, pos.x() / cm);
      analysisManager->FillNtupleDColumn(3, pos.y() / cm);
      analysisManager->FillNtupleDColumn(4, pos.x() / cm);
      analysisManager->FillNtupleIColumn(5, eventID);
      analysisManager->FillNtupleIColumn(6, chamberNb);
      analysisManager->AddNtupleRow();
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}

