#ifndef MYSTEPPINGACTION_HH
#define MYSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

// Forward declaration to avoid circular dependency
class MyEventAction;

class MySteppingAction : public G4UserSteppingAction {
public:
    MySteppingAction(MyEventAction* eventAction);
    virtual ~MySteppingAction();

    virtual void UserSteppingAction(const G4Step* step) override;

private:
    MyEventAction* fEventAction;  // Pointer to EventAction for storing angles
    G4int currentEventID = -1;  // Track event ID
};

#endif  // MYSTEPPINGACTION_HH