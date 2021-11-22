main: src/driver.cpp src/channel.cpp
	g++ -g -pthread -ggdb -o bin/main src/driver.cpp src/channel.cpp

clean:
	rm -rf *.o main
