main: main.o libuv/libuv.a
	$(CC) main.o libuv/libuv.a -o main -lpthread

%.o: %.c
	$(CC) --std=c89 -D_GNU_SOURCE -Wall -c $< -o $@ -Ilibuv/include

libuv/libuv.a:
	CPPFLAGS=-fPIC $(MAKE) -C libuv

clean:
	make -C libuv clean
	rm -f *.o main
