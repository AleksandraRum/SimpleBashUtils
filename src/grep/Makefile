CC= gcc  
CFLAGS= -Wall -Wextra -Werror
GREP= s21_grep.c

all: s21_grep

s21_cat:
	$(CC) $(CFLAGS) $(GREP) -o s21_grep

test:
	chmod 777 test.sh
	./test.sh

clean:
	rm -rf s21_grep
	rm -rf *.out *.o

rebuild:
	make clean
	make all
	