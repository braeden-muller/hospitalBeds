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
 */
class Client {
public:
    /*!
     * Creates a new client at the specified address. Can only send GETs and POSTS
     * \param addr The URL at which to listen on
     */
    explicit Client();
    ~Client();
    void sendRequest(const std::string& command, const web::json::value& hospital); //sends a request to server
    http_client * client;
private:
    std::string address;

};

#endif //HOSPITALBEDS_CLIENT_H
