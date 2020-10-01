#ifndef HOSPITALBEDS_CLIENT_H
#define HOSPITALBEDS_CLIENT_H

#include <algorithm>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include "db_connection.h"
#pragma comment(lib, "cpprest_2_10")

/*!
 * The client sends requests to retrieve and modify resources 
 */
class Client {
public:
    /*!
     * Creates a new client at the specified address. Can only send GETs and POSTS
     * \param addr The URL at which to listen on
     */
    explicit Client(const std::wstring key, const std::wstring value, const std::string command);
    ~Client();
private:
    std::string address;

};

//void make_request(http_client & client, method mtd, json::value const & jvalue);
//pplx::task<http_response> make_task_request(http_client & client, 
                              //              method mtd, 
                               //             json::value const & jvalue);


#endif //HOSPITALBEDS_CLIENT_H
