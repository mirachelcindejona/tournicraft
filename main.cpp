#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "auth/auth.h"
#include "systems/ligaMix.h"
#include "systems/gugur_bye.h"
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

void buatTurnamenBaru();

void menuOperator() {
      int pilihan;
      bool aktif = true;

      while (aktif) {
            cout << "\n===== MENU OPERATOR =====\n";
            cout << "1. Buat turnamen baru\n";
            cout << "2. Keluar\n";
            cout << "Pilih menu: ";
            if (!(cin >> pilihan)) {
                cout << "Input tidak valid. Silakan coba lagi.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (pilihan) {
            case 1:
                  buatTurnamenBaru();
                  break;
            case 2:
                  cout << "Keluar dari akun operator.\n";
                  isLoggedIn = false;
                  aktif = false;
                  break;
            default:
                  cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                  break;
            }
      }
}

void menuUtama() {
      int pilihan;
      bool berjalan = true;

      while (berjalan) {
            cout << "\n====== TOURNICRAFT ======\n";
            cout << "1. Registrasi sebagai Operator\n";
            cout << "2. Login sebagai Operator\n";
            cout << "3. Keluar Aplikasi\n";
            cout << "Pilih menu: ";
            if (!(cin >> pilihan)) {
                cout << "Input tidak valid. Silakan coba lagi.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (pilihan) {
            case 1:
                  registerOperator();
                  break;
            case 2:
                  if (loginOperator()) {
                        menuOperator();
                  }
                  break;
            case 3:
                  cout << "Terima kasih telah menggunakan TourniCraft!\n";
                  berjalan = false;
                  break;
            default:
                  cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                  break;
            }
      }
}

int main() {
      menuUtama();
      return 0;
}
