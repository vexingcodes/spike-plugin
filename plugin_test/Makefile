plugin_test: plugin_test.S
	@riscv64-linux-gnu-gcc -nostdlib -static -Wl,-Ttext-segment,0x80000000 plugin_test.S -o plugin_test

.PHONY: clean
clean:
	@rm -f plugin_test
