cc=gcc
cc_standard = -std=c99
cc_optimization = -Ofast -march=native
cc_warnings = -Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -pedantic-errors -lpthread
cc_link = -lgmp

fib:cal_fib2.c
	$(cc) $(cc_standard) $(cc_optimization) $(cc_warnings)  -o fib cal_fib2.c $(cc_link)

.PHONY: clean

clean:
	rm -rf fib