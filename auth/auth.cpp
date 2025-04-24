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

// Fungsi untuk validasi username hanya mengizinkan huruf, angka, dan underscore
bool isValidUsername(const string& username) {
   for (char c : username) {
      if (!isalnum(c) && c != '_') {
         return false;
      }
   }
   return true;
}

// Fungsi untuk validasi password harus mengandung setidaknya satu karakter alfanumerik
bool isValidPassword(const string& password) {
   for (char c : password) {
      if (isalnum(c)) {
         return true;
      }
   }
   return false;
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

   if (username.empty() || username.find_first_not_of(' ') == string::npos) {
      cout << "Username tidak boleh kosong!\n";
      return;
   }

   if (username.length() < 3) {
      cout << "Username harus minimal 3 karakter!\n";
      return;
   }

   if (!isValidUsername(username)) {
      cout << "Username tidak boleh mengandung simbol!\n";
      return;
   }

   json users = loadOperators();
   if (usernameExists(users, username)) {
      cout << "Username sudah digunakan!\n";
      return;
   }

   cout << "Masukkan password: ";
   cin >> password;

   if (password.length() < 6) {
      cout << "Password harus minimal 6 karakter!\n";
      return;
   }

   if (!isValidPassword(password)) {
      cout << "Password harus mengandung setidaknya satu karakter alfanumerik!\n";
      return;
   }

   string hashedPassword = hashPassword(password);

   json newUser = {
      {"username", username},
      {"password", hashedPassword}
   };

   for (const auto& user : users) {
      if (user["password"] == hashedPassword) {
         cout << "Password sudah digunakan, silakan gunakan password lain!\n";
         return;
      }
   }

   users.push_back(newUser);
   saveOperators(users);

   cout << "Registrasi berhasil!\n";
}

bool loginOperator() {
   string username, password;
   cout << "\n=== LOGIN OPERATOR ===\n";
   cout << "Username: ";
   cin >> username;

   if (username.empty() || username.find_first_not_of(' ') == string::npos) {
      cout << "Username tidak boleh kosong!\n";
      return false;
   }

   if (username.length() < 3) {
      cout << "Username harus minimal 3 karakter!\n";
      return false;
   }

   if (!isValidUsername(username)) {
      cout << "Username tidak boleh mengandung simbol!\n";
      return false;
   }

   cout << "Password: ";
   cin >> password;

   if (password.length() < 6) {
      cout << "Password harus minimal 6 karakter!\n";
      return false;
   }

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
