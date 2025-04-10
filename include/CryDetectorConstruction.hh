#ifndef SnowSimDetectorConstruction_h
#define SnowSimDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class CryDetectorMessenger;

/// Detector construction class to define materials and geometry.

class CryDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    CryDetectorConstruction();
    ~CryDetectorConstruction() override = default;

    G4VPhysicalVolume *Construct() override;
    void SetRockMaterial(G4String rockMaterial);
    void SetSnowDepth(G4double snowDepth);
    void SetRockThickness(G4double rockThickness);

private:
    G4double fSnowDepth = 0.0;
    G4double fRockThickness = 0.0;
    G4double fAirHeight = 0.0;
    G4double fAirZ = 0.0;
    G4double fTubeGroundOffset = 0.0;
    CryDetectorMessenger *fMessenger = nullptr;
};

#endif