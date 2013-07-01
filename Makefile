hash_table: src/hash_table.c src/hash_table.h src/main.c
	gcc -g -Wall src/*.c -o hash_table

clean:
	rm hash_table
