DIR_CUR = ../..

DIR_INN = ./WS-PSNR



objects = $(DIR_INN)/main.o

 

main: $(objects)

	g++ $(objects) -o main



main.o : $(DIR_INN)/main.cpp

	g++ -c $(DIR_INN)/main.cpp

		

clean : 

	rm $(objects)