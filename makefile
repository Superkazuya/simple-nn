#main : main.c
#	gcc $^ -o $@ -lm
train : train.c
	gcc $^ -o $@ -lm

main : main.c
	gcc $^ -o $@ -I/usr/include/libxml2 -lm -lxml2
