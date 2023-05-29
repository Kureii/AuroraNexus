#ifndef AURORANEXUS_DATABASE_CONNECTION_HPP
#define AURORANEXUS_DATABASE_CONNECTION_HPP

namespace aurora_nexus {

class DatabaseConnection {
 public:
  virtual ~DatabaseConnection() = default;
  virtual void connect() = 0;
};

}

#endif  // AURORANEXUS_DATABASE_CONNECTION_HPP
