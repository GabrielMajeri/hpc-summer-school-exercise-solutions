.PHONY: clean

clean:
	-rm *.exe

%.exe: %.c
	mpicc $< -o $@
