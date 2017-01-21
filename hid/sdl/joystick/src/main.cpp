#include <iostream>
#include <stdint.h>
#include <SDL2/SDL.h>

// log SDL error
void logSDLError(std::ostream &os, const std::string &msg){
  os << msg << " error: " << SDL_GetError() << std::endl;
}

// handle josystick axis movement
static int32_t ax[6] = { 0, 0, 0, 0, 0, 0 };
void handle_joy_axis(SDL_Event* e) {
  SDL_JoyAxisEvent* j = (SDL_JoyAxisEvent*)e;
  int idx = j->axis;
  if(idx < 6) {
	ax[idx] = j->value;
	printf("%d\t%d\t%d\t%d\t%d\t%d\r\n",
		   ax[0], ax[1], ax[2], ax[3], ax[4], ax[5]);
  }
}

int main(int, char**){
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
	logSDLError(std::cout, "SDL_Init()");
	return 1;
  }
  
  SDL_Quit();

  printf("SDL_Init() ok! \r\n");
	
  SDL_Joystick *joy;
  int i;
  
  // init joysticks
  SDL_InitSubSystem(SDL_INIT_JOYSTICK);

  if (SDL_NumJoysticks() > 0 ) { 
	joy = SDL_JoystickOpen(0);

	if (joy) {
	  printf("js0: %s \r\n", SDL_JoystickNameForIndex(0));
	} else {
	  printf("couldn't open Joystick 0\n");
	}
  }

  // event loop
  SDL_Event e;
  bool quit = false;
  
  while (!quit){
	
	while (SDL_PollEvent(&e)) {
	  
	  switch(e.type) {
	  case SDL_KEYDOWN:
		printf(" SDL_KEYDOWN: \r\n");
		break;
	  case SDL_KEYUP:
		printf(" SDL_KEYUP: \r\n");
		break;
	  case SDL_MOUSEMOTION:
		//		printf(" SDL_MOUSEMOTION: \r\n");
		break;
	  case SDL_MOUSEBUTTONDOWN:
		//		printf(" SDL_MOUSEBUTTONDOWN: \r\n");
		break;
	  case SDL_MOUSEBUTTONUP:
		//		printf(" SDL_MOUSEBUTTONUP: \r\n");
		break;
	  case SDL_MOUSEWHEEL:
		//		printf(" SDL_MOUSEWHEEL: \r\n");
		break;
	  case SDL_JOYAXISMOTION:
		handle_joy_axis(&e);
		break;
	  case SDL_JOYHATMOTION:
		//		printf(" SDL_JOYHATMOTION: \r\n");
		break;
	  case SDL_JOYBUTTONDOWN:
		printf(" SDL_JOYBUTTONDOWN: \r\n");
		break;
	  case SDL_JOYBUTTONUP:
		printf(" SDL_JOYBUTTONUP: \r\n");
		break;
	  case SDL_JOYDEVICEADDED:
		//		printf(" SDL_JOYDEVICEADDED: \r\n");
		break;		
	  case SDL_JOYDEVICEREMOVED:
		//		printf(" SDL_JOYDEVICEREMOVED: \r\n");
		break;
	  case SDL_QUIT:
		//		printf(" SDL_QUIT: \r\n");
		quit = true;
		break;
	  default:
		;; // nothing
	  }
	}
  }

  // cleanup joystick
  if (SDL_JoystickGetAttached(joy)) {
	SDL_JoystickClose(joy);
  }

  //  SDL_Quit();
	
  return 0;
}

