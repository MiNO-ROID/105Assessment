//
// Created by GGPC on 2/09/2026.
//

#include "../include/Menu.h"

#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

MenuItem* MenuManager::findMenuItemById(int itemId) {
    for (MenuItem& item : menuItems) {
        if (item.itemId == itemId) {
            return &item;
        }
    }
    return nullptr;
}

void MenuManager::addMenuItem() {
    if (menuItems.size() >= MAX_MENU_ITEMS) {
        cout << "Cannot add a new menu item. "
             << "The menu has reached its maximum limit of "
             << MAX_MENU_ITEMS << " items.\n";
        return;
    }

    MenuItem newItem;

    cout << "\n========== ADD MENU ITEM ==========\n";

    cout << "Enter menu item ID: ";
    cin >> newItem.itemId;

    if (newItem.itemId <= 0) {
        cout << "Menu item ID must be greater than zero.\n";
        return;
    }

    if (findMenuItemById(newItem.itemId) != nullptr) {
        cout << "A menu item with that ID already exists.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter item name: ";
    getline(cin, newItem.name);

    if (newItem.name.empty()) {
        cout << "Item name cannot be empty.\n";
        return;
    }

    cout << "Enter item description: ";
    getline(cin, newItem.description);

    cout << "Enter item category: ";
    getline(cin, newItem.category);

    if (newItem.category.empty()) {
        cout << "Category cannot be empty.\n";
        return;
    }

    cout << "Enter item price: $";
    cin >> newItem.price;

    if (newItem.price <= 0 || newItem.price > MAX_MENU_PRICE) {
        cout << "Price must be between $0.01 and $"
             << MAX_MENU_PRICE << ".\n";
        return;
    }

    int availabilityChoice;

    cout << "Is the item available?\n";
    cout << "1. Yes\n";
    cout << "2. No\n";
    cout << "Choose an option: ";
    cin >> availabilityChoice;

    if (availabilityChoice == 1) {
        newItem.isAvailable = true;
    } else if (availabilityChoice == 2) {
        newItem.isAvailable = false;
    } else {
        cout << "Invalid availability option.\n";
        return;
    }

    menuItems.push_back(newItem);

    cout << "Menu item \"" << newItem.name
         << "\" added successfully.\n";
}

void MenuManager::viewMenuItems() const {
    cout << "\n========== VIEW MENU ITEMS ==========\n";

    if (menuItems.empty()) {
        cout << "No menu items have been added.\n";
        return;
    }

    cout << "Current menu items: " << menuItems.size()
         << " / " << MAX_MENU_ITEMS << "\n\n";

    for (const MenuItem& item : menuItems) {
        cout << "Item ID: " << item.itemId << "\n";
        cout << "Name: " << item.name << "\n";
        cout << "Description: " << item.description << "\n";
        cout << "Category: " << item.category << "\n";
        cout << "Price: $" << item.price << "\n";
        cout << "Availability: "
             << (item.isAvailable ? "Available" : "Unavailable")
             << "\n";
        cout << "------------------------------------\n";
    }
}

void MenuManager::editMenuItem() {
    int itemId;

    cout << "\n========== EDIT MENU ITEM ==========\n";
    cout << "Enter menu item ID to edit: ";
    cin >> itemId;

    MenuItem* item = findMenuItemById(itemId);

    if (item == nullptr) {
        cout << "Menu item not found.\n";
        return;
    }

    int choice;

    do {
        cout << "\nEditing: " << item->name << "\n";
        cout << "1. Change Name\n";
        cout << "2. Change Description\n";
        cout << "3. Change Category\n";
        cout << "4. Change Price\n";
        cout << "5. Change Availability\n";
        cout << "6. Finish Editing\n";
        cout << "Choose an option: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                cout << "Enter new name: ";
                getline(cin, item->name);

                if (item->name.empty()) {
                    cout << "Name cannot be empty.\n";
                } else {
                    cout << "Name updated successfully.\n";
                }
                break;

            case 2:
                cout << "Enter new description: ";
                getline(cin, item->description);
                cout << "Description updated successfully.\n";
                break;

            case 3:
                cout << "Enter new category: ";
                getline(cin, item->category);

                if (item->category.empty()) {
                    cout << "Category cannot be empty.\n";
                } else {
                    cout << "Category updated successfully.\n";
                }
                break;

            case 4: {
                double newPrice;

                cout << "Enter new price: $";
                cin >> newPrice;

                if (newPrice <= 0 || newPrice > MAX_MENU_PRICE) {
                    cout << "Price must be between $0.01 and $"
                         << MAX_MENU_PRICE << ".\n";
                } else {
                    item->price = newPrice;
                    cout << "Price updated successfully.\n";
                }
                break;
            }

            case 5: {
                int availabilityChoice;

                cout << "1. Available\n";
                cout << "2. Unavailable\n";
                cout << "Choose availability: ";
                cin >> availabilityChoice;

                if (availabilityChoice == 1) {
                    item->isAvailable = true;
                    cout << "Item is now available.\n";
                } else if (availabilityChoice == 2) {
                    item->isAvailable = false;
                    cout << "Item is now unavailable.\n";
                } else {
                    cout << "Invalid option.\n";
                }
                break;
            }

            case 6:
                cout << "Finished editing menu item.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }

    } while (choice != 6);
}

void MenuManager::deleteMenuItem() {
    int itemId;

    cout << "\n========== DELETE MENU ITEM ==========\n";
    cout << "Enter menu item ID to delete: ";
    cin >> itemId;

    for (auto iterator = menuItems.begin();
         iterator != menuItems.end();
         ++iterator) {
        if (iterator->itemId == itemId) {
            cout << "Menu item \"" << iterator->name
                 << "\" deleted successfully.\n";

            menuItems.erase(iterator);
            return;
        }
    }

    cout << "Menu item not found.\n";
}

void MenuManager::loadMenu(const string& filename) {
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Could not open menu data file.\n";
        return;
    }

    menuItems.clear();

    string line;

    getline(inputFile, line);

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }

        size_t firstComma = line.find(',');
        size_t secondComma = line.find(',', firstComma + 1);
        size_t thirdComma = line.find(',', secondComma + 1);
        size_t fourthComma = line.find(',', thirdComma + 1);
        size_t fifthComma = line.find(',', fourthComma + 1);

        if (firstComma == string::npos ||
            secondComma == string::npos ||
            thirdComma == string::npos ||
            fourthComma == string::npos ||
            fifthComma == string::npos) {
            continue;
        }

        MenuItem item;

        item.itemId = stoi(line.substr(0, firstComma));

        item.name = line.substr(
            firstComma + 1,
            secondComma - firstComma - 1
        );

        item.description = line.substr(
            secondComma + 1,
            thirdComma - secondComma - 1
        );

        item.category = line.substr(
            thirdComma + 1,
            fourthComma - thirdComma - 1
        );

        item.price = stod(line.substr(
            fourthComma + 1,
            fifthComma - fourthComma - 1
        ));

        string availableText = line.substr(fifthComma + 1);

        item.isAvailable =
            availableText == "true" || availableText == "1";

        menuItems.push_back(item);
    }

    inputFile.close();
}

void MenuManager::saveMenu(const string& filename) const {
    ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        cout << "Could not save menu data.\n";
        return;
    }

    outputFile << "itemId,name,description,category,price,isAvailable\n";

    for (const MenuItem& item : menuItems) {
        outputFile << item.itemId << ","
                   << item.name << ","
                   << item.description << ","
                   << item.category << ","
                   << item.price << ","
                   << (item.isAvailable ? "true" : "false")
                   << "\n";
    }

    outputFile.close();

    cout << "Menu data saved successfully.\n";
}

MenuItem* MenuManager::getMenuItemById(int itemId) {
    return findMenuItemById(itemId);
}

const vector<MenuItem>& MenuManager::getMenuItems() const {
    return menuItems;
}