#include "systems/swiss.h"

// Array dinamis untuk menyimpan tim
static TimSwiss* tim = nullptr;
// Linked list untuk menyimpan jadwal pertandingan
static JadwalSwiss jadwalS;

static int jumlahTim = 0, jumlahRonde = 0, jumlahHari = 0, jumlahMatchS = 0;
static int jenisOlahraga = 0;
static string namaTurnamen;

// Fungsi untuk memuat data turnamen terakhir
bool loadTurnamenSwissData() {
    json turnamenData = loadTurnamen();
    
    if (turnamenData.empty()) {
        cout << "Tidak ada data turnamen yang tersimpan.\n";
        return false;
    }
    
    // Ambil turnamen terakhir yang dibuat
    json lastTurnamenS = turnamenData[turnamenData.size() - 1];
    
    // Periksa apakah sistem pertandingan adalah "Swiss"
    if (lastTurnamenS.contains("sistem") && lastTurnamenS["sistem"] != "Swiss") {
        cout << "Sistem pertandingan bukan Swiss. Program tidak dapat dijalankan.\n";
        return false;
    }
    
    // Load data turnamen
    namaTurnamen = lastTurnamenS.value("nama", "Turnamen Tanpa Nama");
    jenisOlahraga = lastTurnamenS.value("jenisOlahraga", 0);
    jumlahTim = lastTurnamenS.value("jumlahTim", 0);
    jumlahHari = lastTurnamenS.value("jumlahHari", 1);
    
    // Hitung jumlah ronde berdasarkan jumlah tim
    // Pada sistem Swiss, jumlah ronde adalah log2(jumlahTim) dibulatkan ke atas
    jumlahRonde = ceil(log2(jumlahTim));
    
    cout << "Turnamen " << namaTurnamen << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    cout << "Jumlah Ronde: " << jumlahRonde << "\n";
    
    // Alokasi memori untuk array tim
    tim = new TimSwiss[jumlahTim];
    
    return true;
}

// Fungsi untuk menambahkan pertandingan ke linked list
void tambahPertandinganSwiss(TimSwiss tim1, TimSwiss tim2, bool isBye = false) {
    PertandinganSwiss* newNode = new PertandinganSwiss;
    newNode->tim1 = tim1;
    newNode->tim2 = tim2;
    newNode->skor1 = 0;
    newNode->skor2 = 0;
    newNode->isBye = isBye;
    
    if (isBye) {
        newNode->deskripsi = tim1.nama + " mendapat BYE";
    } else {
        newNode->deskripsi = tim1.nama + " vs " + tim2.nama;
    }
    
    newNode->sudahDimainkan = false;
    newNode->next = nullptr;
    
    if (jadwalS.head == nullptr) {
        jadwalS.head = newNode;
        jadwalS.tail = newNode;
    } else {
        jadwalS.tail->next = newNode;
        jadwalS.tail = newNode;
    }
    
    jadwalS.jumlah++;
    jumlahMatchS++;
}

// Fungsi untuk input data tim
void inputDataTimSwiss() {
    cin.ignore(); // Bersihkan newline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "\nNama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
        
        // Inisialisasi data tim
        tim[i].poin = 0;
        tim[i].matchPlayed = 0;
        tim[i].byeRound = false;
    }
}

// Fungsi untuk mengacak urutan tim pada ronde pertama
void acakTimRondePertama() {
    // Fisher-Yates shuffle algorithm
    for (int i = jumlahTim - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Tukar tim[i] dengan tim[j]
        TimSwiss temp = tim[i];
        tim[i] = tim[j];
        tim[j] = temp;
    }
}

// Fungsi untuk memeriksa apakah dua tim sudah pernah bertemu
bool sudahPertemu(int tim1Idx, int tim2Idx) {
    for (int lawan : tim[tim1Idx].lawan) {
        if (lawan == tim2Idx) {
            return true;
        }
    }
    return false;
}

// Fungsi untuk mengurutkan tim berdasarkan poin
void urutkanTimBerdasarkanPoin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = 0; j < jumlahTim - i - 1; j++) {
            if (tim[j].poin < tim[j + 1].poin) {
                // Tukar tim
                TimSwiss temp = tim[j];
                tim[j] = tim[j + 1];
                tim[j + 1] = temp;
            }
        }
    }
}

// Fungsi untuk membuat pasangan pertandingan untuk satu ronde
void buatPasanganRonde(int ronde) {
    cout << "\n--- Membuat pasangan untuk Ronde " << ronde + 1 << " ---\n";
    
    // Pada ronde pertama, acak urutan tim
    if (ronde == 0) {
        acakTimRondePertama();
    } else {
        // Pada ronde berikutnya, urutkan tim berdasarkan poin
        urutkanTimBerdasarkanPoin();
    }
    
    // Array untuk menandai tim yang sudah dipasangkan pada ronde ini
    bool* sudahDipasangkan = new bool[jumlahTim];
    for (int i = 0; i < jumlahTim; i++) {
        sudahDipasangkan[i] = false;
    }
    
    // Jika jumlah tim ganjil, satu tim akan mendapat bye
    bool adaBye = (jumlahTim % 2 != 0);
    
    // Proses untuk membuat pasangan:
    // 1. Iterasi melalui semua tim yang belum dipasangkan
    // 2. Untuk setiap tim, cari lawan yang memiliki poin sama dan belum pernah bertemu
    // 3. Jika tidak ada lawan dengan poin sama, cari lawan dengan poin terdekat
    
    for (int i = 0; i < jumlahTim; i++) {
        if (sudahDipasangkan[i]) continue;
        
        // Tandai tim ini sebagai sudah dipasangkan
        sudahDipasangkan[i] = true;
        
        // Jika jumlah tim ganjil dan ini tim terakhir, beri bye
        if (adaBye && i == jumlahTim - 1) {
            // Cari tim yang belum pernah mendapat bye
            bool foundBye = false;
            for (int j = jumlahTim - 1; j >= 0; j--) {
                if (!tim[j].byeRound && !sudahDipasangkan[j]) {
                    // Beri bye ke tim ini
                    tim[j].byeRound = true;
                    tim[j].poin += 1; // Bye biasanya memberi 1 poin
                    
                    // Tambahkan ke jadwal sebagai bye
                    TimSwiss byeTim = {"BYE", 0, 0, false};
                    tambahPertandinganSwiss(tim[j], byeTim, true);
                    
                    sudahDipasangkan[j] = true;
                    foundBye = true;
                    break;
                }
            }
            
            // Jika semua tim sudah pernah mendapat bye, berikan ke tim dengan poin terendah
            if (!foundBye) {
                // Cari tim dengan poin terendah yang belum dipasangkan
                int lowestPointIdx = -1;
                int lowestPoint = INT_MAX;
                for (int j = jumlahTim - 1; j >= 0; j--) {
                    if (!sudahDipasangkan[j] && tim[j].poin < lowestPoint) {
                        lowestPoint = tim[j].poin;
                        lowestPointIdx = j;
                    }
                }
                
                if (lowestPointIdx != -1) {
                    // Beri bye ke tim ini
                    tim[lowestPointIdx].byeRound = true;
                    tim[lowestPointIdx].poin += 1;
                    
                    // Tambahkan ke jadwal sebagai bye
                    TimSwiss byeTim = {"BYE", 0, 0, false};
                    tambahPertandinganSwiss(tim[lowestPointIdx], byeTim, true);
                    
                    sudahDipasangkan[lowestPointIdx] = true;
                }
            }
            continue;
        }
        
        // Cari lawan yang memiliki poin sama dan belum pernah bertemu
        int lawan = -1;
        for (int j = i + 1; j < jumlahTim; j++) {
            if (!sudahDipasangkan[j] && tim[j].poin == tim[i].poin && !sudahPertemu(i, j)) {
                lawan = j;
                break;
            }
        }
        
        // Jika tidak ada lawan dengan poin sama, cari lawan dengan poin terdekat
        if (lawan == -1) {
            int poinTerdekat = INT_MAX;
            for (int j = 0; j < jumlahTim; j++) {
                if (!sudahDipasangkan[j] && j != i && !sudahPertemu(i, j)) {
                    int selisihPoin = abs(tim[j].poin - tim[i].poin);
                    if (selisihPoin < poinTerdekat) {
                        poinTerdekat = selisihPoin;
                        lawan = j;
                    }
                }
            }
        }
        
        // Jika lawan ditemukan, buat pertandingan
        if (lawan != -1) {
            // Tambahkan ke daftar lawan dari kedua tim
            tim[i].lawan.push_back(lawan);
            tim[lawan].lawan.push_back(i);
            
            // Tambahkan ke jadwal pertandingan
            tambahPertandinganSwiss(tim[i], tim[lawan]);
            
            // Tandai lawan sebagai sudah dipasangkan
            sudahDipasangkan[lawan] = true;
        }
    }
    
    delete[] sudahDipasangkan;
}

// Fungsi untuk menampilkan jadwal pertandingan
void tampilkanJadwalPertandinganSwiss() {
    if (jadwalS.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = ceil((double)jumlahMatchS / jumlahHari);
    
    PertandinganSwiss* current = jadwalS.head;
    int hariKe = 1;
    int matchHariIni = 0;
    
    while (current != nullptr) {
        if (matchHariIni == 0) {
            cout << "\nHari " << hariKe << ":\n";
        }
        
        if (current->isBye) {
            cout << "- " << current->deskripsi << endl;
        } else {
            cout << "- " << current->deskripsi << endl;
        }
        
        current = current->next;
        matchHariIni++;
        
        if (matchHariIni == matchPerHari) {
            matchHariIni = 0;
            hariKe++;
            if (hariKe > jumlahHari) break;
        }
    }
}

// Fungsi untuk input skor pertandingan pada hari tertentu
void inputSkorHariSwiss(int hari) {
    if (jadwalS.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = ceil((double)jumlahMatchS / jumlahHari);
    
    // Hitung indeks awal untuk hari tertentu
    int startIdx = (hari - 1) * matchPerHari;
    int endIdx = min(startIdx + matchPerHari, jumlahMatchS);
    
    cout << "\n=== Input Skor Pertandingan Hari " << hari << " ===\n";
    
    // Navigasi ke pertandingan pertama hari ini
    PertandinganSwiss* current = jadwalS.head;
    for (int i = 0; i < startIdx && current != nullptr; i++) {
        current = current->next;
    }
    
    // Input skor untuk setiap pertandingan hari ini
    for (int i = startIdx; i < endIdx && current != nullptr; i++) {
        if (!current->isBye) {
            bool inputValid = false;
            while (!inputValid) {
                try {
                    cout << "\nPertandingan: " << current->deskripsi << endl;
                    cout << "Masukkan skor " << current->tim1.nama << ": ";
                    
                    string input;
                    cin >> input;
                    
                    // Verifikasi input adalah angka
                    for (char c : input) {
                        if (!isdigit(c)) {
                            throw runtime_error("Input harus berupa angka");
                        }
                    }
                    
                    current->skor1 = stoi(input);
                    
                    cout << "Masukkan skor " << current->tim2.nama << ": ";
                    cin >> input;
                    
                    // Verifikasi input adalah angka
                    for (char c : input) {
                        if (!isdigit(c)) {
                            throw runtime_error("Input harus berupa angka");
                        }
                    }
                    
                    current->skor2 = stoi(input);
                    
                    // Update poin berdasarkan hasil
                    // Cari indeks tim dalam array
                    int idxTim1 = -1, idxTim2 = -1;
                    for (int j = 0; j < jumlahTim; j++) {
                        if (tim[j].nama == current->tim1.nama) idxTim1 = j;
                        if (tim[j].nama == current->tim2.nama) idxTim2 = j;
                    }
                    
                    if (idxTim1 != -1 && idxTim2 != -1) {
                        tim[idxTim1].matchPlayed++;
                        tim[idxTim2].matchPlayed++;
                        
                        // Update poin berdasarkan jenis olahraga
                        if (jenisOlahraga == 11) { // Catur
                            if (current->skor1 > current->skor2) {
                                tim[idxTim1].poin += 1;
                                cout << current->tim1.nama << " menang dengan skor " << current->skor1 << "-" << current->skor2 << endl;
                            } else if (current->skor1 < current->skor2) {
                                tim[idxTim2].poin += 1;
                                cout << current->tim2.nama << " menang dengan skor " << current->skor2 << "-" << current->skor1 << endl;
                            } else { // Draw
                                tim[idxTim1].poin += 0.5;
                                tim[idxTim2].poin += 0.5;
                                cout << "Hasil imbang dengan skor " << current->skor1 << "-" << current->skor2 << endl;
                            }
                        } else { // E-sports, Kartu, Go
                            if (current->skor1 > current->skor2) {
                                tim[idxTim1].poin += 1;
                                cout << current->tim1.nama << " menang dengan skor " << current->skor1 << "-" << current->skor2 << endl;
                            } else if (current->skor1 < current->skor2) {
                                tim[idxTim2].poin += 1;
                                cout << current->tim2.nama << " menang dengan skor " << current->skor2 << "-" << current->skor1 << endl;
                            } else {
                                cout << "Skor imbang tidak diperbolehkan, silakan input ulang.\n";
                                continue;
                            }
                        }
                    }
                    
                    inputValid = true;
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << ". Silakan coba lagi.\n";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }
            }
        } else {
            cout << "\n" << current->deskripsi << " (Otomatis mendapat 1 poin)\n";
        }
        
        current->sudahDimainkan = true;
        current = current->next;
    }
}

// Fungsi untuk menampilkan klasemen
void tampilkanKlasemenSwiss() {
    // Urutkan tim berdasarkan poin
    urutkanTimBerdasarkanPoin();
    
    cout << "\n===== KLASEMEN SISTEM SWISS =====\n";
    cout << left << setw(20) << "Nama Tim" << setw(15) << "Match Played" 
         << setw(15) << "Poin" << endl;
    cout << string(50, '-') << endl;
    
    for (int i = 0; i < jumlahTim; i++) {
        cout << left << setw(20) << tim[i].nama << setw(15) << tim[i].matchPlayed 
             << setw(15) << tim[i].poin << endl;
    }
    
    cout << string(50, '-') << endl;
}

// Fungsi untuk menampilkan pemenang
void tampilkanPemenangSwiss() {
    // Urutkan tim berdasarkan poin
    urutkanTimBerdasarkanPoin();
    
    cout << "\n****************************************\n";
    cout << "JUARA TURNAMEN SWISS: " << tim[0].nama << endl;
    cout << "Total Poin: " << tim[0].poin << endl;
    cout << "Match Played: " << tim[0].matchPlayed << endl;
    cout << "****************************************\n";
    
    // Tampilkan runner-up
    if (jumlahTim > 1) {
        cout << "Runner-up: " << tim[1].nama << " (Poin: " << tim[1].poin << ")\n";
    }
    
    // Tampilkan tempat ketiga
    if (jumlahTim > 2) {
        cout << "Tempat Ketiga: " << tim[2].nama << " (Poin: " << tim[2].poin << ")\n";
    }
}

// Fungsi untuk menyimpan hasil turnamen
void simpanHasilTurnamenSwiss() {
    json turnamenData = loadTurnamen();
    
    // Buat data tim
    json timData = json::array();
    for (int i = 0; i < jumlahTim; i++) {
        json timObj = {
            {"nama", tim[i].nama},
            {"poin", tim[i].poin},
            {"matchPlayed", tim[i].matchPlayed},
            {"byeRound", tim[i].byeRound}
        };
        
        // Tambahkan daftar lawan
        json lawanData = json::array();
        for (int lawan : tim[i].lawan) {
            lawanData.push_back(lawan);
        }
        timObj["lawan"] = lawanData;
        
        timData.push_back(timObj);
    }
    
    // Buat data jadwal pertandingan
    json jadwalData = json::array();
    PertandinganSwiss* current = jadwalS.head;
    int hariKe = 1;
    int matchPerHari = ceil((double)jumlahMatchS / jumlahHari);
    int matchHariIni = 0;
    
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
        matchHariIni++;
        
        if (matchHariIni == matchPerHari) {
            matchHariIni = 0;
            hariKe++;
        }
    }
    
    // Update turnamen yang sudah ada
    bool turnamenDitemukan = false;
    for (auto& turnamen : turnamenData) {
        if (turnamen["nama"] == namaTurnamen) {
            turnamen["tim"] = timData;
            turnamen["jadwal"] = jadwalData;
            turnamen["jumlahMatch"] = jumlahMatchS;
            turnamen["sistem"] = "Swiss";
            turnamen["format"] = "Swiss System";
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
            {"jumlahMatch", jumlahMatchS},
            {"format", "Swiss System"},
            {"sistem", "Swiss"},
            {"tim", timData},
            {"jadwal", jadwalData}
        };
        
        turnamenData.push_back(turnamenObj);
    }
    
    saveTurnamen(turnamenData);
    cout << "\nTurnamen " << namaTurnamen << " berhasil disimpan!\n";
}

// Fungsi untuk membersihkan memori yang dialokasikan
void bersihkanMemoriSwiss() {
    // Hapus array tim
    if (tim != nullptr) {
        delete[] tim;
        tim = nullptr;
    }
    
    // Hapus linked list jadwal
    PertandinganSwiss* current = jadwalS.head;
    while (current != nullptr) {
        PertandinganSwiss* temp = current;
        current = current->next;
        delete temp;
    }
    jadwalS.head = nullptr;
    jadwalS.tail = nullptr;
    jadwalS.jumlah = 0;
}

// Fungsi utama sistem Swiss
void sistemSwiss() {
    cout << "\n=== SISTEM SWISS ===\n";
    
    // Load data turnamen terakhir
    if (!loadTurnamenSwissData()) {
        return; // Keluar jika tidak ada data turnamen
    }
    
    // Periksa jenis olahraga (11, 14-19 dapat menggunakan sistem Swiss)
    if (jenisOlahraga != 11 && !(jenisOlahraga >= 14 && jenisOlahraga <= 19)) {
        cout << "Cabang olahraga ini tidak mendukung sistem Swiss. Hanya cabang 11 (Catur), 14-19 (E-sports, Kartu, Go) yang dapat menggunakan sistem Swiss.\n";
        return;
    }
    
    // Periksa jumlah tim
    if (jumlahTim < 4) {
        cout << "Jumlah tim minimal harus 4 untuk sistem Swiss.\n";
        return;
    }
    
    // Input data tim
    inputDataTimSwiss();
    
    // Inisialisasi random seed untuk pengacakan tim pada ronde pertama
    srand(time(nullptr));
    
    // Buat pasangan untuk setiap ronde
    for (int ronde = 0; ronde < jumlahRonde; ronde++) {
        cout << "\n=== RONDE " << ronde + 1 << " ===\n";
        buatPasanganRonde(ronde);
        
        // Distribusikan pertandingan ke hari-hari yang tersedia
        cout << "\n--- Jadwal Pertandingan ---\n";
        tampilkanJadwalPertandinganSwiss();
        
        // Tampilkan klasemen sementara setelah setiap ronde
        if (ronde > 0) {
            tampilkanKlasemenSwiss();
        }
    }
    
    // Input skor untuk setiap hari pertandingan
    for (int hari = 1; hari <= jumlahHari; hari++) {
        inputSkorHariSwiss(hari);
    }
    
    // Tampilkan klasemen akhir dan pemenang
    cout << "\n=== Klasemen Akhir ===\n";
    tampilkanKlasemenSwiss();
    tampilkanPemenangSwiss();
    
    // Simpan hasil turnamen
    simpanHasilTurnamenSwiss();
    
    // Bersihkan memori
    bersihkanMemoriSwiss();
}

// Fungsi utama sistem Swiss - backward compatibility
int swiss() {
    sistemSwiss();
    return 0;
}