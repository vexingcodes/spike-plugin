# Spike MMIO Plugin Examples

This repository contains examples of Spike MMIO plugins written in C, C++, and Rust. Also included is a simple
Debian-based Dockerfile that will build and run the examples to ensure they are working. To run the full test issue the
following command from the top-level directory of the repository

    docker build .

# Details

## C Example
The [example_c](example_c) directory contains the C plugin. It simply has a [plugin.c](example_c/plugin.c) file
implementing the plugin and a very simple [Makefile](example_c/Makefile) to build it into a shared object that can be
loaded by Spike.

## C++ Example
The [example_cpp](example_cpp) directory contains the C++ plugin. It simply has a [plugin.cpp](example_cpp/plugin.cpp)
file implementing the plugin and a very simple [Makefile](example_cpp/Makefile) to build it into a shared object that
can be loaded by Spike.

## Rust Example
The [example_rust](example_rust) directory contains the Rust plugin. It simply has a [lib.rs](example_rust/src/lib.rs)
file implementing the plugin and a very simple [Cargo.toml](example_rust/Cargo.toml) to build it into a shared object
that can be loaded by Spike.

## Plugin Test
The [plugin_test](plugin_test) directory contains a simple assembly-language driver to test the functionality of the C
and C++ plugins. It contains a [plugin_test.S](plugin_test/plugin_test.S) file implementing the test driver and a very
simple [Makefile](plugin_test/Makefile) to build it into an ELF file that can be loaded by spike. The driver simply
issues a read and write to address `0x10000000` which is the base address where the plugins will be loaded. It then uses
the `tohost` symbol to communicate to Spike that the program should terminate.

# Writing a Plugin

First, build and install [riscv-isa-sim](https://github.com/riscv/riscv-isa-sim). The installation should place the
`riscv/mmio_plugin.h` header into the include path, usually at `/usr/local/include`. Both C and C++ can simply
`#include <riscv/mmio_plugin.h>`.

## C Plugins
A C plugin uses four functions to implement the MMIO interface -- `alloc`, `load`, `store`, and `dealloc`.

The `alloc` function can be used to allocate space for the plugin to store data. It also allows multiple instances of
the same plugin to be differentiated. A `const char* args` parameter is given to the function representing
initialization arguments that can be passed over the Spike command-line invocation.

The `load` function is called when the memory mapped by the plugin is read. The offset from the base address is given as
the `addr` parameter and the number of bytes to read is given by the `len` parameter. The `bytes` paramter is a buffer
long enough to hold all of the read data, and the plugin may place data in the buffer. The function should return true
if the write succeeds and false otherwise.

The `store` function is called when the memory mapped by the plugin is written. The offset from the base address is
given as the `addr` parameter and the number of bytes to read is given by the `len` parameter. The `bytes` paramter is a
buffer containing `len` bytes which is the data to be written. The function should return true if the write succeeds and
false otherwise.

The `dealloc` function is used to deallocate the memory allocated by the `alloc` function, and marks the end of the
lifetime of the plugin.

Once these four functions have been written, a `mmio_plugin_t` structure can be created which contains function pointers
for each of the functions. That `mmio_plugin_t` and a `const char*` name are then given to the `register_mmio_plugin`
function when the shared object is loaded in order to register the plugin with Spike. To call `register_mmio_plugin`
when the shared object is loaded, a function with `__attribute__((constructor))` can be used, as explaind in the
[gcc documentation](https://gcc.gnu.org/onlinedocs/gcc-4.7.0/gcc/Function-Attributes.html).

The plugin shared object can be built using `gcc` with the `-shared` and `-fPIC` flags.

See the [example_c](example_c) directory in this repository for a complete example.

## C++ Plugins
C++ plugins use the same interface as C plugins, but a wrapper has been provided to make it easier to use. To implement
an object-oriented C++ plugin, make a class or struct with a constructor that accepts a `const std::string&`
representing the initialization arguments. Also, provide `bool load(reg_t addr, size_t len, uint8_t* bytes)` and a
`bool store(reg_t addr, size_t len, const uint8_t* bytes)` functions as described above. These _must_ be named `load`
and `store` respectively. Optionally a destructor may also be provided if custom destruction logic is required.

To register a C++ plugin with spike, a global variable of the `mmio_plugin_registration_t` type may be created. This
type accepts a template parameter which is the plugin object type to register. The `mmio_plugin_registration_t` accepts
a `const std::string&` name parameter that represents the plugin name that is used on the Spike command line.

For example, if the plugin class is named `my_awesome_plugin`, then to register that plugin with Spike on shared object
load with the name `my_plugin`, the following code can be used:

    mmio_plugin_registration<my_awesome_plugin> registration("my_plugin");

The plugin shared object can be built using `g++` with the `-shared` and `-fPIC` flags.

See the [example_cpp](example_cpp) directory in this repository for a complete example.

## Rust Plugins
Rust needs to implement its own version of `riscv/mmio_plugin.h` to define the C interface in a way that Rust
understands. After that is done, implementing the plugin is fairly straightforward.

See the [example_rust](example_rust) directory in this repository for a complete example.
