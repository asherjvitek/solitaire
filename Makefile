build: main.c
	gcc main.c -o solitaire -L ./lib/ -lraylib -I.

run: build
	./solitaire
