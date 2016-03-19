extern "C" {
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
}

#include "Leap.h"
#include "socklib.h"

#define NB_CLIENT 1

using namespace Leap;

// extern "C" {
//     typedef void (*sighandler_t)();
// }

int socketServeur, client[NB_CLIENT];
bool connected = false, resource_ready;

class LeapListener : public Listener {
private:
    Vector handCenter;
public:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
    std::string getHandCenter () { return handCenter.toString(); }
};

void LeapListener::onConnect (const Controller& controller) {
    resource_ready = false;
    //std::cout << "Connected" << std::endl;
}

void LeapListener::onFrame (const Controller& controller) {
    const Frame frame = controller.frame();
    FingerList fingers = frame.fingers().extended();
    Hand hand = frame.hands()[0];

    // Si la main est valide + 5 doigts
    if (hand.isValid() && fingers.count() == 5 && connected) {
        resource_ready = true;
        handCenter = hand.palmPosition();
        assert (EnvoieMessage(client[0],
                              (char*)"%s\n",
                              handCenter.toString().c_str()) != -1);
    }
}

void handler (int sig) {
    std::cout << "Bye bye !" << std::endl;
    close(socketServeur);
    exit (0);
}


int main(int argc, char *argv[]) {

    Controller controller;
    LeapListener listener;
    int i = 0;

    controller.addListener (listener);

    signal(SIGINT, handler);

    if(argc != 2) {
        fprintf(stderr, "usage : %s <port>\n", argv[0]);
        exit(1);
    }

    socketServeur = CreeSocketServeur(argv[1]);
    assert(socketServeur != -1);

    while(1) {
        while(i != NB_CLIENT) {
            client[i] = AcceptConnexion(socketServeur);
            assert(client[i] != -1);
            assert(EnvoieMessage(client[i], (char*)"%s", "Hello") != -1);
            i++;
        }

        if (!connected)
            connected = true;

        if (resource_ready)
            continue;
        else
            sleep(500);

    }

    // N'ai jamais appelé
    //controller.removeListener (listener);

    return 0;
}
