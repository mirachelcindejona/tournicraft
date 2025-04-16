#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

const string TURNAMEN_FILE = "data/turnamen.json";

json loadTurnamen() {
    ifstream inFile(TURNAMEN_FILE);
    if (!inFile) return json::array(); // jika file tidak ada, return array kosong
    json data;
    inFile >> data;
    return data;
}

void saveTurnamen(const json& data) {
    ofstream outFile(TURNAMEN_FILE);
    outFile << data.dump(4); // pretty print
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
      
      // Pilih sistem pertandingan
      int pilihanSistem;
      cout << "Pilih sistem pertandingan (1-" << sistemTersedia.size() << "): ";
      cin >> pilihanSistem;
      
      if (pilihanSistem < 1 || pilihanSistem > sistemTersedia.size()) {
            cout << "Pilihan sistem tidak valid.\n";
            return;
      }
      
      string sistemTerpilih = sistemTersedia[pilihanSistem-1];
      
      // Simpan data turnamen ke JSON
      json turnamenData = loadTurnamen();
      
      json turnamenBaru = {
          {"nama", namaTurnamen},
          {"jenisOlahraga", pilihanCabor},
          {"jumlahTim", jumlahTim},
          {"jumlahHari", jumlahHari},
          {"sistem", sistemTerpilih}
      };
      
      turnamenData.push_back(turnamenBaru);
      saveTurnamen(turnamenData);
      
      cout << "\nTurnamen " << namaTurnamen << " berhasil dibuat!\n";
      cout << "Cabang: " << pilihanCabor << "\n";
      cout << "Jumlah Tim: " << jumlahTim << "\n";
      cout << "Jumlah Hari: " << jumlahHari << "\n";
      cout << "Sistem Pertandingan: " << sistemTerpilih << "\n";
      
      // Jalankan ligaMix.cpp jika sistem yang dipilih adalah Liga
      if (sistemTerpilih == "Liga") {
          cout << "\nMenjalankan sistem Liga...\n";
          system("g++ systems/ligaMix.cpp -o ligaMix && ./ligaMix");
      }
}
