## Date: July 8 (Session 3)

**Duration:** 120 minutes

## Section 1 - Goal

Implement the **URLValidator** module to validate URLs before they are added to the crawler pipeline.

## Section 2 - Problem Faced

While building the crawler project using CMake, the compilation failed in the URLValidator.cpp file. The compiler reported multiple errors indicating that the std::string class did not contain the starts_with() member function.

### Error Summary:

Build failed during the compilation of URLValidator.cpp.
Errors such as:
'std::string' has no member named 'starts_with'
The errors occurred in both:
hasValidProtocol()
hasDomain()

Root Cause:
The project was being compiled using an older version of the MinGW/GCC compiler that targets a C++ standard where std::string::starts_with() is not available. The starts_with() function was introduced in C++20, so using it with an older compiler or an earlier C++ standard results in compilation errors.

### Resolution:

To maintain compatibility with the current compiler, the validation logic was updated to avoid using starts_with(). Instead, the protocol check was implemented using standard string comparison methods (such as compare() or equivalent prefix checking), allowing the project to compile successfully without requiring C++20 support.

## Section 3 - What I Performed

* Created the **`URLValidator`** class to ensure that only valid URLs are accepted by the crawler.
* Designed and implemented the **header file (`URLValidator.h`)** containing the class declaration and public/private function prototypes.
* Implemented the **`URLValidator.cpp`** source file with the complete validation logic.
* Added the following validation functions:

  * `isValid(const std::string &url)` – Main validation function that checks whether a URL is valid.
  * `hasValidProtocol(const std::string &url)` – Verifies that the URL begins with either **`http://`** or **`https://`**.
  * `hasDomain(const std::string &url)` – Ensures that the URL contains a valid domain by checking:

    * Domain exists after the protocol.
    * Domain does not contain spaces.
    * Domain contains at least one dot (`.`).
    * Domain does not end with a dot (`.`).

## Section 4 - Purpose of URLValidator

The **URLValidator** acts as the first validation layer in the web crawler. Its responsibility is to filter out malformed or invalid URLs before they are passed to components such as the URL Normalizer, Seen Storage, or URL Frontier. This prevents unnecessary processing of invalid URLs and improves the reliability of the crawling pipeline.

## section 5 - Outcome

Successfully implemented the **URLValidator** module along with its header and source files. The validator now performs protocol and domain validation, ensuring that only properly formatted URLs proceed further in the crawler workflow.
