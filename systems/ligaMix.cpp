#include "systems/ligaMix.h"
#include <fstream>
#include <sstream>
#include "systems/storage.h"

// Array dinamis untuk menyimpan tim
static TimLiga* tim = nullptr;
// Linked list untuk menyimpan jadwal pertandingan
static JadwalPertandingan jadwal;

static int jumlahTim = 0, jumlahHari = 0, jumlahMatch = 0;
static int jenisOlahraga = 0;
static string namaTurnamen;

// Fungsi untuk memuat data turnamen terakhir
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
    namaTurnamen = lastTurnamen.value("nama", "Turnamen Tanpa Nama");
    jenisOlahraga = lastTurnamen.value("jenisOlahraga", 1);  // default 1
    jumlahTim = lastTurnamen.value("jumlahTim", 0);
    jumlahHari = lastTurnamen.value("jumlahHari", 1);
    
    cout << "Turnamen " << namaTurnamen << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    
    // Alokasi memori untuk array tim
    tim = new TimLiga[jumlahTim];
    
    return true;
}

// Fungsi untuk menambahkan pertandingan ke linked list
void tambahPertandingan(int tim1, int tim2) {
    PertandinganNode* newNode = new PertandinganNode;
    newNode->tim1 = tim1;
    newNode->tim2 = tim2;
    newNode->skor1 = 0;
    newNode->skor2 = 0;
    newNode->sudahDimainkan = false;
    newNode->next = nullptr;
    
    if (jadwal.head == nullptr) {
        jadwal.head = newNode;
        jadwal.tail = newNode;
    } else {
        jadwal.tail->next = newNode;
        jadwal.tail = newNode;
    }
    
    jadwal.jumlah++;
    jumlahMatch++;
}

// Fungsi untuk input data tim
void inputDataTim() {
    cin.ignore(); // Bersihkan newline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "\nNama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
    }
}

// Fungsi untuk membuat jadwal single round robin
void buatJadwalSingleRoundRobin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = i + 1; j < jumlahTim; j++) {
            tambahPertandingan(i, j);
        }
    }
}

// Fungsi untuk membuat jadwal double round robin
void buatJadwalDoubleRoundRobin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = i + 1; j < jumlahTim; j++) {
            tambahPertandingan(i, j);
            tambahPertandingan(j, i);
        }
    }
}

// Fungsi untuk menampilkan jadwal pertandingan
void tampilkanJadwalPertandingan() {
    if (jadwal.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = jumlahMatch / jumlahHari;
    int sisa = jumlahMatch % jumlahHari;
    
    PertandinganNode* current = jadwal.head;
    int idx = 0;
    
    for (int h = 0; h < jumlahHari; h++) {
        int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
        if (sisa > 0) sisa--;
        cout << "\nHari " << h + 1 << ":\n";
        
        for (int m = 0; m < matchHariIni && current != nullptr; m++) {
            cout << "- " << tim[current->tim1].nama << " vs " << tim[current->tim2].nama << endl;
            current = current->next;
            idx++;
        }
    }
}

// Fungsi untuk memperbarui statistik tim berdasarkan hasil pertandingan
void updateStatistikTim(PertandinganNode* match) {
    int tim1 = match->tim1;
    int tim2 = match->tim2;
    
    tim[tim1].main++;
    tim[tim2].main++;
    
    if (jenisOlahraga == 1 || jenisOlahraga == 2) { // Sepak Bola / Futsal
        if (match->skor1 > match->skor2) {
            tim[tim1].menang++;
            tim[tim1].poin += 3;
            tim[tim2].kalah++;
        } else if (match->skor1 < match->skor2) {
            tim[tim2].menang++;
            tim[tim2].poin += 3;
            tim[tim1].kalah++;
        } else {
            tim[tim1].seri++;
            tim[tim2].seri++;
            tim[tim1].poin++;
            tim[tim2].poin++;
        }
    } else if (jenisOlahraga == 3) { // Handball
        if (match->skor1 > match->skor2) {
            tim[tim1].menang++;
            tim[tim1].poin += 2;
            tim[tim2].kalah++;
        } else if (match->skor1 < match->skor2) {
            tim[tim2].menang++;
            tim[tim2].poin += 2;
            tim[tim1].kalah++;
        } else {
            tim[tim1].seri++;
            tim[tim2].seri++;
            tim[tim1].poin++;
            tim[tim2].poin++;
        }
    } else if (jenisOlahraga == 4 || jenisOlahraga == 6) { // Tenis Meja / Badminton
        if (match->skor1 > match->skor2) {
            tim[tim1].menang++;
            tim[tim1].poin += 2;
            tim[tim2].kalah++;
        } else if (match->skor1 < match->skor2) {
            tim[tim2].menang++;
            tim[tim2].poin += 2;
            tim[tim1].kalah++;
        }
    } else if (jenisOlahraga == 5) { // Voli
        if (match->skor1 == 3 && match->skor2 <= 1) {
            tim[tim1].menang++;
            tim[tim1].poin += 3;
            tim[tim2].kalah++;
        } else if (match->skor1 == 3 && match->skor2 == 2) {
            tim[tim1].menang++;
            tim[tim1].poin += 2;
            tim[tim2].kalah++;
            tim[tim2].poin += 1;
        } else if (match->skor2 == 3 && match->skor1 <= 1) {
            tim[tim2].menang++;
            tim[tim2].poin += 3;
            tim[tim1].kalah++;
        } else if (match->skor2 == 3 && match->skor1 == 2) {
            tim[tim2].menang++;
            tim[tim2].poin += 2;
            tim[tim1].kalah++;
            tim[tim1].poin += 1;
        }
    }
}

// Fungsi untuk input skor pertandingan pada hari tertentu
void inputSkorHari(int hari) {
    if (jadwal.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = jumlahMatch / jumlahHari;
    int sisa = jumlahMatch % jumlahHari;
    
    // Hitung indeks awal untuk hari tertentu
    int startIdx = 0;
    int sisaTemp = sisa;
    for (int h = 0; h < hari - 1; h++) {
        int matchHariIni = matchPerHari + (sisaTemp > 0 ? 1 : 0);
        if (sisaTemp > 0) sisaTemp--;
        startIdx += matchHariIni;
    }
    
    int matchHariIni = matchPerHari + (sisa > 0 && hari <= sisa ? 1 : 0);
    
    cout << "\n=== Input Skor Pertandingan Hari " << hari << " ===\n";
    
    // Navigasi ke pertandingan pertama hari ini
    PertandinganNode* current = jadwal.head;
    for (int i = 0; i < startIdx && current != nullptr; i++) {
        current = current->next;
    }
    
    // Input skor untuk setiap pertandingan hari ini
    for (int i = 0; i < matchHariIni && current != nullptr; i++) {
        cout << "\n" << tim[current->tim1].nama << " vs " << tim[current->tim2].nama << "\n";
        cout << "Skor " << tim[current->tim1].nama << ": ";
        cin >> current->skor1;
        cout << "Skor " << tim[current->tim2].nama << ": ";
        cin >> current->skor2;
        current->sudahDimainkan = true;
        
        updateStatistikTim(current);
        current = current->next;
    }
}

// Fungsi untuk menukar posisi tim dalam array
void tukarTim(TimLiga* a, TimLiga* b) {
    TimLiga temp = *a;
    *a = *b;
    *b = temp;
}

// Fungsi untuk mengurutkan klasemen menggunakan bubble sort
void urutkanKlasemen() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = 0; j < jumlahTim - i - 1; j++) {
            // Urutkan berdasarkan poin
            if (tim[j].poin < tim[j + 1].poin) {
                tukarTim(&tim[j], &tim[j + 1]);
            }
            // Jika poin sama, urutkan berdasarkan jumlah kemenangan
            else if (tim[j].poin == tim[j + 1].poin && tim[j].menang < tim[j + 1].menang) {
                tukarTim(&tim[j], &tim[j + 1]);
            }
        }
    }
}

// Fungsi untuk menampilkan klasemen
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

// Fungsi untuk menampilkan pemenang
void tampilkanPemenang() {
    urutkanKlasemen();
    
    // Cek jika ada tim yang seimbang di puncak klasemen
    bool adaJuaraBersama = false;
    if (jumlahTim > 1) {
        adaJuaraBersama = (tim[0].poin == tim[1].poin && tim[0].menang == tim[1].menang);
    }
    
    cout << "\n****************************************\n";
    if (adaJuaraBersama) {
        cout << "JUARA BERSAMA:\n";
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

// Konversi linked list jadwal ke format JSON untuk disimpan
json jadwalToJson() {
    json jadwalData = json::array();
    PertandinganNode* current = jadwal.head;
    
    while (current != nullptr) {
        json matchObj = {
            {"tim1", current->tim1},
            {"tim2", current->tim2},
            {"skor1", current->skor1},
            {"skor2", current->skor2},
            {"sudahDimainkan", current->sudahDimainkan}
        };
        jadwalData.push_back(matchObj);
        current = current->next;
    }
    
    return jadwalData;
}

// Fungsi untuk menyimpan turnamen
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
    
    // Buat data jadwal pertandingan
    json jadwalData = jadwalToJson();
    
    // Format turnamen sesuai dengan jenis olahraga
    string formatTurnamen;
    if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
        formatTurnamen = "Single/Double Round Robin";
    } else if (jenisOlahraga == 3) {
        formatTurnamen = "Double Round Robin";
    } else {
        formatTurnamen = "Single Round Robin";
    }
    
    // Update turnamen yang sudah ada
    bool turnamenDitemukan = false;
    for (auto& turnamen : turnamenData) {
        if (turnamen["nama"] == namaTurnamen) {
            turnamen["tim"] = timData;
            turnamen["jadwal"] = jadwalData;
            turnamen["jumlahMatch"] = jumlahMatch;
            turnamen["format"] = formatTurnamen;
            turnamenDitemukan = true;
            break;
        }
    }
    
    // Jika tidak ditemukan, buat turnamen baru
    if (!turnamenDitemukan) {
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
    }
    
    saveTurnamen(turnamenData);
    cout << "\nTurnamen " << namaTurnamen << " berhasil disimpan!\n";
}

// Fungsi untuk membersihkan memori
void bersihkanMemori() {
    // Hapus array tim
    if (tim != nullptr) {
        delete[] tim;
        tim = nullptr;
    }
    
    // Hapus linked list jadwal
    PertandinganNode* current = jadwal.head;
    while (current != nullptr) {
        PertandinganNode* temp = current;
        current = current->next;
        delete temp;
    }
    jadwal.head = nullptr;
    jadwal.tail = nullptr;
    jadwal.jumlah = 0;
}

// Fungsi utama sistem liga
void sistemLiga() {
    cout << "\n=== SISTEM LIGA ===\n";
    
    // Load data turnamen dari file
    if (!loadTurnamenData()) {
        return; // Keluar jika tidak ada data turnamen atau bukan sistem Liga
    }
    
    // Verifikasi jenis olahraga yang valid untuk sistem liga (1-6)
    if (jenisOlahraga < 1 || jenisOlahraga > 6) {
        cout << "Jenis olahraga tidak mendukung sistem liga. Hanya cabang 1-6 yang dapat menggunakan sistem liga.\n";
        bersihkanMemori();
        return;
    }
    
    // Input data tim
    inputDataTim();
    
    // Buat jadwal pertandingan
    if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
        cout << "\n1. Single Round Robin\n2. Double Round Robin\n";
        cout << "Pilih format pertandingan (1/2): ";
        int format;
        cin >> format;
        if (format == 1) buatJadwalSingleRoundRobin();
        else buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 3) {
        buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 4 || jenisOlahraga == 6) {
        buatJadwalSingleRoundRobin();
    }
    
    // Tampilkan jadwal pertandingan
    tampilkanJadwalPertandingan();
    
    // Input skor untuk setiap hari pertandingan
    for (int hari = 1; hari <= jumlahHari; hari++) {
        inputSkorHari(hari);
        cout << "\n=== Klasemen Sementara ===\n";
        tampilkanKlasemen();
    }
    
    // Tampilkan klasemen akhir dan pemenang
    cout << "\n=== Klasemen Akhir ===\n";
    tampilkanKlasemen();
    tampilkanPemenang();
    
    // Simpan data turnamen
    simpanTurnamen();
    
    // Bersihkan memori
    bersihkanMemori();
}