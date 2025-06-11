#include "CrySnowSensitiveDetector.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4CsvAnalysisManager.hh"

CrySnowSensitiveDetector::CrySnowSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    collectionName.insert("CrySnowHitsCollection");
}

void CrySnowSensitiveDetector::Initialize(G4HCofThisEvent *hce)
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

G4bool CrySnowSensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *history)
{
    if (fHit)
    {
        // we've already seen this neutron in the snow
        return false;
    }

    auto pdg = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    if (pdg != 2112)
    {
        return false;
    }

    if (step->GetPreStepPoint()->GetStepStatus() != fGeomBoundary)
    {
        return false;
    }

    fHit = true;
    fEid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    fEk = step->GetTrack()->GetKineticEnergy();
    auto pos = step->GetPreStepPoint()->GetPosition();
    fPosx = pos.x();
    fPosy = pos.y();
    fPosz = pos.z();
    auto dir = step->GetPreStepPoint()->GetMomentumDirection();
    fDirx = dir.x();
    fDiry = dir.y();
    fDirz = dir.z();
    fTglobal = step->GetPreStepPoint()->GetGlobalTime();
    fTlocal = step->GetPreStepPoint()->GetLocalTime();
    fTproper = step->GetPreStepPoint()->GetProperTime();

    return true;
}

void CrySnowSensitiveDetector::EndOfEvent(G4HCofThisEvent *hce)
{
}
