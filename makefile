all:NLP_Earley_Parser_main

NLP_Earley_Parser_main:EarleyParser.o Node.o Production.o State.o NLP_Earley_Parser_main.o
	g++ -o NLP_Earley_Parser_main EarleyParser.o Node.o Production.o State.o NLP_Earley_Parser_main.o

NLP_Earley_Parser_main.o:NLP_Earley_Parser_main.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -c NLP_Earley_Parser_main.cpp

EarleyParser.o:EarleyParser.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -c EarleyParser.cpp

Node.o:Node.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -c Node.cpp

Production.o:Production.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -c Production.cpp

State.o:State.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -c State.cpp

clean:
	rm -rf *o NLP_Earley_Parser_main
