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
      cout << "\n=== CARI TURNAMEN PUBLIK ===\n";

      ifstream file("data/turnamen.json");
      if (!file.is_open()) {
      cout << "Gagal membuka file turnamen.\n";
      return;
      }

      json turnamenJson;
      file >> turnamenJson;

      if (turnamenJson.empty()) {
      cout << "Belum ada turnamen yang terdaftar.\n";
      return;
      }

      cin.ignore(); // flush newline dari input sebelumnya
      string keyword;
      cout << "Masukkan kata kunci nama turnamen: ";
      getline(cin, keyword);

      bool ditemukan = false;
      int no = 1;

      cout << "\nHasil pencarian untuk '" << keyword << "':\n";
      for (const auto& t : turnamenJson) {
      string nama = t["nama"];
            if (nama.find(keyword) != string::npos || nama.find(keyword) != string::npos) {
            cout << no++ << ". Nama: " << t["nama"]
                  << " | Tanggal: " << t["tanggal_mulai"]
                  << " | Peserta: " << t["jumlah_peserta"] << " tim\n";
            ditemukan = true;
            }
      }

      if (!ditemukan) {
      cout << "Tidak ada turnamen dengan kata kunci tersebut.\n";
      }
}


void tampilkanDaftarTurnamen() {
      ifstream file("data/turnamen.json"); // sesuaikan path jika berbeda
      if (!file.is_open()) {
      cout << "Gagal membuka file turnamen.\n";
      return;
      }

      json turnamenJson;
      file >> turnamenJson;

      if (turnamenJson.empty()) {
      cout << "Belum ada turnamen yang terdaftar.\n";
      return;
      }

      cout << "\n=== DAFTAR TURNAMEN TERDAFTAR ===\n";
      int no = 1;
      for (const auto& t : turnamenJson) {
      cout << no++ << ". Nama: " << t["nama"]
            << " | Tanggal Mulai: " << t["tanggal_mulai"]
            << " | Peserta: " << t["jumlah_peserta"] << " tim\n";
      }
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
                  tampilkanDaftarTurnamen();
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
