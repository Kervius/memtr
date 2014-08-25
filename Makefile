
ifeq (${X},1)
CROSS_COMPILE=arm-1136jfs-linux-gnueabihf-
CC=${CROSS_COMPILE}gcc
endif

CFLAGS=-Wall

ifeq (${D},1)
CFLAGS+=-ggdb
endif

all: libmemtr.so

libmemtr.so: memtr.o
	${CC} -fPIC $^ -ldl -shared -o $@


test: libmemtr.so test1
	LD_PRELOAD=${CURDIR}/libmemtr.so /bin/sleep 0
	MEMTR=./test1.out LD_PRELOAD=${CURDIR}/libmemtr.so /bin/sleep 0
	test -s ./test1.out
	rm -f ./test1.out
	MEMTR=./test1.out LD_PRELOAD=${CURDIR}/libmemtr.so ./test1
	test -s ./test1.out
	cat ./test1.out | tr '\t' ' ' | fgrep -q "m< 1024"
	cat ./test1.out | tr '\t' ' ' | fgrep -q "m> 1024"
	cat ./test1.out | tr '\t' ' ' | fgrep -q "r< 2048"
	cat ./test1.out | tr '\t' ' ' | fgrep -q "r> 2048"
	cat ./test1.out | tr '\t' ' ' | fgrep -q "c< 4"
	cat ./test1.out | tr '\t' ' ' | fgrep -q "c> 4"
	cat ./test1.out | grep '^f<' | grep -q `grep '^r>' < ./test1.out | awk '{print $$3}'`
	
clean:
	rm -f memtr.o libmemtr.so test1 test*.out
