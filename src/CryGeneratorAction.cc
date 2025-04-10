//******************************************************************************
// PrimaryGeneratorAction.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include "CryGeneratorAction.hh"
using namespace std;

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

//----------------------------------------------------------------------------//
CryGeneratorAction::CryGeneratorAction(const char *inputfile, double zOffset)
{
  G4cout << "CRY PRIMARY GENERATOR ACTION" << G4endl;
  // define a particle gun
  particleGun = new G4ParticleGun();
  fZOffset = zOffset * m;

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile, std::ios::in);
  char buffer[1000];

  if (inputFile.fail())
  {
    if (*inputfile != 0) //....only complain if a filename was given

      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
    InputState = -1;
  }
  else
  {
    std::string setupString("");
    while (!inputFile.getline(buffer, 1000).eof())
    {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup = new CRYSetup(setupString, "../data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(), &CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState = 0;
  }
  // create a vector to store the CRY particle properties
  vect = new std::vector<CRYParticle *>;

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new CryGeneratorMessenger(this);

  G4cout << "Finished CRY constructor, input state is " << InputState << G4endl;
}

//----------------------------------------------------------------------------//
CryGeneratorAction::~CryGeneratorAction()
{
}

//----------------------------------------------------------------------------//
void CryGeneratorAction::InputCRY()
{
  InputState = 1;
}

//----------------------------------------------------------------------------//
void CryGeneratorAction::UpdateCRY(std::string *MessInput)
{
  CRYSetup *setup = new CRYSetup(*MessInput, "../data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(), &CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState = 0;
}

//----------------------------------------------------------------------------//
void CryGeneratorAction::CRYFromFile(G4String newValue)
{
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue, std::ios::in);
  char buffer[1000];

  if (inputFile.fail())
  {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line" << G4endl;
    InputState = -1;
  }
  else
  {
    std::string setupString("");
    while (!inputFile.getline(buffer, 1000).eof())
    {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup = new CRYSetup(setupString, "../data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(), &CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState = 0;
  }
}

void CryGeneratorAction::SetPlaneHeight(double zOffset)
{
  fZOffset = zOffset;
}

//----------------------------------------------------------------------------//
void CryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  if (InputState != 0)
  {
    G4String *str = new G4String("CRY library was not successfully initialized");
    // G4Exception(*str);
    G4Exception("PrimaryGeneratorAction", "1",
                RunMustBeAborted, *str);
  }
  G4String particleName;
  vect->clear();
  gen->genEvent(vect);

  //....debug output
  G4cout << "\nEvent=" << anEvent->GetEventID() << " "
         << "CRY generated nparticles=" << vect->size()
         << G4endl;

  for (unsigned j = 0; j < vect->size(); j++)
  {
    particleName = CRYUtils::partName((*vect)[j]->id());

    //....debug output
    cout << "  " << particleName << " "
         << "charge=" << (*vect)[j]->charge() << " "
         << setprecision(4)
         << "energy (MeV)=" << (*vect)[j]->ke() * MeV << " "
         << "pos (m)"
         << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
         << " " << "direction cosines "
         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
         << " " << endl;

    particleGun->SetParticleDefinition(particleTable->FindParticle((*vect)[j]->PDGid()));
    particleGun->SetParticleEnergy((*vect)[j]->ke() * MeV);
    particleGun->SetParticlePosition(G4ThreeVector((*vect)[j]->x() * m, (*vect)[j]->y() * m, (*vect)[j]->z() * m + fZOffset));
    particleGun->SetParticleMomentumDirection(G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
    particleGun->SetParticleTime((*vect)[j]->t());
    particleGun->GeneratePrimaryVertex(anEvent);
    delete (*vect)[j];
  }
}
