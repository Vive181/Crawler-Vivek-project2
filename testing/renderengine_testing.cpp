#include <gtest/gtest.h>

#include "../include/renderengine.h"

class RenderEngineTest : public ::testing::Test {
protected:
  RenderEngine engine;

  void TearDown() override { engine.disconnect(); }
};

// Test 1: Cannot connect before creating a Chrome tab
TEST_F(RenderEngineTest, ConnectFailsWithoutWebSocketURL) {
  EXPECT_FALSE(engine.connect());
}

// Test 2: Cannot navigate before WebSocket connection
TEST_F(RenderEngineTest, NavigateFailsWithoutConnection) {
  EXPECT_FALSE(engine.navigate("https://example.com"));
}

// Test 3: Cannot send CDP command without connection
TEST_F(RenderEngineTest, SendCommandFailsWithoutConnection) {
  nlohmann::json params;

  EXPECT_FALSE(engine.sendCommand("Page.enable", params));
}

// Test 4: Last response is initially empty
TEST_F(RenderEngineTest, LastResponseInitiallyEmpty) {
  EXPECT_TRUE(engine.getLastResponse().empty());
}

// Test 5: Disconnecting without connection should be safe
TEST_F(RenderEngineTest, DisconnectWithoutConnectionDoesNotThrow) {
  EXPECT_NO_THROW(engine.disconnect());
}

// Test 6: Creates a new Chrome tab
TEST_F(RenderEngineTest, CreatesChromeTab) {
  EXPECT_TRUE(engine.createTab("https://example.com"));
}

// Test 7: Creates tab and connects to Chrome CDP
TEST_F(RenderEngineTest, ConnectsToChrome) {
  ASSERT_TRUE(engine.createTab("https://example.com"));

  EXPECT_TRUE(engine.connect());
}

// Test 8: Sends Page.enable command
TEST_F(RenderEngineTest, SendsPageEnableCommand) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  nlohmann::json params;

  EXPECT_TRUE(engine.sendCommand("Page.enable", params));
}

// Test 9: Navigates to a webpage
TEST_F(RenderEngineTest, NavigatesToPage) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  EXPECT_TRUE(engine.navigate("https://example.com"));
}

// Test 10: Gets rendered HTML
TEST_F(RenderEngineTest, GetsRenderedHTML) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  ASSERT_TRUE(engine.navigate("https://example.com"));

  std::string html = engine.getRenderedHTML();

  EXPECT_FALSE(html.empty());
}

// Test 11: Rendered HTML contains HTML content
TEST_F(RenderEngineTest, RenderedHTMLContainsHTMLTag) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  ASSERT_TRUE(engine.navigate("https://example.com"));

  std::string html = engine.getRenderedHTML();

  EXPECT_NE(html.find("<html"), std::string::npos);
}

// Test 12: CDP response is stored
TEST_F(RenderEngineTest, StoresLastCDPResponse) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  nlohmann::json params;

  ASSERT_TRUE(engine.sendCommand("Page.enable", params));

  EXPECT_FALSE(engine.getLastResponse().empty());
}

// Test 13: Disconnect is safe
TEST_F(RenderEngineTest, DisconnectsSafely) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  EXPECT_NO_THROW(engine.disconnect());
}

// Test 14: Repeated disconnect is safe
TEST_F(RenderEngineTest, MultipleDisconnectCallsAreSafe) {
  ASSERT_TRUE(engine.createTab("https://example.com"));
  ASSERT_TRUE(engine.connect());

  engine.disconnect();

  EXPECT_NO_THROW(engine.disconnect());
}