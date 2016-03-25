/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include <iostream>


const int RX_PORT = 9998;
// const int TX_PORT = 9999;
const int BUFSIZE = 0xffff;
// const int TIMEOUT = 1000; // in millisecs
const int TIMEOUT = -1; // wait forever

DatagramSocket sock;
char buf[BUFSIZE];

// diagnostic: ringbuffer of reception milliseconds
const int MS_BUF_SIZE = 128;
long int msBuf[MS_BUF_SIZE];
int msBufIdx = 0;

//==============================================================================
int main (int argc, char* argv[])
{
    char ch;
    bool quit = false;
    int bytes;
    
    int port;
    String address;
    
    if(argc > 1) { port = atoi(argv[1]); }
    else { port = RX_PORT; }
    
    if(argc > 2) { address = argv[2]; }
    else { address = "127.0.0.1"; }
    
    sock.bindToPort(port, address);
    setbuf(stdout, NULL);
    
    while(!quit) {
        
        sock.waitUntilReady(true, TIMEOUT);
        bytes = sock.read(buf, BUFSIZE, false);
        
        if(bytes > 0) {
            buf[bytes] = 0;
            msBuf[msBufIdx] = Time::currentTimeMillis();
            msBufIdx = (msBufIdx + 1) & (MS_BUF_SIZE - 1);
            printf("rx: %s ; t: %lld, \r\n", buf, Time::currentTimeMillis());
            fflush(stdout);
        } else {
            printf("no rx \r\n");
        }
        
    }
    
    sock.shutdown();

    return 0;
}
