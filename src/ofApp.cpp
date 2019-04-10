#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetBackgroundColor(0,0,0);

    setupTCP();

    loadTCPSettings();

    // default warper size at creation
    defWarperX = 40;
    defWarperY = 40;
    defWarperWidth = ofGetWidth()/4.5f;
    defWarperHeight = ofGetHeight()/4.5f;

    //loading font and bounding boxes for surface numbers, one time for all.
    if(!surf_nb_font.load("cmu_typewriter_regular.ttf", 32, false, false, true)){
        ofLog() << "Please provide ./bin/cmu_typewriter_regular.ttf file";
    }
    nb_size_factor = 0.75f;
    bB_1digit = surf_nb_font.getStringBoundingBox("9", 0, 0);
    bB_2digit = surf_nb_font.getStringBoundingBox("99", 0, 0);
    // would you really use 99+ mappers ?
}

void ofApp::setupTCP(){
    // our send and recieve strings
    msgTx	= "";
    msgRx	= "";

    dir.open("videos");

    lastConnectTime = -15.0f;
    deltaTime = 0.0f;

    // draw debug test
    ofBitmapFont f;
    SNF_1 = "SERVER NOT FOUND. Reconnecting";
    SNF_2 = ". . . . .";
    SNF_str_size = static_cast<int>(f.getBoundingBox(SNF_1 + " in " + ofToString(15.0f, 2) + " seconds.", 0, 0).width - f.getBoundingBox(SNF_2, 0, 0).width);
}
//--------------------------------------------------------------

void ofApp::loadTCPSettings(){

    xmlIP = "NO IP";
    xmlPORT = 0;
    xmlDelim = ";\n";

    xmlIP_bis = "NO IP";
    xmlPORT_bis = 0;
    // xmlDelim = ";\n";

    //we load our settings file
    if (xml_tcp.load("TCPSettings.xml")) {
        auto server = xml_tcp.getChild("SERVER");
        if (server && server.getChild("IP") && server.getChild("PORT")){
            xmlIP = server.getChild("IP").getValue();
            xmlPORT = server.getChild("PORT").getIntValue();
        }else{
            ofLog() << "TCPSettings.xml file miss some SERVER child, with IP and PORT grandchildren !";
        }
        auto server_bis = xml_tcp.getChild("SERVER_BIS");
        if (server_bis && server_bis.getChild("IP") && server_bis.getChild("PORT")){
            xmlIP_bis = server_bis.getChild("IP").getValue();
            xmlPORT_bis = server_bis.getChild("PORT").getIntValue();
        }else{
            ofLog() << "TCPSettings.xml file miss some SERVER_BIS child, with IP and PORT grandchildren !";
        }
    }else{
        ofLog() << "TCPSettings.xml file was not found in bin/data folder.";
    }
}


void ofApp::update(){
    deltaTime = ofGetElapsedTimef() - lastConnectTime;

    if (!tcpConnecter.isThreadRunning()){

        if(tcpConnecter.tcpClientObject.isConnected()){
            // we are connected - lets try to receive from the server and do what he says to do !
            std::string receive_temp_str = tcpConnecter.tcpClientObject.receive();
            if( receive_temp_str.length() > 0 ){
                msgRx = receive_temp_str;
                std::vector<std::string> dividedCmds = ofSplitString(receive_temp_str, "+", true);
                for (unsigned int var = 0; var < dividedCmds.size(); ++var) {
                    cmdVector = ofSplitString(dividedCmds[var], " ", true);

                    if (cmdVector.size() > 0){cmdRxTCP();}
                }
            }
            lastConnectTime = ofGetElapsedTimef();

        }else{
            // we are not connected - lets try again every 15 seconds
            if( deltaTime > 15.0f ){
                static bool bSwitch = false;
                if (bSwitch && xmlIP_bis != "NO IP"){
                    tcpConnecter.setup(xmlIP_bis, xmlPORT_bis, xmlDelim);
                    ofLog() << "Attempting connection to "+ xmlIP_bis + ":" + ofToString(xmlPORT_bis) + " as SERVER IP:PORT.";
                    bSwitch = false;
                    tcpConnecter.startThread();
                }else if (xmlIP != "NO IP"){
                    tcpConnecter.setup(xmlIP, xmlPORT, xmlDelim);
                    ofLog() << "Attempting connection to "+ xmlIP + ":" + ofToString(xmlPORT) + " as SERVER IP:PORT. ";
                    bSwitch = true;
                    tcpConnecter.startThread();
                }
                loadTCPSettings(); // reload the file (can be manually edited via SSH)
                lastConnectTime = ofGetElapsedTimef(); // let's wait again
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    for (unsigned int i=0; i < nbSurfaces; ++i){
        if (pWarpers[i]){

            pWarpers[i]->begin();	//all the things that are drawn AFTER ofxGLWarper's begin method are afected by it.

            pOmxPlayers[i]->draw(vProps[i].x, vProps[i].y, vProps[i].width, vProps[i].height);

            if (bShowTestScreen){
                // drawing blank test screen
                ofPushStyle();
                (bInvertColors) ? ofSetColor(colors.testScreenInverted, colors.testScreenAlpha) : ofSetColor(colors.testScreen, colors.testScreenAlpha);
                ofDrawRectangle(pWarpers[i]->getBaseRectangle());

                // drawing surface number at the center of the warper.
                ofPushMatrix();
                unsigned int surf_nb = i + static_cast<unsigned int>(1);
                ofTranslate(vProps[i].getTranslationVector(surf_nb));
                ofScale(vProps[i].getFontFactor(surf_nb));
                (bInvertColors) ? ofSetColor(colors.numberInverted) : ofSetColor(colors.number);
                surf_nb_font.drawStringAsShapes(ofToString(surf_nb), 0, 0);
                ofPopMatrix();

                ofPopStyle();
            }
            pWarpers[i]->end();
        }
    }

    if(bDrawDebugText){
        drawTCP();
    }
}

void ofApp::drawTCP(){

    ofPushStyle();
    (bInvertColors)?ofSetColor(0, 0, 0):ofSetColor(255, 255, 255);

    if(!tcpConnecter.tcpClientObject.isConnected()){
        static short int inc = 0; ++inc; static short int prop_of_SNF; prop_of_SNF = inc%SNF_str_size;
        ofDrawBitmapString(SNF_2, 15 + SNF_str_size -(prop_of_SNF), 105);
        ofDrawBitmapString(SNF_2, 15 + (prop_of_SNF), 105);
        if (tcpConnecter.isThreadRunning()){
            ofDrawBitmapString(SNF_1, 15, 100);
        }else{
            ofDrawBitmapString(SNF_1 + " in " + ofToString(15.0f - deltaTime, 2) + " seconds.", 15, 100);
        }
    } else{
        ofDrawBitmapString("Received from server:\n    " + msgRx, 15, 25);
        if(!msgTx.empty()){
            ofDrawBitmapString("Execute commands:\n    " + msgTx, 15, 60);
        }
    }
    ofDrawBitmapString("selectedSurface: " + ofToString(selectedSurface) +
                       ", nbSurfaces: " + ofToString(nbSurfaces)
                       , 15, ofGetHeight()-30);
    ofDrawBitmapString("tcpConnecter.tcpClientObject.isConnected(): " + ofToString(tcpConnecter.tcpClientObject.isConnected()) + " || tcpConnecter.isThreadRunning(): " + ofToString(tcpConnecter.isThreadRunning())
                       , 15, ofGetHeight()-15);
    ofPopStyle();
}

void ofApp::keyPressed(int key){

    keyPressedTCP(key);

    if (key==OF_KEY_TAB && nbSurfaces > 0){
        if (selectedSurface < (nbSurfaces -1)){
            selectedSurface++;
        }else {
            selectedSurface = 0;
        }
    }

    if (selectedSurface < nbSurfaces && pWarpers[selectedSurface]){
        glm::vec2 moveBy;
        if (key == OF_KEY_UP){
            moveBy = glm::vec2(0,-1);
        } else if (key == OF_KEY_DOWN){
            moveBy = glm::vec2(0,1);
        } else if (key == OF_KEY_LEFT){
            moveBy = glm::vec2(-1,0);
        } else if (key == OF_KEY_RIGHT){
            moveBy = glm::vec2(1,0);
        }
        moveBy *= 4;
        pWarpers[selectedSurface]->moveAllCorners(moveBy);
    }
}

void ofApp::keyPressedTCP(int key){
    if(key == OF_KEY_BACKSPACE || key == OF_KEY_DEL){
        if(!msgTx.empty()){
            msgTx = msgTx.substr(0, msgTx.size()-1);
        }
    // Enter key is 13 on Windows and 10 on linux (at least for me)
    }else if(key==OF_KEY_RETURN){
        if(tcpConnecter.tcpClientObject.isConnected() && !tcpConnecter.isThreadRunning()){
            if( msgTx.length() > 0 ){
                std::vector<std::string> dividedCmds = ofSplitString(msgTx, "+", true);
                for (unsigned int var = 0; var < dividedCmds.size(); ++var) {
                    cmdVector = ofSplitString(dividedCmds[var], " ", true);
                    if (cmdVector.size() > 0){cmdRxTCP();}
                }
            }
            msgTx.append(";");
            tcpConnecter.tcpClientObject.sendRaw(msgTx);
            //tcpConnecter.tcpClientObject.send(msgTx);
            msgTx.clear();
        }
    }else{
        msgTx += static_cast<char> (key);
    }
}

void ofApp::addSurface(const bool &enableTexture){
    defWarperX += 5;
    defWarperY += 5;

    pOmxSettings.emplace_back(new ofxOMXPlayerSettings);
    pOmxSettings[nbSurfaces]->enableTexture = enableTexture; //default true

    pOmxPlayers.emplace_back(new ofxOMXPlayer);

    if (enableTexture){
        pWarpers.emplace_back(new ofxGLWarper);
        pWarpers[nbSurfaces]->setup(defWarperX, defWarperY, defWarperWidth, defWarperHeight); //initializes ofxGLWarper
        pWarpers[nbSurfaces]->setCornerSensibility(0.1f);
        if (bGlobalForceDrawing) {pWarpers[nbSurfaces]->drawSettings.bForceDrawing = true;}
    }else{
        pWarpers.emplace_back(nullptr);
    }

    vProps.emplace_back(defWarperX, defWarperY, defWarperWidth, defWarperHeight, &bB_1digit, &bB_2digit, &nb_size_factor);

    //pOmxSettings[nbSurfaces]->useHDMIForAudio =   true;	//default true
    pOmxSettings[nbSurfaces]->enableLooping =       false;	//default true
    //pOmxSettings[nbSurfaces]->autoStart =         false;  //default true
    //pOmxSettings[nbSurfaces]->enableAudio =       true;	//default true, save resources by disabling
    //pOmxSettings[nbSurfaces]->doFlipTexture =     false;	//default false
    //pOmxSettings[nbSurfaces]->enableFilters =     false;  //default false
    //pOmxSettings[nbSurfaces]->doFlipTexture =     false;  //default false
    //pOmxSettings[nbSurfaces]->initialVolume =     0.5;    //default 0.5

    if (pWarpers[selectedSurface]){
        pWarpers[selectedSurface]->drawSettings.cornersColor = colors.basic_corn; // warper colors: not selected
        pWarpers[selectedSurface]->drawSettings.rectangleColor = colors.basic_rect; // warper colors: not selected
    }
    selectedSurface=nbSurfaces++;
    if (pWarpers[selectedSurface]){
        pWarpers[selectedSurface]->drawSettings.cornersColor = colors.selsurf_corn; // warper colors: selected
        pWarpers[selectedSurface]->drawSettings.rectangleColor = colors.selsurf_rect; // warper colors: selected
    }
}

void ofApp::centerAlign(){

    ofRectangle warper_rect = pWarpers[selectedSurface]->getBaseRectangle();

    if (static_cast<float>(pOmxPlayers[selectedSurface]->getWidth()) / static_cast<float>(pOmxPlayers[selectedSurface]->getHeight())
            > warper_rect.width/warper_rect.height){
        vProps[selectedSurface].x = warper_rect.x;
        vProps[selectedSurface].width = warper_rect.width;
        vProps[selectedSurface].height = warper_rect.height * static_cast<float>(pOmxPlayers[selectedSurface]->getHeight()) / static_cast<float>(pOmxPlayers[selectedSurface]->getWidth()) * (warper_rect.width / warper_rect.height);
        vProps[selectedSurface].y = warper_rect.y + ((warper_rect.height - vProps[selectedSurface].height) / 2.0f);
    }else{
        vProps[selectedSurface].width = warper_rect.width * (static_cast<float>(pOmxPlayers[selectedSurface]->getWidth()) / static_cast<float>(pOmxPlayers[selectedSurface]->getHeight())) * (warper_rect.height / warper_rect.width);
        vProps[selectedSurface].x = warper_rect.x + ((warper_rect.width - vProps[selectedSurface].width) / 2.0f);
        vProps[selectedSurface].y = warper_rect.y;
        vProps[selectedSurface].height = warper_rect.height;
    }

    if (vProps[selectedSurface].bAdapt_to_warper){
        vProps[selectedSurface].adapt_to_warper(warper_rect);
    }else{
        vProps[selectedSurface].adapt_to_media();
    }
}

void ofApp::reSetupWarp(){

    pWarpers[selectedSurface]->reSetupWarped(
                vProps[selectedSurface].x,
                vProps[selectedSurface].y,
                vProps[selectedSurface].width,
                vProps[selectedSurface].height
                );

    if (vProps[selectedSurface].bAdapt_to_warper){
        vProps[selectedSurface].adapt_to_warper(pWarpers[selectedSurface]->getBaseRectangle());
    }else{
        vProps[selectedSurface].adapt_to_media();
    }
}

void ofApp::deleteSurface(){
    if (nbSurfaces == 0){return;}
    defWarperX -= 5;
    defWarperY -= 5;

    pOmxSettings.erase(pOmxSettings.begin() + selectedSurface);

    pOmxPlayers.erase(pOmxPlayers.begin() + selectedSurface);

    pWarpers.erase(pWarpers.begin() + selectedSurface);

    vProps.erase(vProps.begin() + selectedSurface);

    if (selectedSurface > 0){
        selectedSurface = selectedSurface - 1;
    }

    if (pWarpers[selectedSurface]){
        pWarpers[selectedSurface]->drawSettings.cornersColor = colors.selsurf_corn; // warper colors: selected
        pWarpers[selectedSurface]->drawSettings.rectangleColor = colors.selsurf_rect; // warper colors: selected
    }

    --nbSurfaces;
}

void ofApp::exit(){
    tcpConnecter.close();
}
