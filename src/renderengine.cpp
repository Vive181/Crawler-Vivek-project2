#include "../include/renderengine.h"

#include <curl/curl.h>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

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
  messageID = 1;
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

  std::string endpoint = "http://127.0.0.1:9222/json/new?" + url;

  curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  CURLcode result = curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  if (result != CURLE_OK) {
    return false;
  }

  try {
    json data = json::parse(response);

    webSocketURL = data["webSocketDebuggerUrl"];

    connected = false;

    return true;
  } catch (...) {
    return false;
  }
}

bool RenderEngine::navigate(const std::string &url) {
  json params;

  params["url"] = url;

  return sendCommand("Page.navigate", params);
}

std::string RenderEngine::getRenderedHTML() {
  json params;

  params["expression"] = "document.documentElement.outerHTML";

  params["returnByValue"] = true;

  bool success = sendCommand("Runtime.evaluate", params);

  if (!success) {
    return "";
  }

  try {
    json response = json::parse(lastResponse);

    if (!response.contains("result"))
      return "";

    if (!response["result"].contains("result"))
      return "";

    if (!response["result"]["result"].contains("value"))
      return "";

    return response["result"]["result"]["value"];

  } catch (...) {
    return "";
  }
}

bool RenderEngine::sendCommand(const std::string &method, const json &params) {
  if (!connected) {
    return false;
  }

  if (webSocket == nullptr) {
    return false;
  }

  int currentID = messageID++;

  json request;

  request["id"] = currentID;
  request["method"] = method;
  request["params"] = params;

  webSocket->write(boost::asio::buffer(request.dump()));

  while (true) {
    boost::beast::flat_buffer responseBuffer;

    webSocket->read(responseBuffer);

    lastResponse = boost::beast::buffers_to_string(responseBuffer.data());

    try {
      json response = json::parse(lastResponse);

      // Event hai
      if (!response.contains("id")) {
        continue;
      }

      // Kisi aur request ka response
      if (response["id"] != currentID) {
        continue;
      }

      // Ye isi command ka response hai
      return true;

    } catch (...) {
      return false;
    }
  }
}

void RenderEngine::disconnect() {}

std::string RenderEngine::getLastResponse() const { return lastResponse; }