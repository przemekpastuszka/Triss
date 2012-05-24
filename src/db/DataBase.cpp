#include "DataBase.h"

void DataBase::initialize(void) {
}

void DataBase::fill_table_with_docs(const std::string name, std::string file,
                                    const char delim) {
    std::ifstream ifs(file.c_str());
    std::string line;
    TableRow* row = tables[name]->createTableRow();
    while(std::getline(ifs, line)) {
        std::vector<std::string> *vals = Helpers::split(line, delim);
        for (int i = 0; i < vals -> size(); ++i) {
            switch(tables[name]->schema[i].type) {
                case Schema::NUMERICAL:
                    row->set<double>(i, std::atoi((*vals)[i].c_str()));
                    break;
                case Schema::STRING:
                    row->set<std::string>(i, (*vals)[i]);
                    break;
                case Schema::NUMERICAL_LIST:
                    row->set< std::list<double> >(
                        i, Helpers::to_num_list((*vals)[i]));
                    break;
                case Schema::STRING_LIST:
                    std::vector<std::string> *tmp =
                        Helpers::split((*vals)[i], ',');
                    std::list<std::string> str_list(tmp->begin(), tmp->end());
                    row->set< std::list<std::string> >(i, str_list);
                    break;
            }
        }
        delete vals;
        tables[name]->addRow(*row);
    }
    tables[name]->prepareStructure();
}

void DataBase::createTable(std::string name, std::vector<ColumnDesc> &columns) {
    /*** create new table and add it to db ***/
    Table *t = new Table();
    std::vector<ColumnDesc> *s = new std::vector<ColumnDesc>;
    s->resize(columns.size(), ColumnDesc());
    std::vector<ColumnDesc>::iterator it;
    for (int i = 0; i < columns.size(); ++i) {
        (*s)[i] = columns[i];
    }
    t->setSchema(*s);
    tables[name] = t;
    files[name] = name;
}

void DataBase::loadTable(std::string name, std::string file) {
    tables[name] = new Table();
    tables[name]->deserialize(file);
    files[name] = name;
}

void DataBase::save(void) {
    std::ofstream db_file("database");
    {
        boost::archive::text_oarchive oa(db_file);
        oa << (*this);
    }
}

void DataBase::dropTable(std::string name) {
    tables.erase(name);
    /* delete table file */
    std::remove(files[name].c_str());
    files.erase(name);
}

Result *DataBase::select(std::string name, Query &q) {
    Table *t = tables[name];
    return t->select(q);
}

#include <list>

int main(void) {
    DataBase db;

    std::vector<ColumnDesc> s;
    s.resize(1, ColumnDesc("marka", Schema::STRING));

    db.createTable("auto", s);
    db.fill_table_with_docs("auto", "./build/autos.txt", ';');
    Query q;
    Result *r = db.select("auto", q);

    return 0;
}
