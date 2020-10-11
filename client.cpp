#include "client.h"
#include <iostream>
#include <cpprest/http_listener.h>

#define JSTR json::value::string

using namespace web;
using namespace web::http;
using namespace web::http::client;

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
 * \brief Used to return the value that was sent back as a repsonse
 * \from the server
 */
void return_response(json::value const & jvalue)
{
   std::cout << jvalue.serialize() << std::endl;
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
      .then([](pplx::task<json::value> feedback)
      {
         try
         {
            return_response(feedback.get());
         }
         catch (http_exception const & e)
         {
            std::cout << e.what() << endl;
         }
      })
      .wait();
}

//Set up at localhost with port 8080 for proper communication
Client::Client()
{
   client = new http_client("http://localhost:8080");
    //Useful video https://www.youtube.com/watch?v=D7fiNQX7P5w
}

/*!
 * \brief Sends request to the server in the form of a post (all that is needed for now, GET is used for debug)
 * \param client command used to determine the type of commmand
 * \param hospital JSON value to be sent to the server 
 */
void Client::sendRequest(std::string command, web::json::value hospital)
{
   if (command == "POST")
   {
      make_request(*client, methods::POST,hospital); //post value
   }
   else if (command == "GET")
   {
      make_request(*client, methods::GET, json::value::null());// make get request
   }
}

Client::~Client() {
}
