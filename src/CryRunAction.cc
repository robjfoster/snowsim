#include "CryRunAction.hh"

#include "G4RunManager.hh"
#include "G4CsvAnalysisManager.hh"

CryRunAction::CryRunAction()
    : G4UserRunAction()
{
    fMessenger = new CryAnalysisMessenger(this);
    // set printing event number per each 100 events
    G4RunManager::GetRunManager()->SetPrintProgress(1000);

    auto am = G4CsvAnalysisManager::Instance();
    am->SetVerboseLevel(1);
    am->SetNtupleMerging(true);

    am->CreateNtuple("NeutronHits", "NeutronHits");
    am->CreateNtupleIColumn("eid");
    am->CreateNtupleDColumn("Ek");
    am->CreateNtupleDColumn("posx");
    am->CreateNtupleDColumn("posy");
    am->CreateNtupleDColumn("posz");
    am->CreateNtupleDColumn("dirx");
    am->CreateNtupleDColumn("diry");
    am->CreateNtupleDColumn("dirz");
    am->CreateNtupleDColumn("tglobal");
    am->CreateNtupleDColumn("tlocal");
    am->CreateNtupleDColumn("tproper");

    am->CreateNtupleIColumn("init_n_eid");
    am->CreateNtupleDColumn("init_n_Ek");
    am->CreateNtupleDColumn("init_n_posx");
    am->CreateNtupleDColumn("init_n_posy");
    am->CreateNtupleDColumn("init_n_posz");
    am->CreateNtupleDColumn("init_n_dirx");
    am->CreateNtupleDColumn("init_n_diry");
    am->CreateNtupleDColumn("init_n_dirz");
    am->CreateNtupleDColumn("init_n_tglobal");
    am->CreateNtupleDColumn("init_n_tlocal");
    am->CreateNtupleDColumn("init_n_tproper");

    am->FinishNtuple();
}

void CryRunAction::BeginOfRunAction(const G4Run *run)
{
    auto am = G4CsvAnalysisManager::Instance();
    am->OpenFile(fOutputFileName);
}

void CryRunAction::EndOfRunAction(const G4Run *run)
{
    auto am = G4CsvAnalysisManager::Instance();
    am->Write();
    am->CloseFile();
}
