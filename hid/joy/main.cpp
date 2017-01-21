#include <stdio.h>

#include "joystick.h"

#include <thread>

using namespace std;

void gotEvent(struct js_event e, bool *finalEvent) {
	cout << "Event at time: " << (int)e.time << endl
		<< "\tType: " << (int)e.type << endl
		<< "\tAxis/Btn Number: " <<  (int)e.number << endl
		<< "\tValue: " << (int)e.value << endl;
	*finalEvent = false;
}

void followJoystick(Joystick* inp) {
	inp -> getInput();
}

int main() {
	Joystick *inp = new Joystick("/dev/input/js0", gotEvent);
	thread t1(followJoystick, inp);
	t1.join();
	return 0;
}
