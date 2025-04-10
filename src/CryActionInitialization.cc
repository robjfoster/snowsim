#include "CryActionInitialization.hh"

#include "CryGeneratorAction.hh"
#include "CryRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CryActionInitialization::BuildForMaster() const
{
    SetUserAction(new CryRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CryActionInitialization::Build() const
{
    SetUserAction(new CryGeneratorAction("", 10.0));
    SetUserAction(new CryRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
