.PHONY:all
all:a b

a:a.cpp
	g++ -o $@ $^ -std=c++11
b:b.cpp
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f a b
