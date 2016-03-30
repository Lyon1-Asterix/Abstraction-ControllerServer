#include <iostream>
#include "LeapServer.hpp"

int main(int argc, char *argv[]) {

    //LeapServer* server;
     LeapServer* server;

    if(argc != 3) {
        fprintf(stderr, "usage : %s <nb_client> <port>\n", argv[0]);
        exit(1);
    }

    server = new LeapServer(argv[1], argv[2]);
    server->run();

    return 0;
}
