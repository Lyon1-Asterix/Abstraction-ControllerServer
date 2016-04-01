extern "C" {
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
}

#ifndef SERVER_HPP
#define SERVER_HPP

#include "socklib.h"

class Server {
private:
    int socketServeur;
    int* client;
    const char* port;
    unsigned int nb_client;
	unsigned int nb_client_connected;
    bool connected; // Renvoie vrai si tout les clients sont connectés
public:
    Server();
    Server(const char* nb_client, const char* port);
    ~Server();
    void run();
    int* getClient() { return client; }
    bool isConnected() { return connected; }
    bool resource_ready;
};

#endif // SERVER_HPP
