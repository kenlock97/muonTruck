#include "MySteppingAction.hh"
#include "MyEventAction.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include <fstream>
#include <cmath>

MySteppingAction::MySteppingAction(MyEventAction* eventAction)
    : G4UserSteppingAction(), fEventAction(eventAction), currentEventID(-1) {}  // Initialize currentEventID

MySteppingAction::~MySteppingAction() {}

void MySteppingAction::UserSteppingAction(const G4Step* step) {
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Open file in append mode
    std::ofstream outFile;
    outFile.open("steps_inside_box.txt", std::ios::app);

    if (!outFile.is_open()) {
        G4cerr << "Error: Could not open steps_inside_box.txt for writing!" << G4endl;
        return;
    }

    if (eventID != currentEventID) {
        outFile << "Event ID: " << eventID << ")\n";
        currentEventID = eventID;  // Update to track event
    }

    G4Track* track = step->GetTrack();
    if (track->GetParentID() == 0) {  // Only primary particles
        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();

        G4ThreeVector prePosition = preStepPoint->GetPosition();
        G4ThreeVector preMomentum = preStepPoint->GetMomentum();
        G4double preMomentumMag = preMomentum.mag();

        G4ThreeVector postPosition = postStepPoint->GetPosition();
        G4ThreeVector postMomentum = postStepPoint->GetMomentum();
        G4double postMomentumMag = postMomentum.mag();

        // Define G4Box dimensions
        G4double X_min = -25 * m, X_max = 25 * m;
        G4double Y_min = -25 * m, Y_max = 25 * m;
        G4double Z_min = -25 * m, Z_max = 25 * m;

        // Check if the step is inside the box
        bool preInsideBox = (prePosition.x() >= X_min && prePosition.x() <= X_max &&
                            prePosition.y() >= Y_min && prePosition.y() <= Y_max &&
                            prePosition.z() >= Z_min && prePosition.z() <= Z_max);

        bool postInsideBox = (postPosition.x() >= X_min && postPosition.x() <= X_max &&
                             postPosition.y() >= Y_min && postPosition.y() <= Y_max &&
                             postPosition.z() >= Z_min && postPosition.z() <= Z_max);

        if (!preInsideBox && !postInsideBox) return;

        // Compute scattering angle and momentum change
        G4double scatteringAngle = preMomentum.angle(postMomentum) * (180.0 / CLHEP::pi);
        G4double momentumChange = postMomentumMag - preMomentumMag;

        // Write step data
        if (preInsideBox) {
            outFile << prePosition.x() / mm << " " 
                    << prePosition.y() / mm << " " 
                    << prePosition.z() / mm << " "
                    << scatteringAngle << " "
                    << momentumChange << "\n";
        }
    }

    outFile.close();
}