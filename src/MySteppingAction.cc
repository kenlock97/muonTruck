#include "MySteppingAction.hh"
#include "MyEventAction.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction) {}

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();

    if (track->GetParentID() == 0) {  // Only primary particles
        G4ThreeVector preMomentum = step->GetPreStepPoint()->GetMomentumDirection();
        G4ThreeVector postMomentum = step->GetPostStepPoint()->GetMomentumDirection();

        double cosTheta = preMomentum.dot(postMomentum);
        double scatteringAngle = std::acos(cosTheta) * 180.0 / CLHEP::pi;

        fEventAction->AddScatteringAngle(scatteringAngle);
    }
}