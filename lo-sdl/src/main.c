#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <lo/lo.h>
#include <SDL2/SDL.h>

int quit = 0;
lo_address remote_addr;

void error_handler(int num, const char *m, const char *path) {
  printf("liblo server error %d in path %s: %s\n", num, path, m);
  fflush(stdout);
}

void init_sdl(void) {
    SDL_InitSubSystem(SDL_INIT_EVENTS);
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	if (SDL_NumJoysticks() > 0 ) {
	  SDL_Joystick *joy;
	  printf("found joystick, opening js0 \r\n");
	  joy = SDL_JoystickOpen(0);
	  if (joy) {
		printf("js0: %s \r\n", SDL_JoystickNameForIndex(0));
	  } else {
		printf("failed to open js0\n");
	  }
	}
}

void handle_sdl_event(SDL_Event* e) {
  switch(e->type) {
  case SDL_JOYAXISMOTION:
	lo_send(remote_addr, "/joystick/axis", "iii",
			e->jaxis.which, e->jaxis.axis, e->jaxis.value);
	break;
  case SDL_JOYBALLMOTION:
	lo_send(remote_addr, "/joystick/ball", "iiii",
			e->jball.which, e->jball.ball, e->jball.xrel, e->jball.yrel);
	break;
  case SDL_JOYHATMOTION:
	lo_send(remote_addr, "/joystick/hat", "iii",
			e->jhat.which, e->jhat.hat, e->jhat.value);
	break;
  case SDL_JOYBUTTONDOWN:
	lo_send(remote_addr, "/joystick/button/down", "iii",
			e->jbutton.which, e->jbutton.button, e->jbutton.state);
	break;
  case SDL_JOYBUTTONUP:
	lo_send(remote_addr, "/joystick/button/up", "iii",
			e->jbutton.which, e->jbutton.button, e->jbutton.state);
	break;
  case SDL_JOYDEVICEADDED:
	lo_send(remote_addr, "/joystick/device/added", "i", e->jdevice.which);
	break;
  case SDL_JOYDEVICEREMOVED:
	lo_send(remote_addr, "/joystick/device/removed", "i", e->jdevice.which);
	break;

  case SDL_QUIT:
	quit = 1;
  }
}


void event_loop(void) {
  SDL_Event e;
  while(!quit) {
	if(SDL_WaitEvent(&e)) {
	  handle_sdl_event(&e);
	}
  }
}


int main(int argc, char** argv) {
  char remote_port[32] = "57120";
  char local_port[32] = "8888";
  if(argc > 1) {
	strncpy(remote_port, argv[1], 32);
  }
  if(argc > 1) {
	strncpy(local_port, argv[2], 32);
  }

  // send a request
  lo_address remote_addr = lo_address_new(NULL, remote_port);


  init_sdl();
  event_loop();
  
  //lo_server_thread_free(st);
  
}
