.PHONY: clean

clean:
	-rm *.exe

%.exe: %.c
	$(CC) -fopenmp -std=c17 -Wall -Wextra -g -O3 $< -o $@

%-fortran.exe: %.f90
	$(FC) -fopenmp -std=f2018 -Wall -Wextra -g -O3 $< -o $@
