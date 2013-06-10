#include <vector>
#include <triss/src/db/DataBase.h>
#include <triss/src/utils/Tools.h>

void queryAndPrint(DataBase& db, Query& q);

int main(void) {
    DataBase db;
    
    // prepare schema of the table
    std::vector<ColumnDesc> schema;
    schema.push_back(ColumnDesc("name", Schema::STRING));
    schema.push_back(ColumnDesc("age", Schema::NUMERICAL));
    schema.push_back(ColumnDesc("hobbies", Schema::STRING_LIST));
    
    // create and load people table with ; as field delimiter (comma is used to delimit list values)
    db.createTable("people", schema);
    db.loadTable("people", "people.csv", ';');
    
    Query q;  // we will find people older than 30
    std::list<unsigned int> projection = Tools::listFrom(Tools::vector<unsigned int>(3, 0, 1, 2));  // all columns will be selected
    q.selectColumns(projection);  // remember to choose selected columns!
    q.addConstraint(TypedConstraint<double>::greater(1, 30));  // specify column by its index
    q.limit(10);  // you can set limit (though it's not necessary in this case)  
    
    std::cout << "People over 30: " << std::endl;
    queryAndPrint(db, q);
    
    // let's add some constraint on hobby
    std::cout << "People over 30 playing squash: " << std::endl;
    q.addConstraint(TypedConstraint<std::string>::contains(2, "squash"));
    queryAndPrint(db, q);
    
    db.dropTable("people");
    return 0;
}

void queryAndPrint(DataBase& db, Query& q) {
    Result * result = db.select("people", q);
    while(result -> hasNext()) {
        Row * row = result -> next();
        std::cout << "Name: " << row -> get<std::string>(0) << ", Age: " << row -> get<double>(1) <<  std::endl;  // prints name of the guy
    }
    delete result;  // remember to remove the result object!
}
