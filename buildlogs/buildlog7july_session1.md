# Build Log – Session 1

**Date:** June 7

**Duration:** 120 minutes

## Section 1 - Today'sGoal:

 Design the complete web crawler workflow and identify the core crawling components.

## Section 2 - Work Completed

* Designed the overall crawling workflow from seed URL to page fetching and link extraction.
* Added a **URL Validator** component to verify whether a URL is valid before processing.
* Added a **URL Normalizer** component to convert URLs into a canonical format before they are stored or processed, preventing duplicate URLs caused by formatting differences.
* Finalized the fetching strategy:

  * **libcurl** will be used for fetching static HTML pages.
  * **Chrome DevTools Protocol (CDP)** will be used for JavaScript-rendered pages to obtain the fully rendered DOM.
* Defined the interaction between the major crawler components, including URL Frontier, Fetcher, HTML Parser, and Page Storage.
* Updated the workflow to clearly show the flow of URLs through validation, normalization, fetching, parsing, and storage.
* Created the initial project structure for the web crawler.
* Added the **CollectionsLib-RedisLite** repository to the crawler project as a **Git submodule** so that it can be maintained as an external dependency while remaining synchronized with its original repository.
* Integrated the submodule into the crawler directory, making the previously implemented reusable data structures available throughout the project.
* Prepared the crawler architecture to reuse existing data structures instead of reimplementing them from scratch.
* Planned to use the **Doubly Linked List** implementation from the library as the underlying data structure for the **URL Frontier**, enabling efficient FIFO queue operations.
* Planned to use the **HashMap** implementation from the library to implement **URL Seen Storage**, allowing fast duplicate URL detection and lookup during crawling.
* Established a modular project structure where crawler components can directly leverage the reusable data structure library, reducing code duplication and simplifying future maintenance and extension.

### Updates to the Collections Library

* To support the Queue implementation required by the crawler, the Doubly Linked List was extended with additional operations:

* Implemented deleteFront() to efficiently remove the first element of the list. This operation enables the Queue to perform dequeue operations in O(1) time.
* Implemented front() (non-const version) to return a reference to the first element without removing it, allowing efficient peek operations.
* Implemented front() const to provide read-only access to the first element while maintaining const correctness.
* These additions completed the functionality required for implementing a reusable Queue abstraction on top of the Doubly Linked List.

## section 3 - Outcome

The overall crawler architecture is now clearly defined. The workflow includes all essential components required for efficient crawling, duplicate prevention, and support for both static and JavaScript-based websites. This provides a solid foundation for implementing the crawler in subsequent development sessions.
