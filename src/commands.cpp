#include "ofApp.h"

void ofApp::cmdRxTCP(){
    bool validSurf = selectedSurface < nbSurfaces;
//    "move_corner [1-4] x y,"
//    "move_warper x y,"
//    "set_corner [1-4] x y,"
//    "set_warper_pos x y (position of TOP LEFT corner),"
//    "set_all_corners x1 y1 x2 y2 x3 y3 x4 y4 (reading order),"
//    "setDisplayRect <x> <y> <width> <height>, -,"
//    "get_all_corners,"

//    "load_video <int>(pathNumber) <optional: paused>(default: playing), -,"

//    "create_surface <opt:direct>,"
//    "delete_surface,"
//    "select_surface <positive int>, -,"
//    "select_corner <1-4> (reading order),"
//    "deselect_corner,"

//    "enableLooping(PLAYER) <true/false> or <0/1>,"
//    "pause <true/false> or <0/1>,"
//    "set_volume(PLAYER) <0<float<=1>,"
//    "step_frame <int>,"
//    "seek_to <int:time in sec>,"

//    "bShowWarpers <true/false> or <0/1>, -,"
//    "bShowTestScreen <true/false> or <0/1>,"
//    "bInvertColors <true/false> or <0/1>,"
//    "bShowDebugText <true/false> or <0/1>,"

//    "adapt_warper_to_media,"
//    "adapt_font_to_warper,"
//    "set_alpha_testScreen <0-255>,"
//    "warpers_colors 'attribute' R G B opt:A (0-255),"

//    "settings <true/false>(enableLooping def:false) <true/false>(enableFilters def:false),"
//    "audio_settings <true/false>(enable) <opt:0 to 1>(initialVol def:0.5) <opt:true/false>(HDMIsnd def:true), -,"

//    "you_are <name>,"
//    "who_is_there,"
//    "list_files,"
//    "exit,"
//    "reset,"
//    "disconnect,"
//    "info"

    //move_corner, [1-4], x, y ///////////////////////////////////////////////////////////////////
    if (validSurf && cmdVector[0]=="move_corner" && pWarpers[selectedSurface] && cmdVector.size() > 3) {

        switch (ofToInt(cmdVector[1])) {
        case 1:
            pWarpers[selectedSurface]->moveCorner(ofxGLWarper::TOP_LEFT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        case 2:
            pWarpers[selectedSurface]->moveCorner(ofxGLWarper::TOP_RIGHT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        case 3:
            pWarpers[selectedSurface]->moveCorner(ofxGLWarper::BOTTOM_LEFT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        case 4:
            pWarpers[selectedSurface]->moveCorner(ofxGLWarper::BOTTOM_RIGHT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        }
    }

    //move_warper, x, y //////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="move_warper" && cmdVector.size() > 2) {
        if (pWarpers[selectedSurface]){
            pWarpers[selectedSurface]->moveAllCorners(ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2]));
        }
    }

    //set_corner, [1-4], x, y ///////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="set_corner" && pWarpers[selectedSurface] && cmdVector.size() > 3 && ofToInt(cmdVector[1]) >0 && ofToInt(cmdVector[1]) <5) {
        switch (ofToInt(cmdVector[1])) {
        case 1:
            pWarpers[selectedSurface]->setCorner(ofxGLWarper::TOP_LEFT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        case 2:
            pWarpers[selectedSurface]->setCorner(ofxGLWarper::TOP_RIGHT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        case 3:
            pWarpers[selectedSurface]->setCorner(ofxGLWarper::BOTTOM_LEFT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        case 4:
            pWarpers[selectedSurface]->setCorner(ofxGLWarper::BOTTOM_RIGHT, ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]));
            break;
        }
    }

    //set_warper_pos, x, y //////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="set_warper_pos" && pWarpers[selectedSurface] && cmdVector.size() > 2) {
        glm::vec2 diff = glm::vec2(ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2])) - pWarpers[selectedSurface]->getCorner(ofxGLWarper::TOP_LEFT);
        pWarpers[selectedSurface]->moveAllCorners(diff);
    }

    //set_all_corners x1 y1 x2 y2 x3 y3 x4 y4 (reading order) ///////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="set_all_corners" && pWarpers[selectedSurface] && cmdVector.size() > 8) {
        pWarpers[selectedSurface]->setCorner(ofxGLWarper::TOP_LEFT, ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2]));
        pWarpers[selectedSurface]->setCorner(ofxGLWarper::TOP_RIGHT, ofToFloat(cmdVector[3]), ofToFloat(cmdVector[4]));
        pWarpers[selectedSurface]->setCorner(ofxGLWarper::BOTTOM_LEFT, ofToFloat(cmdVector[5]), ofToFloat(cmdVector[6]));
        pWarpers[selectedSurface]->setCorner(ofxGLWarper::BOTTOM_RIGHT, ofToFloat(cmdVector[7]), ofToFloat(cmdVector[8]));
    }

    // setDisplayRect, x, y, width, height //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="setDisplayRect" && cmdVector.size() > 4 && pOmxPlayers[selectedSurface]) {
        if(!pWarpers[selectedSurface]){
            if (pOmxPlayers[selectedSurface]->isOpen()){
                pOmxPlayers[selectedSurface]->draw(ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]), ofToFloat(cmdVector[4]));
            }
            pOmxSettings[selectedSurface]->directDrawRectangle = ofRectangle(ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]), ofToFloat(cmdVector[4]));
        }else{
            glm::vec2 top_left = glm::vec2(ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2]));
            glm::vec2 top_right = glm::vec2(ofToFloat(cmdVector[1]) + ofToFloat(cmdVector[3]), ofToFloat(cmdVector[2]));
            glm::vec2 bottom_left = glm::vec2(ofToFloat(cmdVector[1]), ofToFloat(cmdVector[2]) + ofToFloat(cmdVector[4]));
            glm::vec2 bottom_right = glm::vec2(ofToFloat(cmdVector[1]) + ofToFloat(cmdVector[3]), ofToFloat(cmdVector[2]) + ofToFloat(cmdVector[4]));

            pWarpers[selectedSurface]->setAllCorners(top_left, top_right, bottom_left, bottom_right);
        }
    }

    //get_all_corners //////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="get_all_corners" && pWarpers[selectedSurface]) {
        std::string corn_pos_str = "get_all_corners " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::TOP_LEFT).x) + " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::TOP_LEFT).y);
        corn_pos_str += " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::TOP_RIGHT).x) + " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::TOP_RIGHT).y);
        corn_pos_str += " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::BOTTOM_LEFT).x) + " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::BOTTOM_LEFT).y);
        corn_pos_str += " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::BOTTOM_RIGHT).x) + " " + ofToString(pWarpers[selectedSurface]->getCorner(ofxGLWarper::BOTTOM_RIGHT).y) + ";";

        tcpConnecter.tcpClientObject.sendRaw(corn_pos_str);
    }

    // load_video, pathNumber, optional: paused(default: playing) //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="load_video" && cmdVector.size() > 1 && pOmxSettings[selectedSurface] && pOmxPlayers[selectedSurface]) {
        dir.listDir();
        bool setup_ok = false;
        if (ofToInt(cmdVector[1]) >= 0 && ofToInt(cmdVector[1]) < dir.size()){
            pOmxSettings[selectedSurface]->videoPath = ofToDataPath(dir.getPath( ofToInt(cmdVector[1]) ) );
            setup_ok = pOmxPlayers[selectedSurface]->setup(*pOmxSettings[selectedSurface]);
            if (cmdVector.size() > 2 && cmdVector[2]=="paused"){pOmxPlayers[selectedSurface]->setPaused(true);}
        }
        if (setup_ok) { // print infos à supprimer plus tard
            tcpConnecter.tcpClientObject.sendRaw("loaded: " + ofToString(dir.getPath(ofToInt(cmdVector[1]))) + ";");
            if (pWarpers[selectedSurface]) {centerAlign();}
        }else{
            tcpConnecter.tcpClientObject.sendRaw("load NOT ok;");
        }
    }

    // create_surface <opt:direct> //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="create_surface") {
        if (cmdVector.size() > 1 && cmdVector[1] == "direct"){
            addSurface(false);
        }else{
            addSurface(true);
        }
    }

    // delete_surface //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="delete_surface") {
        deleteSurface();
    }

    // select_surface, int (1 - nb of surfaces) OR 'next' ///////////////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="select_surface" && cmdVector.size() > 1 && nbSurfaces > 1) {
        if (pWarpers[selectedSurface]){
            pWarpers[selectedSurface]->deselectCorner(); // back to default
            pWarpers[selectedSurface]->drawSettings.cornersColor = colors.basic_corn; // back to default
            pWarpers[selectedSurface]->drawSettings.rectangleColor = colors.basic_rect; // back to default
        }

        if ( 0 < ofToInt(cmdVector[1]) && ofToInt(cmdVector[1]) <= nbSurfaces ){
            selectedSurface = ofToInt(cmdVector[1]) -1;

        }else if (cmdVector[1] == "next"){
            if (selectedSurface < (nbSurfaces -1))
                {++selectedSurface;}
            else
                {selectedSurface = static_cast<unsigned int>(0);}

        }else if (cmdVector[1] == "previous"){
            if (selectedSurface > static_cast<unsigned int>(0))
                {--selectedSurface;}
            else
                {selectedSurface = nbSurfaces - static_cast<unsigned int>(1);}
        }

        if (pWarpers[selectedSurface]){
            pWarpers[selectedSurface]->drawSettings.cornersColor = colors.selsurf_corn; // gets 'selected' colors
            pWarpers[selectedSurface]->drawSettings.rectangleColor = colors.selsurf_rect; // gets 'selected' colors
        }
    }
    // select_corner <1-4> (reading order) ////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="select_corner" && cmdVector.size() > 1 && pWarpers[selectedSurface]) {
        int corner = ofToInt(cmdVector[1]);
        // rearrange in reading order
        switch (corner) {
        case 1:
            pWarpers[selectedSurface]->selectCorner(ofxGLWarper::TOP_LEFT);
            break;
        case 2:
            pWarpers[selectedSurface]->selectCorner(ofxGLWarper::TOP_RIGHT);
            break;
        case 3:
            pWarpers[selectedSurface]->selectCorner(ofxGLWarper::BOTTOM_LEFT);
            break;
        case 4:
            pWarpers[selectedSurface]->selectCorner(ofxGLWarper::BOTTOM_RIGHT);
            break;
        }
    }

    // deselect_corner //////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="deselect_corner" && cmdVector.size() > 1 && pWarpers[selectedSurface]) {

    }

    // enableLooping(PLAYER), true/false   //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="enableLooping" && cmdVector.size() > 1 && pOmxPlayers[selectedSurface]) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            pOmxPlayers[selectedSurface]->enableLooping();
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            pOmxPlayers[selectedSurface]->disableLooping();
        }
    }

    // pause <true/false> (alternative: <0/1>)  //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="pause" && cmdVector.size() > 1 && pOmxPlayers[selectedSurface]) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            pOmxPlayers[selectedSurface]->setPaused(true);
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            pOmxPlayers[selectedSurface]->setPaused(false);
        }
    }


    // set_volume(PLAYER), (0<float<=1) //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="set_volume" && cmdVector.size() > 1 && ofToFloat(cmdVector[1]) >= 0.0f && ofToFloat(cmdVector[1]) <= 1.0f && pOmxPlayers[selectedSurface]) {
        pOmxPlayers[selectedSurface]->setVolume(ofToFloat(cmdVector[1]));
    }

    // step_frame, int //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="step_frame" && cmdVector.size() > 1 && pOmxPlayers[selectedSurface]) {
        if (ofToInt(cmdVector[1]) > 0) {
            pOmxPlayers[selectedSurface]->stepNumFrames(ofToInt(cmdVector[1]));
        }
    }
    // seek_to, int //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="seek_to" && cmdVector.size() > 1 && pOmxPlayers[selectedSurface]) {
        pOmxPlayers[selectedSurface]->seekToTimeInSeconds(ofToInt(cmdVector[1]));
    }

    // bShowWarpers <true/false> (alternative: <0/1>)
    else if (cmdVector[0]=="bShowWarpers" && cmdVector.size() > 1) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            bGlobalForceDrawingWarpers = true;
            for (unsigned int i=0; i < nbSurfaces; ++i){
                if (pWarpers[i]){
                    pWarpers[i]->drawSettings.bForceDrawing = true;
                }
            }
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            bGlobalForceDrawingWarpers = false;
            for (unsigned int i=0; i < nbSurfaces; ++i){
                if (pWarpers[i]){
                    pWarpers[i]->drawSettings.bForceDrawing= false;
                }
            }
        }
    }

    // bShowTestScreen /////////////////////////////////
    else if (cmdVector[0]=="bShowTestScreen" && cmdVector.size() > 1) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            bShowTestScreen = true;
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            bShowTestScreen = false;
        }
    }
    // bInvertColors /////////////////////////////////
    else if (cmdVector[0]=="bInvertColors" && cmdVector.size() > 1) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            bInvertColors = true;
            ofSetBackgroundColor(255,255,255);
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            bInvertColors = false;
            ofSetBackgroundColor(0,0,0);
        }
    }
    // bShowDebugText /////////////////////////////////
    else if (cmdVector[0]=="bShowDebugText" && cmdVector.size() > 1) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            bShowDebugText = true;
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            bShowDebugText = false;
        }
    }

    // adapt_warper_to_media ///////////////////////////////////////
    else if (validSurf && cmdVector[0]=="adapt_warper_to_media"){
        reSetupWarp();
    }

    // adapt_font_to_warper true/false/0/1 ///////////////////////////////////////
    else if (validSurf && cmdVector[0]=="adapt_font_to_warper" && cmdVector.size() > 1) {
        if (cmdVector[1] == "true" || cmdVector[1] == "1") {
            vProps[selectedSurface].adapt_to_warper(pWarpers[selectedSurface]->getBaseRectangle());
        }else if (cmdVector[1] == "false" || cmdVector[1] == "0") {
            vProps[selectedSurface].adapt_to_media();
        }
    }

    // set_alpha_testScreen       /////////////////////////////////////////////////
    else if (cmdVector[0]=="set_alpha_testScreen" && cmdVector.size() > 1) {
            colors.testScreenAlpha = ofToInt(cmdVector[1]);
    }

    // warpers_colors attribute R G B opt:A (0-255) ////////////////////////////////////////////////////////////////////////////////////////
    // --attribute: basic_corn basic_rect selsurf_corn selsurf_rect
    else if (cmdVector[0]=="warpers_colors" && cmdVector.size() > 4) {
        ofColor colorToApply;
        if (cmdVector.size() > 5){
            colorToApply = ofColor(ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]), ofToFloat(cmdVector[4]), ofToFloat(cmdVector[5]));
        }else{
            colorToApply = ofColor(ofToFloat(cmdVector[2]), ofToFloat(cmdVector[3]), ofToFloat(cmdVector[4]));
        }
        if (cmdVector[1] == "basic_corn") {
            colors.basic_corn = colorToApply;
            for (unsigned int i=0; i < nbSurfaces; ++i){
                if (pWarpers[i] && i != selectedSurface){
                    pWarpers[i]->drawSettings.cornersColor = colors.basic_corn; // warper colors: not selected
                }
            }
        }else if (cmdVector[1] == "basic_rect") {
            colors.basic_rect = colorToApply;
            for (unsigned int i=0; i < nbSurfaces; ++i){
                if (pWarpers[i] && i != selectedSurface){
                    pWarpers[i]->drawSettings.rectangleColor = colors.basic_rect; // warper colors: not selected
                }
            }
        }else if (cmdVector[1] == "selsurf_corn") {
            colors.selsurf_corn = colorToApply;
            if (pWarpers[selectedSurface]){
                pWarpers[selectedSurface]->drawSettings.cornersColor = colors.selsurf_corn; // warper colors: selected
            }
        }else if (cmdVector[1] == "selsurf_rect") {
            colors.selsurf_rect = colorToApply;
            if (pWarpers[selectedSurface]){
                pWarpers[selectedSurface]->drawSettings.rectangleColor = colors.selsurf_rect; // warper colors: selected
            }
        }else if (cmdVector[1] == "selsurf_selcorn") {
            colors.selsurf_selcorn = colorToApply;
            for (unsigned int i=0; i < nbSurfaces; ++i){
                if (pWarpers[i]){
                    pWarpers[i]->drawSettings.selectedCornerColor = colors.selsurf_rect; // warper colors: selected
                }
            }
        }
    }

    // settings <true/false>(enableLooping def:false) <true/false>(enableFilters def:false) //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="settings" && cmdVector.size() > 1 && pOmxSettings[selectedSurface]) {
        if (cmdVector[1]=="true"){
            pOmxSettings[selectedSurface]->enableLooping = true;
        }else if (cmdVector[1]=="false"){
            pOmxSettings[selectedSurface]->enableLooping = false;
        }
        if (cmdVector.size() > 2){
            if (cmdVector[3]=="true") {
                pOmxSettings[selectedSurface]->enableFilters = true;
            }else if (cmdVector[3]=="false") {
                pOmxSettings[selectedSurface]->enableFilters = false;
            }
        }
    }

    // audio_settings <true/false>(enable) <0 to 1>(initialVol) <true/false>(HDMIsnd) //////////////////////////////////////////////////////////////////////////////////////
    else if (validSurf && cmdVector[0]=="audio_settings" && cmdVector.size() > 1 && pOmxSettings[selectedSurface]) {
        if (cmdVector[1]=="true"){
            pOmxSettings[selectedSurface]->enableAudio = true;
        }else if (cmdVector[1]=="false"){
            pOmxSettings[selectedSurface]->enableAudio = false;
        }
        if (cmdVector.size() > 2 && ofToFloat(cmdVector[2]) >= 0.0f && ofToFloat(cmdVector[2]) <= 1.0f){
            pOmxSettings[selectedSurface]->initialVolume = ofToFloat(cmdVector[2]);
        }
        if (cmdVector.size() > 3) {
            if (cmdVector[3]=="true"){
                pOmxSettings[selectedSurface]->useHDMIForAudio = true;
            }else if (cmdVector[3]=="false"){
                pOmxSettings[selectedSurface]->useHDMIForAudio = false;
            }
        }
    }

    // you_are //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="you_are" && cmdVector.size() > 1) {
            xml_memory.load("Memory.xml");
            auto NAME = xml_memory.getChild("NAME");
            if(!NAME){NAME = xml_memory.appendChild("NAME");}
            NAME.set(cmdVector[1]);
            xml_memory.save("Memory.xml");
    }

    // who_is_there //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="who_is_there") {
        xml_memory.load("Memory.xml");
        auto NAME = xml_memory.getChild("NAME");
        if(!NAME){tcpConnecter.tcpClientObject.sendRaw("NAME no_name_provided;");}
        else{tcpConnecter.tcpClientObject.sendRaw("NAME " + NAME.getValue() + ";");}
    }

    // list_files //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="list_files") {
        dir.listDir();
        if(dir.size() > 0){
            std::string fileInPath = "";
            if (cmdVector[1]=="dropdown"){
                fileInPath = "dropdown names ";
                for(unsigned int i = 0; i < dir.size(); i++){
                    std::string file_name = dir.getName(i);
                    ofStringReplace(file_name, " ", "_");
                    fileInPath += file_name;
                    fileInPath.append(" ");
                }
            }else{
                for(unsigned int i = 0; i < dir.size(); i++){
                    fileInPath += ofToString(i) + " -- " + dir.getName(i);
                    fileInPath.append(",");
                }
            }
            fileInPath.append(";");
            tcpConnecter.tcpClientObject.sendRaw(fileInPath);
        }
    }

    // exit //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="exit") {
        ofExit();
    }

    // reset /////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="reset") {
        while (nbSurfaces > 0){
            deleteSurface();
        }
    }

    // disconnect //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="disconnect") {
        tcpConnecter.close();
    }

    // info //////////////////////////////////////////////////////////////////////////////////////
    else if (cmdVector[0]=="info") {
        tcpConnecter.tcpClientObject.sendRaw(
                    "---------------------------------------,"
                    "exit,"
                    "reset,"
                    "disconnect,"
                    "you_are <name>,"
                    "who_is_there,"
                    "list_files,"
                    "           - - - -,"
                    "create_surface <opt:direct>,"
                    "delete_surface,"
                    "select_surface <positive int>, -,"
                    "select_corner <1-4> (reading order),"
                    "deselect_corner,"

                    "settings <true/false>(enableLooping def:false) <true/false>(enableFilters def:false),"
                    "audio_settings <true/false>(enable) <opt:0 to 1>(initialVol def:0.5) <opt:true/false>(HDMIsnd def:true), -,"

                    "load_video <int>(pathNumber) <optional: paused>(default: playing), -,"

                    "enableLooping(PLAYER) <true/false> or <0/1>,"
                    "pause <true/false> or <0/1>,"
                    "set_volume(PLAYER) <0<float<=1>,"
                    "step_frame <int>,"
                    "seek_to <int:time in sec>,"
                    "           - - - -,"
                    "move_warper x y,"
                    "move_corner [1-4] x y,"
                    "set_all_corners x1 y1 x2 y2 x3 y3 x4 y4 (reading order),"
                    "set_warper_pos x y (position of TOP LEFT corner),"
                    "set_corner [1-4] x y,"
                    "setDisplayRect <x> <y> <width> <height>, -,"

                    "get_all_corners,"
                    "              - - - -,"
                    "bShowTestScreen <true/false> or <0/1>,"
                    "bInvertColors <true/false> or <0/1>,"
                    "bShowDebugText <true/false> or <0/1>,"
                    "bShowWarpers <true/false> or <0/1>, -,"

                    "adapt_warper_to_media,"
                    "adapt_font_to_warper true/false/0/1 else to default -> media,"
                    "set_alpha_testScreen <0-255>,"
                    "warpers_colors 'attribute' R G B opt:A (0-255),"
                    "--attribute: basic_corn basic_rect selsurf_corn selsurf_rect selsurf_selcorn,"
                    "you can use '+' to make mutliple commands"
                    ";");
    }

    //  wrong command 1 //////////////////////////////////////////////////////////////////////////////////////
    else {
        std::string unkCMD = "Unknown or wrong command: '";
        for (unsigned int var = 0; var < cmdVector.size(); ++var) {
            unkCMD += cmdVector[var];
            if (cmdVector.size()-1 != var){unkCMD += " ";}
        }
        unkCMD += "';";
        tcpConnecter.tcpClientObject.sendRaw(unkCMD);
    }
}
