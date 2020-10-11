FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: cluster

clean:
	rm -rf *.o cluster

cluster: cluster.o LinkedList.o ModularityGroupMatrix.o SpAdjacencyMatrix.o Stack.o Utils.o ErrorHandler.o
	gcc cluster.o LinkedList.o ModularityGroupMatrix.o SpAdjacencyMatrix.o Stack.o Utils.o ErrorHandler.o -o cluster $(LIBS)


LinkedList.o: LinkedList.c LinkedList.h ErrorHandler.h
	gcc $(FLAGS) -c LinkedList.c

ModularityGroupMatrix.o: ModularityGroupMatrix.c ModularityGroupMatrix.h ErrorHandler.h
	gcc $(FLAGS) -c ModularityGroupMatrix.c

SpAdjacencyMatrix.o: SpAdjacencyMatrix.c SpAdjacencyMatrix.h ErrorHandler.h
	gcc $(FLAGS) -c SpAdjacencyMatrix.c

Stack.o: Stack.c Stack.h ErrorHandler.h
	gcc $(FLAGS) -c Stack.c
 
Utils.o: Utils.c Utils.h
	gcc $(FLAGS) -c Utils.c

ErrorHandler.o: ErrorHandler.c ErrorHandler.h
	gcc $(FLAGS) -c ErrorHandler.c

cluster.o: cluster.c SpAdjacencyMatrix.h ModularityGroupMatrix.h Utils.h Stack.h ErrorHandler.h
	gcc $(FLAGS) -c cluster.c