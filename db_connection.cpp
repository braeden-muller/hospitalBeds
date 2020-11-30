
#include <chrono>
#include "db_connection.h"


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

bool DBConnection::registerHospital(Hospital h, std::string &msgs) {
    hospitals.push_back(std::make_tuple(hospitals.size(),h,false));
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
    int pID = 0;
    std:: string msgs;
    for (int i = 0; i < argc; i++) {
      if((strcmp(columnName[1], "name") == 0)) {
          if(strcmp(columnName[i], "ID") == 0) {
              h.set_id(atoi(argv[i]));
          }
          else if(strcmp(columnName[i], "name") == 0) {
              h.set_name(argv[i]);
          }
          else if(strcmp(columnName[i], "location") == 0) {
              std::stringstream s_stream(argv[i]);
              std::string latString, lonString;
              getline(s_stream, latString, ',');
              getline(s_stream, lonString);
              h.set_location(atof(latString.c_str()), atof(lonString.c_str()));
              registerHospital(h, msgs);
          }
      }
      else if((strcmp(columnName[1], "parentHospitalID") == 0)) {
          if(strcmp(columnName[i], "ID") == 0) {
              b.set_id(atoi(argv[i]));
          }
          else if(strcmp(columnName[i], "parentHospitalID") == 0) {
              pID = atoi(argv[i]);
          }
          else if(strcmp(columnName[i], "handles") == 0) {
              std::set<condition> handles;
              std::stringstream s_stream(argv[i]);
              std::vector<utility::string_t> handleVec;
              while(s_stream.good()) {
                  std::string substr;
                  getline(s_stream, substr, ',');
                  handleVec.push_back(utility::conversions::to_string_t(substr));
              }
              
              for (auto it : handleVec) {
                  handles.emplace(conditions_by_name[it].c);
              }
                  
              b.set_handles(handles);
          }
          else if(strcmp(columnName[i], "special") == 0) {
              std::set<condition> specialties;
              std::stringstream s_stream(argv[i]);
              std::vector<utility::string_t> specialVec;
              while(s_stream.good()) {
                  std::string substr;
                  getline(s_stream, substr, ',');
                  specialVec.push_back(utility::conversions::to_string_t(substr));
              }
              
              for (auto it : specialVec) {
                  specialties.emplace(conditions_by_name[it].c);
              }
                  
              b.set_special(specialties);
          }
          else if(strcmp(columnName[i], "occupied") == 0) {
              b.set_full(argv[i]);
          }
          else if(strcmp(columnName[i], "timestamp") == 0) {
              b.set_timestamp(atoi(argv[i]));
              // adds the bed to the hospital.
              std::get<1>(hospitals[pID]).add_bed(b);
          }
      }
          //std::cout<<columnName[i]<<": "<<argv[i]<<std::endl;

    }
    return 0;
}

bool DBConnection::updateDatabase(Hospital h, std::string & msgs) {
    //iterate through hospital beds and find which beds need to be updated
    int error = 0;
    int bedID;
    char *zerrMsg = 0;
    std::stringstream updateStream;
    int pID = -1;

    for (int hIt=0; hIt < hospitals.size(); hIt++) {
      if (std::get<1>(hospitals[hIt]).get_name() == h.get_name())
          pID = hIt;
    }
    std::cout<<"the size of the hospitals data structure is: "<<hospitals.size()<<std::endl;

    if (pID == -1) {
      msgs.append("\nCould not find given hospital, adding new hospital");
        pID = hospitals.size();
        updateStream<<"INSERT INTO hospital(ID,name,location) VALUES("<<pID;
        updateStream<<", \""<<h.get_name();
        //TODO: convert location into string
        updateStream<<"\", \""<<std::get<0>(h.get_location());
        updateStream<<", "<<std::get<1>(h.get_location());
        updateStream<<"\")";
        msgs.append("\n\tadded new hospital");
        std::cout<<"\n"<<updateStream.str()<<std::endl;
        error = sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);
        registerHospital(h, msgs);
        if (error) {
            error = 0;
            msgs.append("\n\t\tCould not create new hospital");
        }
    }
    pID = 0;
    
    for (int i=0; i < h.get_size(); i++)
    {
        bedID = h.get_bed(i).get_id();
        updateStream.str("");
        updateStream<<"UPDATE bed SET occupied="<<h.get_bed(i).get_full()<<" WHERE ID="<<bedID;
        std::cout<<updateStream.str()<<std::endl;
        error = sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);

        updateStream.str("");
        updateStream<<"UPDATE bed SET handles=\""<<h.get_bed(i).get_handles()<<"\" WHERE ID="<<bedID;
        std::cout<<updateStream.str()<<std::endl;
        error |= sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);
	
        updateStream.str("");	
        updateStream<<"UPDATE bed SET specialties=\""<<h.get_bed(i).get_special()<<"\" WHERE ID="<<bedID;
        std::cout<<updateStream.str()<<std::endl;
        error |= sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);
	
        updateStream.str("");	
        updateStream<<"UPDATE bed SET timestamp=\""<<h.get_bed(i).get_timestamp()<<"\" WHERE ID="<<bedID;
        std::cout<<updateStream.str()<<std::endl;
        error |= sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);

        if (error) {
            error = 0;
            msgs.append("\nadded new bed to bed table with id=");
            msgs.append(std::to_string(bedID));
            updateStream.str("");
            updateStream<<"INSERT INTO bed(ID,parentHospitalID,handles,specialties,occupied,timestamp) VALUES("<<bedID;
            updateStream<<", "<<pID;
            updateStream<<", \""<<h.get_bed(i).get_handles();
            updateStream<<"\", \""<<h.get_bed(i).get_special();
            updateStream<<"\", "<<h.get_bed(i).get_full();
            updateStream<<", "<<h.get_bed(i).get_timestamp();
            updateStream<<")";
            std::cout<<updateStream.str()<<std::endl;
            error = sqlite3_exec(db, const_cast<char*>(updateStream.str().c_str()), NULL, this, &zerrMsg);
            if (error) {
                msgs.append("\n\t something went wrong with the insertion of this bed");
            }
        }
        updateStream.clear();
    }
    msgs.append("\nsuccessfully updated hospital");
    return true;
}
