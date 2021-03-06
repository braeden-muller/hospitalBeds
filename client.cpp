#include "client.h"
#include <iostream>
#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

/* Sources Used for Code Reference (Charles Ranson)
 * https://mariusbancila.ro/blog/2017/11/19/revisited-full-fledged-client-server-example-with-c-rest-sdk-2-10/
 * https://mariusbancila.ro/blog/2013/08/19/full-fledged-client-server-example-with-cpprest-sdk-110/
 */
using std::cout;
using std::cerr;
using std::endl;
web::json::value body;

/*!
 * \brief Dispatches the request
 * \param clientt http_client used to send request
 * \param mtd Used to determine if the method is safe or not (specifically if it is GET)
 * \param jvalue The json value that can be sent to the console
 */
pplx::task<http_response> make_task_request(http_client & client,
                                            const method& mtd,
                                            json::value const & jvalue)
{
   auto path = utility::conversions::to_string_t("/hospital"); //port for localhost/hospital
   return (mtd == methods::GET) ? client.request(mtd,path) : client.request(mtd,path,jvalue);
}

/*!
 * \brief dispatches the request and when the response arrives it displays to std out
 * \param client http_client used to send request
 * \param mtd Used to determine if the method is safe or not (specifically if it is GET)
 * \param jvalue The json value that can be sent to the console
 */
void make_request(http_client & client, const method& mtd, json::value const & jvalue)
{
   pplx::task_group_status r = make_task_request(client, mtd, jvalue)
      .then([](const http_response& response)
      {
        if (response.status_code() == status_codes::OK){
          body = response.extract_json().get();
          std::cout << "Response : " << body.serialize() << '\n';
        }
      })
      .wait();
}

Client::Client()
{
    // Set up at localhost with port 8080 for proper communication
    client = new http_client("http://localhost:8080");
}

web::json::value Client::sendRequest(const std::string& command, const web::json::value& hospital)
{
   if (command == "POST")
   {
      make_request(*client, methods::POST,hospital); //post value
   }
   else if (command == "DELETE")
   {
      make_request(*client, methods::DEL, hospital); //delete value 
   }
   return body;
}

Client::~Client() = default;
