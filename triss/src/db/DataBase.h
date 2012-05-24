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
#include "../common/Helpers.h"
#include "../common/ColumnDesc.h"
#include "../common/Result.h"
#include "../common/Query.h"
#include "../engine/Table.h"

class DataBase {
    public:
    typedef std::map< std::string, Table * > tables_map_type;
    tables_map_type tables;

    void initialize(void);
    //void save(void);
    void createTable(std::string name, std::vector<ColumnDesc> &columns);
    void loadTable(std::string name, std::string file);
    void loadTable(std::string name, std::string file,
                   std::vector<ColumnDesc> &columns);
    void loadTable(std::string name, std::string file, char delim);
    void loadTable(std::string name, std::string file, char delim, char esc);
    void dropTable(std::string name);
    void fill_table_with_docs(const std::string name, std::string file,
                              const char delim);
    Result *select(std::string name, Query &q);

    /*** For serialization purposes ***/
    friend class boost::serialization::access;

    template <class Archive>
    void save(Archive &ar, const unsigned int version) {
        std::vector< std::string > table_names;
        std::vector< std::vector< ColumnDesc > > schemas;
        tables_map_type::const_iterator it;
        for (it = tables.begin(); it != tables.end(); it++) {
            table_names.push_back(it->first);
            schemas.push_back(it->second->schema);
        }
        ar & table_names;
        ar & schemas;
    }
    template <class Archive>
    void load(Archive &ar, const unsigned int version) {
        std::vector< std::string > table_names;
        std::vector< std::vector< ColumnDesc > > schemas;
        ar & table_names;
        ar & schemas;

        for (int i = 0; i < table_names.size(); ++i) {
            createTable(table_names[i], schemas[i]);
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};
