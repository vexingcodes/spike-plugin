plugin.so: plugin.cpp
	@g++ -shared -o plugin.so -Wall -Werror -fPIC plugin.cpp

.PHONY: clean
clean:
	@rm -f plugin.so
