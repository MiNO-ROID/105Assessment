#ifndef INC_105ASSESSMENT_TABLE_H
#define INC_105ASSESSMENT_TABLE_H

#include <string>
#include <vector>
#include "manager.h"

using namespace std;

const int MAX_TABLES = 20;
const int MAX_TABLE_CAPACITY = 12;

struct RestaurantTable {
    int tableNumber;
    int capacity;
    bool isOccupied;
    int partySize;
};

class TableManager : public manager {
private:
    vector<RestaurantTable> tables;

    RestaurantTable* findTableByNumber(int tableNumber);

public:
    void addTable();

    void viewTables() const;

    void editTable();

    void deleteTable();

    void assignTable();

    void freeTable();

    void loadTables(const string& filename);

    void saveTables(const string& filename) const;

    void displayData() const override;

    void loadData(const string& filename) override;

    void saveData(const string& filename) const override;

    bool isTableOccupied(int tableNumber) const;

    int getTableCount() const;

    int getOccupiedTableCount() const;
};

#endif // INC_105ASSESSMENT_TABLE_H