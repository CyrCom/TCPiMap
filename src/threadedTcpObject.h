#ifndef THREADEDTCPOBJECT_H
#define THREADEDTCPOBJECT_H
#include "ofMain.h"
#include "ofxNetwork.h"

class TcpConnecter: public ofThread{
public:
    void close(){
        waitForThread();
        tcpClientObject.close();
    }

    void setup(string ip, int port, string delim){
        this->ip = ip;
        this->port = port;
        this->delim = delim;
    }

    void threadedFunction(){
        if (tcpClientObject.setup(ip, port)){
            ofLog() << "Successful connexion to " + ip + ":" + ofToString(port);
        }
        tcpClientObject.setMessageDelimiter(delim);
    }

    ofxTCPClient tcpClientObject;
private:
    string ip;
    int port;
    string delim;

};
//tcpConnecter


#endif // THREADEDTCPOBJECT_H
