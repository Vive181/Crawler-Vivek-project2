#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include <memory>
#include <nlohmann/json.hpp>
#include <string>

#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>

class RenderEngine {
public:
  RenderEngine();

  bool createTab(const std::string &url);

  bool connect();

  bool navigate(const std::string &url);

  std::string getRenderedHTML();

  bool sendCommand(const std::string &method, const nlohmann::json &params);

  void disconnect();

  std::string getLastResponse() const;

private:
  std::string webSocketURL;

  bool connected;

  boost::asio::io_context ioContext;

  std::unique_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>
      webSocket;

  int messageID;

  std::string lastResponse;
};

#endif