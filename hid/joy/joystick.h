#include <linux/joystick.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <linux/ioctl.h>

class Joystick {
public:
	Joystick(const char *js_device, void (*setCallback)(struct js_event e, bool *finalEvent));
	~Joystick();
	void getInput();

	/* Some state things */
	bool busy() { return _busy; };
	bool connected() { return _connected; };
	int number_of_buttons() { return _number_of_buttons; };
	int number_of_axes() { return _number_of_axes; };
	__s16 button(int index);
	__s16 axis(int index);

private:
	bool _busy;
	bool _connected;
	int _number_of_buttons;
	int _number_of_axes;
	__s16 *_axis;
	__s16 *_button;

	void (*callback)(struct js_event e, bool *finalEvent);
	int fd;
	bool wait_for_next_event(struct js_event *e);
};

