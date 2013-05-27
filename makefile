#main : main.c
#	gcc $^ -o $@ -lm
train : neuralnetwork.c
	gcc $^ -o $@ -lm
