# ste-testing

A C++ single-header, minimalistic tool for unit-testing.

# Usage

## Linking

### The correct way

```cmake
find_package(ste-testing)
target_link_libraries(your_target PUBLIC|PRIVATE ste-testing)
```
You should then need to specify where your build of `ste-testing` is.

### The barbarian way

Just put the header somewhere in your project and add it the way you want. It's `BSD-2`, I don't care.

## Adding a test

1. Write a test function which is of type `bool(void)`.
2. Add it to the current test batch with **`ste_add_test(your_test);`**, or if you really want to type more, `ste::testing::add_test(ste_testing_function_info(your_test));`
3. That's all. You can launch the pending tests by calling `ste::testing::launch()` if needed, but the pending ones are executed upon manager destruction.

## Recommandations

1. Tests **SHOULD NOT** rely on external global variables. This may cause the test to use already-freed memory. It's not a good practice to write unit-tests rely on a global state anyway. 
2. Should you write tests for a class with `private` or `protected` members, separate the tests declaration from their definition and use a anonymous `namespace` in a source file. See `/examples/testing_classes` example for more details.

# Examples

See `/examples` directories.

# License

> **BSD 2-Clause License**
> 
> Copyright (c) 2022, Erwan DUHAMEL
> All rights reserved.
> 
> Redistribution and use in source and binary forms, with or without
> modification, are permitted provided that the following conditions are met:
> 
> 1. Redistributions of source code must retain the above copyright notice, this
>    list of conditions and the following disclaimer.
> 
> 2. Redistributions in binary form must reproduce the above copyright notice,
>    this list of conditions and the following disclaimer in the documentation
>    and/or other materials provided with the distribution.
> 
> THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
> AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
> IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
> DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
> FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
> DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
> SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
> CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
> OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
> OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Author

Erwan DUHAMEL
