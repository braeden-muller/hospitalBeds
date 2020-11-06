#include "hospital.h"
#include "db_connection.h"

Hospital::Hospital(const std::string & name, std::pair<double, double> loc, int id) {
    _name = name;
    location = loc;
    _id = id;
}

std::string Hospital::get_name() const {
    return _name;
}

void Hospital::set_name(const std::string &name) {
    _name = name;
}

std::pair<double, double> Hospital::get_location() const {
    return location;
}

void Hospital::set_location(double lat, double lon) {
    location.first = lat;
    location.second = lon;
}

int Hospital::get_id() const {
    return _id;
}

void Hospital::set_id(int id) {
  _id = id;
}

bool Hospital::add_bed(const Bed& bed) {
    beds.push_back(bed);
    return true;
}

Bed Hospital::get_bed(int index) const {
  // bounds checking is handled within updateDatabase
  return beds[index];
}

int Hospital::bed_index_by_id(int id) const {
    for (int i = 0; i < beds.size(); i++) {
        if (beds[(i + id) % beds.size()].get_id() == id)
            return static_cast<int>(i + id) % beds.size(); // NOLINT(cppcoreguidelines-narrowing-conversions)
    }

    return -1;
}

size_t Hospital::get_size() const {
  return beds.size();
}

void Hospital::update(const Hospital & other, std::vector<Patient>& accepted, std::vector<Patient>& declined) {
    _name = other.get_name();
    // Make a delta hospital that only contains altered and meta information
    Hospital delta(other.get_name(), other.get_location(), other.get_id());

    this->patientQueue.clear();

    // Keep track of whether the patient was accepted or declined
    for (int i = 0; i < other.patientQueue.size(); i++) {
        // If the patient was declined, sort it into declined
        if (other.patientQueue[i].get_assigned_hospital() == "DECLINED") {
            Patient thisPatient = other.patientQueue[i];
            thisPatient.set_assigned_hospital("NONE");
            declined.push_back(thisPatient);
        }
        // If no action, keep hold of it
        else if (other.patientQueue[i].get_assigned_hospital() == "NONE" || other.patientQueue[i].get_assigned_hospital().empty()) {
            this->patientQueue.push_back(patientQueue[i]);
        }
        // If there is a hospital name, sort it into accepted
        else {
            accepted.push_back(other.patientQueue[i]);
        }
    }

    // Since delta always contains all meta information, add a flag if the meta information has changed
    delta.metaAltered = delta.get_location() != this->get_location();

    // Update current information stored in memory
    this->set_location(delta.get_location().first, delta.get_location().second);

    // Go through every bed that was received
    for (int i = 0; i < other.get_size(); i++) {
        Bed oBed = other.get_bed(i);
        // Find the local bed by the same id of the received bed
        int tBedIndex = bed_index_by_id(oBed.get_id());

        // If it wasn't found, then add a new bed
        if (tBedIndex == -1) {
            add_bed(oBed);
            delta.add_bed(oBed);
        }
        // If it was found and is older than the received bed, update the information
        else {
            if (beds[tBedIndex] < oBed) {
                beds[tBedIndex] = oBed;
                delta.add_bed(oBed);
            }
        }
    }

    // If there are updates, then send them to the database
    if (delta.metaAltered || delta.get_size() > 0) {
        std::string msgs;
        DBConnection::getInstance()->updateDatabase(delta, msgs);
        if (!msgs.empty())
            std::cerr << msgs << std::endl;
    }
}

web::json::value Hospital::jsonify() {
    web::json::value j_hospital;
    j_hospital["hospital"] = JSTR(_name);
    j_hospital["location"][0] = location.first;
    j_hospital["location"][1] = location.second;

    for (int i = 0; i < beds.size(); i++) {
        j_hospital["beds"][i] = beds[i].jsonify();
    }

    for (int i = 0; i < patientQueue.size(); i++) {
        j_hospital["patientQueue"][i] = patientQueue[i].jsonify();
    }

    return j_hospital;
}

bool Hospital::isMetaAltered() const {
    return metaAltered;
}

void Hospital::add_patient(Patient & patient) {
    patientQueue.push_back(patient);
    patient.add_attempt(_name);
}

void Hospital::push_patient(const Patient& patient) {
    patientQueue.push_back(patient);
}

void Hospital::remove_patient(const std::string &id) {
    for (auto it = patientQueue.begin(); it < patientQueue.end(); it++) {
        if (it->get_id() == id)
            patientQueue.erase(it);
    }
}

double Hospital::rank(const Patient &patient) const {
    if (patient.has_attempted(_name))
        return 0;

    double highest = 0;
    for (auto & b : beds) {
        double rank = b.rank(patient);
        highest = rank > highest ? rank : highest;

        if (highest == 1.5)
            break;
    }

    double dlat = patient.get_location().first - location.first;
    double dlon = patient.get_location().second - location.second;
    double distance = sqrt((dlat * dlat) + (dlon * dlon));
    distance = distance == 0 ? 0.000001 : distance;
    return (1 / distance) * highest;
}
