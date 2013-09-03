libuv/libuv.a:
	CPPFLAGS=-fPIC $(MAKE) -C libuv

clean:
	make -C libuv clean
