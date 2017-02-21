
all: sockwrap echo blah

sockwrap: sockwrap.c
	gcc sockwrap.c -o sockwrap

echo: echo.c
	gcc echo.c -o echo

blah: blah.c
	gcc blah.c -o blah -lpthread

clean:
	rm sockwrap echo blah

dualpipe: dualpipe.c
	gcc dualpipe.c -o dualpipe -lpthread
