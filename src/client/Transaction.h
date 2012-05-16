/*
* Copyright 2012 Michał Rychlik
*/
#ifndef TRISS_SRC_CLIENT_TRANSACTION_H_
#define TRISS_SRC_CLIENT_TRANSACTION_H_

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

#include <src/common/ColumnDesc.h>
#include <src/common/Result.h>
#include <src/common/Query.h>
#include "Connection.h"

class Command {
private:
    std::string tableName;
protected:
    Command(const std::string& tableName);
public:
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version);
};

class Transaction {
private:
    Connection connection;
    std::vector<Command> commands;
    std::vector<Result*> results;
public:
    Transaction(Connection);
    void createTable(const std::string& tableName,
                     std::vector<ColumnDesc> columns);
    void loadTable(const std::string& tableName, const std::string& filePath,
                   const std::string& delimiter=",",
                   const std::string& escapeSequence = "\\");
    void dropTable(const std::string& tableName);
    void select(const std::string& tableName, Query q);
    void commit();
    std::vector<Result*> getResults();
    void handle_write(const boost::system::error_code& e,
                      boost::shared_ptr<Connection> conn);
    void handle_read(const boost::system::error_code& e);
};

#endif /* TRISS_SRC_CLIENT_TRANSACTION_H_ */


