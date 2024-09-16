mybin:mytest.o
	gcc -o mybin mytest.o
mytest.o:mytest.s
	gcc -c -o mytest.o mytest.s
mytest.s:mytest.i
	gcc -S -o mytest.s mytest.i
mytest.i:mytest.c
	gcc -E -o mytest.i mytest.c


.PHONY:clean
clean:
	rm -f mytest.s mytest.i mytest.o mybin
