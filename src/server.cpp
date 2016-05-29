#include "server.hpp"

Server::Server(const char *nb_client_, const char *port_) : Server() {
  assert(atoi(nb_client_) > 0);
  assert(port_ != NULL);

  nb_client = atoi(nb_client_);
  port = port_;

  // On créer les sockets clients
  client = new int[nb_client];

  // On créer la socke d'écoute (le serveur quoi ...)
  socketServeur = CreeSocketServeur(port_);
  assert(socketServeur != -1);
}

Server::Server()
    : socketServeur(-1), client(NULL), port(NULL), nb_client(0),
      nb_client_connected(0), connected(false), resource_ready(false) {}

void closeSocketClient(int client) {
  assert(EnvoieMessage(client, (char *)"%s\n", "Bye bye !\n") != -1);
  close(client);
}

Server::~Server() {
  // Fermeture des sockets clients
  for (unsigned i = 0; i < nb_client_connected; i++) {
    closeSocketClient(client[i]);
  }
  delete[] client;
  close(socketServeur);
}

void Server::run() {
  while (1) {
    while (nb_client_connected != nb_client) {
      // On attend les clients + petit message de bienvenue
      fprintf(stdout, "Attente des clients %d/%d\n", nb_client_connected,
              nb_client);
      client[nb_client_connected] = AcceptConnexion(socketServeur);
      assert(client[nb_client_connected] != -1);
      assert(EnvoieMessage(client[nb_client_connected], (char *)"%s\n",
                           "Hello !") != -1);
      nb_client_connected++;
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
