#include "joystick.h"

Joystick :: ~Joystick() {
	close(fd);
}

Joystick :: Joystick(const char *js_device, void (*setCallback)(struct js_event e, bool *finalEvent)) {
	_busy = false;
	_connected = false;

	fd = open (js_device, O_RDONLY);
	if(fd < 0) {
		std::cerr << "Could not open joystick device: " << js_device << std::endl;
		return;
	}

	/* Get info about the joystick */
	char axes, buttons;
	ioctl (fd, JSIOCGAXES, &axes);
	ioctl (fd, JSIOCGBUTTONS, &buttons);
	_number_of_axes = (unsigned int)axes;
	_number_of_buttons = (unsigned int)buttons;
	_axis = new __s16[_number_of_axes];
	_button = new __s16[_number_of_buttons];
	std::cerr << "Using joystick with " << _number_of_axes << " axes, " << _number_of_buttons << " buttons" << std::endl;

	/* Set everything to 0 (just for sanity until the driver overrides it) */
	int i;
	for(i = 0; i < _number_of_axes; i++) {
		_axis[i] = 0;
	}
	for(i = 0; i < _number_of_buttons; i++) {
		_button[i] = 0;
	}

	/* A few things to set */
	callback = setCallback;
	_busy = false;
	_connected = true;
}

bool Joystick :: wait_for_next_event(struct js_event *e) {
	return read (fd, e, sizeof(struct js_event)) >= 0;
}

void Joystick :: getInput() {
	bool finalEvent = false;
	/* A flag to stop this being run concurrently,
		and to make it easy to spot an unplugged joystick */
	if(_busy || !_connected) {
		return;
	}
	_busy = true;

	struct js_event e;
	while(!finalEvent && wait_for_next_event(&e)) {
		switch(e.type) {
			/* Button events and button init messages */
			case JS_EVENT_BUTTON:
			case JS_EVENT_BUTTON | JS_EVENT_INIT:
				/* Always update state */
				if(e.number < _number_of_buttons) {
					_button[e.number] = e.value;
				}

				if(e.type == JS_EVENT_BUTTON) {
					/* Run callback for non-init events */
					callback(e, &finalEvent);
				}
				break;

			/* Axis events and axis init messages */
			case JS_EVENT_AXIS:
			case JS_EVENT_AXIS | JS_EVENT_INIT:
				/* Always update state */
				if(e.number < _number_of_axes) {
					_axis[e.number] = e.value;
				}

				if(e.type == JS_EVENT_AXIS) {
					/* Run callback for non-init events */
					callback(e, &finalEvent);
				}
				break;
			default:

				break;
		}

	}

	_connected = false;
	_busy = false;
}


__s16 Joystick :: button(int index) {
	if(index >= 0 && index < _number_of_buttons) {
		return _button[index];
	}
	return 0;
}

__s16 Joystick :: axis (int index) {
	if(index >= 0 && index < _number_of_axes) {
		return _axis[index];
	}
	return 0;
}
