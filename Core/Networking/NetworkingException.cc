#include "ORCA/Networking/NetworkingException.hh"

#include <errno.h>           // For errno
#include <string.h>

ORCA::Networking::NetworkingException::NetworkingException(const std::string &message, bool inclSysMsg)
  throw() : userMessage(message) {
  if (inclSysMsg) {
    userMessage.append(": ");
    userMessage.append(strerror(errno));
  }
}

ORCA::Networking::NetworkingException::~NetworkingException() throw() {
}

const char* ORCA::Networking::NetworkingException::what() const throw() {
  return userMessage.c_str();
}