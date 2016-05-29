#include <stdlib.h>

#include "LeapServer.hpp"
#include "XboxServer.hpp"

LeapServer *server;
XboxServer *xbox;

void handler(int sig) {
  fprintf(stdout, "%d signal detected\n", sig);
  delete server;
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "usage : %s <nb_client> <port>\n", argv[0]);
    exit(1);
  }
  signal(SIGINT, handler);
  //server = new LeapServer(argv[1], argv[2]);
  //server->run();
  xbox = new XboxServer(argv[1], argv[2]);
  xbox->run();
  return 0;
}
