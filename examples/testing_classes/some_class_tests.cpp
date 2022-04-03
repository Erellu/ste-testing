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

#include "some_class_tests.hpp"

#include "../../include/testing.hpp"

namespace some_framework
{

namespace tests
{

/*
 * Should you need to test private members of a class, simply replicate its definition, but remove
 * all protected/private qualifiers and put it in an ANONYMOUS namespace, which makes it available
 * only here (it's a bit oversimplified I know, take a look at anonymous namespaces on cppreference
 * or something.
 *
 * For public API, simply write declare and / or write tests in a header.
 */
namespace
{

/// Test-dedicated definition of the class, with all members public.
struct some_class
{
    inline bool foo()
    {
        if(state == 0)
        {
            ++state;
            return true;
        }

        return false;
    }

    inline void bar()
    {
        state = 0;
    }

    // Previously private members, here exposed for the test.

    int state = 0;
};

} // anonymous namespace

bool some_class_foo()
{
    // This takes the anonymous namespace definition
    some_class s;

    // Public API tests
    ste_fail_test_if(!s.foo());

    // Private API tests related to foo()
    ste_test_success_requires(s.state == 1);
    s.bar();
    ste_fail_test_if(s.state == 0);

    return true;
}

} //namespace tests

} //namespace some_framework
