#include "hospital.h"

//Provide a location and the hospital will be assigned based on the given city 
Hospital::Hospital(std::string newLoc)
{
    location = (newLoc == "Christiansburg") ? "2900 Lamb Cir, Christiansburg, VA 24073" :
               (newLoc == "Lynchburg") ? "1901 Tate Springs Rd, Lynchburg, VA 24501" :
               (newLoc == "Roanoke") ? "1906 Belleview Ave SE, Roanoke, VA 24014" :
               (newLoc == "Bristol") ? "1 Medical Park Blvd, Bristol, TN 37620" :
               (newLoc == "Princeton") ? "122 12th St, Princeton, WV 24740" : "INVALID";

    name =  (newLoc == "Christiansburg") ? "Carilion New River Valley Medical Center" :
               (newLoc == "Lynchburg") ? "Centra Lynchburg General Hospital" :
               (newLoc == "Roanoke") ? "Carilion Roanoke Memorial Hospital" :
               (newLoc == "Bristol") ? "Bristol Regional Medical Center" :
               (newLoc == "Princeton") ? "Princeton Community Hospital" : "INVALID";

    totalBeds = (newLoc == "Christiansburg") ? 110 :
                (newLoc == "Lynchburg") ? 358 :
                (newLoc == "Roanoke") ? 703 :
                (newLoc == "Bristol") ? 269 :
                (newLoc == "Princeton") ? 267 : -1;
}

Hospital::~Hospital()
{
    beds.clear();
    beds.shrink_to_fit();
}