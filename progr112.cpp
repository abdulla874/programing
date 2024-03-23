#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int SHIFT = 3;
const string USERS_FILE = "users.txt";
const string PASSWORDS_FILE = "passwords.txt";

void caesarCipherEncrypt(string& text) {
    for (char& c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + SHIFT) % 26 + base;
        }
    }
}

void caesarCipherDecrypt(string& text) {
    for (char& c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base - SHIFT + 26) % 26 + base;
        }
    }
}

void registerUser() {
    string username, password;
    cout << "Register new user\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    caesarCipherEncrypt(password);

    ofstream usersFile(USERS_FILE, ios::app);
    if (usersFile.is_open()) {
        usersFile << username << " " << password << endl;
        usersFile.close();
    }
    else {
        cout << "Unable to open the file\n";
    }
}

bool loginUser(string& currentUser) {
    string username, password, fileUsername, filePassword;
    cout << "Login\nUsername: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    caesarCipherEncrypt(password);

    ifstream usersFile(USERS_FILE);
    if (usersFile.is_open()) {
        while (usersFile >> fileUsername >> filePassword) {
            if (username == fileUsername && password == filePassword) {
                currentUser = username;
                usersFile.close();
                return true; // Login successful
            }
        }
        usersFile.close();
    }
    return false; // Login failed
}

void addPassword(const string& username) {
    string service, password;
    cout << "Enter the service name: ";
    cin >> service;
    cout << "Enter the password: ";
    cin >> password;

    caesarCipherEncrypt(password);

    ofstream passwordsFile(PASSWORDS_FILE, ios::app);
    if (passwordsFile.is_open()) {
        passwordsFile << username << " " << service << " " << password << endl;
        passwordsFile.close();
    }
    else {
        cout << "Unable to open the file\n";
    }
}

void viewPasswords(const string& username) {
    ifstream passwordsFile(PASSWORDS_FILE);
    string fileUsername, service, password;
    if (passwordsFile.is_open()) {
        cout << "Service\tPassword\n";
        while (passwordsFile >> fileUsername >> service >> password) {
            if (username == fileUsername) {
                caesarCipherDecrypt(password);
                cout << service << "\t" << password << endl;
            }
        }
        passwordsFile.close();
    }
    else {
        cout << "Unable to open the file\n";
    }
}

int main() {
    int choice;
    string currentUser;
    bool loggedIn = false;

    do {
        cout << "1. Register\n2. Login\n3. Add Password\n4. View Passwords\n5. Exit\nChoose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            loggedIn = loginUser(currentUser);
            if (loggedIn) {
                cout << "Login successful! Welcome, " << currentUser << endl;
            }
            else {
                cout << "Login failed. Please try again.\n";
            }
            break;
        case 3:
            if (loggedIn) {
                addPassword(currentUser);
            }
            else {
                cout << "Please login first.\n";
            }
            break;
        case 4:
            if (loggedIn) {
                viewPasswords(currentUser);
            }
            else {
                cout << "Please login first.\n";
            }
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}