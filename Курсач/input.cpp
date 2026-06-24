#include "input.h"
#include <iostream>
#include <string>

using namespace std;

double inp_pos_dob() {

    double value;

    while (!(cin >> value) || value <= 0) {

        cout << "Некорректный ввод. Повторите: ";

        cin.clear();
        cin.ignore(10000, '\n');
    }

    return value;
}

double inp_dob() {

    double value;

    while (!(cin >> value)) {

        cout << "Некорректный ввод. Повторите: ";

        cin.clear();
        cin.ignore(10000, '\n');
    }

    return value;
}

int inp_int() {

    string input;
    int value;

    while (true) {

        cin >> input;

        try {

            if (!input.empty() &&
                input.find_first_not_of("-0123456789") == string::npos) {

                value = stoi(input);

                return value;
            }
        }

        catch (...) {
        }

        cout << "Некорректный ввод\n";
    }
}

int inp_pos_int() {

    string input;
    int value;

    while (true) {

        cin >> input;

        try {

            if (!input.empty() &&
                input.find_first_not_of("0123456789") == string::npos) {

                value = stoi(input);

                if (value > 0)
                    return value;
            }
        }

        catch (...) {
        }

        cout << "Некорректный ввод\n";
    }
}