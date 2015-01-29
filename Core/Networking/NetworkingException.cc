#include "Deuterium/Networking/NetworkingException.hh"

#include <errno.h>           // For errno
#include <string.h>

Deuterium::Networking::NetworkingException::NetworkingException(const std::string &message, bool inclSysMsg)
  throw() : userMessage(message) {
  if (inclSysMsg) {
    userMessage.append(": ");
    userMessage.append(strerror(errno));
  }
}

Deuterium::Networking::NetworkingException::~NetworkingException() throw() {
}

const char* Deuterium::Networking::NetworkingException::what() const throw() {
  return userMessage.c_str();
}