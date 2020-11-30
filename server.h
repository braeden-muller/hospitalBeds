#ifndef HOSPITALBEDS_SERVER_H
#define HOSPITALBEDS_SERVER_H

#include <string>
#include "db_connection.h"

/*!
 * The server manages outside connections and the internal database
 * Authors: Braeden Muller
 */
class Server {
public:
    /*!
     * Creates a new server at the specified address. Can only respond to GETs and POSTs
     * \param addr The URL at which to listen on
     */
    DBConnection db;
    /// Creates a server that binds to the given address
    explicit Server(const std::string & addr);
    ~Server();
private:
    std::string address; // The address this server is listening on
};


#endif //HOSPITALBEDS_SERVER_H
