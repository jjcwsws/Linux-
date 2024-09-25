.PHONY:all
all:myprocess mytest

myprocess:myprocess.c
	gcc -o $@ $^
mytest:mytest.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f myprocess mytest
