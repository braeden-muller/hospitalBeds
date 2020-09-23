#ifndef HOSPITALBEDS_CLIENT_H
#define HOSPITALBEDS_CLIENT_H

#include <string>
#include "db_connection.h"

/*!
 * The server manages outside connections and the internal database
 */
class Client {
public:
    /*!
     * Creates a new client at the specified address. Can only respond to GETs and POSTs
     * \param addr The URL at which to listen on
     */
    explicit Server(const std::string & addr);
    ~Server();
private:
    std::string address;
};


#endif //HOSPITALBEDS_SERVER_H
