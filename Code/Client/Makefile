Client : Main.o QC_Long.o QC_Long.o QC_Base.o Client.o
	g++ -o Client Main.o QC_Long.o QC_Base.o Client.o

Main.o : Main.C
	g++ -o Main.o -c Main.C -std=c++11

QC_Long.o : QC_Long.C
	g++ -o QC_Long.o -c QC_Long.C -std=c++11

QC_Base.o : QC_Base.C
	g++ -o QC_Base.o -c QC_Base.C -std=c++11

Client.o : Client.C
	g++ -o Client.o -c Client.C -std=c++11

clean :
	rm *.o
