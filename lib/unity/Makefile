OUTPUTFILE=../libunity.a
ARFLAGS=rcs

$(OUTPUTFILE): unity.o
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o ../libunity.a
