#ifndef INC_105ASSESSMENT_MENU_H
#define INC_105ASSESSMENT_MENU_H

#include <string>
#include <vector>
#include "manager.h"

using namespace std;

const int MAX_MENU_ITEMS = 50;
const int MAX_MENU_PRICE = 1000;

struct MenuItem {
    int itemId;
    string name;
    string description;
    string category;
    double price;
    bool isAvailable;
};

class MenuManager : public manager {
private:
    vector<MenuItem> menuItems;

    MenuItem* findMenuItemById(int itemId);

public:
    void addMenuItem();

    void viewMenuItems() const;

    void editMenuItem();

    void deleteMenuItem();

    void loadMenu(const string& filename);

    void saveMenu(const string& filename) const;
    
    void displayData() const override;

    void loadData(const string& filename) override;

    void saveData(const string& filename) const override;

    MenuItem* getMenuItemById(int itemId);

    const vector<MenuItem>& getMenuItems() const;
};

#endif // INC_105ASSESSMENT_MENU_H