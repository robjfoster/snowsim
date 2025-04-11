#ifndef CryRunAction_h
#define CryRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "CryAnalysisMessenger.hh"

class G4Run;

class CryRunAction : public G4UserRunAction
{
public:
    CryRunAction();
    ~CryRunAction() override = default;

public:
    void BeginOfRunAction(const G4Run *run) override;
    void EndOfRunAction(const G4Run *run) override;
    void SetOutputFile(G4String name) { fOutputFileName = name; }

private:
    G4String fOutputFileName = "output.csv";
    CryAnalysisMessenger *fMessenger;
};

#endif