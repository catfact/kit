#pragma once

class FitState{
 public:
  FitState() :
	in(NULL), out(NULL), numIns(0), numOuts(0)
  {}

  ~FitState() { free(in); free(out); }
  
  void addInput(float x) {
	in = (float*)realloc(in, sizeof(float) * (numIns + 1));
	in[numIns++] = x;
  }
  
  void addOutput(float y) {
	out = (float*)realloc(out, sizeof(float) * (numOuts + 1));
	out[numOuts++] = y;
  }
  
  void setInput(unsigned int i, float x) {
	if(i < numIns) { in[i] = x; }
  }

  void setOutput(unsigned int i, float x) {
	if(i < numOuts) { out[i] = x; }
  }

 public:
  static const unsigned int INTERP_SIZE = 512;
  
 private:
  float *in; // input values
  float *out; // output values
  int numIns, numOuts;
  float spect[INTERP_SIZE];
};
