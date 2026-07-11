# Project 2 - Web Crawler


## Introduction

A **Web Crawler** is a software system that automatically discovers, downloads, and processes web pages by following hyperlinks. It begins with one or more **seed URLs**, fetches their content, extracts new links, validates and normalizes them, and continues this process recursively. The primary objective is to build a collection of web pages that can later be used for applications such as search engines, data mining, website analysis, and information retrieval.

This crawler is designed using a **modular architecture**, where each component is responsible for a single task. This separation of responsibilities improves maintainability, scalability, testing, and allows new features to be added without affecting existing modules.

The crawler supports both **static HTML pages** and **JavaScript-rendered pages**. Static pages are fetched using **libcurl**, while dynamically rendered pages are fetched using the **Chrome DevTools Protocol (CDP)**. A lightweight threshold-based scoring system is used to determine which fetching strategy should be applied.

---

# Main Components

## 1. URL Validator

### Purpose

The URL Validator ensures that only valid URLs enter the crawling pipeline.

### Responsibilities

- Verify that the URL is not empty.
- Validate supported protocols (`http://` and `https://`).
- Ensure that a valid domain name exists.
- Reject malformed or incomplete URLs.
- Prevent invalid URLs from entering the URL Frontier.

---

## 2. URL Frontier

### Purpose

The URL Frontier manages all URLs waiting to be crawled.

### Responsibilities

- Store discovered URLs.
- Schedule URLs using FIFO order.
- Add newly discovered URLs.
- Return the next URL for crawling.
- Track crawl depth (if required).
- Prevent duplicate scheduling.

---

## 3. URL Normalizer

### Purpose

The URL Normalizer converts different representations of the same webpage into one canonical form.

### Responsibilities

- Convert protocol and hostname to lowercase.
- Remove default ports.
- Remove URL fragments (`#fragment`).
- Normalize trailing slashes.
- Resolve relative URLs into absolute URLs.
- Produce a standardized URL before duplicate checking.

### Example

**Input**

```
HTTP://Example.com/
https://example.com:443
https://example.com/#about
```

**Normalized Output**

```
https://example.com
```

---

## 4. Seen URL Storage

### Purpose

Seen URL Storage prevents duplicate crawling.

### Responsibilities

- Store normalized URLs.
- Check whether a URL has already been discovered.
- Prevent duplicate crawling.
- Provide fast lookup using a hash-based data structure.

---

## 5. Page Storage

### Purpose

Page Storage stores downloaded webpages for future processing.

### Responsibilities

- Store webpage URL.
- Store HTML content.
- Store metadata such as:
  - HTTP status code
  - Crawl timestamp
  - Crawl depth
- Retrieve stored pages efficiently.
- Provide data for indexing and further processing.

---

## 6. Fetcher

### Purpose

The Fetcher downloads webpage content.

The crawler supports two different fetching mechanisms.

---

### Static HTML Fetcher

Used for websites whose content is directly available in the server response.

#### Technology

- libcurl

#### Advantages

- Fast execution
- Low memory usage
- High crawling throughput
- Suitable for traditional websites

---

### Rendered HTML Fetcher

Used for websites that generate content dynamically using JavaScript.

#### Technology

- Chrome DevTools Protocol (CDP)

The browser loads the webpage, executes JavaScript, waits for rendering to complete, and extracts the final DOM.

---

# Static vs Rendered HTML Detection

Before selecting the fetcher, the crawler analyzes the initial HTML response and assigns a **JavaScript Rendering Score**.

Each detected indicator contributes points toward the final score.

| Indicator | Points |
|-----------|-------:|
| Large number of `<script>` tags | +2 |
| Root application container (`#root`, `#app`) | +3 |
| Very small visible HTML body | +2 |
| React, Vue, Angular, Next.js keywords | +3 |
| Large number of empty placeholder elements | +2 |
| `<noscript>` requesting JavaScript | +2 |
| Fully populated HTML content | -3 |

---

## Threshold Decision

After calculating the total score:

| Score | Decision |
|-------:|----------|
| Less than **5** | Fetch using **libcurl** |
| Greater than or equal to **5** | Fetch using **Chrome DevTools Protocol (CDP)** |

This approach avoids launching a browser for every webpage, improving crawling speed while still supporting modern JavaScript-based websites.

---
# Overall Crawling Workflow

![alt text](<../Memory_management_pics/WhatsApp Image 2026-07-09 at 6.42.41 AM.jpeg>)

# Component 1 - URL Frontier

## Objective

The **URL Frontier** is responsible for managing the list of URLs that the web crawler needs to visit. Its primary objective is to organize, prioritize, and schedule URLs for crawling while ensuring that each webpage is visited efficiently and without unnecessary duplication.

The URL Frontier maintains a queue of pending URLs, adds newly discovered links from the HTML Parser, and provides the next URL to the crawler based on the crawling strategy (e.g., FIFO, priority-based, or depth-first).

By controlling the order and frequency of URL visits, the URL Frontier improves crawling efficiency, prevents repeated visits to the same webpage, and helps ensure comprehensive coverage of the target websites.

---

# Section 1 - Public API

The **Public API** defines the operations that other modules of the crawler can perform on the **URL Frontier**. It provides a simple interface for adding URLs, retrieving the next URL to crawl, inspecting the queue, and managing the frontier without exposing its internal implementation. This abstraction allows other components (such as the crawler controller and fetcher) to interact with the frontier without needing to know how the queue is implemented internally.

| Function | Parameters | Return Type | Purpose |
|----------|------------|-------------|---------|
| `addURL(const URLNode& node)` | `node : URLNode` | `void` | Adds a new URL node, containing the URL and its crawl depth, to the end of the frontier queue so it can be crawled later. |
| `getNextURL()` | None | `URLNode` | Removes and returns the URL node at the front of the queue, ensuring URLs are processed in First-In, First-Out (FIFO) order. |
| `peekNextURL()` | None | `const URLNode&` | Returns a reference to the URL node at the front of the queue without removing it, allowing the caller to inspect the next URL to be crawled. |
| `isEmpty()` | None | `bool` | Returns `true` if the frontier contains no URL nodes; otherwise returns `false`. |
| `size()` | None | `size_t` | Returns the current number of URL nodes stored in the frontier queue. |
| `clear()` | None | `void` | Removes all URL nodes from the frontier queue, leaving it empty and ready for reuse. |

### Class Definition

```cpp
class URLFrontier
{
public:
    void addURL(const URLNode& node);
    URLNode getNextURL();
    const URLNode& peekNextURL() const;
    bool isEmpty() const;
    size_t size() const;
    void clear();
};
```

---

# Section 2 - Internal Representation

![alt text](<../Memory_management_pics/WhatsApp Image 2026-07-09 at 7.04.12 AM (1).jpeg>)
![alt text](<../Memory_management_pics/WhatsApp Image 2026-07-09 at 7.25.36 AM.jpeg>)

A URL Frontier can be implemented as a **Doubly Linked List** or **Priority Queue**.

Each node stores:

- URL
- Depth
- Retry count
- Pointer to previous node
- Pointer to next node



# Section 3 - Failure Handling in URL Frontier

The URL Frontier is responsible for managing URLs waiting to be crawled. Although it does not perform network requests itself, it supports the crawler by handling URLs whose fetching process fails.

## Failure Scenarios

A URL may fail to be crawled due to several reasons, including:

- Network timeout
- DNS resolution failure
- Connection refused
- HTTP server errors (5xx)
- Temporary internet connectivity issues
- Browser rendering failure (CDP)

## Failure Handling Strategy

When the Fetcher reports a failure, the crawler follows these steps:

1. Remove the URL from the frontier for processing.
2. Attempt to fetch the webpage.
3. If the fetch succeeds:
   - Store the webpage.
   - Extract links.
   - Continue crawling.
4. If the fetch fails:
   - Increase the URL's retry count.
   - If the retry count is less than the maximum allowed retries, place the URL back into the frontier.
   - If the retry limit is reached, permanently discard the URL and record the failure in a log.

## Retry Policy

Each URL node stores a retry counter.

Example:

```
URL: https://example.com
Depth: 2
Retry Count: 1
```

If `Retry Count < MAX_RETRIES`

```
Reinsert into URL Frontier
```

Otherwise

```
Move to Failed URL Log
```

# Section 4 - Complexity Analysis

The **URL Frontier** is implemented using a queue based on a **Doubly Linked List**. Since the queue maintains pointers to both the front and rear nodes, most operations execute in constant time regardless of the number of URLs stored. The only exception is `clear()`, which must remove every node individually.

| Operation | Best | Average | Worst | Reason |
|-----------|------|----------|-------|--------|
| `addURL()` | **O(1)** | **O(1)** | **O(1)** | The new `URLNode` is always inserted at the rear of the queue. Since the queue maintains a direct pointer to the last node, no traversal is required. Whether the frontier contains one URL or thousands of URLs, the insertion process always performs the same fixed number of operations, so the running time remains constant. |
| `getNextURL()` | **O(1)** | **O(1)** | **O(1)** | The next URL is always removed from the front of the queue. Because the queue maintains a pointer to the front node, the operation simply updates the front pointer and removes the first node. No searching or traversal of the remaining URLs is needed, so the execution time is constant in all cases. |
| `peekNextURL()` | **O(1)** | **O(1)** | **O(1)** | This function only returns a reference to the URL node at the front of the queue without removing it. The front node is accessed directly through the front pointer, making the operation independent of the number of URLs stored in the frontier. |
| `isEmpty()` | **O(1)** | **O(1)** | **O(1)** | The function simply checks whether the queue contains any nodes, usually by testing whether the front pointer is `nullptr` or whether the stored size is zero. Since it performs only a single comparison, the execution time is always constant. |
| `size()` | **O(1)** | **O(1)** | **O(1)** | The queue maintains the current number of stored URL nodes in a dedicated member variable that is updated whenever a URL is added or removed. Therefore, determining the size only requires returning this stored value instead of counting every node in the queue. |
| `clear()` | **O(n)** | **O(n)** | **O(n)** | To completely empty the frontier, every URL node must be removed and its memory released. Since each of the **n** nodes is processed exactly once, the running time grows linearly with the number of stored URLs. Regardless of the queue's contents, all nodes must be visited before the frontier becomes empty. |

---

## Summary

- **Constant Time (O(1))**
  - `addURL()`
  - `getNextURL()`
  - `peekNextURL()`
  - `isEmpty()`
  - `size()`

  These operations access either the **front pointer**, **rear pointer**, or the maintained **size variable** directly. They never traverse the queue, so their execution time does not depend on the number of URLs stored.

- **Linear Time (O(n))**
  - `clear()`

  This operation must remove every URL node individually. As the number of stored URLs increases, the time required increases proportionally, resulting in **O(n)** complexity.

# Section 5 - Future Compatibility with Project 03 (Indexer)

The URL Frontier and Page Storage are designed so they can later be integrated with the **Indexer**.

| Function | Future Compatibility |
|----------|----------------------|
| `storePage(url, html, depth)` | Stores each crawled page and assigns it a unique ID. The Indexer later processes these pages. |
| `getPage(url)` | Retrieves the HTML corresponding to a URL. |
| `hasPage(url)` | Prevents duplicate page storage and verifies page existence. |
| `getURLByID(id)` | Enables sequential traversal of stored pages using page IDs. |
| `pageCount()` | Returns the total number of stored pages for indexing. |

---

## Example Usage by Project 03 (Indexer)

```cpp
for (int id = 0; id < pageCount(); id++)
{
    string url = getURLByID(id);
    string html = getPage(url);

    // Extract words and build the inverted index
}
```

# Component 2 - Seen Storage

## Objective

The **Seen Storage** is responsible for ensuring that each URL is discovered only once during the crawling process. Its primary objective is to prevent duplicate URL discovery, avoid infinite crawling loops, and improve the overall efficiency of the web crawler.

Whenever the HTML Parser discovers a new URL, the crawler first checks the Seen Storage. If the URL has not been seen before, it is added to both the Seen Storage and the URL Frontier. Otherwise, the URL is ignored.

---

# Section 1 – Public API

The **Seen Storage** module provides a minimal set of operations required to maintain the collection of previously discovered URLs. Its primary responsibility is to prevent duplicate URLs from being scheduled for crawling, thereby ensuring that each unique web page is processed only once.

| Method | Parameters | Return Type | Purpose |
|---------|------------|-------------|---------|
| **`addSeenURL(url)`** | `url : String` | `bool` | Inserts a normalized URL into the Seen Storage if it does not already exist. Returns `true` when the URL is successfully inserted and `false` if the URL has already been recorded, preventing duplicate entries from being added to the crawler pipeline. |
| **`isSeen(url)`** | `url : String` | `bool` | Determines whether the specified URL is already present in the Seen Storage. This method is invoked before scheduling a URL for crawling to ensure that previously discovered pages are not processed multiple times. |
| **`removeSeenURL(url)`** *(Optional)* | `url : String` | `bool` | Removes the specified URL from the Seen Storage if it exists. Although not required during normal crawling, this operation supports testing, storage maintenance, debugging, and future features such as URL expiration or recrawling policies. |
| **`seenCount()`** | None | `int` | Returns the total number of unique URLs currently maintained by the Seen Storage. This information can be used to monitor crawler progress, generate runtime statistics, and validate the correctness of storage operations. |
| **`clearSeenStorage()`** | None | `void` | Removes every stored URL from the Seen Storage and releases all associated resources, restoring the storage to its initial empty state. This operation is typically performed when resetting the crawler or performing application cleanup before termination. |

### Class Definition

```cpp
class SeenStorage
{
public:
    bool addSeenURL(string url);
    bool isSeen(string url);
    bool removeSeenURL(string url);
    int seenCount();
    void clearSeenStorage();
};
```

---

# Section 2 - Internal Implementation

![alt text](../Memory_management_pics/seen_url_storage.jpeg)

### Internal Data Structure

- Hash Set (`unordered_set<string>`)
- Stores only unique URLs.
- Duplicate URLs are automatically rejected.
- Supports constant-time lookup on average.

Workflow:

```text
New URL
    │
    ▼
isSeen(URL)?
    │
 ┌──┴──────┐
 │         │
Yes        No
 │          │
Ignore   addSeenURL()
            │
            ▼
      Add to Frontier
```

---

# Section 3 - Failure Handling

Seen Storage must handle several exceptional situations while recording discovered URLs.

| Failure | Handling |
|---------|----------|
| **Invalid URLs** | Validate the URL before storing it. Reject malformed URLs or unsupported schemes such as `ftp://`, `mailto:`, or `javascript:`. |
| **Duplicate URLs** | Check `isSeen(url)` before insertion. If the URL already exists, ignore it and return `false`. |
| **Hash Collision** | Resolve collisions using the hash table implementation (separate chaining or open addressing). Unique URLs remain accessible. |
| **Empty URL** | Reject empty strings or URLs containing only whitespace. |
| **Memory Limit** | If storage reaches its limit, report the failure gracefully, log the error, and continue processing existing URLs. |

---

## Examples

### Invalid URL

```text
https://example.com/page1     ✓ Added

javascript:void(0)            ✗ Ignored

htp://example.com             ✗ Invalid
```

---

### Duplicate URL

```text
Seen Storage

https://example.com

New URL

https://example.com

Result

Duplicate → Not Stored
```

---

### Empty URL

```text
URL

""

Result

Rejected
```

---

# Section 4 – Complexity Analysis

| Method | Best Case | Average Case | Worst Case | Purpose |
|--------|-----------|--------------|------------|---------|
| **`addSeenURL()`** | **O(1)** | **O(1)** | **O(n)** | Inserts a normalized URL into the hash map only if it has not been encountered before. The operation computes the hash of the URL, locates the corresponding bucket, and stores the entry while preventing duplicates. Maintaining constant-time insertion enables the crawler to efficiently process millions of URLs without repeatedly visiting the same page. |
| **`isSeen()`** | **O(1)** | **O(1)** | **O(n)** | Determines whether a URL already exists in the hash map before it is scheduled for crawling. Efficient lookup prevents duplicate URLs from entering the URL Frontier, reducing unnecessary network requests and improving the overall efficiency of the crawler. |
| **`removeSeenURL()`** | **O(1)** | **O(1)** | **O(n)** | Removes a URL from the hash map by locating its corresponding bucket and deleting the stored entry. Although rarely used during normal crawling, this operation supports storage maintenance, testing, debugging, and future enhancements such as URL expiration or recrawling policies. |
| **`seenCount()`** | **O(1)** | **O(1)** | **O(1)** | Returns the total number of unique URLs currently stored by accessing an internally maintained counter. Since no traversal of the hash map is required, the execution time remains constant regardless of the number of stored URLs. This information is useful for monitoring crawler progress and collecting runtime statistics. |
| **`clearSeenStorage()`** | **O(n)** | **O(n)** | **O(n)** | Clears the entire hash map by removing every stored URL and releasing all associated memory. Each entry must be visited exactly once during cleanup, making the running time proportional to the total number of stored URLs. This operation is typically performed when resetting the crawler or releasing resources before program termination. |

### Complexity Justification

- **Best Case – O(1):** The hash function maps the URL directly to its bucket with no collisions, allowing insertion, lookup, or deletion to complete in constant time.

- **Average Case – O(1):** Assuming a well-designed hash function and a balanced distribution of keys across buckets, hash map operations require only a constant number of comparisons on average, even as the number of stored URLs grows.

- **Worst Case – O(n):** If many URLs hash to the same bucket due to excessive collisions or poor hash distribution, the bucket may need to be traversed linearly, causing insertion, lookup, or deletion to degrade to **O(n)**.

- **`seenCount()`** remains **O(1)** because the total number of stored URLs is maintained as an internal member variable rather than being calculated by traversing the hash map.

- **`clearSeenStorage()`** is **O(n)** in all cases because every stored entry must be removed and its allocated memory released exactly once.
---

# Section 5 - Future Compatibility

The Seen Storage is used only during the **crawling phase** to prevent duplicate URL discovery.

It is **not accessed by Project 03 (Indexer)** because the Indexer processes pages stored in Page Storage rather than URLs waiting to be crawled.

Its purpose is to ensure that each URL is discovered and processed only once, improving crawler efficiency and preventing duplicate page downloads.

| Method | Future Compatibility |
|---------|----------------------|
| `addSeenURL(url)` | Prevents duplicate URL discovery during crawling. Not used by the Indexer. |
| `isSeen(url)` | Ensures each URL is crawled only once. Not used by the Indexer. |
| `removeSeenURL(url)` *(Optional)* | Used only for crawler maintenance. Not required by the Indexer. |
| `seenCount()` | Provides crawler statistics only. |
| `clearSeenStorage()` | Resets the crawler before starting a new crawl session. |

---

## Relationship with the Web Crawler

```text
HTML Parser
      │
Extract URLs
      │
      ▼
 Seen Storage
      │
isSeen(URL)?
      │
 ┌────┴────┐
 │         │
Yes        No
 │          │
Ignore   addSeenURL()
            │
            ▼
      URL Frontier
```

The Seen Storage acts as a **filter** between the HTML Parser and the URL Frontier, ensuring that each unique URL enters the frontier only once.

# Component 3 - Page Storage

## Objective

The **Page Storage** module is responsible for efficiently storing all web pages collected by the crawler in a structured and organized manner.

It maintains the HTML content together with important metadata such as the page URL, crawl timestamp, HTTP status code, content type, and crawl status. The module eliminates duplicate pages to ensure efficient use of storage space while enabling fast retrieval of stored pages for indexing, searching, and further analysis.

The Page Storage module is also designed to support future scalability for handling large collections of crawled web pages.

---

# Section 1 - Public API

| Method | Return Type | Parameters | Purpose |
|---------|-------------|------------|---------|
| `storePage(Page page)` | `bool` | `Page page` | Stores a newly crawled web page and returns `true` if the operation succeeds. |
| `getPage(string url)` | `Page` | `string url` | Retrieves the stored page corresponding to the specified URL. |
| `updatePage(Page page)` | `bool` | `Page page` | Updates the HTML content or metadata of an existing page. |
| `deletePage(string url)` | `bool` | `string url` | Removes the page associated with the specified URL. |
| `pageExists(string url)` | `bool` | `string url` | Checks whether a page with the specified URL already exists. |
| `getAllPages()` | `vector<Page>` | None | Returns all stored pages for indexing or further processing. |
| `getPageMetadata(string url)` | `PageMetadata` | `string url` | Returns metadata associated with the specified page. |
| `clearStorage()` | `void` | None | Removes all stored pages and resets the storage. |

---

## Class Definition

```cpp
class PageStorage
{
public:
    bool storePage(Page page);

    Page getPage(string url);

    bool updatePage(Page page);

    bool deletePage(string url);

    bool pageExists(string url);

    vector<Page> getAllPages();

    PageMetadata getPageMetadata(string url);

    void clearStorage();
};
```

---

# Section 2 - Internal Implementation

![alt text](../Memory_management_pics/page_storage.jpeg)
---

## Workflow

1. The **Web Crawler** downloads a webpage.
2. The downloaded page is sent to the **Page Storage API**.
3. The **Duplicate Checker** verifies whether the page has already been stored.
4. The **Metadata Manager** extracts metadata including:
   - URL
   - Crawl timestamp
   - HTTP status code
   - Content type
   - Crawl depth
5. The **Storage Manager** stores both the HTML content and metadata.
6. The **Database/File System** persistently stores the page for later retrieval, indexing, and analysis.

---

## Internal Representation

Each stored page consists of two parts:

### HTML Content

```text
URL

↓

HTML Document
```

### Metadata

```text
URL

Crawl Timestamp

Status Code

Content Type

Depth

Content Length
```

Example:

```text
Page

URL:
https://example.com

HTML:
<html>...</html>

Metadata

Timestamp:
2026-07-04 10:20:30

Status:
200 OK

Content-Type:
text/html

Depth:
2
```

---

# Section 3 - Failure Handling

The Page Storage module must handle several failure scenarios to ensure reliable operation.

| Exception | Handling Strategy |
|-----------|-------------------|
| **Invalid URLs** | Validate the URL before storing. Reject malformed or unsupported URLs, log the error, and return failure. |
| **Duplicate URLs** | Check whether the page already exists. Skip storing or update the existing page according to the crawler's update policy. |
| **Download Failures** | Do not store pages that failed to download due to network errors, timeouts, or HTTP failures. Log the error and optionally schedule a retry. |
| **Malformed HTML** | Store the HTML if possible and mark the page as **Malformed** in the metadata. |
| **Empty Pages** | Store only minimal metadata or discard the page according to crawler policy. Mark the page with an **Empty Page** status. |

---

## Examples

### Invalid URL

```text
URL

htp://example.com

Result

Rejected
```

---

### Duplicate Page

```text
Stored Pages

https://example.com

New Page

https://example.com

Result

Already Exists

Skipped
```

---

### Download Failure

```text
Download

https://example.com

Timeout

Result

Not Stored

Logged
```

---

### Malformed HTML

```html
<html>

<body>

<a href="page.html">

<div>

<p>Missing closing tags
```

Result

```text
Stored

Metadata

Malformed = true
```

---

### Empty Page

```text
Downloaded HTML

(empty)
```

Result

```text
Status

Empty Page

Minimal Metadata Stored
```

---

# Section 4 - Complexity Analysis

| Method | Purpose | Best | Average | Worst |
|---------|---------|------|----------|-------|
| `storePage()` | Stores a newly crawled page. | O(1) | O(1) | O(n) |
| `getPage()` | Retrieves a page using its URL. | O(1) | O(1) | O(n) |
| `updatePage()` | Updates page content or metadata. | O(1) | O(1) | O(n) |
| `deletePage()` | Removes a stored page. | O(1) | O(1) | O(n) |
| `pageExists()` | Checks whether a page exists. | O(1) | O(1) | O(n) |
| `getAllPages()` | Returns all stored pages. | O(n) | O(n) | O(n) |
| `getPageMetadata()` | Retrieves metadata for a page. | O(1) | O(1) | O(n) |
| `clearStorage()` | Removes all stored pages. | O(n) | O(n) | O(n) |

---

# Section 5 - Future Compatibility

The **Indexer (Project 03)** will use the Page Storage module as its primary data source.

The crawler stores every downloaded webpage, and the indexer later retrieves those stored pages to:

- Extract words
- Build the inverted index
- Generate document IDs
- Support keyword searching

---

## Future API

| Method | Return Type | Parameters | Purpose | Used by Indexer |
|---------|-------------|------------|---------|-----------------|
| `storePage(string url, string html, int depth)` | `void` | `string url, string html, int depth` | Stores the crawled page. | No |
| `getPage(string url)` | `string` | `string url` | Returns the HTML content for indexing. | Yes |
| `hasPage(string url)` | `bool` | `string url` | Checks whether a page already exists. | Yes |
| `getURLByID(int id)` | `string` | `int id` | Returns the URL corresponding to a page ID. | Yes |
| `pageCount()` | `int` | None | Returns the total number of stored pages. | Yes |

---

## Example Usage by Project 03 (Indexer)

```cpp
for (int id = 0; id < pageCount(); id++)
{
    string url = getURLByID(id);

    string html = getPage(url);

    // Extract words

    // Build inverted index

    // Store term frequencies
}
```

---

# Relationship with the Web Crawler

```text
Fetcher
    │
    ▼
Downloaded HTML
    │
    ▼
Page Storage
    │
    ├───────────────┐
    │               │
    ▼               ▼
Metadata        HTML Content
    │               │
    └──────┬────────┘
           ▼
     Database / MySQL
           │
           ▼
     Indexer (Project 03)
```

The Page Storage module serves as the persistent storage layer of the crawler. Every successfully downloaded webpage is stored once, enriched with metadata, and made available for future indexing, searching, and analysis.

# Component 4 - HTML parser

## objective 

The objective of the HTML Parser module is to analyze the downloaded HTML content of a webpage, convert it into a structured format, and extract useful information such as page titles, headings, paragraphs, hyperlinks, images, and metadata. This structured data enables the web crawler to discover new URLs for further crawling and store relevant webpage content for indexing, analysis, or search.

# Section 1 Public API


| Method | Return Type | Parameters | Purpose |
|---------|------------|------------|---------|
| `bool parseHTML(const std::string& html)` | `bool` | `html` - Raw HTML document | Parses the HTML document and initializes the parser. Returns `true` if parsing is successful; otherwise returns `false`. |
| `std::string extractTitle() const` | `std::string` | None | Returns the content of the `<title>` tag. |
| `std::vector<std::string> extractHeadings() const` | `std::vector<std::string>` | None | Extracts all heading elements (`<h1>` to `<h6>`). |
| `std::vector<std::string> extractParagraphs() const` | `std::vector<std::string>` | None | Extracts text from all paragraph (`<p>`) elements. |
| `std::vector<std::string> extractLinks() const` | `std::vector<std::string>` | None | Extracts all hyperlinks from `<a href="">` elements. |
| `std::vector<std::string> extractImages() const` | `std::vector<std::string>` | None | Extracts image URLs from `<img src="">` elements. |
| `std::map<std::string, std::string> extractMetaData() const` | `std::map<std::string, std::string>` | None | Extracts metadata such as description, keywords, author, and other meta tags. |
| `std::string getPlainText() const` | `std::string` | None | Returns the visible text content of the webpage after removing HTML tags. |
| `bool isParsed() const` | `bool` | None | Returns `true` if the HTML document has been successfully parsed; otherwise returns `false`. |
| `void clear()` | `void` | None | Clears all stored parser data and resets the parser state. |

# Section 2 Internal Implementation

The HTML Parser uses a lightweight sequential scanning approach to extract hyperlinks from an HTML document. Instead of constructing a Document Object Model (DOM), the parser directly scans the HTML string to locate anchor (`<a>`) tags and their corresponding `href` attributes. This approach reduces memory usage and improves performance, making it suitable for web crawling applications where only hyperlinks are required.

### Processing Steps

1. Receive the raw HTML document as a string.
2. Initialize the scanning position at the beginning of the document.
3. Search for the next occurrence of the `<a` tag.
4. Locate the `href` attribute within the anchor tag.
5. Verify that the `href` attribute contains a valid quoted URL.
6. Extract the URL between the quotation marks.
7. Store the extracted URL in a `std::vector<std::string>`.
8. Continue scanning from the end of the current tag until the end of the HTML document is reached.
9. Return the collection of extracted hyperlinks.

---

## Internal Data Structures

| Data Structure | Purpose |
|----------------|---------|
| `std::string` | Stores the raw HTML document. |
| `std::vector<std::string>` | Stores the extracted hyperlinks. |
| `size_t` | Maintains the current scanning position within the HTML document. |

---

## Internal Workflow

```text
Raw HTML
    │
    ▼
Sequential Scanner
    │
    ▼
Find "<a"
    │
    ▼
Find "href="
    │
    ▼
Extract URL
    │
    ▼
Validate URL
    │
    ▼
Store in Vector
    │
    ▼
Repeat Until End of Document
    │
    ▼
Return Extracted Links
```

---

## Design Characteristics

- Single-pass sequential scan of the HTML document.
- No DOM tree construction.
- Low memory consumption.
- Efficient hyperlink extraction.
- Gracefully skips malformed or invalid anchor tags.
- Suitable for large-scale web crawling.
# Section 3 Failure Handling

The Link Extractor is designed to handle malformed or incomplete HTML gracefully without interrupting the crawling process.

| Failure Scenario | Handling Strategy |
|------------------|-------------------|
| Empty HTML document | Return an empty list of links. |
| `<a>` tag not found | Continue scanning until the end of the document and return an empty list. |
| Missing `href` attribute | Skip the tag and continue scanning. |
| Empty `href` value | Ignore the link and continue scanning. |
| Unclosed `<a>` tag | Skip the malformed tag and continue scanning from the next position. |
| Invalid URL format | Ignore the URL or log it for debugging. |
| Duplicate links | Store links in a hash set before adding them to the output list to avoid duplicates. |
| Unexpected exceptions | Catch the exception, log the error, and return the links extracted so far. |

# Section 4 Complexity analysis

## Complexity Analysis

| Method | Purpose | Best Case | Average Case | Worst Case |
|--------|---------|-----------|--------------|------------|
| `parseHTML(const std::string& html)` | Scans the HTML document and extracts all hyperlinks. | **O(1)** (empty document or no content) | **O(n)** | **O(n)** |
| `extractLinks()` | Returns the collection of extracted links. | **O(1)** | **O(k)** | **O(k)** |
| `clear()` | Clears all extracted links and resets the parser. | **O(1)** (already empty) | **O(k)** | **O(k)** |
| `isParsed()` | Checks whether the HTML document has been parsed successfully. | **O(1)** | **O(1)** | **O(1)** |

### Notation

- **n** = Size of the HTML document (number of characters).
- **k** = Number of extracted hyperlinks.

# Section 5 Future Compatibility 

The current HTML Parser is designed to extract hyperlinks for web crawling. Its modular design allows it to be extended to support an Indexer without significant architectural changes.

### Future Enhancements

- Extract visible text from web pages for content indexing.
- Extract page titles to improve search result relevance.
- Extract metadata (description, keywords, author) for document indexing.
- Extract heading elements (`<h1>`–`<h6>`) to assign importance to page content.
- Compute term frequencies for ranking algorithms.
- Remove HTML tags and normalize text before indexing.
- Provide structured document data directly to the Indexer module.

### Integration Workflow

```text
Web Fetcher
      │
      ▼
HTML Parser
      │
      ├── Hyperlinks ─────────► URL Queue (Crawler)
      │
      └── Page Content ───────► Indexer
                                    │
                                    ▼
                             Inverted Index
                                    │
                                    ▼
                              Search Engine
```

### Benefits

- Reuses the existing parser without modifying the crawler.
- Supports efficient document indexing.
- Enables keyword-based searching.
- Provides a foundation for page ranking and search result generation.
- Simplifies future integration with search engine components.

# Component 5 - Fetcher

## Objective

The Fetcher is responsible for downloading web pages from the internet by sending HTTP/HTTPS requests to the specified URLs. It retrieves HTML or rendered page content, handles redirects and network errors, and provides the downloaded content to the HTML Parser for further processing and link extraction.

# Section 1 - Public API

| Return Type | Method | Parameters | Purpose |
|------------|--------|------------|---------|
| `FetchResult` | `fetch()` | `const std::string& url` | Downloads the HTML content of the specified URL and returns the response along with status information. |
| `void` | `setUserAgent()` | `const std::string& userAgent` | Sets the User-Agent header for HTTP requests. |
| `void` | `setTimeout()` | `int timeoutSeconds` | Sets the maximum time to wait for a server response before timing out. |
| `void` | `setMaxRedirects()` | `int maxRedirects` | Specifies the maximum number of HTTP redirects to follow automatically. |

## FetchResult Structure

```cpp
struct FetchResult {
    bool success;
    int statusCode;
    std::string finalUrl;
    std::string html;
    std::string errorMessage;
};
```
# Section 2 - Internal implementation 

The Fetcher is internally implemented using an HTTP client library (e.g., libcurl) to retrieve web pages over HTTP/HTTPS. It is responsible only for downloading page content and handling network communication.

## Working Steps

1. Accept a URL from the URL Frontier.
2. Validate the URL format.
3. Initialize the HTTP client (libcurl).
4. Configure request parameters:
   - User-Agent
   - Timeout
   - Maximum redirects
5. Send an HTTP/HTTPS GET request.
6. Receive the server's response.
7. If the server returns redirects (301/302), follow them up to the configured limit.
8. Store the downloaded HTML content in memory.
9. Record metadata such as:
   - Success status
   - HTTP status code
   - Final URL after redirects
   - Error message (if any)
10. Return the result to the HTML Parser.
11. Release all allocated network resources.

## Internal Workflow

```text
URL Frontier
      │
      ▼
 Receive URL
      │
      ▼
 Validate URL
      │
      ▼
 Configure libcurl
(User-Agent, Timeout, Redirects)
      │
      ▼
 Send HTTP GET Request
      │
      ▼
 Receive Response
      │
      ▼
 Store HTML + Status Information
      │
      ▼
 Return FetchResult
      │
      ▼
 HTML Parser
```
# Section 3 - Failure Handling 

The Fetcher includes failure handling to ensure that network and server errors do not interrupt the crawling process. Instead of terminating the crawler, it captures the error, records the failure information, and returns an appropriate `FetchResult` to the caller.

## Failure Handling Process

1. **Validate the URL**
   - Check whether the URL is properly formatted.
   - If invalid, stop the request and return an error.

2. **Send HTTP Request**
   - Attempt to establish a connection with the target server.

3. **Detect Network Errors**
   - Handle failures such as:
     - DNS resolution failure
     - Connection refused
     - Network unavailable
     - Connection reset

4. **Handle Timeout**
   - If the server does not respond within the configured timeout period, abort the request and report a timeout error.

5. **Handle SSL/TLS Errors**
   - Detect invalid or expired SSL certificates and other HTTPS-related issues.

6. **Process HTTP Status Codes**
   - **200 OK:** Download completed successfully.
   - **301/302:** Follow redirects until the configured redirect limit is reached.
   - **403 Forbidden:** Report access denied.
   - **404 Not Found:** Report that the requested page does not exist.
   - **500 Internal Server Error:** Report a server-side failure.

7. **Handle Redirect Limit**
   - If the number of redirects exceeds the configured maximum, stop following redirects and return an error.

8. **Return Failure Information**
   - Populate the `FetchResult` object with:
     - `success = false`
     - HTTP status code (if available)
     - Error message describing the failure
     - Final URL reached (if applicable)

9. **Continue Crawling**
   - The crawler logs the failure, skips the failed URL, and proceeds to fetch the next URL from the URL Frontier.

## Example

```text
URL
 │
 ▼
Send HTTP Request
 │
 ▼
Was the request successful?
 ├── Yes ─► Return HTML Content
 │
 └── No
      │
      ├── Invalid URL
      ├── DNS Failure
      ├── Timeout
      ├── SSL Error
      ├── HTTP 404
      ├── HTTP 403
      ├── HTTP 500
      └── Redirect Limit Exceeded
               │
               ▼
      Create FetchResult (success = false)
               │
               ▼
      Log Error and Continue Crawling
```
# Section 4 - Complexity Analysis

| Method | Best Case | Average Case | Worst Case | Reason |
|--------|-----------|--------------|------------|--------|
| `FetchResult fetch(const std::string& url)` | **O(1)** | **O(n)** | **O(n)** | Best case occurs when the request fails immediately (e.g., invalid URL or connection refused). Average and worst cases depend on downloading the HTML response, where **n** is the size of the downloaded content. Network latency is not included in Big-O analysis. |
| `void setUserAgent(const std::string& userAgent)` | **O(1)** | **O(m)** | **O(m)** | Stores or copies the User-Agent string. If the string is copied, the complexity depends on its length **m**. |
| `void setTimeout(int timeoutSeconds)` | **O(1)** | **O(1)** | **O(1)** | Simply assigns the timeout value to an internal configuration variable. |
| `void setMaxRedirects(int maxRedirects)` | **O(1)** | **O(1)** | **O(1)** | Simply stores the maximum redirect limit in an internal configuration variable. |

### Notes
- **n** = Size of the downloaded HTML response (number of characters/bytes).
- **m** = Length of the User-Agent string.
- Network latency, DNS lookup, TCP handshake, SSL negotiation, and server processing time are external factors and are **not represented in Big-O complexity**.
- The complexity shown is only for the algorithmic work performed by the Fetcher component.

# Section 5 - Future Compatibility 

The **Fetcher** component is designed to integrate seamlessly with the **Indexer** in future development phases. Instead of only downloading HTML, it provides the raw page content required for indexing and search.

### Integration Points

- **Provides HTML Source**
  - The Fetcher downloads the complete HTML document, which serves as the primary input for the Indexer.

- **Supports HTML Parser**
  - The fetched HTML is passed to the HTML Parser, which extracts the page title, headings, body text, metadata, and hyperlinks before indexing.

- **Maintains URL Association**
  - Each fetched response remains associated with its source URL, allowing the Indexer to map indexed content back to the correct webpage.

- **Status-Aware Processing**
  - The Fetcher returns HTTP status information, enabling the Indexer to skip pages that failed to download or returned invalid responses (e.g., 404 or 500).

- **Scalable Crawling Pipeline**
  - The Fetcher operates independently of the Indexer, allowing multiple pages to be fetched first and indexed later, making the architecture suitable for large-scale web crawling.

- **Supports Incremental Indexing**
  - Newly fetched pages can be sent directly to the Indexer without rebuilding the entire index, improving efficiency during recrawling.

- **Extensible Metadata Support**
  - Future versions can expose additional metadata such as HTTP headers, content type, encoding, response time, and last-modified timestamps, enabling richer indexing and ranking strategies.

### Data Flow

```text
URL Frontier
      │
      ▼
   Fetcher
      │
      ▼
 HTML Content
      │
      ▼
 HTML Parser
      │
      ▼
 Extracted Text + Metadata
      │
      ▼
   Indexer
      │
      ▼
 Search Index
```
# Component 6 - URL validator

## Objective

The **URL Validator** is responsible for verifying that a URL is syntactically valid before it enters the crawler pipeline. Its primary purpose is to prevent malformed, incomplete, or unsupported URLs from being processed by downstream components such as the **URL Normalizer**, **Seen URL Storage**, **URL Frontier**, and **Fetcher**.

By filtering invalid URLs at the earliest stage, the validator improves the reliability of the crawler, reduces unnecessary network requests, and prevents resources from being wasted on URLs that cannot be successfully fetched.

In the current implementation, the URL Validator performs the following checks:

- Ensures that the URL is not empty.
- Verifies that the URL begins with a supported protocol (`http://` or `https://`).
- Confirms that a domain name exists after the protocol.
- Rejects URLs containing whitespace within the domain.
- Ensures that the domain contains at least one period (`.`), indicating a valid domain structure.
- Rejects domains that end with a trailing period (`.`).

Only URLs that satisfy all of these validation rules are accepted and passed to the next stage of the crawling pipeline. Any URL that fails one or more validation checks is immediately rejected, preventing invalid input from propagating through the system.

# Section 1 - Public API

The **Public API** of the **URL Validator** provides a simple interface for validating URLs before they enter the crawler pipeline. Instead of exposing the internal validation logic, the API allows other components (such as the URL Frontier or Crawler Controller) to determine whether a URL is valid by calling a single public function. The helper functions are kept private because they are only used internally to perform individual validation checks.

| Function | Parameters | Return Type | Purpose |
|----------|------------|-------------|---------|
| `isValid(const std::string& url)` | `url : const std::string&` | `bool` | Acts as the main entry point of the URL Validator. It performs all validation checks in sequence, including verifying that the URL is not empty, checking for a supported protocol, and ensuring that a valid domain is present. If every validation rule succeeds, the function returns `true`; otherwise, it immediately returns `false`, preventing invalid URLs from entering the crawler pipeline. |

---

# Internal Helper Functions

Although these functions are **private** and cannot be called directly by other components, they are responsible for performing specific validation tasks used by `isValid()`.

| Function | Parameters | Return Type | Purpose |
|----------|------------|-------------|---------|
| `hasValidProtocol(const std::string& url)` | `url : const std::string&` | `bool` | Checks whether the URL begins with one of the supported protocols (`http://` or `https://`). This ensures that the crawler only accepts URLs that can be fetched using supported network protocols. If the protocol is missing or unsupported, the URL is rejected immediately. |
| `hasDomain(const std::string& url)` | `url : const std::string&` | `bool` | Verifies that the URL contains a valid domain name after the protocol. It ensures that the domain is not empty, contains no whitespace, includes at least one period (`.`), and does not end with a trailing period. These checks help reject malformed or incomplete URLs before they are processed further by the crawler. |

# Section 2 - Internal implementation

![alt text](../Memory_management_pics/URLvalidator_implementation.jpeg)

 Instead of performing all validation checks at once, the validator follows a step-by-step process where each validation rule is executed in sequence. If any validation step fails, the URL is immediately marked as invalid and no further checks are performed. This approach avoids unnecessary processing and improves the efficiency of the validation process.

The validation process begins by checking whether the input URL string is empty. If the string is empty, the URL is immediately rejected. Otherwise, the validator verifies that the URL starts with a supported protocol (`http://` or `https://`). Once the protocol is confirmed, the validator extracts the domain portion of the URL and performs several domain-level checks.

The extracted domain is then examined to ensure that it does not contain whitespace characters, includes at least one period (`.`), and does not end with a trailing period. These checks help identify malformed or incomplete URLs before they are passed to the remaining crawler components.

Only when every validation step succeeds does the validator classify the URL as a **Valid URL**. Otherwise, the URL is rejected as **Invalid**, preventing malformed URLs from entering the crawler pipeline.

# Section 3 - Failure handling 

The URL Validator is responsible for identifying invalid URLs before they enter the crawler pipeline. Rather than attempting to correct malformed URLs, it follows a **fail-fast** approach, where validation stops as soon as an error is detected.

## Failure Scenarios

The validator rejects a URL if any of the following conditions are encountered:

- The input URL is empty.
- The URL does not begin with a supported protocol (`http://` or `https://`).
- No domain name exists after the protocol.
- The domain contains one or more whitespace characters.
- The domain does not contain a period (`.`).
- The domain ends with a trailing period (`.`).

## Failure Handling Strategy

The validator performs validation checks sequentially. If any check fails:

1. The corresponding validation function returns `false`.
2. The `isValid()` function immediately returns `false`.
3. The URL is rejected and is **not** forwarded to the URL Normalizer or URL Frontier.
4. The crawler can optionally log the invalid URL for debugging or analysis.

# Section 4 - Complexity Analysis

The URL Validator performs a fixed sequence of validation checks on the input URL. Most of these checks, such as verifying whether the string is empty or checking the protocol, require only a constant amount of work. However, validating the domain requires scanning the characters of the domain to search for spaces and a period (`.`). Therefore, the running time depends on the length of the input URL.

Let **n** represent the length of the input URL string.

| Operation | Best | Average | Worst | Reason |
|-----------|------|----------|-------|--------|
| `isValid()` | **O(n)** | **O(n)** | **O(n)** | The function performs multiple validation checks, including verifying the protocol and examining the domain. Although some checks execute in constant time, operations such as searching the domain for whitespace and a period require scanning the characters of the string. In the worst, average, and even the successful validation case, the algorithm may need to inspect most or all of the URL before reaching a decision. Since the amount of work grows proportionally with the length of the input URL, the overall time complexity is **O(n)**. |

---

## Summary

- **Time Complexity:** **O(n)**
- **Space Complexity:** **O(n)**

### Explanation

The validator performs several checks one after another without using nested loops or recursive calls. Each character of the input URL is examined at most a small constant number of times. As the length of the URL increases, the time required to validate it increases proportionally, resulting in **linear time complexity (O(n))**.

The current implementation also creates a new string when extracting the domain using `substr()`. Since this operation copies the domain into a separate string, additional memory proportional to the domain length is required. Therefore, the auxiliary space complexity is also **O(n)**.

# Section 5 - Future Compatibility with the Indexer

Although the URL Validator does not communicate directly with the Indexer, it plays an important role in ensuring that only valid webpages reach the indexing stage. By rejecting malformed or unsupported URLs before they are fetched, the validator improves the overall quality of the data available for indexing.

In future versions of the crawler, the URL Validator can be extended to perform additional validation checks that further improve indexing quality, such as:

- Rejecting unsupported URL schemes (e.g., `ftp://`, `file://`, `mailto:`).
- Blocking duplicate URLs after normalization.
- Filtering URLs based on allowed or blocked domains.
- Ignoring URLs that point to non-HTML resources such as images, videos, PDFs, or executable files.
- Enforcing URL length limits to prevent processing excessively long or malformed URLs.
- Supporting whitelist and blacklist rules for domain filtering.
- Integrating with the URL Normalizer to ensure that only canonical URLs are indexed.

These enhancements will help ensure that the Indexer receives a clean, consistent, and high-quality collection of webpages, reducing unnecessary processing and improving the efficiency and accuracy of the indexing process.

# component 7 - URL normalizer

## Objective

The **URL Normalizer** is responsible for converting valid URLs into a **standardized (canonical) format** before they are stored or processed by the crawler. Different URLs can often refer to the same webpage even though they appear different in their textual representation. The primary objective of the URL Normalizer is to eliminate these differences so that every webpage is represented by a single, consistent URL.

By normalizing URLs before they are inserted into the **Seen URL Storage** and **URL Frontier**, the crawler can accurately identify duplicate pages, avoid unnecessary crawling, and make better use of system resources. This reduces duplicate downloads, improves crawling efficiency, and ensures that the same webpage is not processed multiple times under different URL representations.

In the current design, the URL Normalizer performs the following operations:

- Converts the protocol (`HTTP`, `HTTPS`) to lowercase.
- Converts the domain name to lowercase.
- Removes default ports such as `:80` for HTTP and `:443` for HTTPS.
- Removes URL fragments (the portion after `#`) because they do not affect the actual webpage content.
- Removes unnecessary trailing slashes where appropriate.
- Produces a single canonical representation of every valid URL before it is passed to the next stage of the crawler.

After normalization, the standardized URL is forwarded to the **Seen URL Storage** for duplicate detection and then to the **URL Frontier** for scheduling. This ensures that the crawler processes each unique webpage only once, improving both performance and data consistency throughout the crawling pipeline.

# Section 1 - Public API

The **Public API** of the **URL Normalizer** provides a simple interface for converting a valid URL into its canonical (standardized) form before it is processed by the remaining crawler components. Other modules, such as the **Crawler Controller**, **Seen URL Storage**, and **URL Frontier**, only need to call a single public function to obtain the normalized URL. The individual normalization operations are implemented as private helper functions, ensuring that the internal implementation remains hidden and can be modified without affecting other components.

| Function | Parameters | Return Type | Purpose |
|----------|------------|-------------|---------|
| `normalize(const std::string& url)` | `url : const std::string&` | `std::string` | Acts as the main entry point of the URL Normalizer. It receives a validated URL and applies all normalization rules one after another, including converting the protocol and domain to lowercase, removing default ports, removing URL fragments, and eliminating unnecessary trailing slashes. Once all transformations have been completed, the function returns a single standardized version of the URL that can safely be used for duplicate detection and future crawling. |

---

# Internal Helper Functions

The following helper functions are **private** and are used internally by `normalize()` to perform individual normalization operations.

| Function | Parameters | Return Type | Purpose |
|----------|------------|-------------|---------|
| `normalizeProtocol(const std::string& url)` | `url : const std::string&` | `std::string` | Converts the protocol portion of the URL into lowercase. For example, `HTTP://` becomes `http://` and `HTTPS://` becomes `https://`. This ensures that protocol capitalization does not cause duplicate URLs to be treated as different webpages. |
| `normalizeDomain(const std::string& url)` | `url : const std::string&` | `std::string` | Converts the domain name to lowercase because domain names are case-insensitive. This guarantees that URLs such as `Example.com` and `example.com` are treated as the same webpage. |
| `removeDefaultPort(const std::string& url)` | `url : const std::string&` | `std::string` | Removes default port numbers such as `:80` for HTTP and `:443` for HTTPS. Since these ports are automatically assumed by web browsers, removing them prevents multiple representations of the same URL from being stored separately. |
| `removeFragment(const std::string& url)` | `url : const std::string&` | `std::string` | Removes the fragment identifier (the portion beginning with `#`). Fragments are used only for navigation within a webpage and do not change the actual content returned by the web server, so they are excluded from the normalized URL. |
| `normalizeTrailingSlash(const std::string& url)` | `url : const std::string&` | `std::string` | Removes unnecessary trailing slashes from the URL where appropriate. This prevents URLs such as `https://example.com/page` and `https://example.com/page/` from being considered different when they refer to the same resource. |

# Section 2 - Internal implementation of URL Normalizer

## Workflow

![alt text](../Memory_management_pics/URLnormalizer_workflow.jpeg)

The **URL Normalizer** is implemented as a sequence of independent normalization operations. Instead of performing all transformations inside a single block of code, the normalizer delegates each task to a dedicated helper function. This modular approach makes the implementation easier to understand, maintain, and extend.

When a valid URL is received, the `normalize()` function acts as the controller of the normalization process. It passes the URL through each normalization stage one after another. Every helper function performs exactly one transformation and returns the updated URL to the next stage.

The normalization process is carried out in the following order:

1. **Normalize the Protocol**
   - Converts the protocol to lowercase.
   - Example:
     ```
     HTTP://example.com
     ```
     becomes
     ```
     http://example.com
     ```

2. **Normalize the Domain**
   - Converts the domain name to lowercase because domain names are case-insensitive.
   - Example:
     ```
     http://Example.COM
     ```
     becomes
     ```
     http://example.com
     ```

3. **Remove Default Port**
   - Removes port `80` from HTTP URLs and port `443` from HTTPS URLs because these ports are automatically assumed by web browsers.
   - Example:
     ```
     https://example.com:443
     ```
     becomes
     ```
     https://example.com
     ```

4. **Remove URL Fragment**
   - Removes everything after the `#` symbol because fragments only identify a location inside the webpage and do not affect the resource returned by the server.
   - Example:
     ```
     https://example.com/page#section
     ```
     becomes
     ```
     https://example.com/page
     ```

5. **Normalize Trailing Slash**
   - Removes unnecessary trailing slashes when they do not change the webpage being referenced.
   - Example:
     ```
     https://example.com/about/
     ```
     becomes
     ```
     https://example.com/about
     ```

After all normalization steps have been completed, the resulting URL is returned as the canonical representation. This standardized URL is then forwarded to the **Seen URL Storage**, where duplicate detection is performed before the URL is inserted into the **URL Frontier**.

# Section 3 - Failure handling 

## Failure Scenarios

The normalizer may fail in the following situations:

- The input URL is empty.
- The URL has not been validated and contains an unsupported format.
- The protocol is missing or malformed.
- The domain name cannot be identified correctly.
- The URL becomes invalid after a normalization step.
- An unexpected format is encountered that cannot be processed by the current normalization rules.

## Failure Handling Strategy

The URL Normalizer applies each normalization rule one after another. After every transformation, it verifies that the URL is still valid.

The normalization process follows these steps:

1. Receive a validated URL.
2. Apply one normalization rule.
3. Verify that the transformed URL remains valid.
4. If the URL is still valid, continue to the next normalization step.
5. If any normalization step fails, stop the normalization process immediately.
6. Return an error or an invalid result to the crawler.
7. The URL is discarded and is not inserted into the Seen URL Storage or the URL Frontier.

## Failure workflow

```text
Validated URL
      │
      ▼
normalize()
      │
      ▼
Normalization Step
      │
      ├── Success ─────► Next Step
      │
      └── Failure
             │
             ▼
      Stop Normalization
             │
             ▼
      Report Failure
             │
             ▼
      Reject URL
```

# Section 4 - Complexity Analysis

Let **n** be the length of the input URL.

| Operation | Best Case | Average Case | Worst Case | Reason |
|-----------|-----------|--------------|------------|--------|
| `normalize()` | **O(1)** | **O(n)** | **O(n)** | In the best case, the URL is already in canonical form and only a few fixed checks are required before returning it. In the average case, the normalizer performs a small number of transformations such as converting the domain to lowercase or removing a fragment. In the worst case, every normalization rule is applied and the entire URL must be examined, making the running time proportional to the length of the URL. |

---

## Internal Helper Functions

| Function | Best Case | Average Case | Worst Case | Reason |
|----------|-----------|--------------|------------|--------|
| `normalizeProtocol()` | **O(1)** | **O(1)** | **O(1)** | The protocol is always located at the beginning of the URL. Only a fixed number of characters need to be checked or converted, so the execution time never depends on the URL length. |
| `normalizeDomain()` | **O(1)** | **O(n)** | **O(n)** | If the domain is already lowercase, only a few comparisons are needed. Otherwise, the function scans the complete domain and converts uppercase characters to lowercase. |
| `removeDefaultPort()` | **O(1)** | **O(n)** | **O(n)** | When the URL does not contain an explicit port, the function quickly determines that no modification is required. If a port is present, it searches and removes it, requiring traversal of part or all of the URL. |
| `removeFragment()` | **O(1)** | **O(n)** | **O(n)** | If the URL does not contain a fragment identifier (`#`), the function completes after a simple check. Otherwise, it searches for the fragment and removes everything that follows it. |
| `normalizeTrailingSlash()` | **O(1)** | **O(1)** | **O(1)** | The function only examines the last character of the URL and removes a trailing slash if necessary. Since it accesses a fixed position, its execution time is always constant. |

---

## Summary

### Time Complexity

- **Best Case:** O(1)
- **Average Case:** O(n)
- **Worst Case:** O(n)

The overall complexity is determined by the `normalize()` function because it invokes all normalization steps. Most helper functions either perform constant-time checks or scan the URL once. Since no nested traversal occurs, the normalizer remains efficient even for long URLs.

# Section 5 - Future Compatibility with the Indexer

The **URL Normalizer** plays an important role in ensuring that the Indexer works with a clean and consistent collection of webpages. Before a webpage is fetched and stored, the normalizer converts its URL into a canonical format. As a result, different textual representations of the same webpage are treated as a single resource throughout the crawler pipeline.

When the Indexer processes stored webpages in the future, it can rely on these normalized URLs as unique identifiers. This prevents the same webpage from being indexed multiple times simply because it was discovered through different URL representations.

For example, the following URLs all refer to the same webpage:

```
HTTP://Example.com
http://example.com/
http://example.com:80
http://example.com/#about
```

After normalization, they become:

```
http://example.com
```

Since only the normalized URL is stored, the Indexer creates a single index entry instead of multiple duplicate entries.

## Future Enhancements

The URL Normalizer can be extended with additional normalization rules to further improve indexing quality, such as:

- Removing common tracking parameters (for example, `utm_source`, `utm_medium`, and `utm_campaign`) that do not change the webpage content.
- Sorting query parameters into a consistent order so that URLs containing the same parameters in different sequences are treated as identical.
- Resolving relative paths (such as `.` and `..`) into their absolute form.
- Decoding percent-encoded characters where appropriate to create a consistent URL representation.
- Supporting internationalized domain names (IDNs) by converting them into a canonical format.
- Applying configurable normalization rules for specific websites that have unique URL structures.

These enhancements will allow the Indexer to process a cleaner dataset, reduce duplicate index entries, improve search accuracy, and make the indexing process more efficient as the crawler grows.

