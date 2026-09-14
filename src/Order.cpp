#include "../include/Order.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Order* OrderManager::findOrderById(int orderId) {
    for (Order& order : orders) {
        if (order.orderId == orderId) {
            return &order;
        }
    }

    return nullptr;
}

string OrderManager::statusToString(OrderStatus status) const {
    switch (status) {
        case OrderStatus::New:
            return "New";
        case OrderStatus::Preparing:
            return "Preparing";
        case OrderStatus::Ready:
            return "Ready";
        case OrderStatus::Served:
            return "Served";
        case OrderStatus::Paid:
            return "Paid";
        case OrderStatus::Cancelled:
            return "Cancelled";
    }

    return "New";
}

OrderStatus OrderManager::numberToStatus(int choice) const {
    switch (choice) {
        case 1:
            return OrderStatus::New;
        case 2:
            return OrderStatus::Preparing;
        case 3:
            return OrderStatus::Ready;
        case 4:
            return OrderStatus::Served;
        case 5:
            return OrderStatus::Paid;
        case 6:
            return OrderStatus::Cancelled;
        default:
            return OrderStatus::New;
    }
}

double OrderManager::calculateOrderTotal(const Order& order) const {
    double total = 0.0;

    for (const OrderItem& item : order.orderItems) {
        total += item.quantity * item.unitPrice;
    }

    return total;
}

void OrderManager::displayOrderDetails(const Order& order) const {
    cout << "\nOrder ID: " << order.orderId << "\n";
    cout << "Table: " << order.tableNumber << "\n";
    cout << "Date: " << order.orderDate << "\n";
    cout << "Time: " << order.orderTime << "\n";
    cout << "Status: " << statusToString(order.status) << "\n";

    if (order.orderItems.empty()) {
        cout << "No items in this order.\n";
        return;
    }

    cout << "Items:\n";

    for (const OrderItem& item : order.orderItems) {
        double lineTotal = item.quantity * item.unitPrice;

        cout << "  " << item.itemName
             << " x" << item.quantity
             << " @ $" << fixed << setprecision(2) << item.unitPrice
             << " = $" << lineTotal << "\n";

        if (!item.specialInstructions.empty()) {
            cout << "  Instructions: "
                 << item.specialInstructions << "\n";
        }
    }

    cout << "Total: $"
         << fixed << setprecision(2)
         << calculateOrderTotal(order) << "\n";
}

void OrderManager::addOrder(TableManager& tableManager,
                            const MenuManager& menuManager) {
    if (orders.size() >= MAX_ORDERS) {
        cout << "The maximum number of orders has been reached.\n";
        return;
    }

    int tableNumber;

    cout << "\n========== ADD ORDER ==========\n";
    cout << "Enter table number: ";
    cin >> tableNumber;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid table number.\n";
        return;
    }

    if (!tableManager.isTableOccupied(tableNumber)) {
        cout << "The table does not exist or is not occupied.\n";
        return;
    }

    Order newOrder;
    newOrder.orderId = 1;

    for (const Order& order : orders) {
        if (order.orderId >= newOrder.orderId) {
            newOrder.orderId = order.orderId + 1;
        }
    }

    newOrder.tableNumber = tableNumber;
    newOrder.status = OrderStatus::New;

    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    char dateBuffer[20];
    char timeBuffer[20];

    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", localTime);
    strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", localTime);

    newOrder.orderDate = dateBuffer;
    newOrder.orderTime = timeBuffer;

    int itemCount;

    cout << "How many menu items would you like to add? ";
    cin >> itemCount;

    if (cin.fail() || itemCount <= 0 || itemCount > MAX_ORDER_ITEMS) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number of items.\n";
        return;
    }

    for (int i = 0; i < itemCount; i++) {
        int menuItemId;
        int quantity;

        cout << "\nAvailable menu items:\n";

        for (const MenuItem& menuItem : menuManager.getMenuItems()) {
            if (menuItem.isAvailable) {
                cout << menuItem.itemId
                     << ". " << menuItem.name
                     << " - $" << fixed << setprecision(2)
                     << menuItem.price << "\n";
            }
        }

        cout << "Enter menu item ID: ";
        cin >> menuItemId;

        MenuItem* selectedItem =
            const_cast<MenuManager&>(menuManager)
            .getMenuItemById(menuItemId);

        if (selectedItem == nullptr || !selectedItem->isAvailable) {
            cout << "That menu item is unavailable or does not exist.\n";
            i--;
            continue;
        }

        cout << "Enter quantity: ";
        cin >> quantity;

        if (cin.fail() || quantity <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Quantity must be greater than zero.\n";
            i--;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        OrderItem orderItem;
        orderItem.menuItemId = selectedItem->itemId;
        orderItem.itemName = selectedItem->name;
        orderItem.quantity = quantity;
        orderItem.unitPrice = selectedItem->price;

        cout << "Enter special instructions, or press Enter for none: ";
        getline(cin, orderItem.specialInstructions);

        newOrder.orderItems.push_back(orderItem);
    }

    orders.push_back(newOrder);

    cout << "\nOrder added successfully.\n";
    displayOrderDetails(newOrder);
}

void OrderManager::viewOrders() const {
    cout << "\n========== VIEW ORDERS ==========\n";

    if (orders.empty()) {
        cout << "No orders have been created.\n";
        return;
    }

    for (const Order& order : orders) {
        displayOrderDetails(order);
        cout << "------------------------------\n";
    }
}

void OrderManager::viewOrdersForTable() const {
    int tableNumber;
    bool found = false;

    cout << "\n========== VIEW ORDERS FOR TABLE ==========\n";
    cout << "Enter table number: ";
    cin >> tableNumber;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid table number.\n";
        return;
    }

    for (const Order& order : orders) {
        if (order.tableNumber == tableNumber) {
            displayOrderDetails(order);
            cout << "------------------------------\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No orders found for table "
             << tableNumber << ".\n";
    }
}

void OrderManager::editOrder(const MenuManager& menuManager) {
    int orderId;

    cout << "\n========== EDIT ORDER ==========\n";
    cout << "Enter order ID: ";
    cin >> orderId;

    Order* order = findOrderById(orderId);

    if (order == nullptr) {
        cout << "Order not found.\n";
        return;
    }

    if (order->status == OrderStatus::Paid ||
        order->status == OrderStatus::Cancelled) {
        cout << "Paid or cancelled orders cannot be edited.\n";
        return;
    }

    int choice;

    do {
        cout << "\nEditing order " << orderId << "\n";
        cout << "1. Add item\n";
        cout << "2. Change item quantity\n";
        cout << "3. Change special instructions\n";
        cout << "4. Finish editing\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            if (order->orderItems.size() >= MAX_ORDER_ITEMS) {
                cout << "This order has reached the item limit.\n";
                continue;
            }

            int menuItemId;
            int quantity;

            cout << "Enter menu item ID: ";
            cin >> menuItemId;

            MenuItem* selectedItem =
                const_cast<MenuManager&>(menuManager)
                .getMenuItemById(menuItemId);

            if (selectedItem == nullptr || !selectedItem->isAvailable) {
                cout << "That menu item is unavailable or does not exist.\n";
                continue;
            }

            cout << "Enter quantity: ";
            cin >> quantity;

            if (quantity <= 0) {
                cout << "Quantity must be greater than zero.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            OrderItem newItem;
            newItem.menuItemId = selectedItem->itemId;
            newItem.itemName = selectedItem->name;
            newItem.quantity = quantity;
            newItem.unitPrice = selectedItem->price;

            cout << "Enter special instructions: ";
            getline(cin, newItem.specialInstructions);

            order->orderItems.push_back(newItem);
            cout << "Item added.\n";
        } else if (choice == 2) {
            if (order->orderItems.empty()) {
                cout << "There are no items to edit.\n";
                continue;
            }

            for (size_t i = 0; i < order->orderItems.size(); i++) {
                cout << i + 1 << ". "
                     << order->orderItems[i].itemName
                     << " x" << order->orderItems[i].quantity << "\n";
            }

            int itemNumber;
            int quantity;

            cout << "Select item number: ";
            cin >> itemNumber;

            if (itemNumber < 1 ||
                itemNumber > static_cast<int>(order->orderItems.size())) {
                cout << "Invalid item number.\n";
                continue;
            }

            cout << "Enter new quantity: ";
            cin >> quantity;

            if (quantity <= 0) {
                cout << "Quantity must be greater than zero.\n";
                continue;
            }

            order->orderItems[itemNumber - 1].quantity = quantity;
            cout << "Quantity updated.\n";
        } else if (choice == 3) {
            if (order->orderItems.empty()) {
                cout << "There are no items to edit.\n";
                continue;
            }

            for (size_t i = 0; i < order->orderItems.size(); i++) {
                cout << i + 1 << ". "
                     << order->orderItems[i].itemName << "\n";
            }

            int itemNumber;

            cout << "Select item number: ";
            cin >> itemNumber;

            if (itemNumber < 1 ||
                itemNumber > static_cast<int>(order->orderItems.size())) {
                cout << "Invalid item number.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Enter new special instructions: ";
            getline(cin,
                    order->orderItems[itemNumber - 1]
                    .specialInstructions);

            cout << "Instructions updated.\n";
        } else if (choice != 4) {
            cout << "Invalid option.\n";
        }
    } while (choice != 4);

    cout << "Order editing finished.\n";
}

void OrderManager::deleteOrder() {
    int orderId;

    cout << "\n========== DELETE ORDER ==========\n";
    cout << "Enter order ID: ";
    cin >> orderId;

    for (auto iterator = orders.begin();
         iterator != orders.end();
         ++iterator) {
        if (iterator->orderId == orderId) {
            if (iterator->status != OrderStatus::Cancelled) {
                cout << "Only cancelled orders can be deleted.\n";
                return;
            }

            orders.erase(iterator);
            cout << "Order deleted successfully.\n";
            return;
        }
    }

    cout << "Order not found.\n";
}

void OrderManager::updateOrderStatus() {
    int orderId;

    cout << "\n========== UPDATE ORDER STATUS ==========\n";
    cout << "Enter order ID: ";
    cin >> orderId;

    Order* order = findOrderById(orderId);

    if (order == nullptr) {
        cout << "Order not found.\n";
        return;
    }

    cout << "1. New\n";
    cout << "2. Preparing\n";
    cout << "3. Ready\n";
    cout << "4. Served\n";
    cout << "5. Paid\n";
    cout << "6. Cancelled\n";

    int choice;
    cout << "Choose new status: ";
    cin >> choice;

    if (choice < 1 || choice > 6) {
        cout << "Invalid status.\n";
        return;
    }

    order->status = numberToStatus(choice);

    cout << "Order status updated to "
         << statusToString(order->status) << ".\n";
}

void OrderManager::viewOrdersByStatus(OrderStatus status) const {
    bool found = false;

    cout << "\n========== ORDERS BY STATUS ==========\n";

    for (const Order& order : orders) {
        if (order.status == status) {
            displayOrderDetails(order);
            cout << "------------------------------\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No orders found with status "
             << statusToString(status) << ".\n";
    }
}

void OrderManager::findMostOrderedItem() const {
    if (orders.empty()) {
        cout << "No orders available.\n";
        return;
    }

    vector<string> itemNames;
    vector<int> quantities;

    for (const Order& order : orders) {
        if (order.status == OrderStatus::Cancelled) {
            continue;
        }

        for (const OrderItem& item : order.orderItems) {
            int index = -1;

            for (size_t i = 0; i < itemNames.size(); i++) {
                if (itemNames[i] == item.itemName) {
                    index = static_cast<int>(i);
                    break;
                }
            }

            if (index == -1) {
                itemNames.push_back(item.itemName);
                quantities.push_back(item.quantity);
            } else {
                quantities[index] += item.quantity;
            }
        }
    }

    if (itemNames.empty()) {
        cout << "No ordered items available.\n";
        return;
    }

    int mostOrderedIndex = 0;

    for (size_t i = 1; i < quantities.size(); i++) {
        if (quantities[i] > quantities[mostOrderedIndex]) {
            mostOrderedIndex = static_cast<int>(i);
        }
    }

    cout << "Most ordered item: "
         << itemNames[mostOrderedIndex]
         << " (" << quantities[mostOrderedIndex]
         << " ordered).\n";
}

double OrderManager::calculateOverallIncome() const {
    double income = 0.0;

    for (const Order& order : orders) {
        if (order.status != OrderStatus::Cancelled) {
            income += calculateOrderTotal(order);
        }
    }

    return income;
}

void OrderManager::generateRestaurantReport(
    const TableManager& tableManager) const {
    cout << "\n========== RESTAURANT REPORT ==========\n";
    cout << "Total tables: "
         << tableManager.getTableCount() << "\n";
    cout << "Occupied tables: "
         << tableManager.getOccupiedTableCount() << "\n";
    cout << "Total orders: "
         << orders.size() << "\n";
    cout << "Overall income: $"
         << fixed << setprecision(2)
         << calculateOverallIncome() << "\n";

    findMostOrderedItem();
}

void OrderManager::loadOrders(const string& ordersFilename,
                              const string& orderItemsFilename) {
    orders.clear();

    ifstream orderFile(ordersFilename);

    if (orderFile.is_open()) {
        string line;
        getline(orderFile, line);

        while (getline(orderFile, line)) {
            if (line.empty()) {
                continue;
            }

            stringstream stream(line);
            string orderIdText;
            string tableNumberText;
            string date;
            string time;
            string status;

            getline(stream, orderIdText, ',');
            getline(stream, tableNumberText, ',');
            getline(stream, date, ',');
            getline(stream, time, ',');
            getline(stream, status, ',');

            if (orderIdText.empty() ||
                tableNumberText.empty()) {
                continue;
            }

            Order order;
            order.orderId = stoi(orderIdText);
            order.tableNumber = stoi(tableNumberText);
            order.orderDate = date;
            order.orderTime = time;
            order.status = OrderStatus::New;

            if (status == "Preparing") {
                order.status = OrderStatus::Preparing;
            } else if (status == "Ready") {
                order.status = OrderStatus::Ready;
            } else if (status == "Served") {
                order.status = OrderStatus::Served;
            } else if (status == "Paid") {
                order.status = OrderStatus::Paid;
            } else if (status == "Cancelled") {
                order.status = OrderStatus::Cancelled;
            }

            orders.push_back(order);
        }

        orderFile.close();
    }

    ifstream itemFile(orderItemsFilename);

    if (itemFile.is_open()) {
        string line;
        getline(itemFile, line);

        while (getline(itemFile, line)) {
            if (line.empty()) {
                continue;
            }

            stringstream stream(line);

            string orderIdText;
            string menuItemIdText;
            string itemName;
            string quantityText;
            string priceText;
            string instructions;

            getline(stream, orderIdText, ',');
            getline(stream, menuItemIdText, ',');
            getline(stream, itemName, ',');
            getline(stream, quantityText, ',');
            getline(stream, priceText, ',');
            getline(stream, instructions);

            if (orderIdText.empty()) {
                continue;
            }

            OrderItem item;
            item.menuItemId = stoi(menuItemIdText);
            item.itemName = itemName;
            item.quantity = stoi(quantityText);
            item.unitPrice = stod(priceText);
            item.specialInstructions = instructions;

            for (Order& order : orders) {
                if (order.orderId == stoi(orderIdText)) {
                    order.orderItems.push_back(item);
                    break;
                }
            }
        }

        itemFile.close();
    }
}

void OrderManager::saveOrders(const string& ordersFilename,
                              const string& orderItemsFilename) const {
    ofstream orderFile(ordersFilename);

    if (!orderFile.is_open()) {
        cout << "Could not save orders.\n";
        return;
    }

    orderFile << "orderId,tableNumber,orderDate,orderTime,status\n";

    for (const Order& order : orders) {
        orderFile << order.orderId << ","
                  << order.tableNumber << ","
                  << order.orderDate << ","
                  << order.orderTime << ","
                  << statusToString(order.status) << "\n";
    }

    orderFile.close();

    ofstream itemFile(orderItemsFilename);

    if (!itemFile.is_open()) {
        cout << "Could not save order items.\n";
        return;
    }

    itemFile << "orderId,menuItemId,itemName,quantity,unitPrice,"
             << "specialInstructions\n";

    for (const Order& order : orders) {
        for (const OrderItem& item : order.orderItems) {
            itemFile << order.orderId << ","
                     << item.menuItemId << ","
                     << item.itemName << ","
                     << item.quantity << ","
                     << item.unitPrice << ","
                     << item.specialInstructions << "\n";
        }
    }

    itemFile.close();

    cout << "Orders saved successfully.\n";
}