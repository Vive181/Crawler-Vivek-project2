#include "../include/renderengine.h"

#include <curl/curl.h>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                            void *userData) {
  std::string *response = static_cast<std::string *>(userData);

  response->append(static_cast<char *>(contents), size * nmemb);

  return size * nmemb;
}

RenderEngine::RenderEngine() {
  connected = false;
  webSocket = nullptr;
}

bool RenderEngine::connect() {
  if (webSocketURL.empty()) {
    return false;
  }

  try {
    std::string host;
    std::string port;
    std::string target;

    std::string url = webSocketURL;

    size_t protocolEnd = url.find("://");

    if (protocolEnd == std::string::npos) {
      return false;
    }

    std::string address = url.substr(protocolEnd + 3);

    size_t pathStart = address.find("/");

    if (pathStart == std::string::npos) {
      return false;
    }

    target = address.substr(pathStart);

    std::string hostPort = address.substr(0, pathStart);

    size_t colon = hostPort.find(":");

    if (colon == std::string::npos) {
      return false;
    }

    host = hostPort.substr(0, colon);

    port = hostPort.substr(colon + 1);

    auto resolver = boost::asio::ip::tcp::resolver(ioContext);

    auto endpoints = resolver.resolve(host, port);

    webSocket = std::make_unique<
        boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>(
        ioContext);

    boost::asio::connect(webSocket->next_layer(), endpoints);

    webSocket->handshake(host, target);

    connected = true;

    return true;

  } catch (...) {
    connected = false;
    return false;
  }
}

bool RenderEngine::createTab(const std::string &url) {
  CURL *curl = curl_easy_init();

  if (curl == nullptr) {
    return false;
  }

  std::string response;

  std::string endpoint = "http://127.0.0.1:9222/json/new?url=" + url;

  curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode result = curl_easy_perform(curl);

  std::cout << "Response:\n" << response << "\n";

  curl_easy_cleanup(curl);

  if (result != CURLE_OK) {
    return false;
  }

  try {
    json data = json::parse(response);

    webSocketURL = data["webSocketDebuggerUrl"];

    return true;
  } catch (...) {
    return false;
  }
}
bool RenderEngine::navigate(const std::string &url) { return false; }

std::string RenderEngine::getRenderedHTML() { return ""; }

void RenderEngine::disconnect() {}