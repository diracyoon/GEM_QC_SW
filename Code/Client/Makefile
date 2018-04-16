Client : Main.o Client.o
	g++ -o Client Main.o Client.o

Main.o : Main.C
	g++ -o Main.o -c Main.C -std=c++11

Client.o : Client.C
	g++ -o Client.o -c Client.C -std=c++11

clean :
	rm *.o
