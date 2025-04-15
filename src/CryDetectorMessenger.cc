#include "CryDetectorMessenger.hh"

#include "CryDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

CryDetectorMessenger::CryDetectorMessenger(CryDetectorConstruction *det) : fDetectorConstruction(det)
{
    fDirectory = new G4UIdirectory("/snowsim/");
    fDirectory->SetGuidance("UI commands specific to snowsim");

    fDetDirectory = new G4UIdirectory("/snowsim/det/");
    fDetDirectory->SetGuidance("Detector construction commands");

    fRockMaterialCmd = new G4UIcmdWithAString("/snowsim/det/setRockMaterial", this);
    fRockMaterialCmd->SetGuidance("Select material of the rock");
    fRockMaterialCmd->SetParameterName("choice", false);
    fRockMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fRockThicknessCmd = new G4UIcmdWithADoubleAndUnit("/snowsim/det/setRockThickness", this);
    fRockThicknessCmd->SetGuidance("Set rock thickness");
    fRockThicknessCmd->SetParameterName("thickness", false);
    fRockThicknessCmd->SetUnitCategory("Length");
    fRockThicknessCmd->SetRange("thickness>0.");
    fRockThicknessCmd->SetDefaultUnit("m");
    fRockThicknessCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fSnowDepthCmd = new G4UIcmdWithADoubleAndUnit("/snowsim/det/setSnowDepth", this);
    fSnowDepthCmd->SetGuidance("Set snow depth");
    fSnowDepthCmd->SetParameterName("depth", false);
    fSnowDepthCmd->SetUnitCategory("Length");
    fSnowDepthCmd->SetRange("depth>0.");
    fSnowDepthCmd->SetDefaultUnit("m");
    fSnowDepthCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fEmbedTubeCmd = new G4UIcmdWithABool("/snowsim/det/embedTube", this);
    fEmbedTubeCmd->SetGuidance("Embed tube in snow");
    fEmbedTubeCmd->SetParameterName("embed", false);
    fEmbedTubeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

CryDetectorMessenger::~CryDetectorMessenger()
{
    delete fRockMaterialCmd;
    delete fSnowDepthCmd;
    delete fDetDirectory;
    delete fDirectory;
}

void CryDetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fRockMaterialCmd)
    {
        fDetectorConstruction->SetRockMaterial(newValue);
    }
    if (command == fSnowDepthCmd)
    {
        G4cout << "Setting snow depth to " << fSnowDepthCmd->GetNewDoubleValue(newValue) << " mm" << G4endl;
        fDetectorConstruction->SetSnowDepth(fSnowDepthCmd->GetNewDoubleValue(newValue));
    }
    if (command == fRockThicknessCmd)
    {
        G4cout << "Setting rock thickness to " << fRockThicknessCmd->GetNewDoubleValue(newValue) << " mm" << G4endl;
        fDetectorConstruction->SetRockThickness(fRockThicknessCmd->GetNewDoubleValue(newValue));
    }
    if (command == fEmbedTubeCmd)
    {
        G4cout << "Setting embed tube to " << fEmbedTubeCmd->GetNewBoolValue(newValue) << G4endl;
        fDetectorConstruction->SetEmbedTube(fEmbedTubeCmd->GetNewBoolValue(newValue));
    }
}