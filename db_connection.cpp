#include <chrono>
#include "db_connection.h"

extern void addConditions(std::set<condition> * receptacle, const web::json::value & j_conditions);

DBConnection::DBConnection() {
    int error = sqlite3_open("../hospitalBeds.db", &db);
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

std::vector<std::tuple<std::size_t, Hospital,bool>>* DBConnection::getHospitals()
{
    return &hospitals;
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
    for(int i=0; i < hospitals.size(); i++)
    {
        if (std::get<1>(hospitals[i]).get_name() == name) {
            msgs = "Hospital already exists";
            return false;
        }
    }
    

    // Add hospital
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    //hospitals.emplace(name, std::ctime(&now));
    Hospital newHospital;
    newHospital.set_name(name);
    hospitals.push_back(std::make_tuple(size_t(hospitals.size()), newHospital, true));
    msgs = "Successfully added hospital";
    return true;
}

bool DBConnection::unregisterHospital(const std::string &name, std::string &msgs) {
    // Handle fail conditions
    if (name.empty()) {
        msgs = "No hospital name specified";
        return false;
    }
    // if (hospitals.count(name) < 1) {
    //     msgs = "No hospital exists by that name";
    //     return false;
    // }

    // Remove hospital
    //hospitals.erase(name);
    msgs = "Successfully removed hospital";
    return true;
}

bool DBConnection::getHospitalData(const std::vector<std::string> &queries,
                                   std::vector<web::json::value> &results, std::string &msgs) {
    // If no specific hospital, send back all hospitals
    // if (queries.empty()) {
    //   for (auto &h : hospitals) {
    //   auto hosp = web::json::value::value::object();
    //         hosp["name"] = web::json::value::string(h.first);
    //         hosp["registered"] = web::json::value::string(h.second);
    //         results.push_back(hosp);
    //     }
    // }
    // // If specific hospital, send back only matching hospitals
    // else {
    //     for (auto &q : queries) {
    //         if (hospitals.count(q) > 0) {
    //             auto hosp = web::json::value::value::object();
    //             hosp["name"] = web::json::value::string(q);
    //             hosp["registered"] = web::json::value::string(hospitals[q]);
    //             results.push_back(hosp);
    //         }
    //         else {
    //             msgs.append(q + " not found. ");
    //         }
    //     }
    // }
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
    error += sqlite3_exec(db, "SELECT * FROM bed", C_Callback, this, &zerrMsg);
    if (error) {
        msgs.append("could not open bed table");
        sqlite3_close(db);
        return false;
    }    
    return true;
}

int DBConnection::databaseCallback(int argc, char **argv, char **columnName) {
    Hospital h;
    Bed b;
    for (int i = 0; i < argc; i++) {
      //printf("%d: , %s = %s\n", i, columnName[i], argv[i] ? argv[i] : "NULL");
      if((strcmp(columnName[1], "name") == 0)) {
          if(strcmp(columnName[i], "ID") == 0) {
              h.set_id(atoi(argv[i]));
              std::cout<<"id: "<<argv[i]<<std::endl;
          }
          else if(strcmp(columnName[i], "name") == 0) {
              h.set_name(argv[i]);
              std::cout<<"name: "<<argv[i]<<std::endl;
          }
          else if(strcmp(columnName[i], "location") == 0) {

              //TODO: change database contents so that they are in lat, lon
              //h.set_locatione(argv[i]);
              std::cout<<"loc: "<<argv[i]<<std::endl;
          }
          // this adds the hospital to the overarching hospitalVector
          if(i%4 == 0) {
              hospitals.push_back(std::make_tuple(0,h,false));
              std::cout<<"hospital added to hospital vector"<<std::endl;
          }
      }
      else if((strcmp(columnName[1], "parentHospitalID") == 0)) {
          if(strcmp(columnName[i], "ID") == 0) {
              b.set_id(atoi(argv[i]));
              std::cout<<"bed id: "<<argv[i]<<std::endl;
          }
          else if(strcmp(columnName[i], "handles") == 0) {
              std::set<condition> handles;
              std::vector<std::string> result;
              std::stringstream s_stream(argv[i]);
              std::vector<utility::string_t> handleVec;
              while(s_stream.good()) {
                std::string substr;
                  getline(s_stream, substr, ',');
                  //handleVec.push_back(utility::conversions::to_string_t(substr);
                  result.push_back(substr);
              }
              //addConditions(&handles, utility::conversions::to_string_t(argv[i]));
              
              for (int j = 0; j < result.size(); j++)
                {
                    std::cout<< result.at(j)<<std::endl;
                }
              //TODO: add content of database to handles set.
              //std::cout<<"handles: "<<argv[i]<<std::endl;
              //b.set_handles(argv[i]);
          }
          else if(strcmp(columnName[i], "special") == 0) {
              std::set<condition> special;
            
              //TODO: add content of database to handles set.
              //std::cout<<"handles: "<<argv[i]<<std::endl;
              //b.set_handles(argv[i]);
          }
          else if(strcmp(columnName[i], "occupied") == 0) {
              b.set_full(argv[i]);
              std::cout<<"occupied: "<<argv[i]<<std::endl;
          }
          else if(strcmp(columnName[i], "timestamp") == 0) {
              b.set_timestamp(atoi(argv[i]));
              std::cout<<"timestamp: "<<argv[i]<<std::endl;
          }
          //TODO: add error checking for position
          if(i%6 == 0) {
            std::get<1>(hospitals[0]).add_bed(b);
              //std::get<1>(hospitalTuple);
              
              std::cout<<"bed added to hospital vector"<<std::endl;
          }
      }
          //std::cout<<columnName[i]<<": "<<argv[i]<<std::endl;

    }
    return 0;
}

bool DBConnection::updateDatabase(Hospital h, std::string & msgs) {
    //iterate through hospital beds and find which beds need to be updated
    std::cout<<"in update"<<std::endl;
    int error = 0;
    char *zerrMsg = 0;
    std::stringstream updateStream;
    for (int i=0; i < h.get_size(); i++)
    {
        int bedID = h.get_bed(i).get_id();
        updateStream<<"UPDATE bed SET occupied = 1 WHERE ID="<<bedID;
        error |= sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);
        updateStream.clear();
    }
    if (error) {
        msgs.append("could not open update bed table");
        sqlite3_close(db);
        return false;
    }
    return true;
}
