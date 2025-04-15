#ifndef SnowSimDetectorConstruction_h
#define SnowSimDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

#include "G4SystemOfUnits.hh"

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
    void SetEmbedTube(G4bool embedTube);

private:
    G4double fWorldXY = 300 * m + 1 * cm;
    G4double fWorldZ = 300 * m + 1 * cm;
    G4double fAirXY = 300 * m;
    G4double fSnowXY = 300 * m;
    G4double fTubeZ = 1.5 * m;
    G4double fTubeXY = 1.5 * m;
    G4double fTubeRadius = 15 * cm;
    G4double fSnowDepth = 0.0;
    G4double fRockThickness = 0.0;
    G4double fAirHeight = 0.0;
    G4double fAirZ = 0.0;
    G4double fTubeGroundOffset = 0.0;
    G4bool fEmbedTube = false;
    CryDetectorMessenger *fMessenger = nullptr;
};

#endif