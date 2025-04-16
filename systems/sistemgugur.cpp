#include "systems/sistemGugur.h"
#include "systems/storage.h"

// Array dinamis untuk menyimpan tim
static TimGugur* tim = nullptr;
// Linked list untuk menyimpan jadwal pertandingan
static JadwalGugur jadwalG;

static int jumlahTim = 0, jumlahHari = 0, jumlahMatchG = 0;
static int jenisOlahraga = 0;
static string namaTurnamen;

// Fungsi untuk memuat data turnamen terakhir
bool loadTurnamenGugurData() {
    json turnamenData = loadTurnamen();
    
    if (turnamenData.empty()) {
        cout << "Tidak ada data turnamen yang tersimpan.\n";
        return false;
    }
    
    // Ambil turnamen terakhir yang dibuat
    json lastTurnamenG = turnamenData[turnamenData.size() - 1];
    
    // Periksa apakah sistem pertandingan adalah "Knockout"
    if (lastTurnamenG.contains("sistem") && lastTurnamenG["sistem"] != "Knockout") {
        cout << "Sistem pertandingan bukan Knockout. Program tidak dapat dijalankan.\n";
        return false;
    }
    
    // Load data turnamen
    namaTurnamen = lastTurnamenG.value("nama", "Turnamen Tanpa Nama");
    jenisOlahraga = lastTurnamenG.value("jenisOlahraga", 0);
    jumlahTim = lastTurnamenG.value("jumlahTim", 0);
    jumlahHari = lastTurnamenG.value("jumlahHari", 1);
    
    cout << "Turnamen " << namaTurnamen << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    
    // Alokasi memori untuk array tim
    tim = new TimGugur[jumlahTim];
    
    return true;
}

// Fungsi untuk menambahkan pertandingan ke linked list
void tambahPertandinganGugur(TimGugur tim1, TimGugur tim2, int babak) {
    PertandinganGugur* newNode = new PertandinganGugur;
    newNode->tim1 = tim1;
    newNode->tim2 = tim2;
    newNode->skor1 = 0;
    newNode->skor2 = 0;
    newNode->babak = babak;
    newNode->deskripsi = tim1.nama + " vs " + tim2.nama;
    newNode->sudahDimainkan = false;
    newNode->next = nullptr;
    
    if (jadwalG.head == nullptr) {
        jadwalG.head = newNode;
        jadwalG.tail = newNode;
    } else {
        jadwalG.tail->next = newNode;
        jadwalG.tail = newNode;
    }
    
    jadwalG.jumlah++;
    jumlahMatchG++;
}

// Fungsi untuk input data tim
void inputDataTimGugur() {
    cin.ignore(); // Bersihkan newline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "\nNama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
        tim[i].skor = 0;
        tim[i].isEliminated = false;
    }
}

// Fungsi untuk mengacak susunan tim
void acakSusunanTim() {
    cout << "\nMengacak susunan tim...\n";
    srand(time(nullptr)); // Inisialisasi seed untuk random
    
    for (int i = jumlahTim - 1; i > 0; i--) {
        // Pilih indeks acak dari 0 hingga i
        int j = rand() % (i + 1);
        
        // Tukar tim[i] dengan tim[j]
        TimGugur temp = tim[i];
        tim[i] = tim[j];
        tim[j] = temp;
    }
    
    cout << "Susunan tim berhasil diacak!\n";
}

// Fungsi untuk menghitung jumlah babak yang dibutuhkan
int hitungJumlahBabak() {
    return ceil(log2(jumlahTim));
}

// Fungsi untuk membuat bracket sistem gugur
void buatBracketGugur() {
    // Acak susunan tim terlebih dahulu
    acakSusunanTim();
    
    // Hitung jumlah babak yang dibutuhkan
    int jumlahBabak = hitungJumlahBabak();
    int totalMatch = jumlahTim - 1; // Dalam sistem gugur, total match = jumlahTim - 1
    
    cout << "\nJumlah babak yang dibutuhkan: " << jumlahBabak << endl;
    cout << "Total pertandingan: " << totalMatch << endl;
    
    // Buat bracket babak pertama
    int matchBabak1 = jumlahTim / 2;
    for (int i = 0; i < matchBabak1; i++) {
        tambahPertandinganGugur(tim[i*2], tim[i*2+1], 1);
    }
}

// Fungsi untuk menampilkan jadwal pertandingan
void tampilkanJadwalPertandinganGugur() {
    if (jadwalG.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = jumlahMatchG / jumlahHari;
    if (matchPerHari == 0) matchPerHari = 1; // Minimal 1 match per hari
    
    int sisa = jumlahMatchG % jumlahHari;
    
    // Cari babak tertinggi (final)
    int babakTertinggi = 1;
    PertandinganGugur* temp = jadwalG.head;
    while (temp != nullptr) {
        if (temp->babak > babakTertinggi) {
            babakTertinggi = temp->babak;
        }
        temp = temp->next;
    }
    
    // Tampilkan jadwal per babak
    for (int b = 1; b <= babakTertinggi; b++) {
        cout << "\n=== BABAK " << (b == babakTertinggi ? "FINAL" : 
                                  (b == babakTertinggi-1 ? "SEMIFINAL" : 
                                  (b == babakTertinggi-2 ? "PEREMPAT FINAL" : to_string(b)))) << " ===\n";
        
        PertandinganGugur* current = jadwalG.head;
        int idx = 0;
        int hariKe = 1;
        
        while (current != nullptr) {
            if (current->babak == b) {
                // Hitung hari untuk pertandingan ini
                int hariMatch = (idx / matchPerHari) + 1;
                if (hariMatch > jumlahHari) hariMatch = jumlahHari;
                
                if (hariKe != hariMatch) {
                    hariKe = hariMatch;
                    cout << "\nHari " << hariKe << ":\n";
                }
                
                cout << "- " << current->deskripsi << endl;
                idx++;
            }
            current = current->next;
        }
    }
}

// Fungsi untuk update bracket ke babak berikutnya
void updateBracketGugur(PertandinganGugur* pertandingan, TimGugur pemenang, int babakBerikutnya) {
    // Cari dulu apakah sudah ada pertandingan di babak berikutnya yang menunggu
    PertandinganGugur* current = jadwalG.head;
    PertandinganGugur* matchBerikutnya = nullptr;
    int index = 0;
    
    // Hitung berapa posisi match di babaknya
    PertandinganGugur* temp = jadwalG.head;
    while (temp != pertandingan) {
        if (temp->babak == pertandingan->babak) {
            index++;
        }
        temp = temp->next;
    }
    
    // Posisi di babak berikutnya
    int posisiBerikutnya = index / 2;
    
    // Cari apakah match di babak berikutnya sudah ada
    bool found = false;
    int countMatchBabakBerikutnya = 0;
    current = jadwalG.head;
    while (current != nullptr) {
        if (current->babak == babakBerikutnya) {
            if (countMatchBabakBerikutnya == posisiBerikutnya) {
                matchBerikutnya = current;
                found = true;
                break;
            }
            countMatchBabakBerikutnya++;
        }
        current = current->next;
    }
    
    // Jika belum ada match di babak berikutnya, buat baru
    if (!found) {
        TimGugur placeholder = {"TBD", 0, false}; // To Be Determined
        tambahPertandinganGugur(pemenang, placeholder, babakBerikutnya);
    } else {
        // Jika sudah ada, update tim yang masuk
        if (matchBerikutnya->tim1.nama == "TBD") {
            matchBerikutnya->tim1 = pemenang;
            matchBerikutnya->deskripsi = pemenang.nama + " vs " + matchBerikutnya->tim2.nama;
        } else {
            matchBerikutnya->tim2 = pemenang;
            matchBerikutnya->deskripsi = matchBerikutnya->tim1.nama + " vs " + pemenang.nama;
        }
    }
}

// Fungsi untuk input skor pertandingan pada babak tertentu
void inputSkorBabakGugur(int babak) {
    if (jadwalG.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    cout << "\n=== Input Skor Pertandingan Babak " << (babak == 1 ? "1" : 
                                                      (babak == hitungJumlahBabak() ? "FINAL" : 
                                                      (babak == hitungJumlahBabak()-1 ? "SEMIFINAL" : 
                                                      (babak == hitungJumlahBabak()-2 ? "PEREMPAT FINAL" : to_string(babak))))) << " ===\n";
    
    PertandinganGugur* current = jadwalG.head;
    bool adaPertandingan = false;
    
    while (current != nullptr) {
        if (current->babak == babak && !current->sudahDimainkan && current->tim1.nama != "TBD" && current->tim2.nama != "TBD") {
            adaPertandingan = true;
            
            try {
                cout << "\nPertandingan: " << current->deskripsi << endl;
                cout << "Masukkan skor " << current->tim1.nama << ": ";
                cin >> current->skor1;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    throw runtime_error("Input harus berupa angka");
                }
                
                cout << "Masukkan skor " << current->tim2.nama << ": ";
                cin >> current->skor2;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    throw runtime_error("Input harus berupa angka");
                }
                
                // Tentukan pemenang
                TimGugur pemenang;
                if (current->skor1 > current->skor2) {
                    cout << current->tim1.nama << " menang dengan skor " << current->skor1 << "-" << current->skor2 << endl;
                    pemenang = current->tim1;
                    pemenang.skor = current->skor1;
                    current->tim2.isEliminated = true;
                } else if (current->skor1 < current->skor2) {
                    cout << current->tim2.nama << " menang dengan skor " << current->skor2 << "-" << current->skor1 << endl;
                    pemenang = current->tim2;
                    pemenang.skor = current->skor2;
                    current->tim1.isEliminated = true;
                } else {
                    cout << "Skor imbang tidak diperbolehkan dalam sistem gugur." << endl;
                    cout << "Silakan input ulang skor atau tentukan pemenang dengan adu penalti.\n";
                    continue;
                }
                
                current->sudahDimainkan = true;
                
                // Jika bukan babak final, update bracket untuk babak berikutnya
                if (babak < hitungJumlahBabak()) {
                    updateBracketGugur(current, pemenang, babak + 1);
                } else {
                    cout << "\n****************************************\n";
                    cout << "SELAMAT! " << pemenang.nama << " MENJADI JUARA TURNAMEN " << namaTurnamen << "!\n";
                    cout << "****************************************\n";
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << ". Silakan coba lagi.\n";
                continue;
            }
        }
        current = current->next;
    }
    
    if (!adaPertandingan) {
        cout << "Tidak ada pertandingan yang siap dimainkan di babak ini.\n";
    }
}

// Fungsi untuk menyimpan hasil turnamen
void simpanHasilTurnamenGugur() {
    json turnamenData = loadTurnamen();
    
    // Buat data tim
    json timData = json::array();
    for (int i = 0; i < jumlahTim; i++) {
        json timObj = {
            {"nama", tim[i].nama},
            {"isEliminated", tim[i].isEliminated},
            {"skor", tim[i].skor}
        };
        timData.push_back(timObj);
    }
    
    // Buat data jadwal pertandingan
    json jadwalData = json::array();
    PertandinganGugur* current = jadwalG.head;
    
    while (current != nullptr) {
        json matchObj = {
            {"tim1", current->tim1.nama},
            {"tim2", current->tim2.nama},
            {"skor1", current->skor1},
            {"skor2", current->skor2},
            {"babak", current->babak},
            {"deskripsi", current->deskripsi},
            {"sudahDimainkan", current->sudahDimainkan}
        };
        jadwalData.push_back(matchObj);
        
        current = current->next;
    }
    
    // Update turnamen yang sudah ada
    bool turnamenDitemukan = false;
    for (auto& turnamen : turnamenData) {
        if (turnamen["nama"] == namaTurnamen) {
            turnamen["tim"] = timData;
            turnamen["jadwal"] = jadwalData;
            turnamen["jumlahMatch"] = jumlahMatchG;
            turnamen["sistem"] = "Knockout";
            turnamen["format"] = "Single Elimination";
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
            {"jumlahMatch", jumlahMatchG},
            {"format", "Single Elimination"},
            {"sistem", "Knockout"},
            {"tim", timData},
            {"jadwal", jadwalData}
        };
        
        turnamenData.push_back(turnamenObj);
    }
    
    saveTurnamen(turnamenData);
    cout << "\nTurnamen " << namaTurnamen << " berhasil disimpan!\n";
}

// Fungsi untuk membersihkan memori yang dialokasikan
void bersihkanMemoriGugur() {
    // Hapus array tim
    if (tim != nullptr) {
        delete[] tim;
        tim = nullptr;
    }
    
    // Hapus linked list jadwal
    PertandinganGugur* current = jadwalG.head;
    while (current != nullptr) {
        PertandinganGugur* temp = current;
        current = current->next;
        delete temp;
    }
    jadwalG.head = nullptr;
    jadwalG.tail = nullptr;
    jadwalG.jumlah = 0;
}

// Fungsi utama sistem gugur
void sistemGugur() {
    cout << "\n=== SISTEM GUGUR (KNOCKOUT) ===\n";
    
    // Load data turnamen terakhir
    if (!loadTurnamenGugurData()) {
        return; // Keluar jika tidak ada data turnamen
    }
    
    // Periksa jumlah tim
    if (jumlahTim < 2) {
        cout << "Jumlah tim minimal harus 2 untuk sistem gugur.\n";
        return;
    }
    
    // Input data tim
    inputDataTimGugur();
    
    // Buat bracket pertandingan
    buatBracketGugur();
    
    // Tampilkan jadwal pertandingan
    tampilkanJadwalPertandinganGugur();
    
    // Input skor untuk setiap babak pertandingan
    int jumlahBabak = hitungJumlahBabak();
    for (int babak = 1; babak <= jumlahBabak; babak++) {
        inputSkorBabakGugur(babak);
    }
    
    // Simpan hasil turnamen
    simpanHasilTurnamenGugur();
    
    // Bersihkan memori
    bersihkanMemoriGugur();
}