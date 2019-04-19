#include "ofMain.h"
#include "ofApp.h"

int main()
{
    // ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
    //ofSetWindowPosition(1920 - 1280, 0);
	ofRunApp( new ofApp());
}
