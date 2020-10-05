#include "beds.h"

Beds::~Beds()
{
            timestamp = "";
            specialties.clear();
            specialties.shrink_to_fit();
            handles.clear();
            handles.shrink_to_fit();
            occupied = false;
}

void Beds::setBed(void)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    timestamp = std::string(std::ctime(&now));
    occupied = true; 
}