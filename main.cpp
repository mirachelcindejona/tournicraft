#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include "nlohmann/json.hpp"
#include "auth/auth.h"
#include "systems/ligaMix.h"
#include "systems/storage.h"
#include "systems/sistemGugur.h"
#include "systems/swiss.h"

using namespace std;
using json = nlohmann::json;

struct User {
      string username;
      string password;
};

bool isLoggedIn = false;
string currentUser;

// Fungsi untuk membersihkan layar terminal
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Untuk Windows
#else
    system("clear"); // Untuk Unix/Linux/macOS
#endif
}

void buatTurnamenBaru();

void menuOperator() {
      int pilihan;
      bool aktif = true;

      while (aktif) {
            clearScreen();
            cout << "\n===== MENU OPERATOR =====\n";
            cout << "Operator: " << currentUser << "\n";
            cout << "1. Buat turnamen baru\n";
            cout << "2. Keluar\n";
            cout << "Pilih menu: ";
            if (!(cin >> pilihan)) {
                  cout << "Input tidak valid. Silakan coba lagi.\n";
                  cin.clear();
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cout << "Tekan Enter untuk melanjutkan...";
                  cin.get();
                  continue;
            }

            switch (pilihan) {
            case 1:
                  clearScreen();
                  buatTurnamenBaru();
                  cout << "\nTekan Enter untuk kembali ke menu operator...";
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cin.get();
                  break;
            case 2:
                  cout << "Keluar dari akun operator.\n";
                  isLoggedIn = false;
                  aktif = false;
                  break;
            default:
                  cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                  cout << "Tekan Enter untuk melanjutkan...";
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cin.get();
                  break;
            }
      }
}

void menuUtama() {
      int pilihan;
      bool berjalan = true;

      while (berjalan) {
            clearScreen();
            cout << "\n====== TOURNICRAFT ======\n";
            cout << "1. Registrasi sebagai Operator\n";
            cout << "2. Login sebagai Operator\n";
            cout << "3. Keluar Aplikasi\n";
            cout << "Pilih menu: ";
            if (!(cin >> pilihan)) {
                  cout << "Input tidak valid. Silakan coba lagi.\n";
                  cin.clear();
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cout << "Tekan Enter untuk melanjutkan...";
                  cin.get();
                  continue;
            }

            switch (pilihan) {
            case 1:
                  clearScreen();
                  registerOperator();
                  cout << "\nTekan Enter untuk kembali ke menu utama...";
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cin.get();
                  break;
            case 2:
                  clearScreen();
                  if (loginOperator()) {
                        menuOperator();
                  } else {
                        cout << "\nTekan Enter untuk kembali ke menu utama...";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin.get();
                  }
                  break;
            case 3:
                  clearScreen();
                  cout << "Terima kasih telah menggunakan TourniCraft!\n";
                  berjalan = false;
                  break;
            default:
                  cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                  cout << "Tekan Enter untuk melanjutkan...";
                  cin.ignore(numeric_limits<streamsize>::max(), '\n');
                  cin.get();
                  break;
            }
      }
}

int main() {
      clearScreen(); // Bersihkan layar saat program dimulai
      cout << "\n===== SELAMAT DATANG DI TOURNICRAFT =====\n";
      cout << "Aplikasi untuk manajemen turnamen olahraga\n";
      cout << "\nTekan Enter untuk melanjutkan...";
      cin.get();
      
      menuUtama();
      return 0;
}