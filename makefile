test: test.o KahanSum.o Median.o Vector.o Kmeans.o LinearClassifier.o
	g++ -o $@ $^ -std=c++0x
.cpp.o:
	g++ -c -O2 $< -std=c++0x
clean:
	rm *.o
	rm test
