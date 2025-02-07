#ifndef MYEVENTACTION_HH
#define MYEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "CreateNtuple.hh"
#include <vector>

class MyEventAction : public G4UserEventAction {
public:
    MyEventAction(CreateNtuple* runAction);
    virtual ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*) override;
    virtual void EndOfEventAction(const G4Event*) override;

    void AddScatteringAngle(double angle);

private:
    std::vector<double> fScatteringAngles;
    CreateNtuple* fRunAction;  // Pointer to run action for final data storage
};

#endif