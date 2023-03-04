.PHONY: test libco

all: libco-test-64 # libco-test-32

test: libco all  # libco(PHONY), libco-test-64, libco-test-32
	@echo "==== TEST 64 bit ===="
	@LD_LIBRARY_PATH=.. ./libco-test-64
	# @echo "==== TEST 32 bit ===="
	# @LD_LIBRARY_PATH=.. ./libco-test-32

gdb: libco all  # libco(PHONY), libco-test-64, libco-test-32
	@echo "==== TEST 64 bit ===="
	@LD_LIBRARY_PATH=.. gdb ./libco-test-64
	# @echo "==== TEST 32 bit ===="
	# @LD_LIBRARY_PATH=.. gdb ./libco-test-32

libco-test-64: main.c

libco:
	@cd .. && make -s

libco-test-64: main.c
	gcc -I.. -L.. -m64 -ggdb main.c -o libco-test-64 -lco-64 # 链接时会自动加上lib前缀，所以，-lco-64 会依次在库函数的搜索路径中查找 libco-64.so 和 libco-64.a

libco-test-32: main.c
	gcc -I.. -L.. -m32 -ggdb main.c -o libco-test-32 -lco-32

clean:
	rm -f libco-test-*
