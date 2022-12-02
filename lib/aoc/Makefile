OUTPUTFILE=../libaoc.a
GLIB_CFLAGS=$(shell pkg-config --cflags --libs glib-2.0)
CFLAGS=-g $(GLIB_CFLAGS)
ARFLAGS=rcs

$(OUTPUTFILE): aoc_utils.o
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o ../libaoc.a
