extern "C" {
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
}

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socklib.h"

class Server {
protected:
  int socketServeur;
  int *client;
  const char *port;
  unsigned int nb_client;
  unsigned int nb_client_connected;
  bool connected; // Renvoie vrai si tout les clients sont connectés
public:
  Server();
  Server(const char *nb_client, const char *port);
  ~Server();
  void run();
  int *getClient() { return client; }
  bool isConnected() { return connected; }
  bool resource_ready;
};

#endif // SERVER_HPP
