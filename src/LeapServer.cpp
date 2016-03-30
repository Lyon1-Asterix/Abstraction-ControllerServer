#include "LeapServer.hpp"
#include <vector>

LeapServer* pointer;

LeapServer::LeapServer() : server(NULL) {
    controller.addListener(*this);
}

LeapServer::LeapServer(const char* nb_client, const char* port) : LeapServer() {
    server = new Server(nb_client, port);
    pointer = this;
    direction = UNDEFINED;
}

LeapServer::~LeapServer() {
    delete server;
    controller.removeListener(*this);
}

void handler (int sig) {
    fprintf (stdout, "Bye bye !\n");
    delete pointer;
    exit (0);
}

void LeapServer::run() {
    // Plug le Ctrl-C
    signal(SIGINT, handler);
    if(server != NULL)
        server->run();
}

void LeapServer::onConnect (const Controller& controller) {
    server->resource_ready = false;
}

Direction LeapServer::findDirection (const Vector& vector) {
     if (vector.x > 90) { // Droite
          return RIGHT;
     } else if (vector.x < -90) { // Gauche
          return LEFT;
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

     assert (EnvoieMessage(server->getClient()[0],
                           (char*)"%s\n",
                           directionToString[(int)direction].c_str()) != -1);
}

void LeapServer::onFrame (const Controller& controller) {
    const Frame frame = controller.frame();
    FingerList fingers = frame.fingers().extended();
    Hand hand = frame.hands()[0];
    // MAJ de la direction
    //direction = findDirection (hand.palmPosition());
    // Si la main est valide + 5 doigts
    if (hand.isValid() && fingers.count() == 5 && server->isConnected()) {
         //handCenter = hand.palmPosition();
         Direction tempDirection = findDirection (hand.palmPosition());
         if (tempDirection != direction) {
              sendData (tempDirection);
              direction = tempDirection;
         }
    }
    server->resource_ready = true;
}
