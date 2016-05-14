#ifndef LEAP_SERVER_HPP
#define LEAP_SERVER_HPP

#include "../LeapSDK/include/Leap.h"
#include "server.hpp"

enum Direction {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    UNDEFINED
};

using namespace Leap;

class LeapServer : public Listener, public Server {
private:
    Direction direction;
    Controller controller;
public:
    LeapServer();
    LeapServer(const char* nb_client, const char* port);
    ~LeapServer();
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
    // Retourne la direction du vecteur de la main
    Direction findDirection (const Vector&);
    // Envoi la direction de la main au client
    void sendData (const Direction&);
};

#endif // LEAP_SERVER_HPP
