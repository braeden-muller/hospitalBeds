#ifndef HOSPITALBEDS_QUERY_H
#define HOSPITALBEDS_QUERY_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>

/*
 * This class encapsultes data associated with an incoming query attached to
 * every REST call. It creates a map that associates every HTTP parameter with
 * its value for quick reference.
 * Authors: Braeden Muller
 */
class Query {
private:
    // Parameters that have been given in the http query
    std::map<std::string, std::string> params;
public:
    /// Creates a Query by parsing an incoming query string
    explicit Query(const std::string& q_string) {
        std::stringstream pairs(q_string);
        std::string pair;
        while (getline(pairs, pair, '&')) {
            std::stringstream spair(pair);
            std::string first, second;
            getline(spair, first, '=');
            getline(spair, second);
            params.emplace(first, second);
        }

        std::cout << "    Received: " << q_string << std::endl;
    }

    /// Checks if a particular key (parameter) is present
    bool has_key(const std::string& key) {
        return params.find(key) != params.end();
    }

    /// Retrieves the value of a key (parameter)
    std::string operator[](const std::string & key) {
        return params[key];
    }
};

#endif //HOSPITALBEDS_QUERY_H
