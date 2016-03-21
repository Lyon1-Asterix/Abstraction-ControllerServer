#include "LeapServer.hpp"
#include <vector>

LeapServer* pointer;

LeapServer::LeapServer() : server(NULL) {
    controller.addListener(*this);
}

LeapServer::LeapServer(const char* nb_client, const char* port) : LeapServer() {
    server = new Server(nb_client, port);
    pointer = this;
}

LeapServer::~LeapServer() {
    delete server;
    controller.removeListener(*this);
}

void handler (int sig) {
    // TODO call server destructor
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

void LeapServer::onFrame (const Controller& controller) {
    const Frame frame = controller.frame();
    FingerList fingers = frame.fingers().extended();
    Hand hand = frame.hands()[0];

    // Si la main est valide + 5 doigts
    if (hand.isValid() && fingers.count() == 5 && server->isConnected()) {
        handCenter = hand.palmPosition();
        assert (EnvoieMessage(server->getClient()[0],
                              (char*)"%s\n",
                              handCenter.toString().c_str()) != -1);
    }
    server->resource_ready = true;
}
