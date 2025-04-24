#include "systems/gugur_bye.h"
#include "systems/storage.h"

// Array dinamis untuk menyimpan tim
static TimBye* tim = nullptr;
// Linked list untuk menyimpan jadwal pertandingan
static JadwalBye jadwalB;

static int jumlahTim = 0, jumlahHari = 0, jumlahMatchB = 0;
static int jenisOlahraga = 0;
static string namaTurnamen;

// Fungsi untuk menghitung power of two terdekat (lebih besar)
int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

// Fungsi untuk menghitung power of two terdekat (lebih kecil)
int previousPowerOfTwo(int n) {
    int power = 1;
    while (power * 2 < n) {
        power *= 2;
    }
    return power;
}

// Fungsi untuk memuat data turnamen terakhir
// Fungsi untuk memuat data turnamen terakhir
bool loadTurnamenByeData() {
    json turnamenData = loadTurnamen();
    
    if (turnamenData.empty()) {
        cout << "Tidak ada data turnamen yang tersimpan.\n";
        return false;
    }
    
    // Ambil turnamen terakhir yang dibuat
    json lastTurnamenB = turnamenData[turnamenData.size() - 1];
    
    // Periksa apakah sistem pertandingan adalah "Gugur Bye"
    if (lastTurnamenB.contains("sistem") && lastTurnamenB["sistem"] != "Gugur Bye") {
        cout << "Sistem pertandingan bukan Gugur Bye. Program tidak dapat dijalankan.\n";
        return false;
    }
    
    // Load data turnamen - perbaiki kunci JSON
    namaTurnamen = lastTurnamenB.value("nama", "Turnamen Tanpa Nama");
    jenisOlahraga = lastTurnamenB.value("jenisOlahraga", 0); 
    jumlahTim = lastTurnamenB.value("jumlahTim", 0);
    jumlahHari = lastTurnamenB.value("jumlahHari", 1);
    
    cout << "Turnamen " << namaTurnamen << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    
    // Alokasi memori untuk array tim
    tim = new TimBye[jumlahTim];
    
    return true;
}

// Fungsi untuk menambahkan pertandingan ke linked list
void tambahPertandinganBye(TimBye tim1, TimBye tim2, bool isBye = false) {
    PertandinganBye* newNode = new PertandinganBye;
    newNode->tim1 = tim1;
    newNode->tim2 = tim2;
    newNode->skor1 = 0;
    newNode->skor2 = 0;
    newNode->isBye = isBye;
    newNode->deskripsi = tim1.nama + " vs " + tim2.nama;
    newNode->sudahDimainkan = false;
    newNode->next = nullptr;
    
    if (jadwalB.head == nullptr) {
        jadwalB.head = newNode;
        jadwalB.tail = newNode;
    } else {
        jadwalB.tail->next = newNode;
        jadwalB.tail = newNode;
    }
    
    jadwalB.jumlah++;
    jumlahMatchB++;
}

// Fungsi untuk input data tim
void inputDataTimBye() {
    cin.ignore(); // Bersihkan newline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "\nNama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
        tim[i].skor = 0;
        tim[i].isBye = false;
    }
}

// Fungsi untuk menampilkan jadwal pertandingan
void tampilkanJadwalPertandinganBye() {
    if (jadwalB.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = jumlahMatchB / jumlahHari;
    int sisa = jumlahMatchB % jumlahHari;
    
    PertandinganBye* current = jadwalB.head;
    int idx = 0;
    
    for (int h = 0; h < jumlahHari; h++) {
        int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
        if (sisa > 0) sisa--;
        cout << "\nHari " << h + 1 << ":\n";
        
        for (int m = 0; m < matchHariIni && current != nullptr; m++) {
            if (current->isBye) {
                cout << "- " << current->tim1.nama << " mendapat BYE\n";
            } else {
                cout << "- " << current->deskripsi << endl;
            }
            current = current->next;
            idx++;
        }
    }
}

// Fungsi untuk menentukan apakah perlu membuat bye
bool perluMembuatBye() {
    // Jika jumlah tim ganjil atau bukan power of 2, perlu membuat bye
    return (jumlahTim % 2 != 0) || (jumlahTim & (jumlahTim - 1)) != 0;
}

// Fungsi untuk membuat bracket sistem gugur
void buatBracketGugurBye() {
    // Tentukan apakah perlu bye
    bool adaBye = perluMembuatBye();
    
    // Tentukan jumlah tim yang akan mendapat bye
    int targetPower = nextPowerOfTwo(jumlahTim);
    int jumlahBye = targetPower - jumlahTim;
    
    // Tandai tim yang mendapat bye
    if (adaBye) {
        for (int i = jumlahTim - jumlahBye; i < jumlahTim; i++) {
            tim[i].isBye = true;
            cout << "Tim " << tim[i].nama << " mendapat BYE\n";
        }
    }
    
    // Buat bracket pertandingan babak pertama
    for (int i = 0; i < jumlahTim - jumlahBye; i += 2) {
        if (i + 1 < jumlahTim - jumlahBye) {
            tambahPertandinganBye(tim[i], tim[i + 1]);
        }
    }
    
    // Tambahkan tim bye ke bracket babak berikutnya
    for (int i = jumlahTim - jumlahBye; i < jumlahTim; i++) {
        TimBye byeTim = {"BYE", 0, true}; // Placeholder lawan
        tambahPertandinganBye(tim[i], byeTim, true);
    }
}

void inputSkorHariBye(int hari) {
    if (jadwalB.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = jumlahMatchB / jumlahHari;
    int sisa = jumlahMatchB % jumlahHari;
    
    int startIdx = 0;
    int sisaTemp = sisa;
    for (int h = 0; h < hari - 1; h++) {
        int matchHariIni = matchPerHari + (sisaTemp > 0 ? 1 : 0);
        if (sisaTemp > 0) sisaTemp--;
        startIdx += matchHariIni;
    }
    
    int matchHariIni = matchPerHari + (sisa > 0 && hari <= sisa ? 1 : 0);
    
    cout << "\n=== Input Skor Pertandingan Hari " << hari << " ===\n";
    
    PertandinganBye* current = jadwalB.head;
    for (int i = 0; i < startIdx && current != nullptr; i++) {
        current = current->next;
    }
    
    for (int i = 0; i < matchHariIni && current != nullptr; i++) {
        if (!current->isBye) {
            while (true) {
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
                    
                    if (current->skor1 == current->skor2) {
                        cout << "Skor imbang tidak diperbolehkan dalam sistem gugur, silakan input ulang.\n";
                        continue;
                    }
                    
                    break; // valid input, keluar dari loop
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << ". Silakan coba lagi.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            }
        } else {
            cout << "\n" << current->tim1.nama << " mendapat BYE\n";
        }
        
        current->sudahDimainkan = true;
        current = current->next;
    }
}

// Fungsi untuk menyimpan hasil turnamen
void simpanHasilTurnamenBye() {
    json turnamenData = loadTurnamen();
    
    // Buat data tim
    json timData = json::array();
    for (int i = 0; i < jumlahTim; i++) {
        json timObj = {
            {"nama", tim[i].nama},
            {"isBye", tim[i].isBye},
            {"skor", tim[i].skor}
        };
        timData.push_back(timObj);
    }
    
    // Buat data jadwal pertandingan
    json jadwalData = json::array();
    PertandinganBye* current = jadwalB.head;
    int hariKe = 1;
    int matchPerHari = jumlahMatchB / jumlahHari;
    int sisa = jumlahMatchB % jumlahHari;
    int matchHariIni = matchPerHari + (hariKe <= sisa ? 1 : 0);
    
    while (current != nullptr) {
        json matchObj = {
            {"tim1", current->tim1.nama},
            {"tim2", current->tim2.nama},
            {"skor1", current->skor1},
            {"skor2", current->skor2},
            {"isBye", current->isBye},
            {"deskripsi", current->deskripsi},
            {"sudahDimainkan", current->sudahDimainkan},
            {"hari", hariKe}
        };
        jadwalData.push_back(matchObj);
        
        current = current->next;
        matchHariIni--;
        
        if (matchHariIni == 0 && current != nullptr) {
            hariKe++;
            matchHariIni = matchPerHari + (hariKe <= sisa ? 1 : 0);
        }
    }
    
    // Update turnamen yang sudah ada
    bool turnamenDitemukan = false;
    for (auto& turnamen : turnamenData) {
        if (turnamen["nama"] == namaTurnamen) {
            turnamen["tim"] = timData;
            turnamen["jadwal"] = jadwalData;
            turnamen["jumlahMatch"] = jumlahMatchB;
            turnamen["sistem"] = "Gugur Bye";
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
            {"jumlahMatch", jumlahMatchB},
            {"format", "Single Elimination"},
            {"sistem", "Gugur Bye"},
            {"tim", timData},
            {"jadwal", jadwalData}
        };
        
        turnamenData.push_back(turnamenObj);
    }
    
    saveTurnamen(turnamenData);
    cout << "\nTurnamen " << namaTurnamen << " berhasil disimpan!\n";
}

// Fungsi untuk membersihkan memori yang dialokasikan
void bersihkanMemoriBye() {
    // Hapus array tim
    if (tim != nullptr) {
        delete[] tim;
        tim = nullptr;
    }
    
    // Hapus linked list jadwal
    PertandinganBye* current = jadwalB.head;
    while (current != nullptr) {
        PertandinganBye* temp = current;
        current = current->next;
        delete temp;
    }
    jadwalB.head = nullptr;
    jadwalB.tail = nullptr;
    jadwalB.jumlah = 0;
}

// Fungsi utama sistem gugur bye
void sistemGugurBye() {
    cout << "\n=== SISTEM GUGUR BYE ===\n";
    
    // Load data turnamen terakhir
    if (!loadTurnamenByeData()) {
        return; // Keluar jika tidak ada data turnamen
    }
    
    // Periksa jumlah tim
    if (jumlahTim < 3) {
        cout << "Jumlah tim minimal harus 3 untuk sistem gugur bye.\n";
        return;
    }
    
    // Input data tim
    inputDataTimBye();
    
    // Buat bracket pertandingan
    buatBracketGugurBye();
    
    // Tampilkan jadwal pertandingan
    tampilkanJadwalPertandinganBye();
    
    // Input skor untuk setiap hari pertandingan
    for (int hari = 1; hari <= jumlahHari; hari++) {
        inputSkorHariBye(hari);
    }
    
    // Simpan hasil turnamen
    simpanHasilTurnamenBye();
    
    // Bersihkan memori
    bersihkanMemoriBye();
}

// Fungsi utama sistem gugur bye - backward compatibility
int gugurBye() {
    sistemGugurBye();
    return 0;
}