#include <chrono>
#include "db_connection.h"

DBConnection::DBConnection() {
    int error = sqlite3_open("hospitalBeds.db", &db);
    if (error) {
        sqlite3_close(db);
    }
  
}

DBConnection::~DBConnection() {
    sqlite3_close(db);
}

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

static int C_Callback(void *data, int argc, char **argv, char **columnName) {
    DBConnection* database = reinterpret_cast<DBConnection*>(data);
    return database->databaseCallback(argc, argv, columnName);
}


bool DBConnection::importDatabase(std::string & msgs) {
    int error = 0;
    char *zerrMsg = 0;
    error = sqlite3_exec(db, "SELECT * FROM hospital", C_Callback, this, &zerrMsg);
    if (error) {
        msgs.append("could not open hospital table");
        sqlite3_close(db);
        return false;
    }
    return true;
}

int DBConnection::databaseCallback(int argc, char **argv, char **columnName)
{
    for (int i = 0; i < argc; i++) { 
        printf("%s = %s\n", columnName[i], argv[i] ? argv[i] : "NULL");
        testing.append(argv[i]);
    }    
    return 0;
}
