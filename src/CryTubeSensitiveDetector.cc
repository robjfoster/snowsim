#include "CryTubeSensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4CsvAnalysisManager.hh"

CryTubeSensitiveDetector::CryTubeSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    collectionName.insert("CryTubeHitsCollection");
}

void CryTubeSensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
    fEid = -1;
    fEk = -1;
    fPosx = -1;
    fPosy = -1;
    fPosz = -1;
    fDirx = -1;
    fDiry = -1;
    fDirz = -1;
    fTglobal = -1;
    fTlocal = -1;
    fTproper = -1;
    fHit = false;
}

G4bool CryTubeSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *history)
{
    auto pdg = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    if (pdg != 2112)
    {
        return false;
    }
    G4cout << "Saw neutron!" << G4endl;
    fHit = true;
    fEid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    fEk = step->GetTrack()->GetKineticEnergy();
    auto pos = step->GetTrack()->GetPosition();
    fPosx = pos.x();
    fPosy = pos.y();
    fPosz = pos.z();
    auto dir = step->GetTrack()->GetMomentumDirection();
    fDirx = dir.x();
    fDiry = dir.y();
    fDirz = dir.z();
    fTglobal = step->GetTrack()->GetGlobalTime();
    fTlocal = step->GetTrack()->GetLocalTime();
    fTproper = step->GetTrack()->GetProperTime();

    step->GetTrack()->SetTrackStatus(fStopAndKill);
    return true;
}

void CryTubeSensitiveDetector::EndOfEvent(G4HCofThisEvent *hce)
{
    if (!fHit)
    {
        return;
    }
    auto am = G4CsvAnalysisManager::Instance();
    am->FillNtupleIColumn(0, 0, fEid);
    am->FillNtupleDColumn(0, 1, fEk);
    am->FillNtupleDColumn(0, 2, fPosx);
    am->FillNtupleDColumn(0, 3, fPosy);
    am->FillNtupleDColumn(0, 4, fPosz);
    am->FillNtupleDColumn(0, 5, fDirx);
    am->FillNtupleDColumn(0, 6, fDiry);
    am->FillNtupleDColumn(0, 7, fDirz);
    am->FillNtupleDColumn(0, 8, fTglobal);
    am->FillNtupleDColumn(0, 9, fTlocal);
    am->FillNtupleDColumn(0, 10, fTproper);
    am->AddNtupleRow(0);
}
