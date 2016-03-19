#include "LeapServer.hpp"

void LeapServer::onConnect (const Controller& controller) {
    // resource_ready = false;
}

void LeapServer::onFrame (const Controller& controller) {
    const Frame frame = controller.frame();
    FingerList fingers = frame.fingers().extended();
    Hand hand = frame.hands()[0];

    // Si la main est valide + 5 doigts
    if (hand.isValid() && fingers.count() == 5) {
        handCenter = hand.palmPosition();
        // assert (EnvoieMessage(client[0],
        //                       (char*)"%s\n",
        //                       handCenter.toString().c_str()) != -1);
    }
}
