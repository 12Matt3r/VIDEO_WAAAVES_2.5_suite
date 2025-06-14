#include "test_utils.h"
#include "ofApp.h" // Required to test ofApp's LFO function
#include "ofConstants.h" // For OF_PI etc.
#include <cmath> // For std::abs

// Helper for float comparisons
bool approximatelyEqual(float a, float b, float epsilon = 0.0001f) {
    return std::abs(a - b) < epsilon;
}

// Test for ofApp::lfo function
// Note: This requires an ofApp instance or for lfo to be static.
// If ofApp::lfo cannot be easily called, this test needs adjustment
// or lfo needs to be refactored for testability.
TestUtils::TestResult TestUtils::test_ofApp_lfo() {
    TestResult result;
    result.name = "ofApp::lfo Test";

    // ofApp testApp; // Removed: lfo is now static.

    // Test case 1: Zero amplitude
    float amp1 = 0.0f; float rate1 = 1.0f; int shape1 = 0;
    float expected1 = 0.0f;
    float actual1 = ofApp::lfo(amp1, rate1, shape1); // Call statically
    if (!approximatelyEqual(actual1, expected1)) {
        result.passed = false;
        result.message = "Failed: amp=0. Expected " + std::to_string(expected1) + ", got " + std::to_string(actual1);
        return result;
    }

    // Test case 2: Simple sine wave value (rate = PI/2, amp = 1 => sin(PI/2) = 1)
    float amp2 = 1.0f; float rate2 = OF_PI / 2.0f; int shape2 = 0;
    float expected2 = 1.0f; // sin(PI/2) = 1
    float actual2 = ofApp::lfo(amp2, rate2, shape2); // Call statically
    if (!approximatelyEqual(actual2, expected2)) {
        result.passed = false;
        result.message = "Failed: sin(PI/2). Expected " + std::to_string(expected2) + ", got " + std::to_string(actual2);
        return result;
    }

    // Test case 3: Shape parameter (currently unused by lfo implementation)
    // This test will pass if lfo ignores shape, but highlights it's not used.
    float amp3 = 1.0f; float rate3 = OF_PI / 4.0f; int shape3 = 1; // Different shape
    float expected3 = 1.0f * std::sin(OF_PI/4.0f);
    float actual3 = ofApp::lfo(amp3, rate3, shape3); // Call statically
     if (!approximatelyEqual(actual3, expected3)) {
        result.passed = false;
        result.message = "Failed: shape test. Expected " + std::to_string(expected3) + " (shape ignored), got " + std::to_string(actual3);
        return result;
    }

    result.passed = true;
    result.message = "All LFO test cases passed.";
    return result;
}


std::vector<TestUtils::TestResult> TestUtils::runAllTests() {
    std::vector<TestResult> results;
    results.push_back(test_ofApp_lfo());
    // Add more test calls here: results.push_back(test_another_function());
    return results;
}

void TestUtils::printTestResults(const std::vector<TestResult>& results) {
    std::cout << "--- Test Results ---" << std::endl;
    int passed_count = 0;
    for (const auto& result : results) {
        std::cout << "[" << (result.passed ? "PASS" : "FAIL") << "] " << result.name << std::endl;
        if (!result.passed) {
            std::cout << "     " << result.message << std::endl;
        }
        if (result.passed) {
            passed_count++;
        }
    }
    std::cout << "--- Summary ---" << std::endl;
    std::cout << passed_count << " of " << results.size() << " tests passed." << std::endl;
}

// Example of how to run:
// In main.cpp, after ofRunApp:
// auto test_results = TestUtils::runAllTests();
// TestUtils::printTestResults(test_results);
// Or, create a separate "test" configuration/main.
