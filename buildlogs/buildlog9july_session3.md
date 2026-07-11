# Build Log – Session 3

**Date:** July 10
**Duration:** 120 minutes

## Goal

Finalize the browser verification strategy for the Fetcher and begin implementing the HTTP fetching component using **libcurl**.

---

## Section 1 – Problem

Before implementing the Fetcher, I needed to determine how the crawler would retrieve web pages efficiently while remaining compatible with future rendering support.

The crawler should first attempt to download the page using a lightweight HTTP client instead of immediately launching a browser. This approach minimizes resource usage and improves crawling speed for traditional HTML pages.

---

## Section 2 – Browser Verification Strategy

After evaluating different approaches, I finalized the following strategy for the crawler.

1. The crawler first downloads the page using the **Fetcher**.
2. The Fetcher performs a standard HTTP GET request.
3. The downloaded HTML is returned to the crawler.
4. A separate framework detection component will later analyze the response to determine whether the page is a traditional HTML page or requires browser rendering.
5. Only pages that actually require rendering will be processed by the rendering engine in future development.

This design avoids opening a browser for every URL and keeps the crawler efficient for large-scale web crawling.

---

## Section 3 – Fetcher Design

The Fetcher is responsible only for downloading web pages.

Its responsibilities include:

* Accepting a URL from the crawler.
* Sending an HTTP GET request.
* Downloading the response body.
* Returning the HTML content.
* Returning the HTTP status code.
* Reporting any network errors.

The Fetcher does **not** perform HTML parsing, framework detection, or rendering. These responsibilities belong to separate components.

---

## Section 4 – libcurl Implementation

To implement the Fetcher, I selected **libcurl** because it is lightweight, reliable, and widely used for HTTP communication in C++.

The implementation follows this workflow:

1. Create a CURL handle.
2. Configure the target URL.
3. Register a callback function to receive incoming data.
4. Execute the HTTP request.
5. Store the downloaded HTML in a string.
6. Retrieve the HTTP status code.
7. Clean up all allocated resources.

This keeps the Fetcher simple while providing the information required by the remaining crawler components.

---

## Section 5 – Data Reception Logic

Instead of receiving the complete webpage in a single operation, libcurl delivers the response in multiple chunks.

A callback function appends each incoming chunk to a single HTML string until the entire page has been downloaded.

This allows the crawler to handle pages of varying sizes efficiently without assuming that the response will arrive all at once.

---

## Section 6 – Current Status

The basic Fetcher interface has been designed and prepared for implementation.

The current design supports:

* URL input
* HTTP GET requests
* HTML content retrieval
* HTTP status code retrieval
* Error reporting

Additional features such as redirects, timeout configuration, content-type handling, and browser rendering integration will be added in later development sessions.

---

## Result

* Finalized the browser verification strategy for the Fetcher.
* Defined the responsibilities of the Fetcher within the crawler architecture.
* Selected **libcurl** as the HTTP client library.
* Designed the Fetcher workflow for downloading web pages.
* Planned the callback-based mechanism for collecting HTML content.
* Prepared the Fetcher architecture for future integration with framework detection and browser-based rendering.
