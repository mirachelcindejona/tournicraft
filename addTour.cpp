#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <regex>
#include "nlohmann/json.hpp"
#include "systems/ligaMix.h"
#include "systems/storage.h"
#include "systems/sistemGugur.h"
#include "systems/swiss.h"

using namespace std;
using json = nlohmann::json;

// Fungsi clear screen dari main.cpp
extern void clearScreen();

void buatTurnamenBaru() {
    clearScreen();
    cout << "\n=== BUAT TURNAMEN BARU ===\n";

    string namaTurnamen;
    regex validNameRegex("^[a-zA-Z0-9 ]+$");

    // Input Nama Turnamen
    while (true) {
        cout << "Masukkan nama turnamen: ";
        cin.ignore();
        getline(cin, namaTurnamen);
        if (namaTurnamen.empty()) {
            cout << "Nama turnamen tidak boleh kosong!\n";
        } else if (namaTurnamen.length() < 4) {
            cout << "Nama turnamen harus minimal 4 karakter!\n";
        } else if (!regex_match(namaTurnamen, validNameRegex)) {
            cout << "Nama turnamen hanya boleh mengandung huruf, angka, dan spasi!\n";
        } else {
            break;
        }
    }

    // Input Jenis Olahraga
    clearScreen();
    int jenisOlahraga;
    while (true) {
        cout << "\n=== PILIH CABANG OLAHRAGA ===\n";
        cout << "Turnamen: " << namaTurnamen << "\n\n";
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
        if (!(cin >> jenisOlahraga)) {
            cout << "Input tidak valid. Silakan masukkan angka antara 1 sampai 19.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Tekan Enter untuk melanjutkan...";
            cin.get();
            clearScreen();
            continue;
        }
        if (jenisOlahraga < 1 || jenisOlahraga > 19) {
            cout << "Pilihan tidak tersedia. Ulangi input.\n";
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
            continue;
        }
        break;
    }

    // Input Jumlah Tim
    clearScreen();
    cout << "\n=== JUMLAH TIM ===\n";
    cout << "Turnamen: " << namaTurnamen << "\n";
    
    // Tampilkan jenis olahraga yang dipilih
    string namaOlahraga;
    switch (jenisOlahraga) {
        case 1: namaOlahraga = "Sepak Bola"; break;
        case 2: namaOlahraga = "Futsal"; break;
        case 3: namaOlahraga = "Handball"; break;
        case 4: namaOlahraga = "Tenis Meja"; break;
        case 5: namaOlahraga = "Voli"; break;
        case 6: namaOlahraga = "Badminton"; break;
        case 7: namaOlahraga = "Tenis"; break;
        case 8: namaOlahraga = "Taekwondo"; break;
        case 9: namaOlahraga = "Basket"; break;
        case 10: namaOlahraga = "Karate"; break;
        case 11: namaOlahraga = "Catur"; break;
        case 12: namaOlahraga = "Panahan"; break;
        case 13: namaOlahraga = "Gulat"; break;
        case 14: namaOlahraga = "E-sports Valorant"; break;
        case 15: namaOlahraga = "E-sports CS:GO"; break;
        case 16: namaOlahraga = "E-sports Clash Royale"; break;
        case 17: namaOlahraga = "E-sports Dota 2"; break;
        case 18: namaOlahraga = "Kartu (Yu-Gi-Oh)"; break;
        case 19: namaOlahraga = "Go (permainan strategi)"; break;
    }
    cout << "Olahraga: " << namaOlahraga << "\n\n";
    
    int jumlahTim;
    while (true) {
        cout << "Masukkan jumlah tim: ";
        if (!(cin >> jumlahTim)) {
            cout << "Input tidak valid. Masukkan angka!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Tekan Enter untuk melanjutkan...";
            cin.get();
            clearScreen();
            cout << "\n=== JUMLAH TIM ===\n";
            cout << "Turnamen: " << namaTurnamen << "\n";
            cout << "Olahraga: " << namaOlahraga << "\n\n";
            continue;
        }
        if (jumlahTim < 2) {
            cout << "Jumlah tim minimal 2!\n";
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
            cout << "\n=== JUMLAH TIM ===\n";
            cout << "Turnamen: " << namaTurnamen << "\n";
            cout << "Olahraga: " << namaOlahraga << "\n\n";
            continue;
        }
        break;
    }

    // Input Jumlah Hari
    clearScreen();
    cout << "\n=== JUMLAH HARI TURNAMEN ===\n";
    cout << "Turnamen: " << namaTurnamen << "\n";
    cout << "Olahraga: " << namaOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n\n";
    
    int jumlahHari;
    while (true) {
        cout << "Masukkan jumlah hari turnamen: ";
        if (!(cin >> jumlahHari)) {
            cout << "Input tidak valid. Masukkan angka!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Tekan Enter untuk melanjutkan...";
            cin.get();
            clearScreen();
            cout << "\n=== JUMLAH HARI TURNAMEN ===\n";
            cout << "Turnamen: " << namaTurnamen << "\n";
            cout << "Olahraga: " << namaOlahraga << "\n";
            cout << "Jumlah Tim: " << jumlahTim << "\n\n";
            continue;
        }
        if (jumlahHari <= 0) {
            cout << "Jumlah hari harus lebih dari 0!\n";
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
            cout << "\n=== JUMLAH HARI TURNAMEN ===\n";
            cout << "Turnamen: " << namaTurnamen << "\n";
            cout << "Olahraga: " << namaOlahraga << "\n";
            cout << "Jumlah Tim: " << jumlahTim << "\n\n";
            continue;
        }
        break;
    }

    // Tampilkan Sistem yang Direkomendasikan
    clearScreen();
    cout << "\n=== SISTEM PERTANDINGAN ===\n";
    cout << "Turnamen: " << namaTurnamen << "\n";
    cout << "Olahraga: " << namaOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n\n";
    
    vector<string> sistemTersedia;
    switch (jenisOlahraga) {
        case 1: case 2: case 5: case 6:
            sistemTersedia = {"Liga", "Knockout"};
            break;
        case 3: case 4:
            sistemTersedia = {"Liga"};
            break;
        case 7:
            sistemTersedia = {"Knockout"};
            break;
        case 8:
            sistemTersedia = {};  // Perlu ditambahkan sistem lain karena Gugur Bye dihapus
            break;
        case 9:
            sistemTersedia = {"Knockout"};
            break;
        case 10:
            sistemTersedia = {"Knockout"};
            break;
        case 11:
            sistemTersedia = {"Knockout", "Swiss"};
            break;
        case 12: case 13:
            sistemTersedia = {"Knockout"};
            break;
        case 14: case 15: case 16: case 17: case 18: case 19:
            sistemTersedia = {"Swiss"};
            break;
        default:
            cout << "Cabang olahraga tidak valid.\n";
            return;
    }

    // Periksa jika tidak ada sistem tersedia setelah menghapus Gugur Bye
    if (sistemTersedia.empty()) {
        cout << "Maaf, tidak ada sistem pertandingan yang tersedia untuk cabang olahraga ini.\n";
        cout << "Silakan pilih cabang olahraga lain.\n";
        cout << "Tekan Enter untuk kembali...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return;
    }

    cout << "Sistem pertandingan yang direkomendasikan:\n";
    for (int i = 0; i < sistemTersedia.size(); i++) {
        cout << i + 1 << ". " << sistemTersedia[i] << "\n";
    }

    // Input Pilihan Sistem Pertandingan
    int pilihanSistem;
    while (true) {
        cout << "\nPilih sistem pertandingan (input nomor): ";
        if (!(cin >> pilihanSistem)) {
            cout << "Input tidak valid. Masukkan angka!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Tekan Enter untuk melanjutkan...";
            cin.get();
            clearScreen();
            cout << "\n=== SISTEM PERTANDINGAN ===\n";
            cout << "Turnamen: " << namaTurnamen << "\n";
            cout << "Olahraga: " << namaOlahraga << "\n";
            cout << "Jumlah Tim: " << jumlahTim << "\n";
            cout << "Jumlah Hari: " << jumlahHari << "\n\n";
            cout << "Sistem pertandingan yang direkomendasikan:\n";
            for (int i = 0; i < sistemTersedia.size(); i++) {
                cout << i + 1 << ". " << sistemTersedia[i] << "\n";
            }
            continue;
        }
        if (pilihanSistem < 1 || pilihanSistem > sistemTersedia.size()) {
            cout << "Pilihan sistem tidak tersedia. Ulangi.\n";
            cout << "Tekan Enter untuk melanjutkan...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            clearScreen();
            cout << "\n=== SISTEM PERTANDINGAN ===\n";
            cout << "Turnamen: " << namaTurnamen << "\n";
            cout << "Olahraga: " << namaOlahraga << "\n";
            cout << "Jumlah Tim: " << jumlahTim << "\n";
            cout << "Jumlah Hari: " << jumlahHari << "\n\n";
            cout << "Sistem pertandingan yang direkomendasikan:\n";
            for (int i = 0; i < sistemTersedia.size(); i++) {
                cout << i + 1 << ". " << sistemTersedia[i] << "\n";
            }
            continue;
        }
        break;
    }

    string sistemTerpilih = sistemTersedia[pilihanSistem - 1];

    // Format turnamen berdasarkan olahraga
    string formatTurnamen;
    if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
        formatTurnamen = "Single/Double Round Robin";
    } else if (jenisOlahraga == 3) {
        formatTurnamen = "Double Round Robin";
    } else {
        formatTurnamen = "Single Round Robin";
    }

    // Simpan ke JSON
    json turnamenData = loadTurnamen();
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

    clearScreen();
    cout << "\n=== TURNAMEN BERHASIL DIBUAT ===\n";
    cout << "Nama Turnamen: " << namaTurnamen << "\n";
    cout << "Cabang Olahraga: " << namaOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    cout << "Sistem Pertandingan: " << sistemTerpilih << "\n";
    cout << "Format: " << formatTurnamen << "\n\n";
    cout << "Memproses sistem pertandingan...\n";
    clearScreen();

    // Jalankan sistem sesuai pilihan
    if (sistemTerpilih == "Liga") {
        sistemLiga();
    } else if (sistemTerpilih == "Knockout") {
        sistemGugur();
    } else if (sistemTerpilih == "Swiss") {
        swiss();
    }
    
    cout << "\nPembuatan turnamen selesai!\n";
}