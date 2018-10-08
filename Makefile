all : slotted_ALOHA_simul

slotted_ALOHA_simul : main.o
	g++ -g -o slotted_ALOHA_simul main.cpp

main.o :
	g++ -c -g -o main.o main.cpp

clean :
	rm -rf *.o
	rm -rf slotted_ALOHA_simul
