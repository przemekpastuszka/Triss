/*
* Copyright 2012 Michał Rychlik
*/

#include <boost/serialization/base_object.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/vector.hpp>

#include "Transaction.h"

Command::Command(const std::string& tableName) : tableName(tableName) {}

template <typename Archive> void Command::serialize(
    Archive& ar, const unsigned int version) {
    ar & tableName;
}

class CreateTableCommand : public Command {
private:
    std::vector<ColumnDesc> columns;
public:
    CreateTableCommand(const std::string& tableName,
                       std::vector<ColumnDesc> columns);
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version);
};

CreateTableCommand::CreateTableCommand(
    const std::string& tableName, std::vector<ColumnDesc> columns
) : Command(tableName), columns(columns) {}

template <typename Archive> void CreateTableCommand::serialize(
    Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Command>(*this);
    ar & columns;
}

class LoadTableCommand : public Command {
private:
    std::string filePath;
    std::string delimiter;
    std::string escapeSequence;
public:
    LoadTableCommand(const std::string& tableName, const std::string& filePath,
                     const std::string& delimiter,
                     const std::string& escapeSequence);
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version);
};

LoadTableCommand::LoadTableCommand(
    const std::string& tableName, const std::string& filePath,
    const std::string& delimiter, const std::string& escapeSequence
) : Command(tableName), filePath(filePath), delimiter(delimiter),
    escapeSequence(escapeSequence) {}

template <typename Archive> void LoadTableCommand::serialize(
    Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Command>(*this);
    ar & filePath;
    ar & delimiter;
    ar & escapeSequence;
}

class DropTableCommand : public Command {
public:
    DropTableCommand(const std::string& tableName);
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version);
};

DropTableCommand::DropTableCommand(
    const std::string& tableName
) : Command(tableName) {}

template <typename Archive> void DropTableCommand::serialize(
    Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Command>(*this);
}

class SelectCommand : public Command {
private:
    Query q;
public:
    SelectCommand(const std::string& tableName, Query q);
    template <typename Archive> void serialize(Archive& ar,
                                               const unsigned int version);
};

SelectCommand::SelectCommand(
    const std::string& tableName, Query q
) : Command(tableName), q(q) {}

template <typename Archive> void SelectCommand::serialize(
    Archive& ar, const unsigned int version) {
    ar & boost::serialization::base_object<Command>(*this);
    ar & q;
}

Transaction::Transaction(Connection connection) : connection(connection) {}

void Transaction::createTable(const std::string& tableName,
                              std::vector<ColumnDesc> columns) {
    commands.push_back(CreateTableCommand(tableName, columns));
}

void Transaction::loadTable(const std::string& tableName,
                            const std::string& filePath,
                            const std::string& delimiter,
                            const std::string& escapeSequence) {
    commands.push_back(LoadTableCommand(tableName, filePath, delimiter,
                                        escapeSequence));
}

void Transaction::dropTable(const std::string& tableName) {
    commands.push_back(DropTableCommand(tableName));
}

void Transaction::select(const std::string& tableName, Query q) {
    commands.push_back(SelectCommand(tableName, q));
}

void Transaction::commit() {
    connection.async_write(commands,
                           boost::bind(&Transaction::handle_write, this,
                                       boost::asio::placeholders::error,
                                       *connection
                           )
    );
    connection.run_io_service();
}

void Transaction::handle_write(const boost::system::error_code& e,
                               boost::shared_ptr<Connection> conn) {
    conn->async_read(results, boost::bind(&Transaction::handle_read, this,
                                          boost::asio::placeholders::error
                              )
    );
}

void Transaction::handle_read(const boost::system::error_code& e) {
    if (e) {
      std::cerr << e.message() << std::endl;
    }
}

std::vector<Result*> Transaction::getResults() {
    return results;
    commands.clear();
}


