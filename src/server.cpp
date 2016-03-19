#include "server.hpp"

Server::Server() : socketServeur(-1),
                   client(NULL),
                   port(NULL),
                   nb_client(0),
                   connected(false),
                   resource_ready(false) {}

Server::Server(const char* nb_client_, const char* port_)
    : Server() {
    assert (atoi(nb_client_) > 0);
    assert (port_ != NULL);

    nb_client = atoi(nb_client_);
    port = port_;

    client = new int [nb_client];

    // On créer la socket d'écoute
    socketServeur = CreeSocketServeur(port_);
    assert (socketServeur != -1);
}

Server::~Server() {
    delete[] client;
    close (socketServeur);
}

void handler (int sig) {
    exit (0);
}

void Server::run() {
    // Plug le Ctrl-C
    signal(SIGINT, handler);

    unsigned int i = 0;
    while(1) {
        // On attend les clients + petit message de bienvenue
        while(i != nb_client) {
            client[i] = AcceptConnexion(socketServeur);
            assert(client[i] != -1);
            assert(EnvoieMessage(client[i], (char*)"%s", "Hello") != -1);
            i++;
        }

        if (!connected)
            connected = true;

        // Permet de ne pas bousiller le cpu
        if (resource_ready)
            continue;
        else
            sleep(500);
    }
}
