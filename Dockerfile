FROM debian:buster

# Install required Debian packages.
RUN dpkg --add-architecture riscv64 \
 && apt-get update \
 && apt-get install --yes --no-install-recommends \
      build-essential \
      ca-certificates \
      device-tree-compiler \
      gcc-riscv64-linux-gnu \
      git \
 && rm -rf /var/lib/apt/lists/*

# Compile riscv-isa-sim from source.
RUN git clone --branch mmio-plugin https://github.com/vexingcodes/riscv-isa-sim.git \
 && cd riscv-isa-sim \
 && ./configure \
 && make -j$(( $(nproc) + 1 )) \
 && make install \
 && cd ../ \
 && rm -rf riscv-isa-sim

# Compile the example plugin written in C.
COPY ./example_c /example_c
WORKDIR /example_c
RUN make

# Compile the example plugin written in C++.
COPY ./example_cpp /example_cpp
WORKDIR /example_cpp
RUN make

# Compile the plugin test program.
COPY ./plugin_test /plugin_test
WORKDIR /plugin_test
RUN make

# Run both of the example plugins.
RUN spike -m1 --extlib=/example_c/plugin.so --device=test_mmio_plugin,0x10000000,argument /plugin_test/plugin_test \
 && spike -m1 --extlib=/example_cpp/plugin.so --device=test_mmio_plugin,0x10000000,argument /plugin_test/plugin_test 
