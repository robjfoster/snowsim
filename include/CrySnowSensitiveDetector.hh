#ifndef CrySnowSensitiveDetector_h
#define CrySnowSensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class CrySnowSensitiveDetector : public G4VSensitiveDetector
{
public:
    CrySnowSensitiveDetector(G4String);
    ~CrySnowSensitiveDetector() = default;

    void Initialize(G4HCofThisEvent *);
    G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    void EndOfEvent(G4HCofThisEvent *);

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
};

#endif