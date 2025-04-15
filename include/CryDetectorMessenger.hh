#ifndef CryDetectorMessenger_h
#define CryDetectorMessenger_h 1

#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcommand;

class CryDetectorConstruction;

class CryDetectorMessenger : public G4UImessenger
{
public:
    CryDetectorMessenger(CryDetectorConstruction *);
    ~CryDetectorMessenger() override;

    void SetNewValue(G4UIcommand *, G4String) override;

private:
    CryDetectorConstruction *fDetectorConstruction = nullptr;

    G4UIdirectory *fDirectory = nullptr;
    G4UIdirectory *fDetDirectory = nullptr;

    G4UIcmdWithAString *fRockMaterialCmd = nullptr;
    G4UIcmdWithADoubleAndUnit *fRockThicknessCmd = nullptr;
    G4UIcmdWithADoubleAndUnit *fSnowDepthCmd = nullptr;
    G4UIcmdWithADoubleAndUnit *fAirHumidityCmd = nullptr;
    G4UIcmdWithABool *fEmbedTubeCmd = nullptr;
};

#endif