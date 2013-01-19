test: test.o Vector.o KahanSum.o Median.o MLP.o
	g++ -o $@ $^ -std=c++0x
.cpp.o:
	g++ -c -g $< -std=c++0x
clean:
	rm *.o
	rm test
