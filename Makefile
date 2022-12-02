CC = clang
RELFLAGS= -O2
GLIB_FLAGS=$(shell pkg-config --cflags --libs glib-2.0)
CFLAGS = -g -DDEBUG -pedantic -Wall -Wextra -Wpedantic $(GLIB_FLAGS) -I./lib/aoc -I ./lib/unity
GLIB_LDFLAGS=$(shell pkg-config --libs glib-2.0)
AOC_LIB = -laoc
TEST_LIB = -lunity
LDFLAGS = -L./lib $(AOC_LIB) $(TEST_LIB) -lm $(GLIB_LDFLAGS)

src/$(DAY)/aoc_$(DAY).o:	src/$(DAY)/aoc_$(DAY).c lib/aoc/aoc_utils.h
	$(CC) $(RELFLAGS) $(CFLAGS) -o src/$(DAY)/aoc_$(DAY).o -c src/$(DAY)/aoc_$(DAY).c 

src/$(DAY)/test_$(DAY).o:	src/$(DAY)/aoc_$(DAY).c lib/aoc/aoc_utils.h lib/unity/unity.h
	$(CC) $(RELFLAGS) $(CFLAGS) -o src/$(DAY)/test_$(DAY).o -c src/$(DAY)/test_$(DAY).c 

bin/aoc_$(DAY):	src/$(DAY)/aoc_$(DAY).o lib/libaoc.a
	$(CC) $(RELFLAGS) -o bin/aoc_$(DAY)  src/$(DAY)/aoc_$(DAY).o $(LDFLAGS)

bin/test_$(DAY):	src/$(DAY)/test_$(DAY).o lib/libaoc.a lib/libunity.a
	$(CC) $(RELFLAGS) -o bin/test_$(DAY)  src/$(DAY)/test_$(DAY).o $(LDFLAGS)

$(AOC_LIB):	lib/libaoc.a

lib/libaoc.a: lib/aoc/*.c lib/aoc/*.h
	@cd lib/aoc && $(MAKE)

$(TEST_LIB):	lib/libunity.a
lib/libunity.a: lib/unity/*.c lib/unity/*.h
	@cd lib/unity && $(MAKE)

./src/$(DAY)/input:
	@echo "Need to generate an input file for day $(DAY)"

.PHONY:	run test

run: bin/aoc_$(DAY) 
	@echo "Running AoC $(DAY)"
	@echo "============================"
	@( ./bin/aoc_$(DAY) ./src/$(DAY)/input )
	@echo "============================"

test:	bin/test_$(DAY)
	@echo "Running tests: AOC $(DAY)"
	@echo "============================"
	@( ./bin/test_$(DAY) )
	@echo "============================"
