#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <limits>
#include "input.h"
#include <windows.h>

using namespace std;

//====================================================

struct User {

    string login;
    string password;
    string role;
};

struct Device {

    int id;
    string type;
    string model;
    string description;
};

struct Order {

    int id;
    int device_id;
    string status;
    double price;
    string date_created;
    string deadline;
};

//====================================================

string loginSystem();

void adminMenu();
void userMenu();

void addUser();
void deleteUser();
void showUsers();
void sortUsers();
void filterUsers();

void addOrder();
void showOrders();
void deleteOrder();
void editOrder();

void sortOrders();
void filterOrders();

void showDevices();

Device findDeviceById(int id);

void revenueReport();

void searchOrder();

//====================================================

int main() {

    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    while (true) {

        string role = loginSystem();

        if (role == "exit") {

            cout << "Выход из программы\n";
            return 0;
        }

        else if (role == "admin") {

            adminMenu();
        }

        else if (role == "user") {

            userMenu();
        }

        else {

            cout << "Ошибка входа\n";
        }
    }

    return 0;
}

//====================================================

Device findDeviceById(int id) {

    ifstream file("devices.txt");

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        Device d;

        string tempId;

        getline(ss, tempId, '|');
        getline(ss, d.type, '|');
        getline(ss, d.model, '|');
        getline(ss, d.description);

        d.id = stoi(tempId);

        if (d.id == id) {

            return d;
        }
    }

    return { -1, "", "", "" };
}

//====================================================

void addOrder() {

    ofstream orderFile("orders.txt", ios::app);
    ofstream deviceFile("devices.txt", ios::app);

    Order o;
    Device d;

    cout << "Введите ID заказа: ";
    o.id = inp_pos_int();

    cout << "Введите ID устройства: ";
    o.device_id = inp_pos_int();

    Device existing = findDeviceById(o.device_id);

    if (existing.id == -1) {

        cout << "Устройство не найдено. Создание устройства...\n";

        d.id = o.device_id;

        cin.ignore(10000, '\n');

        cout << "Тип устройства: ";
        getline(cin, d.type);

        cout << "Модель устройства: ";
        getline(cin, d.model);

        cout << "Описание устройства: ";
        getline(cin, d.description);

        deviceFile << d.id << "|"
            << d.type << "|"
            << d.model << "|"
            << d.description << endl;

        cout << "Устройство добавлено\n";
    }

    else {

        cout << "Устройство найдено: "
            << existing.type
            << " | "
            << existing.model
            << endl;
    }

    cout << "Введите стоимость: ";
    o.price = inp_pos_dob();

    cout << "Введите дату создания (YYYY-MM-DD): ";
    cin >> o.date_created;

    cout << "Введите срок сдачи (YYYY-MM-DD): ";
    cin >> o.deadline;

    o.status = "принят";

    orderFile << o.id << " "
        << o.device_id << " "
        << o.status << " "
        << o.price << " "
        << o.date_created << " "
        << o.deadline << endl;

    cout << "Заказ успешно создан\n";
}

//====================================================

void showOrders() {

    ifstream file("orders.txt");

    Order o;

    while (file >> o.id
        >> o.device_id
        >> o.status
        >> o.price
        >> o.date_created
        >> o.deadline) {

        Device d = findDeviceById(o.device_id);

        cout << "\n====================================\n";

        cout << "ID заказа: " << o.id << endl;

        if (d.id != -1) {

            cout << "ID устройства: " << d.id << endl;
            cout << "Тип: " << d.type << endl;
            cout << "Модель: " << d.model << endl;
            cout << "Описание: " << d.description << endl;
        }

        cout << "Статус: " << o.status << endl;
        cout << "Стоимость: " << o.price << endl;
        cout << "Дата создания: " << o.date_created << endl;
        cout << "Срок сдачи: " << o.deadline << endl;
    }
}

//====================================================

void sortOrders() {

    Order orders[100];
    int count = 0;

    ifstream file("orders.txt");

    while (file >> orders[count].id
        >> orders[count].device_id
        >> orders[count].status
        >> orders[count].price
        >> orders[count].date_created
        >> orders[count].deadline) {

        count++;
    }

    cout << "1 - Сортировка по ID\n";
    cout << "2 - Сортировка по стоимости\n";
    cout << "3 - Сортировка по сроку сдачи\n";

    int ch = inp_int();

    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            bool condition = false;

            if (ch == 1) {

                condition = orders[j].id > orders[j + 1].id;
            }

            else if (ch == 2) {

                condition = orders[j].price > orders[j + 1].price;
            }

            else if (ch == 3) {

                condition = orders[j].deadline > orders[j + 1].deadline;
            }

            if (condition) {

                Order temp = orders[j];
                orders[j] = orders[j + 1];
                orders[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {

        cout << "ID: " << orders[i].id
            << " | Цена: " << orders[i].price
            << " | Срок: " << orders[i].deadline
            << endl;
    }
}

//====================================================

void filterOrders() {

    Order orders[100];
    int count = 0;

    ifstream file("orders.txt");

    while (file >> orders[count].id
        >> orders[count].device_id
        >> orders[count].status
        >> orders[count].price
        >> orders[count].date_created
        >> orders[count].deadline) {

        count++;
    }

    cout << "1 - Фильтр по статусу\n";
    cout << "2 - Фильтр по типу устройства\n";

    int ch = inp_int();

    if (ch == 1) {

        string status;

        cout << "Введите статус: ";
        cin >> status;

        for (int i = 0; i < count; i++) {

            if (orders[i].status == status) {

                cout << "ID заказа: "
                    << orders[i].id
                    << endl;
            }
        }
    }

    else if (ch == 2) {

        string type;

        cin.ignore();

        cout << "Введите тип устройства: ";
        getline(cin, type);

        for (int i = 0; i < count; i++) {

            Device d = findDeviceById(orders[i].device_id);

            if (d.type == type) {

                cout << "ID заказа: "
                    << orders[i].id
                    << " | "
                    << d.type
                    << endl;
            }
        }
    }
}

//====================================================

void deleteOrder() {

    int id;

    cout << "Введите ID заказа: ";
    cin >> id;

    ifstream file("orders.txt");
    ofstream temp("buf.txt");

    Order o;

    bool found = false;

    while (file >> o.id
        >> o.device_id
        >> o.status
        >> o.price
        >> o.date_created
        >> o.deadline) {

        if (o.id != id) {

            temp << o.id << " "
                << o.device_id << " "
                << o.status << " "
                << o.price << " "
                << o.date_created << " "
                << o.deadline << endl;
        }

        else {

            found = true;
        }
    }

    file.close();
    temp.close();

    remove("orders.txt");
    rename("buf.txt", "orders.txt");

    if (found)
        cout << "Заказ удалён\n";

    else
        cout << "Заказ не найден\n";
}

//====================================================

void editOrder() {

    int id;

    cout << "Введите ID заказа: ";
    cin >> id;

    ifstream file("orders.txt");
    ofstream temp("buf.txt");

    Order o;

    bool found = false;

    while (file >> o.id
        >> o.device_id
        >> o.status
        >> o.price
        >> o.date_created
        >> o.deadline) {

        if (o.id == id) {

            found = true;

            cout << "Введите новый статус: ";
            cin >> o.status;

            cout << "Введите новую стоимость: ";
            o.price = inp_pos_dob();

        }

        temp << o.id << " "
            << o.device_id << " "
            << o.status << " "
            << o.price << " "
            << o.date_created << " "
            << o.deadline << endl;
    }

    file.close();
    temp.close();

    remove("orders.txt");
    rename("buf.txt", "orders.txt");

    if (found)
        cout << "Заказ обновлён\n";

    else
        cout << "Заказ не найден\n";
}

//====================================================

void searchOrder() {

    ifstream file("orders.txt");

    cout << "\n========== ПОИСК ЗАКАЗА ==========\n";

    cout << "1 - Поиск по ID заказа\n";
    cout << "2 - Поиск по статусу\n";

    int ch = inp_int();

    Order o;

    bool found = false;

    if (ch == 1) {

        int id;

        cout << "Введите ID заказа: ";
        id = inp_pos_int();

        while (file >> o.id
            >> o.device_id
            >> o.status
            >> o.price
            >> o.date_created
            >> o.deadline) {

            if (o.id == id) {

                Device d = findDeviceById(o.device_id);

                cout << "\n========== ЗАКАЗ НАЙДЕН ==========\n";

                cout << "ID заказа: " << o.id << endl;
                cout << "Статус: " << o.status << endl;
                cout << "Стоимость: " << o.price << endl;
                cout << "Дата создания: " << o.date_created << endl;
                cout << "Срок сдачи: " << o.deadline << endl;

                if (d.id != -1) {

                    cout << "Устройство: "
                        << d.type
                        << " | "
                        << d.model
                        << endl;
                }

                found = true;
            }
        }
    }

    else if (ch == 2) {

        string status;

        cout << "Введите статус: ";
        cin >> status;

        while (file >> o.id
            >> o.device_id
            >> o.status
            >> o.price
            >> o.date_created
            >> o.deadline) {

            if (o.status == status) {

                cout << "ID заказа: "
                    << o.id
                    << " | Цена: "
                    << o.price
                    << " | Срок: "
                    << o.deadline
                    << endl;

                found = true;
            }
        }
    }

    if (!found) {

        cout << "Заказы не найдены\n";
    }
}

//====================================================

void showDevices() {

    ifstream file("devices.txt");

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        Device d;

        string tempId;

        getline(ss, tempId, '|');
        getline(ss, d.type, '|');
        getline(ss, d.model, '|');
        getline(ss, d.description);

        d.id = stoi(tempId);

        cout << "----------------------------------\n";

        cout << "ID: " << d.id << endl;
        cout << "Тип: " << d.type << endl;
        cout << "Модель: " << d.model << endl;
        cout << "Описание: " << d.description << endl;
    }
}

//====================================================

void revenueReport() {

    ifstream file("orders.txt");

    string startDate;
    string endDate;

    cout << "\n========== ОТЧЁТ ПО ПРИБЫЛИ ==========\n";

    cout << "Введите начальную дату: ";
    cin >> startDate;

    cout << "Введите конечную дату: ";
    cin >> endDate;

    Order o;

    double totalRevenue = 0;
    double maxPrice = 0;
    int count = 0;

    Order expensiveOrder;

    while (file >> o.id
        >> o.device_id
        >> o.status
        >> o.price
        >> o.date_created
        >> o.deadline) {

        if (o.date_created >= startDate &&
            o.date_created <= endDate) {

            totalRevenue += o.price;
            count++;

            if (o.price > maxPrice) {

                maxPrice = o.price;
                expensiveOrder = o;
            }
        }
    }

    cout << "\n====================================\n";
    cout << "Период отчёта: "
        << startDate
        << " - "
        << endDate
        << endl;

    cout << "Количество заказов: "
        << count
        << endl;

    cout << "Общая прибыль: "
        << totalRevenue
        << endl;

    if (count > 0) {

        cout << "Средняя стоимость заказа: "
            << totalRevenue / count
            << endl;

        Device d = findDeviceById(expensiveOrder.device_id);

        cout << "\n========== САМЫЙ ДОРОГОЙ ЗАКАЗ ==========\n";

        cout << "ID заказа: "
            << expensiveOrder.id
            << endl;

        cout << "Стоимость: "
            << expensiveOrder.price
            << endl;

        cout << "Статус: "
            << expensiveOrder.status
            << endl;

        cout << "Дата создания: "
            << expensiveOrder.date_created
            << endl;

        cout << "Срок сдачи: "
            << expensiveOrder.deadline
            << endl;

        if (d.id != -1) {

            cout << "Устройство: "
                << d.type
                << " | "
                << d.model
                << endl;
        }
    }

    else {

        cout << "Заказы за данный период отсутствуют\n";
    }

    cout << "====================================\n";
}

//====================================================

void showUsers() {

    ifstream file("login.txt");

    User u;

    while (file >> u.login
        >> u.password
        >> u.role) {

        cout << "Логин: "
            << u.login
            << " | Роль: "
            << u.role
            << endl;
    }
}

//====================================================

void sortUsers() {

    User users[100];
    int count = 0;

    ifstream file("login.txt");

    while (file >> users[count].login
        >> users[count].password
        >> users[count].role) {

        count++;
    }

    cout << "1 - Сортировка по логину\n";
    cout << "2 - Сортировка по роли\n";

    int ch = inp_int();

    for (int i = 0; i < count - 1; i++) {

        for (int j = 0; j < count - i - 1; j++) {

            bool condition = false;

            if (ch == 1) {

                condition = users[j].login > users[j + 1].login;
            }

            else if (ch == 2) {

                condition = users[j].role > users[j + 1].role;
            }

            if (condition) {

                User temp = users[j];
                users[j] = users[j + 1];
                users[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {

        cout << users[i].login
            << " | "
            << users[i].role
            << endl;
    }
}

//====================================================

void filterUsers() {

    string role;

    cout << "Введите роль: ";
    cin >> role;

    ifstream file("login.txt");

    User u;

    while (file >> u.login
        >> u.password
        >> u.role) {

        if (u.role == role) {

            cout << u.login
                << " | "
                << u.role
                << endl;
        }
    }
}

//====================================================

void userMenu() {

    while (true) {

        cout << "\n========== МЕНЮ ПОЛЬЗОВАТЕЛЯ ==========\n";

        cout << "1 - Показать заказы\n";
        cout << "2 - Сортировка заказов\n";
        cout << "3 - Фильтрация заказов\n";
        cout << "4 - Показать устройства\n";
        cout << "5 - Отчёт по прибыли\n";
        cout << "6 - Поиск заказа\n";
        cout << "0 - Выход\n";

        int ch = inp_int();

        switch (ch) {

        case 1:
            showOrders();
            break;

        case 2:
            sortOrders();
            break;

        case 3:
            filterOrders();
            break;

        case 4:
            showDevices();
            break;

        case 5:
            revenueReport();
            break;
        case 6:
            searchOrder();
            break;
        case 0:
            return;

        default:
            cout << "Неверный выбор\n";
        }
    }
}

//====================================================

void adminMenu() {

    while (true) {

        cout << "\n========== МЕНЮ АДМИНИСТРАТОРА ==========\n";

        cout << "1 - Добавить пользователя\n";
        cout << "2 - Удалить пользователя\n";
        cout << "3 - Показать пользователей\n";
        cout << "4 - Сортировка пользователей\n";
        cout << "5 - Фильтрация пользователей\n";
        cout << "6 - Добавить заказ\n";
        cout << "7 - Редактировать заказ\n";
        cout << "8 - Удалить заказ\n";
        cout << "9 - Поиск заказа\n";
        cout << "10 - Показать заказы\n";
        cout << "11 - Отчёт по прибыли\n";
        cout << "0 - Выход\n";

        int ch = inp_int();

        switch (ch) {

        case 1:
            addUser();
            break;

        case 2:
            deleteUser();
            break;

        case 3:
            showUsers();
            break;

        case 4:
            sortUsers();
            break;

        case 5:
            filterUsers();
            break;

        case 6:
            addOrder();
            break;

        case 7:
            editOrder();
            break;

        case 8:
            deleteOrder();
            break;

        case 9:
            searchOrder();
            break;

        case 10:
            showOrders();
            break;

        case 11:
            revenueReport();
            break;

        case 0:
            return;

        default:
            cout << "Неверный выбор\n";
        }
    }
}

//====================================================

string loginSystem() {

    string login;
    string password;

    cout << "====================================\n";
    cout << "Логин: ";
    cin >> login;

    if (login == "exit") {

        return "exit";
    }

    ifstream file("login.txt");

    string l, p, r;

    while (file >> l >> p >> r) {

        if (l == login) {

            cout << "Пароль: ";
            cin >> password;

            if (p == password) {

                cout << "Успешный вход\n";
                return r;
            }

            else {

                cout << "Неверный пароль\n";
                return "none";
            }
        }
    }

    cout << "Пользователь не найден\n";

    return "none";
}

//====================================================

void addUser() {

    string login;
    string password;
    string role;

    cout << "Введите логин: ";
    cin >> login;

    ifstream file("login.txt");

    string l, p, r;

    while (file >> l >> p >> r) {

        if (l == login) {

            cout << "Пользователь уже существует\n";
            return;
        }
    }

    file.close();

    cout << "Введите пароль: ";
    cin >> password;

    cout << "Введите роль: ";
    cin >> role;

    ofstream out("login.txt", ios::app);

    out << login << " "
        << password << " "
        << role << endl;

    cout << "Пользователь добавлен\n";
}

//====================================================

void deleteUser() {

    string login;

    cout << "Введите логин: ";
    cin >> login;

    ifstream file("login.txt");
    ofstream temp("buf.txt");

    string l, p, r;

    bool found = false;

    while (file >> l >> p >> r) {

        if (l != login) {

            temp << l << " "
                << p << " "
                << r << endl;
        }

        else {

            found = true;
        }
    }

    file.close();
    temp.close();

    remove("login.txt");
    rename("buf.txt", "login.txt");

    if (found)
        cout << "Пользователь удалён\n";

    else
        cout << "Пользователь не найден\n";
}