# Build Log – Session 3

**Date:** July 7
**Duration:** 120 minutes

## Section 1 - Today's Goal

 Implement the URL Frontier component for managing URLs waiting to be crawled.

## Section2 - Problem

After implementing the Queue data structure, the next step was to develop a dedicated **URL Frontier** component. The crawler required a centralized module responsible for storing and scheduling URLs in FIFO order while also keeping track of each URL's crawling depth. The design needed to remain modular so that other crawler components could interact with the frontier without depending on the underlying queue implementation.

## Section 3 - What I Tried

* Created a new **`frontier.h`** header file to declare the `URLFrontier` class and the `URLNode` structure.
* Created the corresponding **`frontier.cpp`** source file to implement the member functions of the `URLFrontier` class.
* Added both files to the crawler project structure:

  * `include/frontier.h`
  * `src/frontier.cpp`
* Defined the `URLNode` structure to store:

  * URL string
  * Crawl depth
* Used the previously implemented **Queue** class as the internal storage mechanism for the frontier, allowing the component to follow FIFO scheduling while remaining independent of the underlying Doubly Linked List implementation.
* Implemented the following public member functions:

  * `addURL()` – Adds a new URL node to the frontier.
  * `getNextURL()` – Removes and returns the next URL node for crawling.
  * `peekNextURL()` – Returns the next URL node without removing it.
  * `isEmpty()` – Checks whether the frontier contains pending URLs.
  * `size()` – Returns the total number of URL nodes currently stored.
  * `clear()` – Removes all URL nodes from the frontier.
* Organized the implementation to keep the interface simple and maintainable so that future components such as the Fetcher and Crawler Engine can easily interact with the frontier.

## Section 4 -Outcome

The **URL Frontier** component was successfully implemented and integrated into the crawler project. It now provides a clean interface for managing pending URLs while internally relying on the reusable Queue data structure. By separating the declaration and implementation into dedicated header and source files, the component follows a modular project structure and is ready to be connected with the URL Validator, URL Normalizer, Fetcher, and other crawler modules in the next phase of development.
