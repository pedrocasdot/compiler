build:
	g++ -o ./bin/lex.o lex.cpp -c -g
	g++ -o ./bin/parser.o parser.cpp -c -g
	g++ -o ./bin/sematic.o semantic.cpp -c -g
	g++ -o ./bin/compile.o compile.cpp -c -g
	g++ -o ./bin/main.o main.cpp -c -g
	g++ -o main ./bin/*.o
run:
	./main
clean:
	rm ./bin/*.o
	rm main