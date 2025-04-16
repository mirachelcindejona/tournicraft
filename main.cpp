#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "auth/auth.h"
#include "systems/ligaMix.h"

using namespace std;
using json = nlohmann::json;

struct User {
      string username;
      string password;
};

bool isLoggedIn = false;
string currentUser;

void buatTurnamenBaru(); // Deklarasi fungsi dari addTour.cpp

void cariTurnamenPublik() {
      cout << "\n=== CARI TURNAMEN (PUBLIK) ===\n";
      cout << "Daftar turnamen yang tersedia akan muncul di sini nanti\n";
}

// ===================== MENU SETELAH LOGIN ===================== //
void menuOperator() {
      int pilihan;
      bool aktif = true;

      while (aktif) {
            cout << "\n===== MENU OPERATOR =====\n";
            cout << "1. Cari turnamen lain\n";
            cout << "2. Buat turnamen baru\n";
            cout << "3. Lihat daftar turnamen\n";
            cout << "4. Keluar\n";
            cout << "Pilih menu: ";
            cin >> pilihan;

            switch (pilihan) {
            case 1:
                  cariTurnamenPublik();
                  break;
            case 2:
                  buatTurnamenBaru();
                  break;
            case 3:
                  cout << "\n=== DAFTAR TURNAMEN ===\n";
                  break;
            case 4:
                  cout << "Keluar dari akun operator.\n";
                  isLoggedIn = false;
                  aktif = false;
                  break;
            default:
                  cout << "Pilihan tidak valid.\n";
            }
      }
}

// ===================== MENU UTAMA ===================== //
void menuUtama() {
      int pilihan;
      bool berjalan = true;

      while (berjalan) {
            cout << "\n====== TOURNICRAFT ======\n";
            cout << "1. Cari Turnamen\n";
            cout << "2. Registrasi sebagai Operator\n";
            cout << "3. Login sebagai Operator\n";
            cout << "4. Keluar Aplikasi\n";
            cout << "Pilih menu: ";
            cin >> pilihan;

            switch (pilihan) {
            case 1:
                  cariTurnamenPublik();
                  break;
            case 2:
                  registerOperator();
                  break;
            case 3:
                  if (loginOperator()) {
                        menuOperator();
                  }
                  break;
            case 4:
                  cout << "Terima kasih telah menggunakan TourniCraft!\n";
                  berjalan = false;
                  break;
            default:
                  cout << "Pilihan tidak valid. Silakan coba lagi.\n";
            }
      }
}

int main() {
      menuUtama();
      return 0;
}
