#ifndef HOSPITALBEDS_CLIENT_H
#define HOSPITALBEDS_CLIENT_H

#include <algorithm>
#include <string>
#include <chrono>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cwchar>
#include "db_connection.h"
#pragma comment(lib, "cpprest_2_10")

using namespace web::http::client;

/*!
 * The client sends requests to retrieve and modify resources
 * Authors: Charles Ranson
 */
class Client {
public:
    /*!
     * Creates a new client at the specified address. Can only send GETs and POSTS
     * \param addr The URL at which to listen on
     */
    explicit Client();
    ~Client();
    /*!
     * \brief Sends request to the server in the form of a post (all that is needed for now, GET is used for debug)
     * \param client command used to determine the type of commmand
     * \param hospital JSON value to be sent to the server
     */
    web::json::value sendRequest(const std::string& command, const web::json::value& hospital);
    http_client * client; // The current client object being used to communicate with the server
private:
    std::string address; // The address the server is at
};

#endif //HOSPITALBEDS_CLIENT_H
