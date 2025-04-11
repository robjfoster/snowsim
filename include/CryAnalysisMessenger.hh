#ifndef CryAnalysisMessenger_h
#define CryAnalysisMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class CryRunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class CryAnalysisMessenger : public G4UImessenger
{
public:
    CryAnalysisMessenger(CryRunAction *);
    ~CryAnalysisMessenger();

    void SetNewValue(G4UIcommand *, G4String);

private:
    CryRunAction *fRunAction;
    G4UIdirectory *fCryDir;
    G4UIcmdWithAString *fOutputFileCmd;
};

#endif