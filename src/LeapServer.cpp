#include "LeapServer.hpp"

LeapServer::LeapServer(const char* nb_client, const char* port) :
    Server (nb_client, port) {
    controller.addListener(*this);
    direction = UNDEFINED;
}

LeapServer::~LeapServer() {
    controller.removeListener(*this);
}

void LeapServer::onConnect (const Controller& controller) {
    fprintf (stdout, "Le Leap Motion est connecté : %d\n",
             controller.isConnected());
    this->resource_ready = true;
}

Direction LeapServer::findDirection (const Vector& vector) {
    if (vector.x > 90) { // Droite
        return LEFT;
    } else if (vector.x < -90) { // Gauche
        return RIGHT;
    } else if (vector.z < -90) { // Haut
        return FORWARD;
    } else if (vector.z > 90) { // Bas
        return BACKWARD;
    } else {
        return UNDEFINED;
    }
}

void LeapServer::sendData (const Direction& direction) {
    std::string directionToString [] = {
        "FORWARD",
        "BACKWARD",
        "RIGHT",
        "LEFT",
        "UNDEFINED"
    };

    assert (EnvoieMessage(this->getClient()[0],
                          (char*)"%s\n",
                          directionToString[(int)direction].c_str()) != -1);
}

void LeapServer::onFrame (const Controller& controller) {
    const Frame frame = controller.frame();
    FingerList fingers = frame.fingers().extended();
    Hand hand = frame.hands()[0];
    // Si les clients sont connectés
    if (this->isConnected ()) {
        // Si la main est valide + 5 doigts
        if (hand.isValid() && fingers.count() == 5) {
            Direction tempDirection = findDirection (hand.palmPosition());
            if (tempDirection != direction) {
                sendData (tempDirection);
                // MAJ de la direction
                direction = tempDirection;
            }
        }
    }
}
