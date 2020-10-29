#include "server.h"
#include "bed.h"
#include "hospital.h"
#include "query.h"
#include "patient.h"
#include <iostream>
#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

using std::cout;
using std::cerr;
using std::endl;

/* SAMPLE HOSPITAL POST REQUEST
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
/* SAMPLE DOCTOR POST REQUEST
{
    "patient":{
        "ailments": [
            "burn",
            "radiation",
            "scan",
            "virus"
        ],
        "id": "c46f590e-e37f-4788-a784-a303412e7a99",
        "isTreated": false,
        "location": [
            -78.5,
            78.33
        ]
    }
}
 */
/* SAMPLE DOCTOR UPDATE POST REQUEST
{
    "patient_statuses": [
        "c46f590e-e37f-4788-a784-a303412e7a99",
        "a47e521f-b3a1-af8f-b348-9d23c15e72a3"
    ]
}
 */
/* SAMPLE HOSPITAL DELETE REQUEST
localhost:8080/hospital?hospital=general_hospital
 */
/* SAMPLE PATIENT DELETE REQUEST
localhost:8080/hospital?patient=c46f590e-e37f-4788-a784-a303412e7a99
 */

static std::map<std::string, Hospital> hospitals;
static std::map<std::string, Patient> patients;

void routePatient(const Patient& patient) {
    //TODO
}

/*!
 * \brief Handles an HTTP DELETE request
 * \param request Request to handle
 */
void handle_delete(const http_request& request) {
    cout << "Received DELETE request" << endl;
    Query q(request.request_uri().query());

    if (q.has_key("hospital")) {
        std::string msgs;
        if (DBConnection::getInstance()->unregisterHospital(q["hospital"], msgs)) {
            hospitals.erase(q["hospital"]);
            request.reply(status_codes::OK, msgs);
        }
        else
            request.reply(status_codes::BadRequest, msgs);
    }
    else if (q.has_key("patient")) {
        if (patients.find(q["patient"]) != patients.end()) {
            patients.erase(q["patient"]);
            request.reply(status_codes::OK);
        }
        else
            request.reply(status_codes::NotFound);
    }

    request.reply(status_codes::BadRequest);
}

/*!
 * \brief Handles an HTTP POST request
 * \param request Request to handle
 */
void handle_post(const http_request& request) {
    cout << "Received POST request" << endl;

    try {
        // This server expects all POST requests to have a purely JSON body
        // Attempt to get the JSON in the body, will fail if the body is not JSON
        auto body = request.extract_json().get();
        cout << "    Received: " << body.serialize() << endl; // Serialize just converts to a UTF-8 string

        if (body.has_field("patient")) {
            Patient patient(body["patient"]);
            patients.emplace(patient.get_id(), patient);
            request.reply(status_codes::OK);
        }
        else if (body.has_field("patient_statuses")) {
            json::value to_reply;
            int i = 0;
            auto rps = body["patient_statuses"].as_array();
            for (auto & rp : rps) {
                const std::string& pid = rp.as_string();
                if (patients.find(pid) != patients.end()) {
                    to_reply[i++] = patients[pid].jsonify();
                }
            }
            request.reply(status_codes::OK, to_reply);
        }
        else {
            // Relevant request data for hospitals
            Hospital hospital;
            Hospital oldHospital;

            // Parse hospital name
            hospital.set_name(body["hospital"].as_string());

            // Parse hospital location
            auto location = body["location"].as_array();
            hospital.set_location(location[0].as_double(), location[1].as_double());

            // Parse hospital beds
            auto j_beds = body["beds"].as_array();
            for (auto &j_bed : j_beds) {
                hospital.add_bed(Bed(j_bed));
            }

            std::vector<Patient> accepted;
            std::vector<Patient> declined;

            oldHospital = hospitals[hospital.get_name()];
            hospitals[hospital.get_name()].update(hospital, accepted, declined);

            for (auto & aPatient : accepted) {
                aPatient.set_treated(true);
                patients[aPatient.get_id()] = aPatient;
            }

            for (auto & dPatient : declined) {
                patients[dPatient.get_id()] = dPatient;
                routePatient(dPatient);
            }

            // Send off the assembled reply back to the client
            request.reply(status_codes::OK, oldHospital.jsonify());
        }
        return;
    }
    catch (const std::exception & e) {
        cout << e.what() << endl;
    }

    request.reply(status_codes::BadRequest);
}

Server::Server(const std::string & addr) {
    address = addr;
    cout << "Starting server at [" << address << "]. Type 'q' to exit." << endl;
    // Begin listening at the specified address and register the two helper functions
    http_listener listener(address);
    listener.support(methods::POST, handle_post);
    listener.support(methods::DEL, handle_delete);

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
