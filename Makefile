main: main.o buv.o libuv/libuv.a
	$(CC) main.o buv.o libuv/libuv.a -o main -lpthread

main.o: main.c
	$(CC) -c $< -o $@ -Wall -Werror -Ilibuv/include -I.

%.o: %.c %.h
	$(CC) -c $< -o $@ -Wall -Werror -Ilibuv/include -I.

libuv/libuv.a:
	CPPFLAGS=-fPIC $(MAKE) -C libuv

clean:
	make -C libuv clean
	rm -f *.o *.a main
