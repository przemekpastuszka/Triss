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
    typedef std::map< std::string, Table * > tables_map_type;
    tables_map_type tables;
    std::map< std::string, std::string > files;

    void initialize(void);
    //void save(void);
    void createTable(std::string name, std::vector<ColumnDesc> &columns);
    void loadTable(std::string name, std::string file);
    void loadTable(std::string name, std::string file, char delim);
    void loadTable(std::string name, std::string file, char delim, char esc);
    void save(void);
    void saveTable(std::string name, std::string file);
    void dropTable(std::string name);
    void fill_table_with_docs(const std::string name, std::string file,
                              const char delim);
    Result *select(std::string name, Query &q);

    /*** for serialization purposes ***/
    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive &ar, const unsigned int version) const {
        std::list<std::string> table_names;
        tables_map_type::const_iterator it;
        for (it = tables.begin(); it != tables.end(); it++) {
            table_names.push_back(it->first);
            //it->second.serialize();
        }
        ar & table_names;
    }
    template <class Archive>
    void load(Archive &ar, const unsigned int version) {
        std::list<std::string> table_names;
        ar & table_names;
        std::list<std::string>::iterator it;
        for (it = table_names.begin(); it != table_names.end(); it++) {
            tables[*it] = new Table();
            //tables[*it].deserialize(*it);
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
