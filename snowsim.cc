#include "CryActionInitialization.hh"
#include "CryDetectorConstruction.hh"
#include "QGSP_BERT_HP.hh"
#include "CryGeneratorAction.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
// #include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char **argv)
{
    // Detect interactive mode (if no arguments) and define UI session
    //
    G4UIExecutive *ui = nullptr;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Optionally: choose a different Random engine...
    // G4Random::setTheEngine(new CLHEP::MTwistEngine);

    // use G4SteppingVerboseWithUnits
    G4int precision = 4;
    G4SteppingVerbose::UseBestUnit(precision);

    // Construct the default run manager
    //
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    // Set mandatory initialization classes
    //
    // Detector construction
    runManager->SetUserInitialization(new CryDetectorConstruction());

    // Physics list
    auto physicsList = new QGSP_BERT_HP(0);
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    // User action initialization
    runManager->SetUserInitialization(new CryActionInitialization());
    // runManager->SetUserAction(new CryGeneratorAction("", 10));
    // G4cout << "Initialized CryGeneratorAction" << G4endl;

    // Initialize visualization with the default graphics system
    auto visManager = new G4VisExecutive();
    // Constructors can also take optional arguments:
    // - a graphics system of choice, eg. "OGL"
    // - and a verbosity argument - see /vis/verbose guidance.
    // auto visManager = new G4VisExecutive(argc, argv, "OGL", "Quiet");
    // auto visManager = new G4VisExecutive("Quiet");

    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    runManager->Initialize();

    // UImanager->ApplyCommand("/control/execute crysetup.mac");
    // runManager->BeamOn(1);

    if (!ui)
    {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }
    else
    {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}