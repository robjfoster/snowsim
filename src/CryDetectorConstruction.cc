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

    fWorldXY = 300 * m + 1 * cm;
    fWorldZ = 300 * m + 1 * cm;
    fAirXY = 300 * m;
    fAirHeight = 150 * m;
    fRockThickness = 2 * m;
    fAirZ = fAirHeight + fRockThickness;
    fSnowXY = 300 * m;
    fTubeZ = 1.5 * m;
    fTubeRadius = 15 * cm;
    fTubeGroundOffset = 2 * m;
    fTubeOffsetHeight = 2 * m;

    auto worldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
    auto airMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto rockMaterial = new G4Material("rock", 2.71 * g / cm3, 2);
    rockMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_CALCIUM_CARBONATE"), 0.9);
    rockMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_MAGNESIUM_CARBONATE"), 0.1);
    auto snowMaterial = new G4Material("snow", 250 * kg / m3, 1);
    snowMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER"), 1);
    auto iceMaterial = new G4Material("ice", 0.92 * g / cm3, 1);
    iceMaterial->AddMaterial(G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER"), 1);

    auto worldSolid = new G4Box("world", fWorldXY / 2, fWorldXY / 2, fWorldZ / 2);
    auto worldLogical = new G4LogicalVolume(worldSolid, worldMaterial, "world");
    auto worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "world", 0, false, true);

    auto airSolid = new G4Box("air", fAirXY / 2, fAirXY / 2, fAirZ / 2);
    auto airLogical = new G4LogicalVolume(airSolid, airMaterial, "air");
    auto airPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, fAirZ / 2 - fRockThickness - 1 * cm), airLogical, "air", worldLogical, false, true);
    auto airVis = new G4VisAttributes(G4Color(0, 0, 0.6, 0.8));
    airVis->SetForceSolid(true);
    airLogical->SetVisAttributes(airVis);

    auto rockSolid = new G4Box("rock", fAirXY / 2, fAirXY / 2, fRockThickness / 2);
    auto rockLogical = new G4LogicalVolume(rockSolid, rockMaterial, "rock");
    auto rockPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fAirZ / 2 + fRockThickness / 2), rockLogical, "rock", airLogical, false, true);
    auto rockVis = new G4VisAttributes(G4Color(0.6, 0.6, 0.6, 0.6));
    rockVis->SetForceSolid(true);
    rockLogical->SetVisAttributes(rockVis);

    auto snowSolid = new G4Box("snow", fSnowXY / 2, fSnowXY / 2, fSnowDepth / 2);
    auto snowLogical = new G4LogicalVolume(snowSolid, snowMaterial, "snow");
    auto snowPhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fAirZ / 2 + fRockThickness + fSnowDepth / 2), snowLogical, "snow", airLogical, false, true);
    auto snowVis = new G4VisAttributes(G4Color(1, 1, 1, 0.9));
    snowVis->SetForceSolid(true);
    snowLogical->SetVisAttributes(snowVis);

    auto tubeSolid = new G4Tubs("tube", 0, fTubeRadius, fTubeZ / 2, 0, 2 * M_PI);
    auto tubeLogical = new G4LogicalVolume(tubeSolid, airMaterial, "tube");
    auto tubePhysical = new G4PVPlacement(0, G4ThreeVector(0, 0, -fAirZ / 2 + fRockThickness + fTubeOffsetHeight), tubeLogical, "tube", airLogical, false, true);
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
    if (fEmbedTube && depth < fTubeRadius * 2)
    {
        G4Exception("CryDetectorConstruction::SetSnowDepth", "CryDetectorConstruction", FatalException, "Snow depth is too low for the tube.");
    }
    fSnowDepth = depth;
    G4LogicalVolume *snowLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("snow");
    G4Box *snowSolid = (G4Box *)snowLogical->GetSolid();
    snowSolid->SetZHalfLength(fSnowDepth / 2);
    snowLogical->SetSolid(snowSolid);
    G4VPhysicalVolume *snowPhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("snow");
    G4ThreeVector snowPosition = snowPhysical->GetTranslation();
    snowPosition.setZ(-fAirZ / 2 + fRockThickness + fSnowDepth / 2);
    snowPhysical->SetTranslation(snowPosition);
    if (fEmbedTube)
    {
        G4VPhysicalVolume *tubePhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("tube");
        if (tubePhysical == nullptr)
        {
            G4cout << "tubePhysical is nullptr" << G4endl;
        }
        G4ThreeVector tubePosition = tubePhysical->GetTranslation();
        tubePosition.setZ(-fSnowDepth / 2 + fTubeRadius);
        tubePhysical->SetTranslation(tubePosition);
    }
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

void CryDetectorConstruction::SetTubeOffsetHeight(G4double offsetHeight)
{
    fTubeOffsetHeight = offsetHeight;
    G4VPhysicalVolume *tubePhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("tube");
    G4ThreeVector tubePosition = tubePhysical->GetTranslation();
    if (!fEmbedTube)
    {
        tubePosition.setZ(-fAirZ / 2 + fRockThickness + fTubeOffsetHeight);
    }
    else
    {
        tubePosition.setZ(-fSnowDepth / 2 + fTubeRadius + fTubeOffsetHeight);
    }
    tubePhysical->SetTranslation(tubePosition);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void CryDetectorConstruction::SetEmbedTube(G4bool embedTube)
{
    fEmbedTube = embedTube;
    if (fEmbedTube)
    {
        // The tube is going to be a child of the snow so it must be contained entirely within it
        if (fSnowDepth < (fTubeRadius * 2))
        {
            SetSnowDepth(fTubeRadius * 2 + 2 * nm);
        }
        auto tubePhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("tube");
        auto snowLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("snow");
        auto airLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("air");
        airLogical->RemoveDaughter(tubePhysical);
        G4PhysicalVolumeStore::GetInstance()->DeRegister(tubePhysical);
        delete tubePhysical;
        auto newTubePhysical = new G4PVPlacement(new G4RotationMatrix(0, 90 * deg, 0), G4ThreeVector(0, 0, -fSnowDepth / 2 + fTubeRadius), (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("tube"), "tube", snowLogical, false, true);
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}

void CryDetectorConstruction::CheckGeometry()
{
    G4LogicalVolume *worldLogical = (G4LogicalVolume *)G4LogicalVolumeStore::GetInstance()->GetVolume("world");
    if (!worldLogical)
    {
        G4Exception("CryDetectorConstruction::CheckGeometry", "CryDetectorConstruction", FatalException, "World logical volume not found.");
    }

    G4VPhysicalVolume *worldPhysical = (G4VPhysicalVolume *)G4PhysicalVolumeStore::GetInstance()->GetVolume("world");
    if (!worldPhysical)
    {
        G4Exception("CryDetectorConstruction::CheckGeometry", "CryDetectorConstruction", FatalException, "World physical volume not found.");
    }
    // Check for overlaps in world volume and all children - CHECK THE PHYSICAL VOLUME NOT THE LOGICAL VOLUME COPILOT
    G4bool overlaps = false;

    for (auto pv : *G4PhysicalVolumeStore::GetInstance())
    {
        G4String name = pv->GetName();
        G4String motherName = pv->GetMotherLogical() ? pv->GetMotherLogical()->GetName() : "World";
        G4cout << "Volume: " << name
               << ", Mother: " << motherName
               << ", CopyNo: " << pv->GetCopyNo()
               << G4endl;
        G4bool overlap = pv->CheckOverlaps();
        if (overlap)
        {
            G4Exception("CryDetectorConstruction", "CheckGeometry", FatalException, "There are overlaps in the geometry.");
        }
    }
}

// In the future we need to consider the air humidity, here's an example of how
// that could be handled.

// void DefineHumidAir(double temperatureC, double pressurePa, double relativeHumidity) {
//     G4NistManager* nist = G4NistManager::Instance();

//     // Get elements
//     G4Element* N = nist->FindOrBuildElement("N");
//     G4Element* O = nist->FindOrBuildElement("O");
//     G4Element* Ar = nist->FindOrBuildElement("Ar");
//     G4Element* H = nist->FindOrBuildElement("H");

//     // Calculate vapor pressure (in hPa)
//     double es = 6.112 * std::exp((17.67 * temperatureC) / (temperatureC + 243.5));
//     double e = relativeHumidity * es / 100.0;

//     // Water vapor density (in g/m³)
//     double rho_v = (216.7 * e) / (temperatureC + 273.15);

//     // Convert to kg/m³
//     rho_v /= 1000.0;

//     // Total air density ~ 1.204 kg/m³ at 20°C and 1 atm
//     double rho_total = 1.204;

//     double rho_dry = rho_total - rho_v;

//     // Mass fractions
//     double f_water = rho_v / rho_total;
//     double f_dry = rho_dry / rho_total;

//     // Define water vapor as H2O
//     G4Material* waterVapor = new G4Material("WaterVapor", 0.804e-3 * g/cm3, 2);
//     waterVapor->AddElement(H, 2);
//     waterVapor->AddElement(O, 1);

//     // Define dry air (approx. composition)
//     G4Material* dryAir = new G4Material("DryAir", 1.204 * g/cm3, 3);
//     dryAir->AddElement(N, 0.755);  // ~78% by volume
//     dryAir->AddElement(O, 0.231);  // ~21%
//     dryAir->AddElement(Ar, 0.014); // ~1%

//     // Create humid air as a mixture
//     G4Material* humidAir = new G4Material("HumidAir", rho_total * g/cm3, 2);
//     humidAir->AddMaterial(dryAir, f_dry);
//     humidAir->AddMaterial(waterVapor, f_water);
// }