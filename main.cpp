#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"
#include "auth/auth.h"

using namespace std;
using json = nlohmann::json;

struct User {
      string username;
      string password;
};

bool isLoggedIn = false;
string currentUser;
void cariTurnamenPublik() {
      cout << "\n=== CARI TURNAMEN (PUBLIK) ===\n";
      cout << "Daftar turnamen yang tersedia akan muncul di sini nanti\n";
}

void buatTurnamenBaru() {
      cout << "\n=== BUAT TURNAMEN BARU ===\n";
      
      string namaTurnamen;
      cout << "Masukkan nama turnamen: ";
      cin.ignore();
      getline(cin, namaTurnamen);
      
      int pilihanCabor;
      cout << "\nPilih cabang olahraga:\n";
      cout << "1. Sepak Bola\n";
      cout << "2. Futsal\n";
      cout << "3. Handball\n";
      cout << "4. Tenis Meja\n";
      cout << "5. Voli\n";
      cout << "6. Badminton\n";
      cout << "7. Tenis\n";
      cout << "8. Taekwondo\n";
      cout << "9. Basket\n";
      cout << "10. Karate\n";
      cout << "11. Catur\n";
      cout << "12. Panahan\n";
      cout << "13. Gulat\n";
      cout << "14. E-sports Valorant\n";
      cout << "15. E-sports CS:GO\n";
      cout << "16. E-sports Clash Royale\n";
      cout << "17. E-sports Dota 2\n";
      cout << "18. Kartu (Yu-Gi-Oh)\n";
      cout << "19. Go (permainan strategi)\n";
      cout << "Pilih opsi (1-19): ";
      cin >> pilihanCabor;
      
      int jumlahTim;
      cout << "Masukkan jumlah tim: ";
      cin >> jumlahTim;
      
      int jumlahHari;
      cout << "Masukkan jumlah hari turnamen: ";
      cin >> jumlahHari;
      
      vector<string> sistemTersedia;
      cout << "\nSistem pertandingan yang direkomendasikan:\n";
      
      switch(pilihanCabor) {
            case 1: // Sepak Bola
            case 2: // Futsal
            case 5: // Voli
            case 6: // Badminton
            sistemTersedia = {"Liga", "Gugur Bye", "Knockout", "Gugur Ganda"};
            break;
            case 3: // Handball
            sistemTersedia = {"Liga"};
            break;
            case 4: // Tenis Meja
            sistemTersedia = {"Liga", "Gugur Ganda"};
            break;
            case 7: // Tenis
            sistemTersedia = {"Gugur Bye", "Knockout"};
            break;
            case 8: // Taekwondo
            sistemTersedia = {"Gugur Bye", "Gugur Ganda"};
            break;
            case 9: // Basket
            sistemTersedia = {"Gugur Bye", "Knockout"};
            break;
            case 10: // Karate
            sistemTersedia = {"Gugur Ganda"};
            break;
            case 11: // Catur
            sistemTersedia = {"Knockout", "Swiss"};
            break;
            case 12: // Panahan
            case 13: // Gulat
            sistemTersedia = {"Knockout"};
            break;
            case 14: // E-sports Valorant
            case 15: // E-sports CS:GO
            case 16: // E-sports Clash Royale
            case 17: // E-sports Dota 2
            case 18: // Kartu
            case 19: // Go
            sistemTersedia = {"Swiss"};
            break;
            default:
            cout << "Cabang olahraga tidak valid.\n";
            return;
      }
      
      // Tampilkan sistem yang tersedia
      for (int i = 0; i < sistemTersedia.size(); i++) {
            cout << i+1 << ". " << sistemTersedia[i] << endl;
      }
      
      // 6. Pilih sistem pertandingan
      int pilihanSistem;
      cout << "Pilih sistem pertandingan: ";
      cin >> pilihanSistem;
      
      if (pilihanSistem < 1 || pilihanSistem > sistemTersedia.size()) {
            cout << "Pilihan sistem tidak valid.\n";
            return;
      }
      
      string sistemTerpilih = sistemTersedia[pilihanSistem-1];
      cout << "\nTurnamen " << namaTurnamen << " berhasil dibuat!\n";
      cout << "Cabang: " << pilihanCabor << "\n";
      cout << "Jumlah Tim: " << jumlahTim << "\n";
      cout << "Jumlah Hari: " << jumlahHari << "\n";
      cout << "Sistem Pertandingan: " << sistemTerpilih << "\n";
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
