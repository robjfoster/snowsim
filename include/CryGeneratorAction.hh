//******************************************************************************
// PrimaryGeneratorAction.hh
//
// This class is a class derived from G4VUserPrimaryGeneratorAction for
// constructing the process used to generate incident particles.
//
// 1.00 JMV, LLNL, JAN-2007:  First version.
//******************************************************************************
//
#ifndef CryGeneratorAction_h
#define CryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "CryGeneratorMessenger.hh"

class G4Event;

class CryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  CryGeneratorAction(const char *inputfile, double zOffset);
  ~CryGeneratorAction();

public:
  void GeneratePrimaries(G4Event *anEvent);
  void InputCRY();
  void UpdateCRY(std::string *MessInput);
  void CRYFromFile(G4String newValue);
  void SetPlaneHeight(G4double newValue);

private:
  std::vector<CRYParticle *> *vect; // vector of generated particles
  G4ParticleTable *particleTable;
  G4ParticleGun *particleGun;
  CRYGenerator *gen;
  G4int InputState;
  double fZOffset;
  CryGeneratorMessenger *gunMessenger;
};

#endif
