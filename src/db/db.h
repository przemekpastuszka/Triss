#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include "helpers.h"
#include "../common/ColumnDesc.h"
#include "../common/Result.h"
#include "../common/Query.h"
#include "../engine/Table.h"

class DataBase {
    public:
    std::map<std::string, Table *> tables;
    std::map<std::string, std::string> files;

    void initialize(void);
    //void save(void);
    void createTable(std::string name, std::vector<ColumnDesc> &columns);
    void loadTable(std::string name, std::string file);
    void loadTable(std::string name, std::string file, char delim);
    void loadTable(std::string name, std::string file, char delim, char esc);
    void saveTable(std::string name, std::string file);
    void dropTable(std::string name);
    void fill_table_with_docs(const std::string name, std::string file,
                              const char delim);
    Result *select(std::string name, Query &q);


    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive &ar, const unsigned int version) {
        std::list<std::string> table_names;
        std::map<std::string, Table *>::iterator it;
        for (it = tables.begin(); it != tables.end(); it++) {
            table_names.push_back(it->first);
            std::ofstream table_file(it->first.c_str());
            {
                boost::archive::text_oarchive oa(table_file);
                oa << *(it->second);
            }
        }
        ar & table_names;
    }
    template <class Archive>
    void load(Archive &ar, const unsigned int version) {
        std::list<std::string> table_names;
        ar & table_names;
        std::list<std::string>::iterator it;
        for (it = table_names.begin(); it != table_names.end(); it++) {
            std::ifstream ifs(*it);
            ar & tables[*it];
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
