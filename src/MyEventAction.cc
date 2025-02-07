#include "MyEventAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>

MyEventAction::MyEventAction(CreateNtuple* runAction)
    : G4UserEventAction(), fRunAction(runAction) {}

MyEventAction::~MyEventAction() {}

void MyEventAction::BeginOfEventAction(const G4Event*) {
    fScatteringAngles.clear();  // Reset angles for each event
}

void MyEventAction::EndOfEventAction(const G4Event*) {
    // Pass event angles to run action for storage
    fRunAction->StoreScatteringAngles(fScatteringAngles);
}

void MyEventAction::AddScatteringAngle(double angle) {
    fScatteringAngles.push_back(angle);
}