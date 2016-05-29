#ifndef XBOX_HPP
#define XBOX_HPP

#include "Server.hpp"

/**! \class XboxServer
 * \brief this class make the bridge between the xboxdrv and the client.
 */
class XboxServer : public Server {

private:
  //! xbobxdrv socket
  int xboxdrv_socket;
public:
  XboxServer(const char *nb_client, const char *port);
  // redefine the run method
  void run();
};

#endif
