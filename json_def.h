#ifndef HOSPITALBEDS_JSON_DEF_H
#define HOSPITALBEDS_JSON_DEF_H

// Includes the json definitions
#include <cpprest/json.h>

/*
 * This header defines some macros for convenient conversion between native C++
 * types and JSON compatible types
 * Authors: Braeden Muller
 */

// Defines macros to uniformly convert C++ values to JSON
#define JSTR(x) web::json::value::string(x)
#define JBOOL(x) web::json::value::boolean(x)

#endif //HOSPITALBEDS_JSON_DEF_H
