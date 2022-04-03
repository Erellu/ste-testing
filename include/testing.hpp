/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022, Erwan DUHAMEL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef STE_TESTING_HPP
#define STE_TESTING_HPP

#include <cstdint>

#include <exception>

#include <functional>

#include <iostream>

#include <optional>

#include <vector>

namespace ste
{

/**
 *  @brief Unit testing-dedicated functions and classes.
 */
namespace testing
{

//---------------------------------------------------------------------------------------------
// Forward declaration(s)

/// Wraps test information.
struct test_info
{
    /// Function to call.
    const std::function<bool(void)> function;

    /// Test name.
    const std::string_view name = "<Unnamed test>";
};

/// Launches a test and displays its result in the specified std::ostream. Default is std::cout.
bool launch(const test_info& test, std::ostream &out = std::cout);

//---------------------------------------------------------------------------------------------
// Classes

/// Manages tests. Launches the remaining upon destruction.
class test_manager
{
public:

    /// Wraps test failure information.
    struct fail_info
    {
        /// Condition evaluated that failed.
        const std::optional<std::string_view> condition_literal = {};

        /// File in which the error occured.
        const std::optional<std::string_view> file = {};

        /// Line where the error occured.
        const std::optional<int> line = {};

        /// Indicates if the condition should have succeed or failed.
        const bool should_have_succeeded = false;
    };


    /// Destructor. Launches pending tests.
    ~test_manager()
    {
        this->launch();
    }

    /// Launches the current test batch.
    inline void launch()
    {
        // Don't increment the batch index the their is no test to launch.
        if(tests.empty())
        {
            return;
        }

        // Indexes of the test that failed.
        std::vector<std::size_t> failed_index;
        failed_index.reserve(tests.size());

        // Launch the tests
        for(std::size_t i = 0; i < tests.size(); ++i)
        {
            const bool r = testing::launch(tests[i]);

            if(!r)
            {
                failed_index.push_back(i);
            }
        }

        if(failed_index.empty())
        {
            std::cout << "All tests ("<< tests.size() << ") passed for batch " << batch << "." << std::endl;
        }
        else
        {
            std::cout << failed_index.size() << " out of " << tests.size() << " test(s) failed for batch " << batch << ": " << "\n";
            std::cout << "Following test(s) failed: " << "\n";
            for(std::size_t i = 0; i < failed_index.size(); ++i)
            {
                const std::size_t index = failed_index[i];
                std::cout << "    " << index << " (" << tests.at(index).name << ")\n";
            }

            std::cout << std::endl;
        }

        tests.clear();
        ++batch;
    }

    /// Adds a test to the current batch.
    inline void add_test(const test_info &test)
    {
        // Preallocation to avoid constant reallocation
        if(tests.size() + 1 >= tests.capacity())
        {
            tests.reserve(tests.size() + 10);
        }

        tests.push_back(test);
    }

private:

    /// Tests launched in the current batch.
    std::vector<test_info> tests {};

    /// Current batch index.
    std::size_t batch{0};
};

//---------------------------------------------------------------------------------------------
// Global variables

/// Default test manager.
inline test_manager default_test_manager;

//---------------------------------------------------------------------------------------------
// Convenience functions

/**
 *  Asserts condition is true. Calls std::abort() and displays the error message otherwise.
 *  For convenience reasons, you should usually prefer ste_fatal_assert macro.
 *  @param condition Condition to evaluate.
 *  @param message (optional) Error message. Prints "<No error message specified>" if not specified or {} is passed.
 *  @param condition_literal (optional) Condition as string literal. Prints "<No condition literal specified>"
 *  if not specified or {} is passed.
 *  @param file (optional) File. Prints "<Unspecified file>" if not specified or {} is passed.
 *  @param line (optional) Line number. Prints "<Unspecified line>" if not specified or {} is passed.
 *
 */
inline void fatal_assert(
    const bool condition,
    const std::optional<const char* const>& message           = {},
    const std::optional<const char* const>& condition_literal = {},
    const std::optional<const char* const>& file              = {},
    const std::optional<int>& line                            = {})
{
    if(!condition)
    {
        std::clog << "Assertion " << condition_literal.value_or("<No condition literal specified>") << " failed.\n"
        << "    Message: " << message.value_or("<No error message specified>") << "\n"
        << "    File: " << file.value_or("<Unspecified file>") << "\n"
        << "    Line: " << (line.has_value() ? std::to_string(line.value()) : "<Unspecified line>") << std::endl;
        std::abort();
    }
}

//---------------------------------------------------------------------------------------------

/// Adds a test to the default manager batch.
inline void add_test(test_info&& test)
{
    default_test_manager.add_test(test);
}

//---------------------------------------------------------------------------------------------

/// Launches the default test manager pending tests.
inline void launch()
{
    default_test_manager.launch();
}

//---------------------------------------------------------------------------------------------

/// Launches a test and displays its result.
inline bool launch(const test_info &test, std::ostream& out)
{
    out << "------------------------------------------------------" << "\n";
    out << "\t" << test.name                                        << "\n";
    out << "------------------------------------------------------" << "\n";

    try
    {
        const bool r = test.function();

        out << "Test " << (r ? "succeeded" : "failed") << "." << std::endl;

        return r;
    }
    catch(const std::runtime_error& except)
    {
        out << "Test failed (std::runtime_error): " << except.what() << std::endl;
    }
    catch(const std::invalid_argument& except)
    {
        out << "Test failed (std::invalid_argument): " << except.what() << std::endl;
    }
    catch(const std::exception& except)
    {
        out << "Test failed (std::exception): " << except.what() << std::endl;
    }
    catch(const test_manager::fail_info& fail_info)
    {
        out << std::boolalpha;
        out << "Test failed: "
        << "    Assertion " << fail_info.condition_literal.value_or("<Unspecified condition literal>")
        << " should have been " << fail_info.should_have_succeeded << " but was " << !fail_info.should_have_succeeded <<".\n"
        << "    File: "     << fail_info.file.value_or("<Unspecified file>")              << "\n";

        if(fail_info.line.has_value())
        {
            out << "    Line: " << fail_info.line.value() << std::endl;
        }
        else
        {
            out << "    Line: " << "<Unspecified line>" << std::endl;
        }
        out << std::noboolalpha;

    }
    catch(...)
    {
        out << "Test failed (unknown error)." << std::endl;
    }

    return false;
}

//---------------------------------------------------------------------------------------------
// Macros

/// Wrapper to obtain function information
#define ste_testing_function_info(function) {function, #function}

/// Adds a test to the current batch. Expects a function pointer to bool(void).
#define ste_add_test(t) ste::testing::add_test(ste_testing_function_info(t))

/**
 *  Asserts condition is true. Calls std::abort() and displays the error message otherwise.
 *  @param condition Condition to evaluate.
 *  @param message Error message.
 */
#define ste_fatal_assert(condition, message) ste::testing::fatal_assert( \
    condition, \
    message, \
    #condition, \
    __FILE__, \
    __LINE__ \
)

/// Makes a test fail if a condition is true by throwing an ste::testing::test_manager::fail_info.
#define ste_fail_test_if(condition) do \
{ \
    if((condition))\
    {\
        throw ste::testing::test_manager::fail_info{#condition, __FILE__, __LINE__, false};\
    }\
}while(false) //do{}while{} to avoid else highjacking.

/// Makes a test fail if a condition is false by throwing an ste::testing::test_manager::fail_info.
#define ste_test_success_requires(condition) do \
{ \
    if(!(condition))\
    {\
        throw ste::testing::test_manager::fail_info{#condition, __FILE__, __LINE__, true};\
    }\
}while(false) //do{}while{} to avoid else highjacking.

} //namespace testing

} // namespace ste

#endif // STE_TESTING_HPP
