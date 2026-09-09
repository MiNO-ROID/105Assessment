#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

#include "../include/Order.h"

using namespace std;

// ---------- Private helpers ----------

Order* OrderManager::findOrderById(int orderId) {
    for (auto& o : orders) {
        if (o.orderId == orderId) {
            return &o;
        }
    }
    return nullptr;
}

string OrderManager::statusToString(OrderStatus status) const {
    switch (status) {
        case OrderStatus::New:        return "New";
        case OrderStatus::Preparing:  return "Preparing";
        case OrderStatus::Ready:      return "Ready";
        case OrderStatus::Served:     return "Served";
        case OrderStatus::Paid:       return "Paid";
        case OrderStatus::Cancelled:  return "Cancelled";
        default:                      return "Unknown";
    }
}

OrderStatus OrderManager::numberToStatus(int choice) const {
    switch (choice) {
        case 1: return OrderStatus::New;
        case 2: return OrderStatus::Preparing;
        case 3: return OrderStatus::Ready;
        case 4: return OrderStatus::Served;
        case 5: return OrderStatus::Paid;
        case 6: return OrderStatus::Cancelled;
        default: return OrderStatus::New;
    }
}

double OrderManager::calculateOrderTotal(const Order& order) const {
    double total = 0.0;
    for (const auto& item : order.orderItems) {
        total += item.quantity * item.unitPrice;
    }
    return total;
}

void OrderManager::displayOrderDetails(const Order& order) const {
    cout << "Order ID: " << order.orderId
         << " | Table: " << order.tableNumber
         << " | " << order.orderDate << " " << order.orderTime
         << " | Status: " << statusToString(order.status) << endl;

    if (order.orderItems.empty()) {
        cout << "  (No items)" << endl;
        return;
    }

    double total = 0.0;
    for (const auto& item : order.orderItems) {
        double lineTotal = item.quantity * item.unitPrice;
        total += lineTotal;

        cout << "  - " << item.itemName
             << " x" << item.quantity
             << " @ $" << fixed << setprecision(2) << item.unitPrice
             << " = $" << lineTotal;

        if (!item.specialInstructions.empty()) {
            cout << " [" << item.specialInstructions << "]";
        }
        cout << endl;
    }

    cout << "  TOTAL: $" << total << endl;
}

// ---------- Public functions (stubbed for now) ----------

void OrderManager::addOrder(TableManager& tableManager,
                            const MenuManager& menuManager) {
    // TODO: implement next
}

void OrderManager::viewOrders() const {
    // TODO: implement next
}

void OrderManager::viewOrdersForTable() const {
    // TODO: implement next
}

void OrderManager::editOrder(const MenuManager& menuManager) {
    // TODO: implement next
}

void OrderManager::deleteOrder() {
    // TODO: implement next
}

void OrderManager::updateOrderStatus() {
    // TODO: implement next
}

void OrderManager::viewOrdersByStatus(OrderStatus status) const {
    // TODO: implement next
}

void OrderManager::findMostOrderedItem() const {
    // TODO: implement next
}

double OrderManager::calculateOverallIncome() const {
    // TODO: implement next
}

void OrderManager::generateRestaurantReport(const TableManager& tableManager) const {
    // TODO: implement next
}

void OrderManager::loadOrders(const string& ordersFilename,
                              const string& orderItemsFilename) {
    // TODO: implement next
}

void OrderManager::saveOrders(const string& ordersFilename,
                              const string& orderItemsFilename) const {
    // TODO: implement next
}