//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: PrimaryGeneratorMessenger.cc,v 1.8 2002/12/16 16:37:27 maire Exp $
// GEANT4 tag $Name: geant4-07-00-patch-01 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "CryGeneratorMessenger.hh"

#include "CryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CryGeneratorMessenger::CryGeneratorMessenger(
    CryGeneratorAction *Gun)
    : Action(Gun)
{
  CRYDir = new G4UIdirectory("/CRY/");
  CRYDir->SetGuidance("CRY initialization");

  FileCmd = new G4UIcmdWithAString("/CRY/file", this);
  FileCmd->SetGuidance("This reads the CRY definition from a file");
  FileCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  InputCmd = new G4UIcmdWithAString("/CRY/input", this);
  InputCmd->SetGuidance("CRY input lines");
  InputCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/CRY/update", this);
  UpdateCmd->SetGuidance("Update CRY definition.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed the CRY definition.");
  UpdateCmd->AvailableForStates(G4State_Idle);

  PlaneCmd = new G4UIcmdWithADoubleAndUnit("/CRY/planeHeight", this);
  PlaneCmd->SetGuidance("Set the CRY plane height in the Z dimension");
  PlaneCmd->SetParameterName("planeHeight", false);
  PlaneCmd->SetUnitCategory("Length");
  PlaneCmd->SetDefaultUnit("m");
  PlaneCmd->AvailableForStates(G4State_Idle);

  MessInput = new std::string;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CryGeneratorMessenger::~CryGeneratorMessenger()
{
  delete CRYDir;
  delete InputCmd;
  delete UpdateCmd;
  delete FileCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
  if (command == InputCmd)
  {
    Action->InputCRY();
    (*MessInput).append(newValue);
    (*MessInput).append(" ");
  }

  if (command == UpdateCmd)
  {
    Action->UpdateCRY(MessInput);
    *MessInput = "";
  }

  if (command == FileCmd)
  {
    Action->CRYFromFile(newValue);
  }

  if (command == PlaneCmd)
  {
    Action->SetPlaneHeight(PlaneCmd->GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
