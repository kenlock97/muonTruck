#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"

class RPCDetector: public G4VSensitiveDetector {

	public:
		RPCDetector(G4String);
		~RPCDetector();
	private:
		virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};

#endif