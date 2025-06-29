#ifndef CryTubeSensitiveDetector_h
#define CryTubeSensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "CrySnowSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class CrySnowSensitiveDetector;

class CryTubeSensitiveDetector : public G4VSensitiveDetector
{
public:
    CryTubeSensitiveDetector(G4String, CrySnowSensitiveDetector *snowSD);
    ~CryTubeSensitiveDetector() = default;

    void Initialize(G4HCofThisEvent *);
    G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    void EndOfEvent(G4HCofThisEvent *);

private:
    G4int fEid;
    G4double fEk;
    G4double fPosx;
    G4double fPosy;
    G4double fPosz;
    G4double fDirx;
    G4double fDiry;
    G4double fDirz;
    G4double fTglobal;
    G4double fTlocal;
    G4double fTproper;
    G4bool fHit;

    CrySnowSensitiveDetector *fSnowSD;
};

#endif