#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

const string TURNAMEN_FILE = "data/turnamen.json";

struct Tim {
    string nama;
    int main = 0;
    int menang = 0;
    int seri = 0;
    int kalah = 0;
    int poin = 0;
};

struct Pertandingan {
    int tim1, tim2;
    int skor1, skor2;
    bool sudahDimainkan = false;
};

vector<Tim> tim;
vector<Pertandingan> jadwal;
int jumlahTim, jumlahHari, jumlahMatch = 0;
int jenisOlahraga;
string namaTurnamen;

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

bool loadTurnamenData() {
    json turnamenData = loadTurnamen();
    
    if (turnamenData.empty()) {
        cout << "Tidak ada data turnamen yang tersimpan.\n";
        return false;
    }
    
    // Ambil turnamen terakhir yang dibuat
    json lastTurnamen = turnamenData[turnamenData.size() - 1];
    
    // Periksa apakah sistem pertandingan adalah "Liga"
    if (lastTurnamen.contains("sistem") && lastTurnamen["sistem"] != "Liga") {
        cout << "Sistem pertandingan bukan Liga. Program tidak dapat dijalankan.\n";
        return false;
    }
    
    // Load data turnamen
    namaTurnamen = lastTurnamen["nama"];
    jenisOlahraga = lastTurnamen["jenisOlahraga"];
    jumlahTim = lastTurnamen["jumlahTim"];
    jumlahHari = lastTurnamen["jumlahHari"];
    
    cout << "Turnamen " << namaTurnamen << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    
    return true;
}

void inputDataTim() {
    tim.resize(jumlahTim);
    cin.ignore(); // Bersihkan newline sebelum getline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "Nama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
    }
}

void buatJadwalSingleRoundRobin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = i + 1; j < jumlahTim; j++) {
            jadwal.push_back({i, j, 0, 0, false});
            jumlahMatch++;
        }
    }
}

void buatJadwalDoubleRoundRobin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = i + 1; j < jumlahTim; j++) {
            jadwal.push_back({i, j, 0, 0, false});
            jadwal.push_back({j, i, 0, 0, false});
            jumlahMatch += 2;
        }
    }
}

void tampilkanJadwalPertandingan() {
    int matchPerHari = jumlahMatch / jumlahHari;
    int sisa = jumlahMatch % jumlahHari;
    int idx = 0;
    for (int h = 0; h < jumlahHari; h++) {
        int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
        if (sisa > 0) sisa--;
        cout << "\nHari " << h + 1 << ":\n";
        for (int m = 0; m < matchHariIni && idx < jumlahMatch; m++, idx++) {
            cout << "- " << tim[jadwal[idx].tim1].nama << " vs " << tim[jadwal[idx].tim2].nama << endl;
        }
    }
}

void inputSkorHari(int hari) {
    int matchPerHari = jumlahMatch / jumlahHari;
    int sisa = jumlahMatch % jumlahHari;
    
    int startIdx = 0;
    for (int h = 0; h < hari - 1; h++) {
        int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
        if (sisa > 0) sisa--;
        startIdx += matchHariIni;
    }
    
    int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
    if (sisa > 0) sisa--;
    
    cout << "\n=== Input Skor Pertandingan Hari " << hari << " ===\n";
    for (int i = startIdx; i < startIdx + matchHariIni && i < jumlahMatch; i++) {
        cout << tim[jadwal[i].tim1].nama << " vs " << tim[jadwal[i].tim2].nama << "\n";
        cout << "Skor " << tim[jadwal[i].tim1].nama << ": ";
        cin >> jadwal[i].skor1;
        cout << "Skor " << tim[jadwal[i].tim2].nama << ": ";
        cin >> jadwal[i].skor2;
        jadwal[i].sudahDimainkan = true;

        tim[jadwal[i].tim1].main++;
        tim[jadwal[i].tim2].main++;

        if (jenisOlahraga == 1 || jenisOlahraga == 2) { // Sepak Bola / Futsal
            if (jadwal[i].skor1 > jadwal[i].skor2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 3;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 < jadwal[i].skor2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 3;
                tim[jadwal[i].tim1].kalah++;
            } else {
                tim[jadwal[i].tim1].seri++;
                tim[jadwal[i].tim2].seri++;
                tim[jadwal[i].tim1].poin++;
                tim[jadwal[i].tim2].poin++;
            }
        } else if (jenisOlahraga == 3) { // Handball
            if (jadwal[i].skor1 > jadwal[i].skor2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 2;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 < jadwal[i].skor2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 2;
                tim[jadwal[i].tim1].kalah++;
            } else {
                tim[jadwal[i].tim1].seri++;
                tim[jadwal[i].tim2].seri++;
                tim[jadwal[i].tim1].poin++;
                tim[jadwal[i].tim2].poin++;
            }
        } else if (jenisOlahraga == 4 || jenisOlahraga == 6) { // Tenis Meja / Badminton
            if (jadwal[i].skor1 > jadwal[i].skor2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 2;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 < jadwal[i].skor2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 2;
                tim[jadwal[i].tim1].kalah++;
            }
        } else if (jenisOlahraga == 5) { // Voli
            if (jadwal[i].skor1 == 3 && jadwal[i].skor2 <= 1) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 3;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 == 3 && jadwal[i].skor2 == 2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 2;
                tim[jadwal[i].tim2].kalah++;
                tim[jadwal[i].tim2].poin += 1;
            } else if (jadwal[i].skor2 == 3 && jadwal[i].skor1 <= 1) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 3;
                tim[jadwal[i].tim1].kalah++;
            } else if (jadwal[i].skor2 == 3 && jadwal[i].skor1 == 2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 2;
                tim[jadwal[i].tim1].kalah++;
                tim[jadwal[i].tim1].poin += 1;
            }
        }
    }
}

void urutkanKlasemen() {
    sort(tim.begin(), tim.end(), [](const Tim &a, const Tim &b) {
        if (a.poin != b.poin)
            return a.poin > b.poin;
        else
            return a.menang > b.menang;
    });
}

void tampilkanKlasemen() {
    urutkanKlasemen();
    cout << left << setw(15) << "Tim" << setw(6) << "M" << setw(6) << "W" 
         << setw(6) << "D" << setw(6) << "L" << setw(6) << "Pts" << endl;
    for (int i = 0; i < jumlahTim; i++) {
        cout << left << setw(15) << tim[i].nama << setw(6) << tim[i].main 
             << setw(6) << tim[i].menang << setw(6) << tim[i].seri 
             << setw(6) << tim[i].kalah << setw(6) << tim[i].poin << endl;
    }
}

void tampilkanPemenang() {
    urutkanKlasemen();
    
    // Cek jika ada tim yang seimbang di puncak klasemen
    bool adaJuaraBersama = false;
    if (jumlahTim > 1) {
        adaJuaraBersama = (tim[0].poin == tim[1].poin && tim[0].menang == tim[1].menang);
    }
    
    cout << "\n****************************************\n";
    if (adaJuaraBersama) {
        cout << "URUTAN PEMENANG:\n";
        int i = 0;
        while (i < jumlahTim - 1 && tim[i].poin == tim[i + 1].poin && tim[i].menang == tim[i + 1].menang) {
            cout << "- " << tim[i].nama << endl;
            i++;
        }
        cout << "- " << tim[i].nama << endl;
    } else {
        cout << "JUARA TURNAMEN: " << tim[0].nama << endl;
        cout << "Total Poin: " << tim[0].poin << endl;
        cout << "Menang: " << tim[0].menang << " | Seri: " << tim[0].seri << " | Kalah: " << tim[0].kalah << endl;
    }
    cout << "****************************************\n";
}

void simpanTurnamen() {
    json turnamenData = loadTurnamen();
    
    // Buat data tim
    json timData = json::array();
    for (int i = 0; i < jumlahTim; i++) {
        json timObj = {
            {"nama", tim[i].nama},
            {"main", tim[i].main},
            {"menang", tim[i].menang},
            {"seri", tim[i].seri},
            {"kalah", tim[i].kalah},
            {"poin", tim[i].poin}
        };
        timData.push_back(timObj);
    }
    
    // Buat data pertandingan
    json jadwalData = json::array();
    for (int i = 0; i < jumlahMatch; i++) {
        json matchObj = {
            {"tim1", jadwal[i].tim1},
            {"tim2", jadwal[i].tim2},
            {"skor1", jadwal[i].skor1},
            {"skor2", jadwal[i].skor2},
            {"sudahDimainkan", jadwal[i].sudahDimainkan}
        };
        jadwalData.push_back(matchObj);
    }
    
    // Buat objek turnamen
    string formatTurnamen = (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) ? 
                           "Single/Double Round Robin" : 
                           ((jenisOlahraga == 3) ? "Double Round Robin" : "Single Round Robin");
    
    // Update turnamen yang sudah ada
    for (auto& turnamen : turnamenData) {
        if (turnamen["nama"] == namaTurnamen) {
            turnamen["tim"] = timData;
            turnamen["jadwal"] = jadwalData;
            turnamen["jumlahMatch"] = jumlahMatch;
            turnamen["format"] = formatTurnamen;
            saveTurnamen(turnamenData);
            cout << "\nTurnamen " << namaTurnamen << " berhasil diperbarui!\n";
            return;
        }
    }
    
    // Jika tidak ditemukan, buat turnamen baru
    json turnamenObj = {
        {"nama", namaTurnamen},
        {"jenisOlahraga", jenisOlahraga},
        {"jumlahTim", jumlahTim},
        {"jumlahHari", jumlahHari},
        {"jumlahMatch", jumlahMatch},
        {"format", formatTurnamen},
        {"sistem", "Liga"},
        {"tim", timData},
        {"jadwal", jadwalData}
    };
    
    turnamenData.push_back(turnamenObj);
    saveTurnamen(turnamenData);
    
    cout << "\nTurnamen " << namaTurnamen << " berhasil disimpan!\n";
}

int main() {
    cout << "\n=== SISTEM LIGA ===\n";
    
    // Load data turnamen dari file
    if (!loadTurnamenData()) {
        return 1; // Keluar jika tidak ada data turnamen atau bukan sistem Liga
    }
    
    // Verifikasi jenis olahraga yang valid untuk sistem liga (1-6)
    if (jenisOlahraga < 1 || jenisOlahraga > 6) {
        cout << "Jenis olahraga tidak mendukung sistem liga. Hanya cabang 1-6 yang dapat menggunakan sistem liga.\n";
        return 1;
    }
    
    inputDataTim();

    if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
        cout << "Pilih format pertandingan:\n";
        cout << "1. Single Round Robin\n2. Double Round Robin\n";
        int format;
        cin >> format;
        if (format == 1) buatJadwalSingleRoundRobin();
        else buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 3) {
        buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 4 || jenisOlahraga == 6) {
        buatJadwalSingleRoundRobin();
    }

    tampilkanJadwalPertandingan();
    
    char inputSkor;
    cout << "\nApakah ingin menginput skor pertandingan? (y/n): ";
    cin >> inputSkor;
    
    if (inputSkor == 'y' || inputSkor == 'Y') {
        for (int hari = 1; hari <= jumlahHari; hari++) {
            inputSkorHari(hari);
            cout << "\n=== Klasemen Sementara ===\n";
            tampilkanKlasemen();
            
            if (hari < jumlahHari) {
                char lanjut;
                cout << "\nApakah ingin menginput skor hari ke-" << hari + 1 << "? (y/n): ";
                cin >> lanjut;
                if (lanjut != 'y' && lanjut != 'Y') {
                    break;
                }
            }
        }
    }

    cout << "\n=== Klasemen Akhir ===\n";
    tampilkanKlasemen();
    tampilkanPemenang();
    
    // Simpan data turnamen ke JSON
    simpanTurnamen();

    return 0;
}