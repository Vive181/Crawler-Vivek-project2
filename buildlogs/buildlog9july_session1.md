# Build Log – Session 1

**Date:** June 9
**Duration:** 120 minutes

## Goal

Implement the `URLNormalizer` module to convert different representations of the same URL into a single canonical format before they are stored or compared.

## Problem

The same webpage can be represented by multiple URLs. For example:

* `HTTP://Example.com`
* `http://example.com`
* `http://example.com:80`
* `http://example.com/#section`
* `http://example.com/`

Without normalization, these URLs would be treated as different entries, leading to duplicate crawling and unnecessary storage.

## Methods Added

### `normalize(const std::string& url)`

**Purpose:**
Acts as the main entry point for URL normalization by calling each normalization step in sequence.

**Why:**
Keeps the normalization process centralized and ensures every URL follows the same pipeline.

---

### `normalizeProtocol(const std::string& url)`

**Purpose:**
Converts the protocol (`HTTP`, `HTTPS`) to lowercase.

**Why:**
URL protocols are case-insensitive, so `HTTP://` and `http://` should be treated as the same URL.

---

### `normalizeDomain(const std::string& url)`

**Purpose:**
Converts the domain name to lowercase.

**Why:**
Domain names are case-insensitive, so `Example.COM` and `example.com` represent the same website.

---

### `removeDefaultPort(const std::string& url)`

**Purpose:**
Removes default ports (`:80` for HTTP and `:443` for HTTPS).

**Why:**
These ports are implied by the protocol and do not change the destination of the URL.

---

### `removeFragment(const std::string& url)`

**Purpose:**
Removes the fragment portion beginning with `#`.

**Why:**
Fragments identify locations within a webpage and do not represent different web pages for crawling purposes.

---

### `normalizeTrailingSlash(const std::string& url)`

**Purpose:**
Removes an unnecessary trailing slash when appropriate.

**Why:**
`http://example.com` and `http://example.com/` usually refer to the same resource, so treating them identically prevents duplicate URLs.

## What I Tried

Designed the normalizer as a sequence of independent helper functions, where each function performs one normalization rule. The main `normalize()` function applies these rules in order to produce a canonical URL. I also created and added a **URLValidator workflow diagram** to document how an input URL is validated before entering the crawler, and a **URLNormalizer workflow diagram** to illustrate each normalization step applied to a URL before it is stored or compared.

## Outcome

Successfully implemented the `URLNormalizer` module. URLs are now normalized before comparison or storage, allowing duplicate URL detection to work more reliably and reducing redundant crawling. Additionally, the **URLValidator** and **URLNormalizer** workflow diagrams were added to improve the project's documentation, making both the validation and normalization processes easier to understand, maintain, and present.
