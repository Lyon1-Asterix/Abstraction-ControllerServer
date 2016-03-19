#ifndef LEAP_SERVER_HPP
#define LEAP_SERVER_HPP

#include "Leap.h"
#include "server.hpp"

using namespace Leap;

class LeapServer : public Listener {
private:
    Server* server;
    Vector handCenter;
    Controller controller;
public:
    LeapServer();
    LeapServer(const char* nb_client, const char* port);
    ~LeapServer();
    void run();
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
    std::string getHandCenter () { return handCenter.toString(); }
};

#endif // LEAP_SERVER_HPP
