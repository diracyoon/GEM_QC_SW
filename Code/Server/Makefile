Server : Main.o Server.o HV_Controller.o
	g++ -DUNIX -o Server Main.o Server.o HV_Controller.o -lcaenhvwrapper -lpthread -lncurses -ldl -lm

Main.o : Main.C
	g++ -DUNIX -o Main.o -c Main.C -std=c++11 -g

Server.o : Server.C
	g++ -DUNIX -o Server.o -c Server.C -std=c++11 -g

HV_Controller.o : HV_Controller.C
	g++ -DUNIX -o HV_Controller.o -c HV_Controller.C -std=c++11 -g  -lcaenhvwrapper

clean :
	rm *.o
