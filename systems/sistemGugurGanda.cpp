#include "systems/sistemGugurGanda.h"
#include "systems/storage.h"

// Array dinamis untuk menyimpan tim
static TimGugurGanda* tim = nullptr;
// Linked list untuk menyimpan jadwal pertandingan
static JadwalGugurGanda jadwalGG;

static int jumlahTim = 0, jumlahHari = 0, jumlahMatchGG = 0;
static int jenisOlahraga = 0;
static string namaTurnamen;

// Fungsi untuk memuat data turnamen terakhir
bool loadTurnamenGugurGandaData() {
    json turnamenData = loadTurnamen();
    
    if (turnamenData.empty()) {
        cout << "Tidak ada data turnamen yang tersimpan.\n";
        return false;
    }
    
    // Ambil turnamen terakhir yang dibuat
    json lastTurnamenGG = turnamenData[turnamenData.size() - 1];
    
    // Periksa apakah sistem pertandingan adalah "Gugur Ganda"
    if (lastTurnamenGG.contains("sistem") && lastTurnamenGG["sistem"] != "Gugur Ganda") {
        cout << "Sistem pertandingan bukan Gugur Ganda. Program tidak dapat dijalankan.\n";
        return false;
    }
    
    // Load data turnamen
    namaTurnamen = lastTurnamenGG.value("nama", "Turnamen Tanpa Nama");
    jenisOlahraga = lastTurnamenGG.value("jenisOlahraga", 0);
    jumlahTim = lastTurnamenGG.value("jumlahTim", 0);
    jumlahHari = lastTurnamenGG.value("jumlahHari", 1);
    
    cout << "Turnamen " << namaTurnamen << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahraga << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    
    // Alokasi memori untuk array tim
    tim = new TimGugurGanda[jumlahTim];
    
    return true;
}

// Fungsi untuk menambahkan pertandingan ke linked list
void tambahPertandinganGugurGanda(TimGugurGanda tim1, TimGugurGanda tim2, int babak, bool isLoserBracket) {
    PertandinganGugurGanda* newNode = new PertandinganGugurGanda;
    newNode->tim1 = tim1;
    newNode->tim2 = tim2;
    newNode->skor1 = 0;
    newNode->skor2 = 0;
    newNode->babak = babak;
    newNode->isLoserBracket = isLoserBracket;
    newNode->deskripsi = tim1.nama + " vs " + tim2.nama + 
        (isLoserBracket ? " (Bracket Pecundang)" : " (Bracket Pemenang)");
    newNode->sudahDimainkan = false;
    newNode->next = nullptr;
    
    if (jadwalGG.head == nullptr) {
        jadwalGG.head = newNode;
        jadwalGG.tail = newNode;
    } else {
        jadwalGG.tail->next = newNode;
        jadwalGG.tail = newNode;
    }
    
    jadwalGG.jumlah++;
    jumlahMatchGG++;
}

// Fungsi untuk input data tim
void inputDataTimGugurGanda() {
    cin.ignore(); // Bersihkan newline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "\nNama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
        tim[i].skor = 0;
        tim[i].isEliminated = false;
        tim[i].kekalahan = 0;
    }
}

// Fungsi untuk mengacak susunan tim
void acakSusunanTimGugurGanda() {
    cout << "\nMengacak susunan tim...\n";
    srand(time(nullptr)); // Inisialisasi seed untuk random
    
    for (int i = jumlahTim - 1; i > 0; i--) {
        // Pilih indeks acak dari 0 hingga i
        int j = rand() % (i + 1);
        
        // Tukar tim[i] dengan tim[j]
        TimGugurGanda temp = tim[i];
        tim[i] = tim[j];
        tim[j] = temp;
    }
    
    cout << "Susunan tim berhasil diacak!\n";
}

// Fungsi untuk menghitung jumlah babak yang dibutuhkan di winner bracket
int hitungJumlahBabakWinnerBracket() {
    return ceil(log2(jumlahTim));
}

// Fungsi untuk membuat bracket sistem gugur ganda
void buatBracketGugurGanda() {
    // Acak susunan tim terlebih dahulu
    acakSusunanTimGugurGanda();
    
    // Hitung jumlah babak yang dibutuhkan di winner bracket
    int jumlahBabakWinner = hitungJumlahBabakWinnerBracket();
    
    cout << "\nJumlah babak di bracket pemenang: " << jumlahBabakWinner << endl;
    
    // Total pertandingan dalam sistem gugur ganda adalah 2*jumlahTim - 2
    int totalMatch = 2 * jumlahTim - 2;
    cout << "Total pertandingan: " << totalMatch << endl;
    
    // Buat bracket pemenang babak pertama
    int matchBabak1 = jumlahTim / 2;
    for (int i = 0; i < matchBabak1; i++) {
        tambahPertandinganGugurGanda(tim[i*2], tim[i*2+1], 1, false);
    }
}

// Fungsi untuk mendapatkan nama babak berdasarkan nomor babak dan jumlah babak
string getNamaBabak(int babak, int jumlahBabak, bool isLoserBracket) {
    if (isLoserBracket) {
        return "LOSER BRACKET BABAK " + to_string(babak);
    } else {
        if (babak == jumlahBabak) {
            return "FINAL";
        } else if (babak == jumlahBabak - 1) {
            return "SEMIFINAL";
        } else if (babak == jumlahBabak - 2) {
            return "PEREMPAT FINAL";
        } else {
            return "BABAK " + to_string(babak);
        }
    }
}

// Fungsi untuk menampilkan jadwal pertandingan
void tampilkanJadwalPertandinganGugurGanda() {
    if (jadwalGG.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = jumlahMatchGG / jumlahHari;
    if (matchPerHari == 0) matchPerHari = 1; // Minimal 1 match per hari
    
    int sisa = jumlahMatchGG % jumlahHari;
    
    // Cari babak tertinggi (final)
    int babakTertinggiWinner = 1;
    int babakTertinggiLoser = 1;
    PertandinganGugurGanda* temp = jadwalGG.head;
    while (temp != nullptr) {
        if (temp->isLoserBracket) {
            if (temp->babak > babakTertinggiLoser) {
                babakTertinggiLoser = temp->babak;
            }
        } else {
            if (temp->babak > babakTertinggiWinner) {
                babakTertinggiWinner = temp->babak;
            }
        }
        temp = temp->next;
    }
    
    // Tampilkan jadwal bracket pemenang per babak
    cout << "\n=== BRACKET PEMENANG ===\n";
    for (int b = 1; b <= babakTertinggiWinner; b++) {
        cout << "\n=== " << getNamaBabak(b, babakTertinggiWinner, false) << " ===\n";
        
        PertandinganGugurGanda* current = jadwalGG.head;
        int idx = 0;
        int hariKe = 1;
        
        while (current != nullptr) {
            if (!current->isLoserBracket && current->babak == b) {
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
    
    // Tampilkan jadwal bracket pecundang per babak
    if (babakTertinggiLoser > 0) {
        cout << "\n=== BRACKET PECUNDANG ===\n";
        for (int b = 1; b <= babakTertinggiLoser; b++) {
            cout << "\n=== " << getNamaBabak(b, babakTertinggiLoser, true) << " ===\n";
            
            PertandinganGugurGanda* current = jadwalGG.head;
            int idx = 0;
            int hariKe = 1;
            
            while (current != nullptr) {
                if (current->isLoserBracket && current->babak == b) {
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
    
    // Tampilkan grand final jika sudah ada
    temp = jadwalGG.head;
    bool foundGrandFinal = false;
    while (temp != nullptr) {
        if (temp->babak == babakTertinggiWinner + 1) {
            if (!foundGrandFinal) {
                cout << "\n=== GRAND FINAL ===\n";
                foundGrandFinal = true;
            }
            cout << "- " << temp->deskripsi << endl;
        }
        temp = temp->next;
    }
}

// Fungsi untuk menemukan tim berdasarkan nama
int findTimByNama(string nama) {
    for (int i = 0; i < jumlahTim; i++) {
        if (tim[i].nama == nama) {
            return i;
        }
    }
    return -1;
}

// Fungsi untuk update bracket pemenang ke babak berikutnya
TimGugurGanda updateBracketPemenang(PertandinganGugurGanda* pertandingan, TimGugurGanda pemenang, TimGugurGanda kalah, int babakBerikutnya) {
    // Cari dulu apakah sudah ada pertandingan di babak berikutnya yang menunggu
    PertandinganGugurGanda* current = jadwalGG.head;
    PertandinganGugurGanda* matchBerikutnya = nullptr;
    int index = 0;
    
    // Hitung berapa posisi match di babaknya
    PertandinganGugurGanda* temp = jadwalGG.head;
    while (temp != pertandingan) {
        if (temp->babak == pertandingan->babak && !temp->isLoserBracket) {
            index++;
        }
        temp = temp->next;
    }
    
    // Posisi di babak berikutnya
    int posisiBerikutnya = index / 2;
    
    // Cari apakah match di babak berikutnya sudah ada
    bool found = false;
    int countMatchBabakBerikutnya = 0;
    current = jadwalGG.head;
    while (current != nullptr) {
        if (current->babak == babakBerikutnya && !current->isLoserBracket) {
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
        TimGugurGanda placeholder = {"TBD", 0, false, 0}; // To Be Determined
        tambahPertandinganGugurGanda(pemenang, placeholder, babakBerikutnya, false);
    } else {
        // Jika sudah ada, update tim yang masuk
        if (matchBerikutnya->tim1.nama == "TBD") {
            matchBerikutnya->tim1 = pemenang;
            matchBerikutnya->deskripsi = pemenang.nama + " vs " + matchBerikutnya->tim2.nama + 
                (matchBerikutnya->isLoserBracket ? " (Bracket Pecundang)" : " (Bracket Pemenang)");
        } else {
            matchBerikutnya->tim2 = pemenang;
            matchBerikutnya->deskripsi = matchBerikutnya->tim1.nama + " vs " + pemenang.nama + 
                (matchBerikutnya->isLoserBracket ? " (Bracket Pecundang)" : " (Bracket Pemenang)");
        }
    }
    
    // Update kekalahan dan status tim yang kalah
    int indexKalah = findTimByNama(kalah.nama);
    if (indexKalah != -1) {
        tim[indexKalah].kekalahan++;
        
        // Jika sudah kalah 1 kali, pindahkan ke bracket pecundang
        if (tim[indexKalah].kekalahan == 1) {
            // Cari babak berikutnya di bracket pecundang
            int babakPecundang = 1;
            
            // Temukan tim placeholder di bracket pecundang
            TimGugurGanda placeholder = {"TBD", 0, false, 0};
            tambahPertandinganGugurGanda(tim[indexKalah], placeholder, babakPecundang, true);
        } 
        // Jika sudah kalah 2 kali, eliminasi
        else if (tim[indexKalah].kekalahan == 2) {
            tim[indexKalah].isEliminated = true;
        }
    }
    
    return pemenang;
}

// Fungsi untuk update bracket pecundang ke babak berikutnya
TimGugurGanda updateBracketPecundang(PertandinganGugurGanda* pertandingan, TimGugurGanda pemenang, TimGugurGanda kalah, int babakBerikutnya) {
    // Cari dulu apakah sudah ada pertandingan di babak berikutnya yang menunggu
    PertandinganGugurGanda* current = jadwalGG.head;
    PertandinganGugurGanda* matchBerikutnya = nullptr;
    int index = 0;
    
    // Hitung berapa posisi match di babaknya
    PertandinganGugurGanda* temp = jadwalGG.head;
    while (temp != pertandingan) {
        if (temp->babak == pertandingan->babak && temp->isLoserBracket) {
            index++;
        }
        temp = temp->next;
    }
    
    // Posisi di babak berikutnya
    int posisiBerikutnya = index / 2;
    
    // Cari apakah match di babak berikutnya sudah ada
    bool found = false;
    int countMatchBabakBerikutnya = 0;
    current = jadwalGG.head;
    while (current != nullptr) {
        if (current->babak == babakBerikutnya && current->isLoserBracket) {
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
        TimGugurGanda placeholder = {"TBD", 0, false, 0}; // To Be Determined
        tambahPertandinganGugurGanda(pemenang, placeholder, babakBerikutnya, true);
    } else {
        // Jika sudah ada, update tim yang masuk
        if (matchBerikutnya->tim1.nama == "TBD") {
            matchBerikutnya->tim1 = pemenang;
            matchBerikutnya->deskripsi = pemenang.nama + " vs " + matchBerikutnya->tim2.nama + 
                (matchBerikutnya->isLoserBracket ? " (Bracket Pecundang)" : " (Bracket Pemenang)");
        } else {
            matchBerikutnya->tim2 = pemenang;
            matchBerikutnya->deskripsi = matchBerikutnya->tim1.nama + " vs " + pemenang.nama + 
                (matchBerikutnya->isLoserBracket ? " (Bracket Pecundang)" : " (Bracket Pemenang)");
        }
    }
    
    // Update kekalahan dan status tim yang kalah
    int indexKalah = findTimByNama(kalah.nama);
    if (indexKalah != -1) {
        tim[indexKalah].kekalahan++;
        // Jika sudah kalah 2 kali (di bracket pecundang), eliminasi
        if (tim[indexKalah].kekalahan == 2) {
            tim[indexKalah].isEliminated = true;
        }
    }
    
    return pemenang;
}

// Fungsi untuk membuat grand final
void buatGrandFinal(TimGugurGanda pemenangWinner, TimGugurGanda pemenangLoser) {
    int babakFinal = hitungJumlahBabakWinnerBracket() + 1;
    tambahPertandinganGugurGanda(pemenangWinner, pemenangLoser, babakFinal, false);
}

// Fungsi untuk input skor pertandingan bracket pemenang pada babak tertentu
void inputSkorBabakPemenang(int babak) {
    if (jadwalGG.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    cout << "\n=== Input Skor Pertandingan " << getNamaBabak(babak, hitungJumlahBabakWinnerBracket(), false) << " ===\n";
    
    PertandinganGugurGanda* current = jadwalGG.head;
    bool adaPertandingan = false;
    
    while (current != nullptr) {
        if (!current->isLoserBracket && current->babak == babak && !current->sudahDimainkan && 
            current->tim1.nama != "TBD" && current->tim2.nama != "TBD") {
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
                TimGugurGanda pemenang, kalah;
                if (current->skor1 > current->skor2) {
                    cout << current->tim1.nama << " menang dengan skor " << current->skor1 << "-" << current->skor2 << endl;
                    pemenang = current->tim1;
                    pemenang.skor = current->skor1;
                    kalah = current->tim2;
                } else if (current->skor1 < current->skor2) {
                    cout << current->tim2.nama << " menang dengan skor " << current->skor2 << "-" << current->skor1 << endl;
                    pemenang = current->tim2;
                    pemenang.skor = current->skor2;
                    kalah = current->tim1;
                } else {
                    cout << "Skor imbang tidak diperbolehkan dalam sistem gugur." << endl;
                    cout << "Silakan input ulang skor atau tentukan pemenang dengan adu penalti.\n";
                    continue;
                }
                
                current->sudahDimainkan = true;
                
                // Update bracket untuk babak berikutnya dan bracket pecundang
                if (babak < hitungJumlahBabakWinnerBracket()) {
                    pemenang = updateBracketPemenang(current, pemenang, kalah, babak + 1);
                } else {
                    // Simpan pemenang untuk grand final
                    // (akan diimplementasikan setelah bracket pecundang selesai)
                    cout << "\n" << pemenang.nama << " masuk ke GRAND FINAL!\n";
                    
                    // Cek apakah sudah ada pemenang di bracket pecundang
                    PertandinganGugurGanda* pecundangFinal = nullptr;
                    PertandinganGugurGanda* temp = jadwalGG.head;
                    while (temp != nullptr) {
                        if (temp->isLoserBracket && temp->babak == hitungJumlahBabakWinnerBracket() && temp->sudahDimainkan) {
                            pecundangFinal = temp;
                            break;
                        }
                        temp = temp->next;
                    }
                    
                    if (pecundangFinal != nullptr) {
                        // Ambil pemenang bracket pecundang
                        TimGugurGanda pemenangPecundang;
                        if (pecundangFinal->skor1 > pecundangFinal->skor2) {
                            pemenangPecundang = pecundangFinal->tim1;
                        } else {
                            pemenangPecundang = pecundangFinal->tim2;
                        }
                        
                        // Buat grand final
                        buatGrandFinal(pemenang, pemenangPecundang);
                    }
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

// Fungsi untuk input skor pertandingan bracket pecundang pada babak tertentu
void inputSkorBabakPecundang(int babak) {
    if (jadwalGG.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    cout << "\n=== Input Skor Pertandingan " << getNamaBabak(babak, hitungJumlahBabakWinnerBracket(), true) << " ===\n";
    
    PertandinganGugurGanda* current = jadwalGG.head;
    bool adaPertandingan = false;
    
    while (current != nullptr) {
        if (current->isLoserBracket && current->babak == babak && !current->sudahDimainkan && 
            current->tim1.nama != "TBD" && current->tim2.nama != "TBD") {
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
                TimGugurGanda pemenang, kalah;
                if (current->skor1 > current->skor2) {
                    cout << current->tim1.nama << " menang dengan skor " << current->skor1 << "-" << current->skor2 << endl;
                    pemenang = current->tim1;
                    pemenang.skor = current->skor1;
                    kalah = current->tim2;
                } else if (current->skor1 < current->skor2) {
                    cout << current->tim2.nama << " menang dengan skor " << current->skor2 << "-" << current->skor1 << endl;
                    pemenang = current->tim2;
                    pemenang.skor = current->skor2;
                    kalah = current->tim1;
                } else {
                    cout << "Skor imbang tidak diperbolehkan dalam sistem gugur." << endl;
                    cout << "Silakan input ulang skor atau tentukan pemenang dengan adu penalti.\n";
                    continue;
                }
                
                current->sudahDimainkan = true;
                
                // Update bracket untuk babak berikutnya di bracket pecundang
                if (babak < hitungJumlahBabakWinnerBracket()) {
                    pemenang = updateBracketPecundang(current, pemenang, kalah, babak + 1);
                } else {
                    // Pemenang dari final bracket pecundang masuk ke grand final
                    cout << "\n" << pemenang.nama << " masuk ke GRAND FINAL dari bracket pecundang!\n";
                    
                    // Cek apakah sudah ada pemenang di bracket pemenang
                    PertandinganGugurGanda* pemenangFinal = nullptr;
                    PertandinganGugurGanda* temp = jadwalGG.head;
                    while (temp != nullptr) {
                        if (!temp->isLoserBracket && temp->babak == hitungJumlahBabakWinnerBracket() && temp->sudahDimainkan) {
                            pemenangFinal = temp;
                            break;
                        }
                        temp = temp->next;
                    }
                    
                    if (pemenangFinal != nullptr) {
                        // Ambil pemenang bracket pemenang
                        TimGugurGanda pemenangBracketUtama;
                        if (p