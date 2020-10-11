#include "server.h"
#include "bed.h"
#include "hospital.h"
#include <iostream>
#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using std::cout;
using std::cerr;
using std::endl;

/* SAMPLE POST REQUEST
{
    "hospital": "general_hospital",
    "location": [-42.629, 70.233],
    "beds": [
        {
            "id": 0,
            "handles": ["injury", "burn", "virus", "radiation"],
            "special": ["radiation"],
            "isFull": false,
            "timestamp": 0
        },
        {
            "id": 1,
            "handles": ["injury", "burn", "virus", "radiation"],
            "special": ["injury", "burn"],
            "isFull": true,
            "timestamp": 0
        },
        {
            "id": 2,
            "handles": ["injury", "burn", "virus", "radiation"],
            "special": ["virus"],
            "isFull": false,
            "timestamp": 0
        }
    ]
}
*/

/*!
 * \brief Handles an HTTP POST request
 * \param request Request to handle
 */
void handle_post(const http_request& request) {
    cout << "Received POST request" << endl;

    // Relevant request data

    Hospital hospital;

    // This server expects all POST requests to have a purely JSON body
    // Attempt to get the JSON in the body, will fail if the body is not JSON
    auto body = request.extract_json().get();
    try {
        cout << "    Received: " << body.serialize() << endl; // Serialize just converts to a UTF-8 string

        // Parse hospital name
        hospital.set_name(body["hospital"].as_string());

        // Parse hospital location
        auto location = body["location"].as_array();
        hospital.set_location(location[0].as_double(), location[1].as_double());

        // Parse hospital beds
        auto j_beds = body["beds"].as_array();
        for (auto & j_bed : j_beds) {
            hospital.add_bed(Bed(j_bed));
        }
    }
    catch (const std::exception & e) {
        cout << e.what() << endl;
    }

    // Form a reply to give back to the client
    //auto reply = body; // DEBUG for now this just mirrors the client request

    // Send off the assembled reply back to the client
    request.reply(status_codes::OK, hospital.jsonify());
}

Server::Server(const std::string & addr) {
    address = addr;
    cout << "Starting server at [" << address << "]. Type 'q' to exit." << endl;
    // Begin listening at the specified address and register the two helper functions
    http_listener listener(address);
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
