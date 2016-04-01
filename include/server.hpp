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
    bool connected;
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
