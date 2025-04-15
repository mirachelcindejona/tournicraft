#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <functional>
#include "nlohmann/json.hpp"
#include "auth/auth.h"

using namespace std;
using json = nlohmann::json;

// Referensi variabel dari main.cpp
extern bool isLoggedIn;
extern string currentUser;

// Path file untuk menyimpan data user
const string USERS_FILE = "data/operators.json";

// Fungsi untuk menghasilkan hash dari password
string hashPassword(const string& password) {
    hash<string> hasher;
    size_t hashValue = hasher(password);
    
    // Konversi hash ke string hexadecimal
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << hashValue;
    return ss.str();
}

json loadOperators() {
   ifstream inFile(USERS_FILE);
   if (!inFile) return json::array(); // jika file tidak ada, return array kosong
   json data;
   inFile >> data;
   return data;
}

void saveOperators(const json& data) {
   ofstream outFile(USERS_FILE);
   outFile << data.dump(4); // pretty print
}

bool usernameExists(const json& users, const string& username) {
   for (const auto& user : users) {
      if (user["username"] == username) return true;
   }
   return false;
}

int findUserIndex(const json& users, const string& username) {
   for (int i = 0; i < users.size(); ++i) {
      if (users[i]["username"] == username) return i;
   }
   return -1;
}

void registerOperator() {
   string username, password;
   cout << "\n=== REGISTRASI OPERATOR ===\n";
   cout << "Masukkan username: ";
   cin >> username;

   json users = loadOperators();
   if (usernameExists(users, username)) {
      cout << "Username sudah digunakan!\n";
      return;
   }

   cout << "Masukkan password: ";
   cin >> password;

   // Hash password sebelum disimpan
   string hashedPassword = hashPassword(password);

   json newUser = {
      {"username", username},
      {"password", hashedPassword}
   };
   users.push_back(newUser);
   saveOperators(users);

   cout << "Registrasi berhasil!\n";
}

bool loginOperator() {
   string username, password;
   cout << "\n=== LOGIN OPERATOR ===\n";
   cout << "Username: ";
   cin >> username;
   cout << "Password: ";
   cin >> password;

   json users = loadOperators();
   int idx = findUserIndex(users, username);

   // Hash password yang diinput dan bandingkan dengan hash yang tersimpan
   string hashedPassword = hashPassword(password);
   
   if (idx != -1 && users[idx]["password"] == hashedPassword) {
      currentUser = username;
      isLoggedIn = true;
      cout << "Login berhasil! Selamat datang, " << username << ".\n";
      return true;
   }

   cout << "Login gagal! Username atau password salah.\n";
   return false;
}
