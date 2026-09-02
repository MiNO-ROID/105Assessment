#include <iostream>

#include "../include/Table.h"
#include "../include/Menu.h"

using namespace std;

void displayMainMenu() {
    cout << "\n========================================\n";
    cout << " EATS & TREATS RESTAURANT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Table Management\n";
    cout << "2. Menu Management\n";
    cout << "3. Save Data\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

void displayTableMenu() {
    cout << "\n========== TABLE MANAGEMENT ==========\n";
    cout << "1. Add Table\n";
    cout << "2. View Tables\n";
    cout << "3. Edit Table\n";
    cout << "4. Delete Table\n";
    cout << "5. Assign Guests to Table\n";
    cout << "6. Free Table\n";
    cout << "7. Back to Main Menu\n";
    cout << "Choose an option: ";
}

void displayMenuManagementMenu() {
    cout << "\n========== MENU MANAGEMENT ==========\n";
    cout << "1. Add Menu Item\n";
    cout << "2. View Menu Items\n";
    cout << "3. Edit Menu Item\n";
    cout << "4. Delete Menu Item\n";
    cout << "5. Back to Main Menu\n";
    cout << "Choose an option: ";
}

void runTableMenu(TableManager& tableManager) {
    int choice;

    do {
        displayTableMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                tableManager.addTable();
                break;

            case 2:
                tableManager.viewTables();
                break;

            case 3:
                tableManager.editTable();
                break;

            case 4:
                tableManager.deleteTable();
                break;

            case 5:
                tableManager.assignTable();
                break;

            case 6:
                tableManager.freeTable();
                break;

            case 7:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }

    } while (choice != 7);
}

void runMenuManagementMenu(MenuManager& menuManager) {
    int choice;

    do {
        displayMenuManagementMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                menuManager.addMenuItem();
                break;

            case 2:
                menuManager.viewMenuItems();
                break;

            case 3:
                menuManager.editMenuItem();
                break;

            case 4:
                menuManager.deleteMenuItem();
                break;

            case 5:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }

    } while (choice != 5);
}

int main() {
    TableManager tableManager;
    MenuManager menuManager;

    tableManager.loadTables("../data/tables.csv");
    menuManager.loadMenu("../data/menu.csv");

    int choice;

    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                runTableMenu(tableManager);
                break;

            case 2:
                runMenuManagementMenu(menuManager);
                break;

            case 3:
                tableManager.saveTables("../data/tables.csv");
                menuManager.saveMenu("../data/menu.csv");
                cout << "All data saved successfully.\n";
                break;

            case 4:
                tableManager.saveTables("../data/tables.csv");
                menuManager.saveMenu("../data/menu.csv");
                cout << "Goodbye.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }

    } while (choice != 4);

    return 0;
}