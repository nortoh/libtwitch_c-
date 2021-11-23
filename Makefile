main: src/driver.cpp src/channel.cpp src/user.cpp src/tsocket.cpp src/config.cpp src/utils.cpp src/tag.cpp
	g++-10 -g -pthread -ggdb -o bin/main src/driver.cpp src/channel.cpp src/user.cpp src/tsocket.cpp src/config.cpp src/utils.cpp src/tag.cpp

clean:
	rm -rf *.o main
