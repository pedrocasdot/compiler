build:
	g++ -o ./objects/lex.o lex.cpp -c -g
	g++ -o ./objects/compile.o compile.cpp -c -g
	g++ -o ./objects/main.o main.cpp -c -g
	g++ -o main ./objects/*.o
run:
	./main
clean:
	rm ./objects/*.o
	rm main