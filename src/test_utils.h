#pragma once
#include <string>
#include <vector>
#include <iostream> // For test output

// Forward declaration of ofApp if needed, or include ofApp.h if it's lightweight
// For simplicity, we might redefine a standalone LFO if ofApp is too heavy to include.
// However, to test ofApp::lfo directly, we'd need an ofApp instance or make lfo static.
// Let's assume for now we can create a simple ofApp instance or make lfo static for testing.
// If not, the test will need to use a reimplemented LFO function.

namespace TestUtils {
    struct TestResult {
        bool passed;
        std::string name;
        std::string message;
    };

    std::vector<TestResult> runAllTests();
    void printTestResults(const std::vector<TestResult>& results);

    // Specific test functions
    TestResult test_ofApp_lfo();
}
