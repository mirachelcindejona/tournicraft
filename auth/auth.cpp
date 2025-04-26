// Modifikasi file auth.cpp untuk menambahkan fungsi clear screen

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <conio.h>
#include <functional>
#include <limits> // Tambahkan header ini untuk numeric_limits
#include "nlohmann/json.hpp"
#include "auth/auth.h"

using namespace std;
using json = nlohmann::json;

// Referensi variabel dari main.cpp
extern bool isLoggedIn;
extern string currentUser;

// Fungsi clear screen dari main.cpp
extern void clearScreen();

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

string inputPassword() {
   string password;
   char ch;
   while (true) {
      ch = _getch(); // baca karakter tanpa echo
      if (ch == 13) { // enter
         cout << endl;
         break;
      } else if (ch == 8) { // backspace
         if (!password.empty()) {
            password.pop_back();
            cout << "\b \b"; // hapus bintang di layar
         }
      } else {
         password.push_back(ch);
         cout << '*';
      }
   }
   return password;
}

void registerOperator() {
   string username, password;
   bool registrationSuccess = false;
   
   while (!registrationSuccess) {
      clearScreen();
      cout << "\n=== REGISTRASI OPERATOR ===\n";
      cout << "Masukkan username: ";
      cin >> username;

      if (username.empty() || username.find_first_not_of(' ') == string::npos) {
         cout << "Username tidak boleh kosong! Silakan coba lagi.\n";
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      if (username.length() < 3) {
         cout << "Username harus minimal 3 karakter! Silakan coba lagi.\n";
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      if (!isValidUsername(username)) {
         cout << "Username tidak boleh mengandung simbol! Silakan coba lagi.\n";
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      json users = loadOperators();
      if (usernameExists(users, username)) {
         cout << "Username sudah digunakan! Silakan coba username lain.\n";
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      bool passwordValid = false;
      while (!passwordValid) {
         cout << "Masukkan password: ";
         password = inputPassword();

         if (password.length() < 6) {
            cout << "Password harus minimal 6 karakter! Silakan coba lagi.\n";
            continue;
         }

         if (!isValidPassword(password)) {
            cout << "Password harus mengandung setidaknya satu karakter alfanumerik! Silakan coba lagi.\n";
            continue;
         }

         // Password valid
         passwordValid = true;
         
         json newUser = {
            {"username", username},
            {"password", hashPassword(password)}
         };

         users.push_back(newUser);
         saveOperators(users);

         cout << "Registrasi berhasil!\n";
         registrationSuccess = true;
      }
   }
}

bool loginOperator() {
   string username, password;
   bool loginSuccess = false;
   int attempts = 0;
   
   while (!loginSuccess && attempts < 3) {
      clearScreen();
      cout << "\n=== LOGIN OPERATOR ===\n";
      if (attempts > 0) {
         cout << "Percobaan login ke-" << attempts + 1 << " dari 3\n";
      }
      
      cout << "Username: ";
      cin >> username;

      if (username.empty() || username.find_first_not_of(' ') == string::npos) {
         cout << "Username tidak boleh kosong! Silakan coba lagi.\n";
         attempts++;
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      if (username.length() < 3) {
         cout << "Username harus minimal 3 karakter! Silakan coba lagi.\n";
         attempts++;
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      if (!isValidUsername(username)) {
         cout << "Username tidak boleh mengandung simbol! Silakan coba lagi.\n";
         attempts++;
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      json users = loadOperators();
      int idx = findUserIndex(users, username);
      
      if (idx == -1) {
         cout << "Username tidak ditemukan! Silakan coba lagi.\n";
         attempts++;
         cout << "Tekan Enter untuk melanjutkan...";
         cin.ignore();
         cin.get();
         continue;
      }

      bool passwordValid = false;
      int passwordAttempts = 0;
      
      while (!passwordValid && passwordAttempts < 3) {
         cout << "Password: ";
         password = inputPassword();

         if (password.length() < 6) {
            cout << "Password harus minimal 6 karakter! Silakan coba lagi.\n";
            passwordAttempts++;
            continue;
         }

         // Hash password yang diinput dan bandingkan dengan hash yang tersimpan
         string hashedPassword = hashPassword(password);
         
         if (users[idx]["password"] == hashedPassword) {
            currentUser = username;
            isLoggedIn = true;
            cout << "Login berhasil! Selamat datang, " << username << ".\n";
            loginSuccess = true;
            passwordValid = true;
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
            return true;
         } else {
            cout << "Password salah! Silakan coba lagi.\n";
            passwordAttempts++;
            if (passwordAttempts >= 3) {
               cout << "Terlalu banyak percobaan password salah!\n";
               attempts++;
               cout << "Tekan Enter untuk melanjutkan...";
               cin.ignore();
               cin.get();
               break;
            }
         }
      }
   }
   
   if (attempts >= 3) {
      cout << "Terlalu banyak percobaan login gagal. Silakan coba lagi nanti.\n";
   }
   
   return false;
}