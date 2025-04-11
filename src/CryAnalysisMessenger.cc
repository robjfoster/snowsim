#include "CryAnalysisMessenger.hh"

#include "CryRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

CryAnalysisMessenger::CryAnalysisMessenger(CryRunAction *runAction)
    : G4UImessenger(), fRunAction(runAction)
{
    fCryDir = new G4UIdirectory("/am/");
    fCryDir->SetGuidance("Controls for analysis manager");

    fOutputFileCmd = new G4UIcmdWithAString("/am/outputfile", this);
    fOutputFileCmd->SetGuidance("Select the output root file");
    fOutputFileCmd->SetParameterName("choice", false);
    fOutputFileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

CryAnalysisMessenger::~CryAnalysisMessenger()
{
    delete fOutputFileCmd;
    delete fCryDir;
}

void CryAnalysisMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fOutputFileCmd)
    {
        fRunAction->SetOutputFile(newValue);
    }
}
