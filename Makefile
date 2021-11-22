main: src/driver.cpp src/channel.cpp src/user.cpp
	g++ -g -pthread -ggdb -o bin/main src/driver.cpp src/channel.cpp src/user.cpp

clean:
	rm -rf *.o main
