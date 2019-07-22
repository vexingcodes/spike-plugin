plugin.so: plugin.c
	@gcc -shared -o plugin.so -Wall -Werror -fPIC plugin.c

.PHONY: clean
clean:
	@rm -f plugin.so
