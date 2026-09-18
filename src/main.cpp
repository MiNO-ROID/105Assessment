#include <iostream>

#include "../include/Table.h"
#include "../include/Menu.h"
#include "../include/Order.h"

using namespace std;

const string TABLES_FILE = "../data/tables.csv";
const string MENU_FILE = "../data/menu.csv";
const string ORDERS_FILE = "../data/orders.csv";
const string ORDER_ITEMS_FILE = "../data/orderItems.csv";

void displayMainMenu() {
    cout << "\n========================================\n";
    cout << " EATS & TREATS RESTAURANT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Table Management\n";
    cout << "2. Menu Management\n";
    cout << "3. Order Management\n";
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

void displayOrderMenu() {
    cout << "\n========== ORDER MANAGEMENT ==========\n";
    cout << "1. Add Order\n";
    cout << "2. View All Orders\n";
    cout << "3. View Orders for a Table\n";
    cout << "4. Edit Order\n";
    cout << "5. Delete Cancelled Order\n";
    cout << "6. Update Order Status\n";
    cout << "7. View New Orders\n";
    cout << "8. View Preparing Orders\n";
    cout << "9. View Ready Orders\n";
    cout << "10. Find Most Ordered Item\n";
    cout << "11. Calculate Overall Income\n";
    cout << "12. Generate Restaurant Report\n";
    cout << "13. Back to Main Menu\n";
    cout << "Choose an option: ";
}

void runTableMenu(TableManager &tableManager) {
    int choice;

    do {
        displayTableMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                tableManager.addTable();
                tableManager.saveTables(TABLES_FILE);
                break;

            case 2:
                tableManager.viewTables();
                break;

            case 3:
                tableManager.editTable();
                tableManager.saveTables(TABLES_FILE);
                break;

            case 4:
                tableManager.deleteTable();
                tableManager.saveTables(TABLES_FILE);
                break;

            case 5:
                tableManager.assignTable();
                tableManager.saveTables(TABLES_FILE);
                break;

            case 6:
                tableManager.freeTable();
                tableManager.saveTables(TABLES_FILE);
                break;

            case 7:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 7);
}

void runMenuManagementMenu(MenuManager &menuManager) {
    int choice;

    do {
        displayMenuManagementMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                menuManager.addMenuItem();
                menuManager.saveMenu(MENU_FILE);
                break;

            case 2:
                menuManager.viewMenuItems();
                break;

            case 3:
                menuManager.editMenuItem();
                menuManager.saveMenu(MENU_FILE);
                break;

            case 4:
                menuManager.deleteMenuItem();
                menuManager.saveMenu(MENU_FILE);
                break;

            case 5:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 5);
}

void runOrderManagementMenu(OrderManager &orderManager,
                            TableManager &tableManager,
                            const MenuManager &menuManager) {
    int choice;

    do {
        displayOrderMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                orderManager.addOrder(tableManager, menuManager);
                orderManager.saveOrders(
                    ORDERS_FILE,
                    ORDER_ITEMS_FILE
                );
                break;

            case 2:
                orderManager.viewOrders();
                break;

            case 3:
                orderManager.viewOrdersForTable();
                break;

            case 4:
                orderManager.editOrder(menuManager);
                orderManager.saveOrders(
                    ORDERS_FILE,
                    ORDER_ITEMS_FILE
                );
                break;

            case 5:
                orderManager.deleteOrder();
                orderManager.saveOrders(
                    ORDERS_FILE,
                    ORDER_ITEMS_FILE
                );
                break;

            case 6:
                orderManager.updateOrderStatus();
                orderManager.saveOrders(
                    ORDERS_FILE,
                    ORDER_ITEMS_FILE
                );
                break;

            case 7:
                orderManager.viewOrdersByStatus(OrderStatus::New);
                break;

            case 8:
                orderManager.viewOrdersByStatus(
                    OrderStatus::Preparing
                );
                break;

            case 9:
                orderManager.viewOrdersByStatus(OrderStatus::Ready);
                break;

            case 10:
                orderManager.findMostOrderedItem();
                break;

            case 11:
                cout << "\nOverall income: $"
                        << orderManager.calculateOverallIncome()
                        << "\n";
                break;

            case 12:
                orderManager.generateRestaurantReport(tableManager);
                break;

            case 13:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 13);
}

int main() {
    TableManager tableManager;
    MenuManager menuManager;
    OrderManager orderManager;

    tableManager.loadTables(TABLES_FILE);
    menuManager.loadMenu(MENU_FILE);
    orderManager.loadOrders(
        ORDERS_FILE,
        ORDER_ITEMS_FILE
    );

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
                runOrderManagementMenu(
                    orderManager,
                    tableManager,
                    menuManager
                );
                break;

            case 4:
                tableManager.saveTables(TABLES_FILE);
                menuManager.saveMenu(MENU_FILE);
                orderManager.saveOrders(
                    ORDERS_FILE,
                    ORDER_ITEMS_FILE
                );

                cout << "All data saved. Goodbye.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 4);

    return 0;
}