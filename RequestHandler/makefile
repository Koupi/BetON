all:	RequestHandler.cpp
	g++ -I "/home/user/boost/boost_1_62_0" -std=c++11 RequestHandler.cpp -fPIC -lfastcgi-daemon2 -shared -o requesthandler.so $(pkg-config --cflags --libs libmongocxx)

clean:
	rm *.so

