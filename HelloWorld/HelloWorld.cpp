#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

#include <iostream>
#include <string>

using namespace std;

class HelloWorldClass : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
        HelloWorldClass(fastcgi::ComponentContext *context) :
                fastcgi::Component(context) {
        }
        virtual ~HelloWorldClass() {
        }

public:
        virtual void onLoad() {
        }
        virtual void onUnload() {
        }
        virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
		
		string str = "Hello, World!\n";
		request->write(str.c_str(), str.length());
                request->setHeader("BetON-Header", "Reply from HelloWorld");
        }

};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", HelloWorldClass)
FCGIDAEMON_REGISTER_FACTORIES_END()
