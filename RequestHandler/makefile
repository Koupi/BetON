all:	RequestHandler.cpp
	g++ RequestHandler.cpp -fPIC -lfastcgi-daemon2 -shared -o requesthandler.so 

clean:
	rm *.so

