# Build Log – Session 2

**Date:** July 10

**Duration:** 120 minutes

## Section 1 - Goal:

Design and implement the **HTML Parser** component responsible for extracting URLs from fetched HTML pages and integrating link discovery into the crawler pipeline.

---

## Section 2 - Work Completed

* Reviewed the crawler workflow after completing the RenderEngine component and identified the HTML Parser as the next required module for URL discovery.

* Defined the role of the **HTML Parser** component in the crawler architecture:

  * Receive raw HTML content from the Fetcher or RenderEngine.
  * Analyze HTML documents.
  * Extract hyperlinks from anchor tags.
  * Convert extracted URLs into crawler-ready absolute URLs.
  * Pass discovered URLs to validation and normalization stages.
---

## HTML Parser Implementation

* Created the HTML Parser class structure with separate responsibilities:

  * `extractLinks()`

    * Responsible for scanning HTML content and extracting URLs.

  * `isIgnoredLink()`

    * Filters invalid or unnecessary URLs.

  * `makeAbsoluteURL()`

    * Converts relative URLs into complete absolute URLs.

* Implemented HTML parsing logic using the custom **KMP string searching algorithm** for pattern matching.

* Added support for detecting anchor tags:

  * Search for `<a>` elements inside HTML.
  * Validate that matched patterns represent actual anchor tags instead of similar text.

* Implemented case-insensitive HTML attribute handling:

  * Created lowercase copies of HTML tags.
  * Supported variations like:

```
href=
HREF =
Href =
```

* Implemented robust `href` extraction:

  * Locate the `href` attribute inside anchor tags.
  * Validate attribute boundaries.
  * Handle spaces, tabs, and newlines around `href` and `=`.

Supported examples:

```html
<a href="page.html">
<a href = "page.html">
<a
href='page.html'>
```

---

## URL Filtering Implementation

Implemented link filtering before adding URLs to the crawler pipeline.

Ignored links:

* Empty URLs.

* Fragment-only URLs:

```
#section
```

* JavaScript links:

```
javascript:void(0)
```

* Email links:

```
mailto:test@example.com
```

* Telephone links:

```
tel:+123456789
```

This prevents invalid resources from entering the crawling queue.

---

## Relative URL Resolution

Implemented URL conversion logic to support different URL formats.

Supported:

### Absolute URLs

Example:

```
https://google.com
```

Returned directly.

---

### Root-relative URLs

Example:

```
/about.html
```

Converted:

```
https://example.com/about.html
```

---

### Current directory URLs

Example:

```
./page.html
```

Converted relative to the current page location.

---

### Parent directory URLs

Implemented support for:

```
../page.html
../../page.html
```

The parser now correctly moves back through parent directories while preventing movement above the domain root.

---

## Testing Performed

Created HTML test cases containing:

* Normal relative links.
* Current directory links.
* Parent directory links.
* Root-relative links.
* Absolute URLs.
* Uppercase HREF attributes.
* Fragment links.
* JavaScript links.

Example input:

```html
<a href="one.html">
<a href="./two.html">
<a href="../three.html">
<a href="/four.html">
<a href="https://google.com">
<a HREF='five.html'>
<a href="#section">
<a href="javascript:void(0)">
```

Output generated:

```
Links Found : 6

https://example.com/docs/tutorial/one.html
https://example.com/docs/tutorial/two.html
https://example.com/docs/three.html
https://example.com/four.html
https://google.com
https://example.com/docs/tutorial/five.html
```

The parser successfully ignored invalid links and generated crawler-ready URLs.

---

## Section 3 - Outcome

The HTML Parser module was successfully implemented and tested.

Completed capabilities:

✅ Anchor tag detection
✅ HREF extraction
✅ Case-insensitive attribute handling
✅ Invalid URL filtering
✅ Relative URL resolution
✅ Absolute URL generation
✅ Integration readiness with Fetcher and RenderEngine

The crawler now has a complete link discovery pipeline:

```
Seed URL
   |
URL Frontier
   |
Fetcher / RenderEngine
   |
HTML Parser
   |
URL Extraction
   |
URL Validation
   |
URL Normalization
   |
Seen Storage
   |
New URLs Added Back To Frontier
```

The next development phase focuses on implementing **Seen Storage** using the custom Set and HashMap data structures to prevent duplicate URL crawling.
