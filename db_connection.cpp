#include <chrono>
#include "db_connection.h"

DBConnection *DBConnection::instance = nullptr;

DBConnection* DBConnection::getInstance() {
    if (!instance)
        instance = new DBConnection();
    return instance;
}

void DBConnection::shutdown() {
    delete instance;
    instance = nullptr;
}

bool DBConnection::registerHospital(const std::string &name, std::string &msgs) {
    // Handle fail conditions
    if (name.empty()) {
        msgs = "No hospital name specified";
        return false;
    }
    if (hospitals.count(name) > 0) {
        msgs = "Hospital already exists";
        return false;
    }

    // Add hospital
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    hospitals.emplace(name, std::ctime(&now));
    msgs = "Successfully added hospital";
    return true;
}

bool DBConnection::unregisterHospital(const std::string &name, std::string &msgs) {
    // Handle fail conditions
    if (name.empty()) {
        msgs = "No hospital name specified";
        return false;
    }
    if (hospitals.count(name) < 1) {
        msgs = "No hospital exists by that name";
        return false;
    }

    // Remove hospital
    hospitals.erase(name);
    msgs = "Successfully removed hospital";
    return true;
}

bool DBConnection::getHospitalData(const std::vector<std::string> &queries,
                                   std::vector<web::json::value> &results, std::string &msgs) {
    // If no specific hospital, send back all hospitals
    if (queries.empty()) {
        for (auto &h : hospitals) {
            auto hosp = web::json::value::value::object();
            hosp["name"] = web::json::value::string(h.first);
            hosp["registered"] = web::json::value::string(h.second);
            results.push_back(hosp);
        }
    }
    // If specific hospital, send back only matching hospitals
    else {
        for (auto &q : queries) {
            if (hospitals.count(q) > 0) {
                auto hosp = web::json::value::value::object();
                hosp["name"] = web::json::value::string(q);
                hosp["registered"] = web::json::value::string(hospitals[q]);
                results.push_back(hosp);
            }
            else {
                msgs.append(q + " not found. ");
            }
        }
    }
    return true;
}

