#include "Deuterium/Exception/NamedException.h"

#include <errno.h>           // For errno
#include <string.h>

Deuterium::Exception::NamedException::NamedException(const std::string &message, bool inclSysMsg) throw() : userMessage(message) {
  if (inclSysMsg) {
    userMessage.append(": ");
    userMessage.append(strerror(errno));
  }
}

Deuterium::Exception::NamedException::~NamedException() throw() {
}

const char* Deuterium::Exception::NamedException::what() const throw() {
  return userMessage.c_str();
}