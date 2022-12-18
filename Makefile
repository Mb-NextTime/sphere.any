CC=g++-11
INCLUDE=include/
EXE=exe/
TMP=tmp/
UNIT=test/unit/
INTEGR=test/integr/
FLAGS=-std=c++23 -I ${INCLUDE} -Wall -Wextra -pedantic -g
SANITIZER=-fsanitize=address,undefined,signed-integer-overflow,pointer-compare,pointer-subtract,leak,bounds,pointer-overflow
TEST_LIBS=-lgtest_main -lgtest -lpthread
DEPS=${INCLUDE}*


.PHONY: all temp test clean

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)
	@mv $@ $(TMP)

all: temp build_main

build_main: $(EXE)main.o $(DEPS)
	$(CC) -o demo $(TMP)main.o $(FLAGS)

build_test: $(EXE)main.o $(DEPS)
	$(CC) -o tmp/demo $(TMP)main.o $(FLAGS) $(SANITIZER)

temp:
	@mkdir -p $(TMP)

test: temp $(UNIT)test.o build_test $(DEPS)
	$(CC) -o test_bin $(TMP)test.o $(FLAGS) $(TEST_LIBS)
	@./test_bin
	@./$(INTEGR)/test.sh

clean:
	@rm -rf $(TMP)
	@rm -f demo
	@rm -f test_bin

