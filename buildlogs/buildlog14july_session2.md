# Build Log — Session 2

**Date:** July 14, 2026  
**Duration:** 3–4 hours  
**Project:** Web Crawler  

## Goal

Test the remaining crawler components, find edge-case bugs, improve component reliability, and begin testing the CDP-based RenderEngine.

---

## SeenStorage Testing

**Problem:**  
SeenStorage was implemented using `Set<std::string>`, but its behavior had not been verified.

**What I Tried:**  
Added tests for adding URLs, duplicate insertion, checking seen URLs, removing URLs, size tracking, empty storage, and clearing storage.

**Outcome:**  
All SeenStorage tests passed. Duplicate URLs are correctly rejected.

---

## HTMLParser Testing

**Problem:**  
Basic HTMLParser tests passed, but stronger edge cases exposed multiple bugs.

**Failures Found:**

- Uppercase `JAVASCRIPT:` and `MAILTO:` links were not ignored.
- `../` could move above the domain root.
- `href` inside another attribute value was incorrectly detected.
- `>` inside quoted attributes broke tag parsing.
- Links inside HTML comments were extracted.

**What I Tried:**  
Improved case-insensitive ignored-link detection, URL resolution, `href` attribute detection, quote-aware tag parsing, and HTML comment handling.

**Outcome:**  
All **33 HTMLParser tests passed**.

---

## FrameworkDetector Testing

**Problem:**  
Tests initially called `FrameworkDetector::detect()` as a static function, causing:

`cannot call member function without object`

**What I Tried:**  
Created a `FrameworkDetector` object and tested Next.js, React, Angular, Vue, heavy JavaScript, static HTML, empty HTML, and framework priority.

**Outcome:**  
All FrameworkDetector tests passed.

---

## PageStorage Testing

**Problem:**  
PageStorage tests initially failed to compile because the Google Test header was missing.

**What I Tried:**  
Fixed the test file and tested MySQL connection, page storage, retrieval, updates, existence checking, removal, page count, clearing, and additional edge cases.

**Outcome:**  
All PageStorage tests passed successfully.

---

## Fetcher Improvements and Testing

**Problem:**  
Fetcher had configurable timeout and redirect APIs, but hardcoded values were still being used internally.

**What I Tried:**  
Updated Fetcher to use configurable user agent, timeout, and maximum redirect values.

Added tests for:

- Successful requests.
- Status codes.
- Final URLs.
- Content types.
- Invalid and malformed URLs.
- Request body storage.
- Custom configuration.
- Empty URLs.
- Redirects.
- 404 responses.
- JSON responses.
- Query parameters.

**Outcome:**  
The initial **12 tests passed**. After adding stronger edge cases, **14 of 18 tests passed**.

The remaining four tests failed because `httpbin.org` returned HTTP `503`, affecting 404, redirect, redirect-limit, and JSON tests. A local HTTP test server was identified as the better solution for reliable integration testing.

---

## RenderEngine Testing

**Goal:**  
Begin testing JavaScript page rendering through Chrome DevTools Protocol (CDP).

**What I Tried:**  
Created tests for:

- Connection without a WebSocket URL.
- Navigation without connection.
- Sending CDP commands.
- Creating Chrome tabs.
- Connecting to Chrome.
- `Page.enable`.
- Navigation.
- Retrieving rendered HTML.
- CDP response storage.
- Safe disconnection.

**Problem:**  
The first CMake setup tried to link `RenderEngineTests` with the `Crawler` executable.

CMake reported:

`Target "Crawler" of type EXECUTABLE may not be linked into another target.`

**Fix:**  
Changed the test target to compile `renderengine.cpp` directly.

A second issue then appeared:

`CURL::libcurl target was not found`

The RenderEngine test target now needs to use the same CURL linking configuration already used by the main Crawler target.

---

## Final Outcome

- SeenStorage testing completed.
- All 33 HTMLParser tests passed after fixing edge cases.
- FrameworkDetector testing completed.
- PageStorage MySQL testing completed.
- Fetcher configuration improved.
- 14 of 18 Fetcher tests passed; 4 depend on an unreliable external server.
- RenderEngine CDP tests were created.
- CMake executable-linking issue was fixed.
- CURL linking for RenderEngine tests remains to be configured.

**Next Step:**  
Fix CURL linking for `RenderEngineTests`, build the test executable, start Chrome on remote debugging port `9222`, and run the CDP integration tests.