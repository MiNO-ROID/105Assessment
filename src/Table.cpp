//
// Created by GGPC on 2/09/2026.
//

#include "../include/Table.h"

#include <fstream>
#include <iostream>

using namespace std;

RestaurantTable* TableManager::findTableByNumber(int tableNumber) {
    for (RestaurantTable& table : tables) {
        if (table.tableNumber == tableNumber) {
            return &table;
        }
    }

    return nullptr;
}

// add new table
void TableManager::addTable() {
    if (tables.size() >= MAX_TABLES) {
        cout << "Cannot add a new table. "
             << "The restaurant has reached the maximum limit of "
             << MAX_TABLES << " tables.\n";
        return;
    }

    RestaurantTable newTable;

    cout << "\n========== ADD TABLE ==========\n";

    cout << "Enter table number: ";
    cin >> newTable.tableNumber;

    if (newTable.tableNumber <= 0) {
        cout << "Table number must be greater than zero.\n";
        return;
    }

    if (findTableByNumber(newTable.tableNumber) != nullptr) {
        cout << "A table with that number already exists.\n";
        return;
    }

    cout << "Enter table capacity: ";
    cin >> newTable.capacity;

    if (newTable.capacity <= 0 ||
        newTable.capacity > MAX_TABLE_CAPACITY) {
        cout << "Table capacity must be between 1 and "
             << MAX_TABLE_CAPACITY << ".\n";
        return;
    }

    newTable.isOccupied = false;
    newTable.partySize = 0;

    tables.push_back(newTable);

    cout << "Table " << newTable.tableNumber
         << " added successfully.\n";
}

// Display table
void TableManager::viewTables() const {
    cout << "\n========== VIEW TABLES ==========\n";

    if (tables.empty()) {
        cout << "No tables have been added.\n";
        return;
    }

    cout << "Current tables: " << tables.size()
         << " / " << MAX_TABLES << "\n\n";

    for (const RestaurantTable& table : tables) {
        cout << "Table Number: " << table.tableNumber << "\n";
        cout << "Capacity: " << table.capacity << "\n";
        cout << "Status: "
             << (table.isOccupied ? "Occupied" : "Available")
             << "\n";
        cout << "Guests: " << table.partySize << "\n";
        cout << "------------------------------\n";
    }
}

// Change the capacity of an existing table
void TableManager::editTable() {
    int tableNumber;
    int newCapacity;

    cout << "\n========== EDIT TABLE ==========\n";
    cout << "Enter table number to edit: ";
    cin >> tableNumber;

    RestaurantTable* table = findTableByNumber(tableNumber);

    if (table == nullptr) {
        cout << "Table not found.\n";
        return;
    }

    cout << "Current capacity: " << table->capacity << "\n";
    cout << "Enter new capacity: ";
    cin >> newCapacity;

    if (newCapacity <= 0 ||
        newCapacity > MAX_TABLE_CAPACITY) {
        cout << "Table capacity must be between 1 and "
             << MAX_TABLE_CAPACITY << ".\n";
        return;
    }

    if (newCapacity < table->partySize) {
        cout << "New capacity cannot be less than the current party size.\n";
        return;
    }

    table->capacity = newCapacity;

    cout << "Table " << tableNumber
         << " updated successfully.\n";
}

// Deletes an available table
void TableManager::deleteTable() {
    int tableNumber;

    cout << "\n========== DELETE TABLE ==========\n";
    cout << "Enter table number to delete: ";
    cin >> tableNumber;

    for (auto iterator = tables.begin();
         iterator != tables.end();
         ++iterator) {
        if (iterator->tableNumber == tableNumber) {
            if (iterator->isOccupied) {
                cout << "Occupied tables cannot be deleted.\n";
                return;
            }

            tables.erase(iterator);

            cout << "Table " << tableNumber
                 << " deleted successfully.\n";
            return;
        }
    }

    cout << "Table not found.\n";
}

// Assigns guests to a table
void TableManager::assignTable() {
    int tableNumber;
    int partySize;

    cout << "\n========== ASSIGN TABLE ==========\n";
    cout << "Enter table number: ";
    cin >> tableNumber;

    RestaurantTable* table = findTableByNumber(tableNumber);

    if (table == nullptr) {
        cout << "Table not found.\n";
        return;
    }

    if (table->isOccupied) {
        cout << "Table is already occupied.\n";
        return;
    }

    cout << "Enter number of guests: ";
    cin >> partySize;

    if (partySize <= 0) {
        cout << "Party size must be greater than zero.\n";
        return;
    }

    if (partySize > table->capacity) {
        cout << "This party is too large for the selected table.\n";
        return;
    }

    table->isOccupied = true;
    table->partySize = partySize;

    cout << "Table " << tableNumber
         << " assigned to " << partySize
         << " guest(s) successfully.\n";
}

// Free a table after guests leave
void TableManager::freeTable() {
    int tableNumber;

    cout << "\n========== FREE TABLE ==========\n";
    cout << "Enter table number: ";
    cin >> tableNumber;

    RestaurantTable* table = findTableByNumber(tableNumber);

    if (table == nullptr) {
        cout << "Table not found.\n";
        return;
    }

    if (!table->isOccupied) {
        cout << "Table is already available.\n";
        return;
    }

    table->isOccupied = false;
    table->partySize = 0;

    cout << "Table " << tableNumber
         << " is now available.\n";
}

// Loads all table
void TableManager::loadTables(const string& filename) {
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Could not open table data file.\n";
        return;
    }

    tables.clear();

    string line;

    getline(inputFile, line);

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        size_t firstComma = line.find(',');
        size_t secondComma = line.find(',', firstComma + 1);
        size_t thirdComma = line.find(',', secondComma + 1);

        if (firstComma == string::npos ||
            secondComma == string::npos ||
            thirdComma == string::npos) {
            continue;
        }

        RestaurantTable table;

        table.tableNumber = stoi(line.substr(0, firstComma));

        table.capacity = stoi(
            line.substr(
                firstComma + 1,
                secondComma - firstComma - 1
            )
        );

        string occupiedText = line.substr(
            secondComma + 1,
            thirdComma - secondComma - 1
        );

        table.isOccupied =
            occupiedText == "true" || occupiedText == "1";

        table.partySize = stoi(line.substr(thirdComma + 1));

        tables.push_back(table);
    }

    inputFile.close();
}

void TableManager::saveTables(const string& filename) const {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        cout << "Could not save table data.\n";
        return;
    }

    outputFile << "tableNumber,capacity,isOccupied,partySize\n";

    for (const RestaurantTable& table : tables) {
        outputFile << table.tableNumber << ","
                   << table.capacity << ","
                   << (table.isOccupied ? "true" : "false") << ","
                   << table.partySize << "\n";
    }

    outputFile.close();

    cout << "Table data saved successfully.\n";
}

bool TableManager::isTableOccupied(int tableNumber) const {
    for (const auto& t : tables) {
        if (t.tableNumber == tableNumber) {
            return t.isOccupied;
        }
    }
    return false; // table not found
}

int TableManager::getTableCount() const {
    return static_cast<int>(tables.size());
}

int TableManager::getOccupiedTableCount() const {
    int count = 0;
    for (const auto& t : tables) {
        if (t.isOccupied) {
            count++;
        }
    }
    return count;
}