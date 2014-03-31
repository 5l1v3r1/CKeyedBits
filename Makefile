CFILES=src/buff_decoder.c
override INCLUDES += -I./include

lib: build
	for file in $(CFILES); do \
		gcc $(CFLAGS) $(INCLUDES) -c $$file -o ./build/`basename $$file .c`.o; \
	done
	
build:
	mkdir build

clean:
	rm -rf build/
