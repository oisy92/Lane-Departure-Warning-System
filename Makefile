output: Main.o PreProcessing.o Processing.o
	g++ Main.o PreProcessing.o Processing.o -o Software

Main.o: Main.cpp
	g++ -c Main.cpp

PreProcessing.o: PreProcessing.cpp PreProcessing.h
	g++ -c PreProcessing.cpp

PreProcessing.o: Processing.cpp Processing.h
	g++ -c Processing.cpp

Clean:
	rm *.o Software

	