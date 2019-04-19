#pragma once

#include "ofxGLWarper.h"
#include "ofMain.h"
#include "threadedTcpObject.h"
#include "ofxOMXPlayer.h"
#include "ofxNetwork.h"
#include "proportions.h"

class ofApp : public ofBaseApp{

    public:
// FUNCTIONS -----------------------------------------------------------------------

		void setup();
		void update();
		void draw();
        void exit();
        void keyPressed(int key);

        void drawTestScreen(const unsigned int &surf_nb);
        void addSurface(const bool &enableTexture);
        void centerAlign();
        void reSetupWarp();
        void deleteSurface();

        void loadTCPSettings();
        void setupTCP();
        void drawTCP();
        void keyPressedTCP(int key);
        void cmdRxTCP();

// DRAWING -----------------------------------------------------------------------
        struct colors{
            ofColor selsurf_selcorn = ofColor(31, 255, 255);
            ofColor selsurf_corn = ofColor(255, 31, 31);
            ofColor selsurf_rect = ofColor(255, 31, 31);
            //ofColor basic_selcorn = ofColor(255, 0, 0);
            ofColor basic_corn = ofColor(31, 255, 31);
            ofColor basic_rect = ofColor(31, 255, 31);

            ofColor testScreen = ofColor(255, 255, 255);
            ofColor testScreenInverted = ofColor(0, 255, 0);
            int testScreenAlpha = 255;
            ofColor number = ofColor(0, 0, 0);
            ofColor numberInverted = ofColor(255, 255, 255);
        };
        colors colors;

        bool bGlobalForceDrawingWarpers = false; // forces warpers to be drawn
        bool bShowTestScreen = false; // draws blank screen on top of warpers with surface number
        bool bInvertColors = false; // projects white everywhere but on the screens
        bool bShowDebugText = false; // draws some maybe usefull infos

        int SNF_str_size; // draw debug
        std::string SNF_1, SNF_2; // draw debug

        ofTrueTypeFont surf_nb_font;
        ofRectangle bB_1digit, bB_2digit;
        float nb_size_factor;

// TCP ---------------------------------------------------------------------------

        TcpConnecter tcpConnecter;
        std::string msgTx, msgRx, xmlIP, xmlIP_bis, xmlDelim;
        int xmlPORT, xmlPORT_bis;
        float lastConnectTime;
        float deltaTime;

        std::vector<std::string> cmdVector{16, ""};
        ofDirectory dir;

        ofXml xml_tcp; // file with server IP(s) PORT(s) and DELIMITER
        ofXml xml_memory; // name and saved params

// WARPING -----------------------------------------------------------------------

        std::vector<std::unique_ptr<ofxOMXPlayer>> pOmxPlayers;
        std::vector<std::unique_ptr<ofxOMXPlayerSettings>> pOmxSettings;
        std::vector<std::unique_ptr<ofxGLWarper>> pWarpers;
        std::vector<struct proportions> vProps;

        // default position and size of warper at creation:
        float defWarperX;
        float defWarperY;
        float defWarperWidth;
        float defWarperHeight;



    private:

        unsigned int selectedSurface = 0;
        unsigned int nbSurfaces = 0;
};

