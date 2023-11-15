CC= gcc -std=c11 -D_GNU_SOURCE
CFLAGS= -Wall -Wextra -Werror
CAT= s21_cat.c

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(CAT) -o s21_cat

test:
	chmod 777 test.sh
	./test.sh

clean:
	rm -rf s21_cat
	rm -rf *.out *.o

rebuild:
	make clean
	make all
	rm -rf  *.o *.a