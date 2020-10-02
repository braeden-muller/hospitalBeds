#include "client.h"
#include <iostream>
#include <cpprest/http_listener.h>

#define JSTR json::value::string

using namespace web;
using namespace web::http;
using namespace web::http::client;
//using namespace web::http::experimental::listener;
/*Sources Used
* https://mariusbancila.ro/blog/2017/11/19/revisited-full-fledged-client-server-example-with-c-rest-sdk-2-10/
* https://mariusbancila.ro/blog/2013/08/19/full-fledged-client-server-example-with-cpprest-sdk-110/
*/
using std::cout;
using std::cerr;
using std::endl;


/*!
 * \brief Dispatches the request 
 * \param clientt http_client used to send request
 * \param mtd Used to determine if the method is safe or not (specifically if it is GET)
 * \param jvalue The json value that can be sent to the console  
 */
pplx::task<http_response> make_task_request(http_client & client, 
                                            method mtd, 
                                            json::value const & jvalue)
{
   auto path = utility::conversions::to_string_t("/hospital");
   return (mtd == methods::GET) ? client.request(mtd,path) : client.request(mtd,path,jvalue);
}
 
/*!
 * \brief dispatches the request and when the response arrives it displays to std out
 * \param client http_client used to send request
 * \param mtd Used to determine if the method is safe or not (specifically if it is GET)
 * \param jvalue The json value that can be sent to the console  
 */ 
void make_request(http_client & client, method mtd, json::value const & jvalue)
{
   make_task_request(client, mtd, jvalue)
      .then([](http_response response)
      {
         return (response.status_code() == status_codes::OK) ?
             response.extract_json() : pplx::task_from_result(json::value());
      })
      .wait();
}


Client::Client()
{
   client = new http_client("http://localhost:8080"); 
    //Useful video https://www.youtube.com/watch?v=D7fiNQX7P5w

}
void Client::sendRequest(std::string aName,std::string hName, const std::string command)
{
 
   //wcout << L"\nput values\n";
   auto postValue = json::value::object();
   utility::string_t action = utility::conversions::to_string_t("action");
   utility::string_t hospital = utility::conversions::to_string_t("hospital");
   utility::string_t actionType = utility::conversions::to_string_t(aName);
   utility::string_t hospitalName = utility::conversions::to_string_t(hName);
   postValue[action] = json::value::string(actionType);
   postValue[hospital] = json::value::string(hospitalName);
   if (command == "POST")
   {
      std::cout << "I am posting a request" << std::endl;
      //postvalue.push_back(std::make_pair(json::value(key), json::value(value)));
      make_request(*client, methods::POST,postValue); //post value
   }
   else if (command == "GET")
   {
      make_request(*client, methods::GET, json::value::null());// make get request
      //auto getvalue = json::value::array(); //get each json value 
   }   
 
   //getvalue[0] = json::value(key);

}

Client::~Client() {
}
