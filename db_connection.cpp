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


