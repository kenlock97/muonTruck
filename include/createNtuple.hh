#ifndef CREATENTUPLE_HH
#define CREATENTUPLE_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include <vector>

class CreateNtuple : public G4UserRunAction {

	public:
		CreateNtuple();
		~CreateNtuple();

		virtual void BeginOfRunAction(const G4Run *);
		virtual void EndOfRunAction(const G4Run *);

		// Method to collect scattering angles from events
    	void StoreScatteringAngles(const std::vector<double>& angles);

	private:
    	std::vector<double> fScatteringAngles;  // Holds all scattering angles from events
		
};


#endif