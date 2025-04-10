#include "CryDetectorConstruction.hh"
#include "CryDetectorMessenger.hh"
#include "CryTubeSensitiveDetector.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

CryDetectorConstruction::CryDetectorConstruction()
{
    fMessenger = new CryDetectorMessenger(this);
}

// CryDetectorConstruction::~CryDetectorConstruction() {}

G4VPhysicalVolume *CryDetectorConstruction::Construct()
{
    fSnowDepth = 10 * cm;

    G4double worldXY = 300 * m + 1 * cm;
    G4double worldZ = 300 * m + 1 * cm;
    G4double airXY = 300 * m;
    fAirHeight = 150 * m;
    fRockThickness = 2 * m;
    fAirZ = fAirHeight + fRockThickness;
    G4double snowXY = 300 * m;
    G4double tubeZ = 1.5 * m;
    G4double tubeRadius = 15 * cm;
    fTubeGroundOffset = 2 * m;

    auto worldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    auto airMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto rockMaterial = new G4Material("rock", 2.71 * g / cm3, 2);
    rockMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_CALCIUM_CARBONATE"), 0.9);
    rockMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_MAGNESIUM_CARBONATE"), 0.1);
    auto snowMaterial = new G4Material("snow", 250 * kg / m3, 1);
    snowMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER"), 1);
    auto iceMaterial = new G4Material("ice", 0.92 * g / cm3, 1);
    iceMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER"), 1);

    auto worldSolid = new G4Box("world", worldXY / 2, worldXY / 2, worldZ / 2);
    auto worldLogical = new G4LogicalVolume(worldSolid, worldMaterial, "world");
    auto worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "world", 0, false, true);

    auto airSolid = new G4Box("air", airXY / 2, airXY / 2, fAirZ / 2);
    auto airLogical = new G4LogicalVolume(airSolid, airMaterial, "air");
    auto airPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, fAirZ / 2 - fRockThickness - 1 * cm), airLogical, "air", worldLogical, false, true);
    auto airVis = new G4VisAttributes(G4Color(0, 0, 0.6, 0.8));
    airVis->SetForceSolid(true);
    airLogical->SetVisAttributes(airVis);

    auto rockSolid = new G4Box("rock", airXY / 2, airXY / 2, fRockThickness / 2);
    auto rockLogical = new G4LogicalVolume(rockSolid, rockMaterial, "rock");
    auto rockPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fAirZ / 2 + fRockThickness / 2), rockLogical, "rock", airLogical, false, true);
    auto rockVis = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 0.6));
    rockVis->SetForceSolid(true);
    rockLogical->SetVisAttributes(rockVis);

    auto snowSolid = new G4Box("snow", snowXY / 2, snowXY / 2, fSnowDepth / 2);
    auto snowLogical = new G4LogicalVolume(snowSolid, snowMaterial, "snow");
    auto snowPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fAirZ / 2 + fRockThickness + fSnowDepth / 2), snowLogical, "snow", airLogical, false, true);
    auto snowVis = new G4VisAttributes(G4Color(1, 1, 1, 0.9));
    snowVis->SetForceSolid(true);
    snowLogical->SetVisAttributes(snowVis);

    auto tubeSolid = new G4Tubs("tube", 0, tubeRadius, tubeZ / 2, 0, 2 * M_PI);
    auto tubeLogical = new G4LogicalVolume(tubeSolid, airMaterial, "tube");
    auto tubePhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fAirZ / 2 + fRockThickness + fTubeGroundOffset), tubeLogical, "tube", airLogical, false, true);
    auto tubeVis = new G4VisAttributes(G4Color(0, 1, 0, 0.8));
    tubeVis->SetForceSolid(true);
    tubeLogical->SetVisAttributes(tubeVis);

    auto tubeSD = new CryTubeSensitiveDetector("tubeSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(tubeSD);
    tubeLogical->SetSensitiveDetector(tubeSD);

    return worldPhysical;
}

void CryDetectorConstruction::SetRockMaterial(G4String materialName)
{
    G4Material *rockMaterial = G4NistManager::Instance()->FindOrBuildMaterial(materialName);
    G4LogicalVolume *rockLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("rock");
    rockLogical->SetMaterial(rockMaterial);
}

void CryDetectorConstruction::SetSnowDepth(G4double depth)
{
    fSnowDepth = depth;
    G4LogicalVolume *snowLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("snow");
    G4Box *snowSolid = (G4Box *)snowLogical->GetSolid();
    snowSolid->SetZHalfLength(fSnowDepth / 2);
    snowLogical->SetSolid(snowSolid);
    G4VPhysicalVolume *snowPhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("snow");
    G4ThreeVector snowPosition = snowPhysical->GetTranslation();
    snowPosition.setZ(-fAirZ / 2 + fRockThickness + fSnowDepth / 2);
    snowPhysical->SetTranslation(snowPosition);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void CryDetectorConstruction::SetRockThickness(G4double thickness)
{
    fRockThickness = thickness;
    G4LogicalVolume *airLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("air");
    G4LogicalVolume *rockLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("rock");
    G4Box *airSolid = (G4Box *)airLogical->GetSolid();
    fAirZ = fAirHeight + fRockThickness;
    airSolid->SetZHalfLength(fAirZ / 2);
    airLogical->SetSolid(airSolid);
    G4Box *rockSolid = (G4Box *)rockLogical->GetSolid();
    rockSolid->SetZHalfLength(fRockThickness / 2);
    rockLogical->SetSolid(rockSolid);
    G4VPhysicalVolume *airPhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("air");
    G4ThreeVector airPosition = airPhysical->GetTranslation();
    airPosition.setZ(fAirZ / 2 - fRockThickness - 1 * cm);
    airPhysical->SetTranslation(airPosition);
    G4VPhysicalVolume *rockPhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("rock");
    G4ThreeVector rockPosition = rockPhysical->GetTranslation();
    rockPosition.setZ(-fAirZ / 2 + fRockThickness / 2);
    rockPhysical->SetTranslation(rockPosition);
    G4VPhysicalVolume *snowPhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("snow");
    G4ThreeVector snowPosition = snowPhysical->GetTranslation();
    snowPosition.setZ(-fAirZ / 2 + fRockThickness + fSnowDepth / 2);
    snowPhysical->SetTranslation(snowPosition);
    G4VPhysicalVolume *tubePhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("tube");
    G4ThreeVector tubePosition = tubePhysical->GetTranslation();
    tubePosition.setZ(-fAirZ / 2 + fRockThickness + fTubeGroundOffset);
    tubePhysical->SetTranslation(tubePosition);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}