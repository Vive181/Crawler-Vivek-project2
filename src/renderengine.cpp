#include "../include/renderengine.h"
#include <nlohmann/json.hpp>

RenderEngine::RenderEngine() { connected = false; }

bool RenderEngine::connect() { return false; }

bool RenderEngine::createTab(const std::string &) { return false; }

bool RenderEngine::navigate(const std::string &) { return false; }

std::string RenderEngine::getRenderedHTML() { return ""; }

void RenderEngine::disconnect() {}