build: algo1 algo2
	touch test.in
	touch test.out
algo1:
	g++ algo1.cpp -o algo1
algo2:
	g++ algo2.cpp -o algo2
run-p1:
	./algo1
run-p2:
	./algo2
run-best:
	./algo2
clean:
	rm algo1 
	rm algo2 
	rm test.in 
	rm test.out
