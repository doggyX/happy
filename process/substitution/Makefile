.POHNY:all
all:test myproc 

test:test.c
	gcc -o $@ $^
myproc:myproc.c
	gcc -o $@ $^
.POHNY:clean
clean:
	rm myproc test -f
