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
bool connected = false;

class LeapListener : public Listener {
public:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
};

void LeapListener::onConnect (const Controller& controller) {
    //std::cout << "Connected" << std::endl;
}

void LeapListener::onFrame (const Controller& controller) {
    //std::cout << "Frame available" << std::endl;
    const Frame frame = controller.frame();
    //Hand firstHand = frame.hands[0];
    FingerList fingers = frame.fingers().extended();
    //std::string dummy = "%d";
    if (connected)
        for (int i = 0; i < NB_CLIENT; i++)
            assert ( EnvoieMessage(client[i], // (char*)dummy.c_str()
                                   (char*)"%d",
                                   fingers.count()) != -1 );
    //std::cout << fingers.count() << std::endl;
}

int main(int argc, char *argv[]) {

    Controller controller;
    LeapListener listener;
    int i = 0;

    controller.addListener (listener);

    // std::cout << "Press Enter to quit ..." << std::endl;
    // std::cin.get ();

    /* int d[2], s, i = 0; */

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

        connected = true;
        //std::cout << "Tout le monde est connecté" << std::endl;
    }

    controller.removeListener (listener);
    close(socketServeur);

    return 0;
}
