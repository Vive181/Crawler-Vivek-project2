#include "../include/URLvalidator.h"

#include <string>

bool URLValidator::isValid(const std::string &url) {
  if (url.empty()) {
    return false;
  }

  if (!hasValidProtocol(url)) {
    return false;
  }

  
  if (!hasDomain(url)) {
    return false;
  }

  return true;
}

bool URLValidator::hasValidProtocol(const std::string &url) {
  return url.compare(0, 7, "http://") == 0 ||
         url.compare(0, 8, "https://") == 0;
}

bool URLValidator::hasDomain(const std::string &url) {

  size_t start;

  if (url.compare(0, 7, "http://") == 0) {
    start = 7;
  } else {
    start = 8;
  }

  if (start >= url.size()) {
    return false;
  }

  std::string domain = url.substr(start);

  if (domain.find(' ') != std::string::npos) {
    return false;
  }

  if (domain.find('.') == std::string::npos) {
    return false;
  }

  if (domain.back() == '.') {
    return false;
  }

  return true;
}