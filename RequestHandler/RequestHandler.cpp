#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include "fastcgi2/stream.h"
#include <fastcgi2/data_buffer.h>

#include <iostream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <bsoncxx/array/view.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/types/value.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using namespace std;

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::array;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

//curl -vX GET "http://localhost/events"
//curl -vX GET "http://localhost/events?event_id=1"
//curl -vX POST "http://localhost/events?name=rides&description=horse_rides&date=01/01/2017"
//curl -vX POST "http://localhost/bets?event_id=0&user_id=2&sum=500&side=1"
//curl -vX DELETE "http://localhost/events?event_id=1"

struct user {
	int id;
	int cash;
};

struct bet {
	int id;
	int event_id;
	int user_id;
	int sum;
	int side;
};

struct event {
	string name;
	int id;
	string description;
	string date;
	int sum_side_1;
	int sum_side_2;
	vector<bet> bets;
};

int current_id = 0;

int generate_id() {
	return current_id++; 
}

string bet_to_string(bet b) {
	string s;
	s += "\"bet\": {\n\t\t\"id\": \"" + boost::lexical_cast<string>(b.id)  + "\",\n";
	s += "\t\t\"event_id\": \"" +  boost::lexical_cast<string>(b.event_id) + "\",\n";
	s += "\t\t\"user_id\": \"" +  boost::lexical_cast<string>(b.user_id) + "\",\n";
	s += "\t\t\"sum\": \"" +  boost::lexical_cast<string>(b.sum) + "\",\n";
	s += "\t\t\"side\": \"" +  boost::lexical_cast<string>(b.side) + "\"\n";
	s += "}";
	return s;
}

string short_bet_to_string(bet b) {
	string s;
	s +="\t\t\t\"bet\": {\n";
	s +="\t\t\t\t\"id\": \"" + boost::lexical_cast<string>(b.id) + "\"\n\t\t\t}";
	return s;
}

string event_to_string(event e, vector<bet> bets_collection) {
	string s;
	s += "\t\"event\": {\n\t\t\"name\" : \"" + e.name + "\",\n";
	s += "\t\t\"id\": \"" +  boost::lexical_cast<string>(e.id) + "\",\n";
	s += "\t\t\"description\": \"" + e.description + "\",\n";
	s += "\t\t\"date\": \"" + e.date + "\",\n";
	s += "\t\t\"sum_side_1\": \"" + boost::lexical_cast<string>(e.sum_side_1) + "\",\n";
	s += "\t\t\"sum_side_2\": \"" + boost::lexical_cast<string>(e.sum_side_2) + "\",\n";
	s += "\t\t\"bets\": {\n";
	for(int i = 0; i < bets_collection.size(); i++) {
		if(bets_collection[i].event_id == e.id) {
			s += short_bet_to_string(bets_collection[i]) + "\n";
		}
	}
	s +="\t\t}\n";
	s += "\t}";
	return s;
}

string short_event_to_string(event e) {
	string s;
	s += "\t\t\"event\": {\n";
	s += "\t\t\t\"name\": \"" + e.name + "\",\n";
	s += "\t\t\t\"id\": \"" + boost::lexical_cast<string>(e.id) + "\"\n";
	s += "\t\t}";
	return s;
}

class RequestHelper{
public:
	static string join(const vector<string>& elements, const char* const separator) {
    		switch (elements.size()) {
        		case 0:
            			return "";
        		case 1:
            			return elements[0];
        		default:
            			ostringstream os; 
            			copy(elements.begin(), elements.end()-1, ostream_iterator<string>(os, separator));
            			os << *elements.rbegin();
            			return os.str();
    		}
	}

	static string convertToValidName(string name) {
		size_t start_pos = 0;
		const string space = " ", under = "_";
   		while((start_pos = name.find(space, start_pos)) != string::npos) {
        		name.replace(start_pos, space.length(), under);
        		start_pos += under.length();
    		}
		return name;
	}

	static document buildEventDocumentByName(const string& name) {
		document doc{};
		doc << "name" << convertToValidName(name);
		return doc;
	}
};



class RequestHandler : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
	
	vector<event> events_collection;
	vector<user> users_collection;
	vector<bet> bets_collection;	

        RequestHandler(fastcgi::ComponentContext *context) :
                fastcgi::Component(context) {
        }

        virtual ~RequestHandler() {
        }
public:
        virtual void onLoad() {
		event e1, e2;
		e1.name = "event1";
		e1.id = generate_id();
		e1.description = "ride";
		e1.date = "10/10/2010";
		e1.sum_side_1=100;
		e1.sum_side_2=200;

		e2.name = "event2";
                e2.id = generate_id();
                e2.description = "battle";
                e2.date = "20/20/2020";
                e2.sum_side_1=300;
                e2.sum_side_2=400;
		
		user u1;
		u1.id = generate_id();
		u1.cash = 1000;		
		
		bet b1;
		b1.id=generate_id();
		b1.event_id = e2.id;
		b1.user_id = u1.id;
		b1.sum = 100;
		b1.side = 1;

		this->events_collection.push_back(e1);
		this->events_collection.push_back(e2);

		this->users_collection.push_back(u1);
		
		this->bets_collection.push_back(b1);
        }

        virtual void onUnload() {
        }

#pragma mark - handle request
        virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
		fastcgi::RequestStream stream(request);
                string scriptName = request->getScriptName();
		string collection = scriptName.substr(1,4);
		if(collection == "even") {
			string requestMethod = request->getRequestMethod();
			if(requestMethod == "GET") {
				handleGetRequest(request, context);
			}
			if(requestMethod == "POST") {
				handlePostRequest(request, context);
			}
			if(requestMethod == "DELETE") {
				handleDeleteRequest(request, context);
			}
		}
		if(collection == "bets") {
			string requestMethod = request->getRequestMethod();
			 if(requestMethod == "POST") {
                                handlePostBetsRequest(request, context);
                        }
		}
        }

#pragma mark - events requests
	void handleGetRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
		/*fastcgi::RequestStream stream(request);
		if(request->countArgs() == 0) {
			if(request->getScriptName() == "/events") {
				request->setStatus(200);
				if(this->events_collection.size()==0) {
					stream << "{}\n";
					return;
				}
				stream << "{\n";
				stream << "\t\"events_collection\": {\n";
				for(int i = 0; i < this->events_collection.size() - 1; i++) {
					 stream << short_event_to_string(this->events_collection[i]) << ",\n";
				}
				stream << short_event_to_string(this->events_collection[this->events_collection.size() - 1]) << "\n";
				stream << "\t}\n";
				stream << "}\n";
				return;
			}
		} else if(request->countArgs() == 1 && request->hasArg("event_id")) {
			int  e_id = boost::lexical_cast<int>(request->getArg("event_id"));
			for(int i = 0; i < this->events_collection.size(); i++) {
				event e = this->events_collection[i];
				if(e.id == e_id){
					request->setStatus(200);
					stream << "{\n" << event_to_string(e, this->bets_collection) << "\n}\n";
					break;
				}
			}
			request->setStatus(404);
		}*/
		fastcgi::RequestStream stream(request);
		if(request->countArgs() == 0) {
			if(request->getScriptName() == "/events") {
				request->setStatus(200);
				string result = "{\"events\":[";
				mongocxx::client conn{mongocxx::uri{}};
				auto collection = conn["beton"]["events"];
				mongocxx::cursor cursor = collection.find(document{} << finalize);
				vector<string> str_events;
				for(const auto& doc : cursor) {
  					str_events.push_back(bsoncxx::to_json(doc));
				}
				result.append(RequestHelper::join(str_events, ","));
				result.append("]}");
				stream << result << "\n";
			}
		} else if(request->countArgs() == 1 && request->hasArg("event_id")) {
			string e_id = request->getArg("event_id");
			mongocxx::client conn{mongocxx::uri{}};
			auto collection = conn["beton"]["events"];
			const auto& searchEvent = RequestHelper::buildEventDocumentByName(e_id) << finalize;
			const auto& result = collection.find_one(searchEvent.view());
    			if (result) {
				request->setStatus(200);
				stream << bsoncxx::to_json(*result) << "/n";
			} else {
				request->setStatus(404);
				stream << "Not found./n";
			}	
		}
	}

	void handlePostRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
		fastcgi::RequestStream stream(request);
		if(request->countArgs() == 3 && request->hasArg("name") && request->hasArg("description") && request->hasArg("date")) {
			event e;
			e.name = request->getArg("name");
			e.description = request->getArg("description");
			e.date = request->getArg("date");
			e.sum_side_1 = 0;
			e.sum_side_2 = 0;
			e.id = generate_id();
			this->events_collection.push_back(e);
			request->setStatus(201);
			stream << "{\n" << event_to_string(e, this->bets_collection) << "\n}\n";			
		} else {
			request->setStatus(400);
			stream << "Error! Wrong request!\n";
		}		
	}

	void handleDeleteRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
                fastcgi::RequestStream stream(request);
		if(request->countArgs() == 1 && request->hasArg("event_id")) {
			int e_id = boost::lexical_cast<int>(request->getArg("event_id"));
			for(int i = 0; i < this->events_collection.size(); i++) {
                                event e = this->events_collection[i];
                                if(e.id == e_id){
					stream << "{\n" << event_to_string(e, this->bets_collection) << "\n}\n";
                                        this->events_collection.erase(this->events_collection.begin() + i); 
					for(int i = 0; i < this->bets_collection.size(); i++) {
                         	        	if(this->bets_collection[i].event_id == e_id) {
                                     			this->bets_collection.erase(this->bets_collection.begin() + i);
                                		}
                        		}
					request->setStatus(200);
					return;
                                }
                        }
			request->setStatus(404);
		} else {
			request->setStatus(400);
		}	
	}

#pragma mark - bets requests
	void handlePostBetsRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
                fastcgi::RequestStream stream(request);
		if(request->countArgs() == 4 && request->hasArg("event_id") && request->hasArg("user_id") && request->hasArg("sum") && request->hasArg("side")) {
			bet b;
			b.event_id = boost::lexical_cast<int>(request->getArg("event_id"));
			b.user_id = boost::lexical_cast<int>(request->getArg("user_id"));
			b.sum = boost::lexical_cast<int>(request->getArg("sum"));
			b.side = boost::lexical_cast<int>(request->getArg("side"));
			b.id = generate_id();
		
			for(int i = 0; i < this->events_collection.size(); i++) {
                        	if(this->events_collection[i].id == b.event_id) {	
					for(int j = 0; j < this->users_collection.size(); j++) {
						if(this->users_collection[j].id == b.user_id) {
							if(this->users_collection[j].cash < b.sum) {
								request->setStatus(403);
								stream << "Not enough cash!\n";
								return;
							} else {
								this->users_collection[j].cash -= b.sum;
		                                                this->bets_collection.push_back(b);
                		                                stream << bet_to_string(b) << "\n";
                                		                return;
							}
						}
					}
				}
			}
			request->setStatus(404);
		} else {
			request->setStatus(400);
		}	
	}
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("simple_factory", RequestHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()
