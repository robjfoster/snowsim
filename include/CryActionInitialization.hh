#ifndef SnowSimActionInitialization_h
#define SnowSimActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class CryActionInitialization : public G4VUserActionInitialization
{
public:
    CryActionInitialization() = default;
    ~CryActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;
};

#endif