main: src/driver.cpp src/channel.cpp src/user.cpp src/tsocket.cpp src/config.cpp src/utils.cpp
	g++-10 -g -pthread -ggdb -o bin/main src/driver.cpp src/channel.cpp src/user.cpp src/tsocket.cpp src/config.cpp src/utils.cpp

clean:
	rm -rf *.o main
