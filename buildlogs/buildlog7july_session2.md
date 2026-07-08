# Build Log – Session 2

**Date:** July 7
**Duration:** 120 minutes

## Section 1 - Today's Goal 

Implement a reusable Queue data structure to support the URL Frontier in the web crawler.

## Section 2- Problem

The crawler requires a FIFO (First-In, First-Out) data structure to manage URLs waiting to be crawled. Rather than directly exposing a Doubly Linked List throughout the project, I wanted to create a separate Queue abstraction that hides the underlying implementation. This would improve modularity, code readability, and make it easier to replace the underlying data structure in the future without affecting other crawler components.

## Section 3 - What I Tried

* Designed and implemented a generic **Queue** wrapper class using C++ templates.
* Used the previously implemented **Doubly Linked List** as the internal storage mechanism to provide efficient insertion and deletion operations.
* Implemented the following public member functions:

  * `enqueue()` – Inserts a new element at the rear of the queue.
  * `dequeue()` – Removes and returns the front element while maintaining FIFO order.
  * `front()` – Returns the front element without removing it from the queue.
  * `empty()` – Checks whether the queue contains any elements.
  * `size()` – Returns the current number of elements stored in the queue.
  * `clear()` – Removes all elements and resets the queue to an empty state.
* Separated the template function definitions into a dedicated **`.tpp`** file while keeping declarations in the header file. This organization improves code readability and follows standard practices for template-based C++ projects.
* Verified that all queue operations correctly interact with the underlying Doubly Linked List and preserve FIFO behavior.
* Ensured that the implementation is generic so it can store any data type, making it reusable across multiple modules of the crawler.

## Section 4 - Outcome

A fully functional and reusable Queue abstraction was successfully implemented. The Queue provides a clean interface while internally relying on a Doubly Linked List for efficient operations. All essential FIFO operations have been implemented and verified. This component is now ready to serve as the underlying data structure for the **URL Frontier**, enabling efficient management of URLs waiting to be crawled while maintaining a modular and extensible project architecture.
