# You can put your build options here
-include config.mk

all: libjsmn.a

libjsmn.a: jsmn.o
	$(AR) rc $@ $^

%.o: %.c jsmn.h
	$(CC) -DJSMN_PARENT_LINKS -c $(CFLAGS) $< -o $@

test: test_default test_strict test_links test_strict_links
test_default: test/tests.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@
test_strict: test/tests.c
	$(CC) -DJSMN_STRICT=1 $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@
test_links: test/tests.c
	$(CC) -DJSMN_PARENT_LINKS=1 $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@
test_strict_links: test/tests.c
	$(CC) -DJSMN_STRICT=1 -DJSMN_PARENT_LINKS=1 $(CFLAGS) $(LDFLAGS) $< -o test/$@
	./test/$@

jsmn_test.o: jsmn_test.c libjsmn.a

myjson_example: example/myjson.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

file_simple: mysource/filesimple.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

productlist: mysource/productlist.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

phonelist: mysource/phonelist.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

jsondump: example/jsondump.o libjsmn.a

tjsondump: example/jsondump.o libjsmn.a
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o example/*.o
	rm -f *.a *.so
	rm -f myjson_example
	rm -f jsondump
	rm -f file_simple
	rm -f productlist
	rm -f phonelist

.PHONY: all clean test
