main: src/driver.cpp src/channel.cpp
	g++ -g -pthread -ggdb -o main src/driver.cpp src/channel.cpp

clean:
	rm -rf *.o main
