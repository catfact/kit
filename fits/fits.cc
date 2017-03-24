/*
  fits.cc

  points-fitting and interpolation testing stuff
*/

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

#include "fitstate.h"
#include "helpers.h"

using namespace cv;
using namespace std;

//---- types and classes
class UI {
public:
  int mx, my, mstate, mwin;
  enum { WIN_IN, WIN_OUT, WIN_SHOW };
  
  void mouse(int x, int y, int w, int s) {
	mx = x;
	my = y;
	mwin = w;
	mstate = s;
  }  
};

//-- static vars
Mat in;
Mat out;
Mat show;
Mat tmp;

const int w = 800;
const int h = 300;

UI ui;  
FitState f;
  
//--- static fns
void handle_mouse_in (int event, int x, int y, int, void* );
void handle_mouse_out (int event, int x, int y, int, void* );
void handle_mouse_show (int event, int x, int y, int, void* );

void draw_in(void);
void draw_out(void);


//--- main function
int main( int, char** argv )
{
  int k;
  in = Mat::zeros(h, w, CV_8UC3);
  out = Mat::zeros(h, w, CV_8UC3);
  show = Mat::zeros(h, w, CV_8UC3);
  tmp = Mat::zeros(h, w, CV_8UC3);
  
  imshow("in", in );
  imshow("out", out );
  imshow("show", show );

  setMouseCallback( "in", handle_mouse_in, 0 );
  setMouseCallback( "out", handle_mouse_out, 0 );
  setMouseCallback( "show", handle_mouse_show, 0 );

  bool quit = false;
  while(!quit) {
	k = waitKey(0);
	printf("key: %d \n", k);
	switch(k) {
	case 113 : // q
	  quit = true;
	  break;
	case 97: // a
	  switch(ui.mwin) {
	  case UI::WIN_IN:
		f.addInput(ui.mx);
		draw_in();
		break;
	  case UI::WIN_OUT:
		f.addOutput(ui.mx);
		draw_out();
		break;

	  }
	  break;
	}
  }
  
  return 0;
}

void handle_mouse_in (int event, int x, int y, int, void* ) {
  printf("mouse_in: %d; (%d, %d) \n", event, x, y);
  ui.mouse(x, y, UI::WIN_IN, event > 0);
  draw_in();
}

void handle_mouse_out (int event, int x, int y, int, void* ) {
  printf("mouse_out: %d; (%d, %d) \n", event, x, y);
  ui.mouse(x, y, UI::WIN_OUT, event > 0);
  draw_out();
}

void handle_mouse_show (int event, int x, int y, int, void* ) {
  printf("mouse_show: %d; (%d, %d) \n", event, x, y);
  ui.mouse(x, y, UI::WIN_SHOW, event > 0);
}

void draw_in(void) {
  in.setTo(0);
  if(ui.mwin == UI::WIN_IN) {
	line(in, Point(ui.mx, h), Point(ui.mx, 0), Scalar(100, 100, 100));
	line(in, Point(0, ui.my), Point(w, ui.my), Scalar(100, 100, 100));
  }
  tmp.setTo(0);
  image_overlay(tmp, in, 0.5);
  imshow("in", in);
}


void draw_out(void) {
  out.setTo(0);
  if(ui.mwin == UI::WIN_OUT) {
	line(out, Point(ui.mx, h), Point(ui.mx, 0), Scalar(100, 100, 100));
	line(out, Point(0, ui.my), Point(w, ui.my), Scalar(100, 100, 100));
  }
  imshow("out", out);
}
