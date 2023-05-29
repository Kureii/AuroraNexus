
#ifndef AURORANEXUS_DATABASE_SERVICE_HPP
#define AURORANEXUS_DATABASE_SERVICE_HPP

#include <iostream>
#include <memory>
#include <utility>

#include "aurora_mysql_connection.hpp"

namespace aurora_nexus {

class DatabaseService {
 public:
  DatabaseService(std::shared_ptr<DatabaseConnection> dbConnection)
      : dbConnection(std::move(dbConnection)) {}

  void connectToDatabase() {
    dbConnection->connect();
  }

 private:
  std::shared_ptr<DatabaseConnection> dbConnection;
};

}

#endif  // AURORANEXUS_DATABASE_SERVICE_HPP
