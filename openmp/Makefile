.PHONY: clean

clean:
	-rm *.exe

%.exe: %.c
	$(CC) -fopenmp -std=c17 -Wall -Wextra -g -O3 $< -o $@
