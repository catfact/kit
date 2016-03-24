/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include <iostream>
#include <string.h>

//const int RX_PORT = 9997;
const int TX_PORT = 9998;
const int BUFSIZE = 0xffff;
const int TIMEOUT = 1000; // in millisecs

DatagramSocket sock;
char buf[BUFSIZE];

//==============================================================================
int main (int argc, char* argv[])
{
    char ch;
    bool quit;
    int port;
    
    if(argc > 1) { port = atoi(argv[1]); }
    else { port = TX_PORT; }
    
    while(!quit) {
        std::cin >> ch;
        if(ch == 'q') {
            quit = true; 
            break;
        }
        if(ch == 's') {
            scanf("%s",buf);
            
            sock.waitUntilReady(false, TIMEOUT);
            sock.write("localhost", port, buf, strlen(buf));
            printf("\n  sent %s ; t: %lld\r\n", buf, Time::currentTimeMillis());
            
            fflush(stdout);
        }
        
    }
    
    sock.shutdown();
    
    return 0;
}
