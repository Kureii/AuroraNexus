#ifndef AURORANEXUS_AURORA_MYSQL_CONNECTION_HPP
#define AURORANEXUS_AURORA_MYSQL_CONNECTION_HPP

#include "../../interfaces/database_connection.hpp"

namespace aurora_nexus {

class MySqlConnection : public DatabaseConnection {
 public:
  void connect() override {
    // Připojení k MySQL databázi
  }
};

}

#endif  // AURORANEXUS_AURORA_MYSQL_CONNECTION_HPP
