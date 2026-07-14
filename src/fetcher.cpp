#include "../include/fetcher.h"

#include <curl/curl.h>

static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                            void *userData) {
  std::string *html = (std::string *)userData;

  html->append((char *)contents, size * nmemb);

  return size * nmemb;
}

Fetcher::Fetcher() : userAgent("Crawler/1.0"), timeout(10L), maxRedirects(5L) {}

FetchResult Fetcher::fetch(const std::string &url) {
  FetchResult result;

  result.requestedURL = url;

  CURL *curl = curl_easy_init();

  if (curl == nullptr) {
    result.errorMessage = "CURL Initialization Failed";
    return result;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result.html);

  curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());

  // Follow redirects
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  // Maximum redirects
  curl_easy_setopt(curl, CURLOPT_MAXREDIRS, maxRedirects);

  // timeout 10 sec
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

  CURLcode response = curl_easy_perform(curl);

  if (response == CURLE_OK) {
    result.success = true;

    // HTTP Status Code
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result.statusCode);

    // Final URL after redirects
    char *finalURL = nullptr;

    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &finalURL);

    if (finalURL != nullptr) {
      result.finalURL = finalURL;
    }

    // Content-Type
    char *contentType = nullptr;

    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);

    if (contentType != nullptr) {
      result.contentType = contentType;
    }
  } else {
    result.success = false;
    result.errorMessage = curl_easy_strerror(response);
  }

  curl_easy_cleanup(curl);

  return result;
}

void Fetcher::setUserAgent(const std::string &agent) { userAgent = agent; }

void Fetcher::setTimeout(long seconds) { timeout = seconds; }

void Fetcher::setMaxRedirects(long redirects) { maxRedirects = redirects; }