#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "nlohmann/json.hpp"
#include "systems/ligaMix.h"
#include "systems/storage.h"
#include "systems/gugur_bye.h"
#include "systems/sistemGugur.h"
#include "systems/swiss.h"

using namespace std;
using json = nlohmann::json;

void buatTurnamenBaru() {
      cout << "\n=== BUAT TURNAMEN BARU ===\n";
      
      string namaTurnamen;
      cout << "Masukkan nama turnamen: ";
      cin.ignore();
      getline(cin, namaTurnamen);
      
      int jenisOlahraga;
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
      cin >> jenisOlahraga;
      
      int jumlahTim;
      do {
            cout << "Masukkan jumlah tim: ";
            cin >> jumlahTim;
            if (jumlahTim < 2) {
                  cout << "Jumlah tim harus minimal 2!\n";
            }
      } while (jumlahTim < 2);
      
      int jumlahHari;
      cout << "Masukkan jumlah hari turnamen: ";
      cin >> jumlahHari;
      
      vector<string> sistemTersedia;
      cout << "\nSistem pertandingan yang direkomendasikan:\n";
      
      switch(jenisOlahraga) {
            case 1: // Sepak Bola
            case 2: // Futsal
            case 5: // Voli
            case 6: // Badminton
            sistemTersedia = {"Liga", "Gugur Bye", "Knockout"};
            break;
            case 3: // Handball
            sistemTersedia = {"Liga"};
            break;
            case 4: // Tenis Meja
            sistemTersedia = {"Liga"};
            break;
            case 7: // Tenis
            sistemTersedia = {"Gugur Bye", "Knockout"};
            break;
            case 8: // Taekwondo
            sistemTersedia = {"Gugur Bye"};
            break;
            case 9: // Basket
            sistemTersedia = {"Gugur Bye", "Knockout"};
            break;
            case 10: // Karate
            sistemTersedia = {"Knockout"};
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
      
      // Pilih sistem pertandingan
      int pilihanSistem;
      cout << "Pilih sistem pertandingan (input nomor): ";
      cin >> pilihanSistem;
      
      if (pilihanSistem < 1 || pilihanSistem > sistemTersedia.size()) {
            cout << "Pilihan sistem tidak valid.\n";
            return;
      }
      
      string sistemTerpilih = sistemTersedia[pilihanSistem-1];
      
      // Simpan data turnamen ke JSON
      json turnamenData = loadTurnamen();
      
      // Format turnamen sesuai dengan jenis olahraga
      string formatTurnamen;
      if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
          formatTurnamen = "Single/Double Round Robin";
      } else if (jenisOlahraga == 3) {
          formatTurnamen = "Double Round Robin";
      } else {
          formatTurnamen = "Single Round Robin";
      }
      
      json turnamenBaru = {
          {"nama", namaTurnamen},
          {"jenisOlahraga", jenisOlahraga},
          {"jumlahTim", jumlahTim},
          {"jumlahHari", jumlahHari},
          {"jumlahMatch", 0},
          {"format", formatTurnamen},
          {"sistem", sistemTerpilih},
          {"tim", json::array()},
          {"jadwal", json::array()}
      };
      
      turnamenData.push_back(turnamenBaru);
      saveTurnamen(turnamenData);
      
      cout << "\nTurnamen " << namaTurnamen << " berhasil dibuat!\n";
      cout << "Cabang: " << jenisOlahraga << "\n";
      cout << "Jumlah Tim: " << jumlahTim << "\n";
      cout << "Jumlah Hari: " << jumlahHari << "\n";
      cout << "Sistem Pertandingan: " << sistemTerpilih << "\n";
      
      // Jalankan ligaMix.cpp jika sistem yang dipilih adalah Liga
      if (sistemTerpilih == "Liga") {
          sistemLiga();
      } else if (sistemTerpilih == "Gugur Bye")  {
         gugurBye();
      } else if (sistemTerpilih == "Knockout") {
         sistemGugur();
      } else if (sistemTerpilih == "Swiss") {
         swiss();
      }
}
