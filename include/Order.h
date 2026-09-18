//
// Created by GGPC on 2/09/2026.
//

#ifndef INC_105ASSESSMENT_ORDER_H
#define INC_105ASSESSMENT_ORDER_H

#include <string>
#include <vector>
#include "Table.h"
#include "Menu.h"
#include "manager.h"

using namespace std;

const int MAX_ORDER_ITEMS = 20;
const int MAX_ORDERS = 100;

enum class OrderStatus {
    New,
    Preparing,
    Ready,
    Served,
    Paid,
    Cancelled
};

struct OrderItem {
    int menuItemId;
    string itemName;
    int quantity;
    double unitPrice;
    string specialInstructions;
};

struct Order {
    int orderId;
    int tableNumber;
    string orderDate;
    string orderTime;
    OrderStatus status;
    vector<OrderItem> orderItems;
};

class OrderManager : public manager {
private:
    vector<Order> orders;

    Order* findOrderById(int orderId);

    string statusToString(OrderStatus status) const;

    OrderStatus numberToStatus(int choice) const;

    double calculateOrderTotal(const Order& order) const;

    void displayOrderDetails(const Order& order) const;

public:
    void addOrder(TableManager& tableManager,
                  const MenuManager& menuManager);

    void viewOrders() const;

    void viewOrdersForTable() const;

    void editOrder(const MenuManager& menuManager);

    void deleteOrder();

    void updateOrderStatus();

    void viewOrdersByStatus(OrderStatus status) const;

    void findMostOrderedItem() const;

    double calculateOverallIncome() const;

    void generateRestaurantReport(
        const TableManager& tableManager
    ) const;

    void loadOrders(const string& ordersFilename,
                    const string& orderItemsFilename);

    void saveOrders(const string& ordersFilename,
                    const string& orderItemsFilename) const;

    void displayData() const override;

    void loadData(const string& filename) override;

    void saveData(const string& filename) const override;
};

#endif // INC_105ASSESSMENT_ORDER_H