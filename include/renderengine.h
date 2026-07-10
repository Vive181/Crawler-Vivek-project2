#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <string>

class RenderEngine {
public:
  RenderEngine();

  bool connect();

  bool createTab(const std::string &url);

  bool navigate(const std::string &url);

  std::string getRenderedHTML();

  void disconnect();

private:
  std::string webSocketURL;

  bool connected;
};

#endif