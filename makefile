all: sort rand
	g++ -o sort sort.cpp
	g++ -o rand rand.cpp
clear:
	rm sort rand
