test: test.o KahanSum.o Median.o Vector.o Kmeans.o
	g++ -o $@ $^
.cpp.o:
	g++ -c -g $<
clean:
	rm *.o
	rm test
