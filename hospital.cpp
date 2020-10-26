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

void Hospital::update(const Hospital & other) {
    // Make a delta hospital that only contains altered and meta information
    Hospital delta(other.get_name(), other.get_location(), other.get_id());

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

Bed Hospital::get_bed(int index) {
  // bounds checking is handled within updateDatabase
  return beds[index];
}

size_t Hospital::get_size() {
  return beds.size();
}

web::json::value Hospital::jsonify() {
    web::json::value j_hospital;
    j_hospital["hospital"] = JSTR(_name);
    j_hospital["location"][0] = location.first;
    j_hospital["location"][1] = location.second;

    for (int i = 0; i < beds.size(); i++) {
        j_hospital["beds"][i] = beds[i].jsonify();
    }

    return j_hospital;
}

bool Hospital::isMetaAltered() const {
    return metaAltered;
}