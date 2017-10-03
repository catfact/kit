// basic program to demonstrate use of libsndfile for direct synthesis


// std
#include <math.h>
#include <stdbool.h>
#include <string.h>

// 3rd party
#include <sndfile.h>

static const double TWOPI = 6.2831853071796;

// convert amp/db
static double ampdb (double amp) { return log10(amp) * 20.0; }
static double dbamp (double db) { return pow(10.0, db * 0.05); }

// zero a buffer
static void buf_zero(double* buf, int nsamps) {
  for(int i=0; i<nsamps; ++i) { buf[i] = 0.0; }
}

// mix a sine wave into a buffer
static void buf_add_sine(double *buf, int nsamps, double hz, double level, int sr) {
  int i = 0;
  double phi = TWOPI * hz / (double)sr;
  double theta = 0.0;
  double amp = dbamp(level);
  while(i<nsamps) {
    buf[i] = sin(theta) * amp;
    theta += phi;
    ++i;
  }
}

// apply fadein (exponential)
static void buf_fadein_exp(double *buf, int nsamps, double dur, int sr) {
  // fade duration in samples
  int n = (int)ceil(dur * (double)sr);
  int i = 0;
  // initial amplitude 
  double x = dbamp(-60.0);
  // exponential coefficient
  double c = 1.0 / (double)n;
  c = exp(c * log(1.0 / x));
  while(i < n) {
    x *= c;
    buf[i] *= x;
    ++i;
  }
  buf[0] = 0.0;
}

// apply fadeout (exponential)
static void buf_fadeout_exp(double *buf, int nsamps, double dur, int sr) {
  int n = (int)ceil(dur * (double)sr);
  int i = nsamps - (n+2);
  double y = dbamp(-60.0);
  double c = 1.0 / (double)n;
  c = exp(c * log(y));
  double x = 1.0;
  while(i < nsamps) {
    x *= c;
    buf[i] *= x;
    ++i;
  }
  buf[nsamps-1] = 0.0;
}

// apply fadein (linear)
static void buf_fadein_lin(double *buf, int nsamps, double dur, int sr) {
  int n = (int)ceil(dur * (double)sr);
  int i = 0;
  double inc = 1.0 / (double)n;
  double x = 0.0;
  // exponential coefficient
  while(i < n) {
    buf[i] *= x;
    x += inc;
    ++i;
  }
}

// apply fadeout (linear)
static void buf_fadeout_lin(double *buf, int nsamps, double dur, int sr) {
  int n = (int)ceil(dur * (double)sr);
  int i = nsamps - (n+2);
  double inc = 1.0 / (double)n;
  double x= 1.0;
  while(i < nsamps) {
    buf[i] *= x;
    x -= inc;
    ++i;
  }
}

// TODO: s-curve i guess

int main(int argc, const char *argv[]) {

  const char* outfilename = "sine.wav";

  if(argc > 1) {
    outfilename = argv[1];
  }
  
  printf("setting up buffer... \n");
  double dur = 10.0;
  int sr = 48000;    
  int nsamps = (int)ceil(dur * (double)sr);
  double buf[nsamps];
  buf_zero(buf, nsamps);

  printf("synthesizing... \n");
  double hz = 220.0;
  double level = -6.0;
  double fadein = 2.0;
  double fadeout = 2.0;
  
  buf_add_sine(buf, nsamps, hz, level, sr);
#if EXP_FADE
  buf_fadein_exp(buf, nsamps, fadein, sr);
  buf_fadeout_exp(buf, nsamps, fadeout, sr);
#else
  buf_fadein_lin(buf, nsamps, fadein, sr);
  buf_fadeout_lin(buf, nsamps, fadeout, sr);
#endif // exp/lin
  
  printf("exporting... \n");
#if STEREO
  // TODO, &c
#else
  
  SNDFILE *outfile;
  SF_INFO outinfo;

  
  memset(&outinfo, 0, sizeof(outinfo));
  outinfo.frames = nsamps;
  outinfo.samplerate = sr;
  outinfo.channels = 1;
  // 24-bit wav
  outinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
  outinfo.sections = 1;
  outinfo.seekable = 1;

  if ((outfile = sf_open(outfilename, SFM_WRITE, &outinfo)) == NULL) {
    printf("failed to open output file: %s\n", outfilename);
    puts (sf_strerror(NULL));
    return 1;
  }
  
  sf_write_double(outfile, buf, nsamps);
  sf_close(outfile);
#endif // stereo/mono

  printf("done! bye. \n");
  return 0;
  
}
