# Build Log – Session 3

**Date:** July 11

**Duration:** 120 minutes

## Section 1 - Today's Goal:

Design and implement the **Seen Storage** component responsible for tracking already discovered URLs and preventing duplicate crawling in the crawler pipeline.

The goal was to create an efficient URL tracking mechanism using the custom data structures developed in the Collections Library.

---

## Section 2 - Work Completed

* Reviewed the crawler workflow after completing the HTML Parser component and identified the need for a duplicate URL management system.

* Defined the role of **Seen Storage** in the crawler architecture:

  * Store URLs that have already been discovered or crawled.
  * Prevent duplicate URLs from being inserted into the URL Frontier.
  * Provide fast lookup operations for URL existence checking.
  * Act as a filter between URL extraction and URL scheduling.

Updated crawler workflow:

```
Fetcher / RenderEngine
          |
          ▼
     HTML Parser
          |
          ▼
   Extracted Links
          |
          ▼
 URL Validator
          |
          ▼
 URL Normalizer
          |
          ▼
   Seen Storage
          |
          ▼
 URL Frontier
```

---

# Seen Storage Design

* Designed Seen Storage as an independent crawler component instead of directly using Set inside the crawler.

* Created a wrapper class around the custom Set implementation to maintain modularity and allow future changes in storage strategy.

* The architecture was designed as:

```
            SeenStorage

                 |
                 |
              Custom Set

                 |
                 |
        HashMap<string,bool>

                 |
                 |
        Collections Library
```

* The crawler does not directly depend on the internal Set implementation.

* Future storage changes can be handled inside SeenStorage without affecting the crawler logic.

---

# Custom Set Implementation

* Integrated the custom **Set** data structure from the Collections Library.

* The Set internally uses:

```
HashMap<T,bool>
```

to maintain unique values.

* Implemented URL storage using:

```cpp
Set<std::string>
```

because crawler URLs are stored as string values.

---

# Set Functionality Implemented

Implemented and tested the following Set operations:

### Insert URL

Adds a new URL into the set.

Example:

```cpp
urlSet.insert("https://example.com");
```

Duplicate URLs are ignored.

---

### Contains URL

Checks whether a URL already exists.

Example:

```cpp
urlSet.contains("https://example.com");
```

Used by Seen Storage to detect duplicate crawling.

---

### Remove URL

Removes a stored URL.

Example:

```cpp
urlSet.remove("https://google.com");
```

---

### Size Tracking

Returns the number of stored URLs.

Example:

```cpp
urlSet.size();
```

---

### Empty Check

Checks whether storage contains any URLs.

Example:

```cpp
urlSet.empty();
```

---

# SeenStorage Wrapper Class

Created the SeenStorage component with a clean public interface.

Responsibilities:

* Add newly discovered URLs.
* Check if a URL was already visited.
* Remove URLs if required.
* Track number of stored URLs.
* Clear stored URL history.

Designed API:

```cpp
class SeenStorage
{

public:

    bool addSeenURL(const std::string& url);

    bool isSeen(const std::string& url) const;

    bool removeSeenURL(const std::string& url);

    size_t size() const;

    void clear();

};
```

---

# Testing Performed

Created testing code for the custom Set implementation.

Test cases included:

### Adding URLs

```cpp
urlSet.insert("https://example.com");
urlSet.insert("https://google.com");
```

---

### Duplicate Handling

Inserted duplicate URL:

```cpp
urlSet.insert("https://example.com");
```

Verified that duplicate entries were not stored.

---

### Searching URLs

Checked URL existence:

```cpp
urlSet.contains("https://example.com");
```

---

### Removing URLs

Removed stored URL:

```cpp
urlSet.remove("https://google.com");
```

---

### Size Verification

Verified size changes after insertion and removal operations.

---

# Integration Planning

Prepared Seen Storage for crawler integration.

Future crawling flow:

```
HTML Parser
      |
      |
Extract Links
      |
      |
URL Validator
      |
      |
URL Normalizer
      |
      |
SeenStorage
      |
      |
Already Seen?
      |
  ----------------
  |              |
 YES             NO
  |              |
Ignore       Store URL
                 |
                 |
            URL Frontier
```

---

## Section 3 - Outcome

The Seen Storage module was successfully designed and implemented.

Completed:

✅ Custom Set integration
✅ URL duplicate tracking
✅ Wrapper class architecture
✅ URL existence checking
✅ Insert/remove operations
✅ Size and clear functionality
✅ Ready for crawler pipeline integration

The crawler now has a mechanism to prevent duplicate URL processing.

Next development phase will focus on implementing **Page Storage** for storing fetched HTML content and preparing the crawler data layer for future indexing support.
