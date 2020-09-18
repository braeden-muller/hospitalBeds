#include "server.h"
#include <iostream>
#include <cpprest/http_listener.h>

#define JSTR json::value::string

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

    // Form a dummy reply to give back to the client
    auto reply = json::value::value::array();

    // localhost:8080/hospital?foo=bar&a=b
    // The query vars are therefore "foo: bar" and "a: b". vars is a map of these pairs.
    auto vars = uri::split_query(request.request_uri().query());

    // Form the query
    std::vector<std::string> hospitalQueries;
    for (auto & var : vars) {
        if (var.first == "hospital")
            hospitalQueries.push_back(var.second);
        cout << "    " << var.first << ": " << var.second << endl;
    }

    // Poll the database for these values
    std::string msgs;
    std::vector<json::value> results;
    DBConnection::getInstance()->getHospitalData(hospitalQueries, results, msgs);

    // Place results into reply
    int i = 0;
    for (auto & hosp : results)
        reply[i++] = hosp;

    cout << "    " << msgs << endl;

    // Send off the assembled reply back to the client
    request.reply(status_codes::OK, reply);
}

/*!
 * \brief Handles an HTTP POST request
 * \param request Request to handle
 */
void handle_post(const http_request& request) {
    cout << "Received POST request" << endl;

    // Relevant request data
    std::string action, hospital;
    std::string feedback = "Action not supported";

    // This server expects all POST requests to have a purely JSON body
    // Attempt to get the JSON in the body, will fail if the body is not JSON
    try {
        auto body = request.extract_json().get();
        cout << "    Received: " << body.serialize() << endl; // Serialize just converts to a UTF-8 string
        action = body["action"].as_string();
        hospital = body["hospital"].as_string();
    }
    catch (const std::exception & e) {
        cout << e.what() << endl;
    }

    // Form a reply to give back to the client
    auto reply = json::value::value::object();
    bool success = false;

    if (!action.empty()) {
        if (action == "register")
            success = DBConnection::getInstance()->registerHospital(hospital, feedback);
        else if (action == "unregister")
            success = DBConnection::getInstance()->unregisterHospital(hospital, feedback);
    }

    // Log what the result is
    cout << "    Result: Request " << (success ? "successful, " : "failed, ") << feedback << endl;

    // Send off the assembled reply back to the client
    reply["feedback"] = JSTR(feedback);
    request.reply(success ? status_codes::OK : status_codes::BadRequest, reply);
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

Server::~Server() {
    DBConnection::shutdown();
}