#main : main.c
#	gcc $^ -o $@ -lm

.PHONY: all
all : train main

train : train.c
	gcc $^ -o $@ -lm

main : main.c
	gcc $^ -o $@ -I/usr/include/libxml2 -lm -lxml2

