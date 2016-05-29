#include "XboxServer.hpp"

XboxServer::XboxServer(const char *nb_client, const char *port)
    : Server(nb_client, port), xboxdrv_socket(-1) {}

void XboxServer::run() {
  while (1) {
    // wait xbox client
    while (xboxdrv_socket == -1) {
      fprintf(stdout, "Waiting for xboxdrv client ...\n");
      xboxdrv_socket = AcceptConnexion(socketServeur);
      assert(xboxdrv_socket != -1);
      assert(EnvoieMessage(xboxdrv_socket, (char *)"%s\n", "Welcome Xbox !") !=
             -1);
    }

    // wait data from xboxdrv
    char *data = RecoieLigne(xboxdrv_socket);
    fprintf(stdout, "%s\n", data);
  }
}
