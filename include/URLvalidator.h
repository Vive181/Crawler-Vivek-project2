#pragma once

#include <string>

class URLValidator {

public:
  static bool isValid(const std::string &url);

private:
  static bool hasValidProtocol(const std::string &url);

  static bool hasDomain(const std::string &url);
};