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

void Beds::setBed(std::string currentTime, bool occupied)
{
    //TODO
}