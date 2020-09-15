#include "server.h"
#include <iostream>
#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using std::cout;
using std::cerr;
using std::endl;

/*!
 * \brief Handles an HTTP GET request
 * \param request Request to handle
 */
void handle_get(const http_request &request) {
    cout << "Received GET request" << endl;

    // localhost:8080/hospital?foo=bar&a=b
    // The query vars are therefore "foo: bar" and "a: b". vars is a map of these pairs.
    auto vars = uri::split_query(request.request_uri().query());
    for (auto & var : vars) {
        cout << "    " << var.first << ": " << var.second << endl;
    }

    // Form a dummy reply to give back to the client
    auto reply = json::value::value::object();
    reply["sample_get_reply"] = json::value::string("sample_value");

    // Send off the assembled reply back to the client
    request.reply(status_codes::OK, reply);
}

/*!
 * \brief Handles an HTTP POST request
 * \param request Request to handle
 */
void handle_post(const http_request& request) {
    cout << "Received POST request" << endl;

    // This server expects all POST requests to have a purely JSON body
    // Attempt to get the JSON in the body, will fail if the body is not JSON
    try {
        auto body = request.extract_json().get();
        cout << "    " << body.serialize() << endl; // Serialize just converts to a UTF-8 string
    }
    catch (const std::exception & e) {
        cout << e.what() << endl;
    }

    // Form a dummy reply to give back to the client
    auto reply = json::value::value::object();
    reply["sample_post_reply"] = json::value::string("sample_value");
    reply["sample_reply"] = json::value::string("sample");

    // Send off the assembled reply back to the client
    request.reply(status_codes::OK, reply);
}

Server::Server(const std::string & addr) {
    address = addr;
    cout << "Starting server at [" << address << "]. Type 'q' to exit." << endl;

    // Begin listening at the specified address and register the two helper functions
    http_listener listener(address);
    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);

    try {
        // Spawns a new listening thread
        listener.open().wait();

        // The listening thread closes once it leaves scope, so this loop keeps it in scope
        // The loop exits when the user presses q and then enter
        unsigned char c;
        while(true) {
            c = getchar();
            if (c == 'q')
                break;
        }
    }
    catch (const std::exception & e) {
        cout << e.what() << endl;
    }
}