#include "action.hh"


Action::Action() {}
Action::~Action() {}

void Action::Build() const {
	muonGenerator *generator = new muonGenerator();
	SetUserAction(generator);
	
	CreateNtuple *runAction = new CreateNtuple(); 
	SetUserAction(runAction);

	MyEventAction* eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);

    SetUserAction(new MySteppingAction(eventAction));

}