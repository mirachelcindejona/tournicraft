#include "systems/ligaMix.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "systems/storage.h"
#include <regex>
#include <algorithm>

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
    cout << "Cabang Olahraga: " << (jenisOlahraga == 1 ? "Sepak Bola" : 
                                 jenisOlahraga == 2 ? "Futsal" : 
                                 jenisOlahraga == 3 ? "Handball" :
                                 jenisOlahraga == 4 ? "Tenis Meja" :
                                 jenisOlahraga == 5 ? "Voli" :
                                 jenisOlahraga == 6 ? "Badminton" : "Tidak diketahui") << "\n";
    cout << "Jumlah Tim: " << jumlahTim << "\n";
    cout << "Jumlah Hari: " << jumlahHari << "\n";
    
    // Alokasi memori untuk array tim
    tim = new TimLiga[jumlahTim];
    
    return true;
}

// Fungsi untuk menambahkan pertandingan ke linked list
void tambahPertandingan(int tim1, int tim2, bool kandang) {
    PertandinganNode* newNode = new PertandinganNode;
    newNode->tim1 = tim1;
    newNode->tim2 = tim2;
    newNode->skor1 = 0;
    newNode->skor2 = 0;
    newNode->kandang = kandang; // Menandai apakah ini pertandingan kandang tim1
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
    regex validTeamNameRegex("^[a-zA-Z0-9 ]+$");
    for (int i = 0; i < jumlahTim; i++) {
        // Reset statistik tim
        tim[i].main = 0;
        tim[i].menang = 0;
        tim[i].seri = 0;
        tim[i].kalah = 0;
        tim[i].poin = 0;
        tim[i].golCetak = 0;
        tim[i].golKebobolan = 0;
        tim[i].selisihGol = 0;
        tim[i].setMenang = 0;
        tim[i].setKalah = 0;
        tim[i].poinCetak = 0;
        tim[i].poinKebobolan = 0;
        
        string inputNama;
        while (true) {
            cout << "\nNama tim ke-" << i + 1 << ": ";
            getline(cin, inputNama);
            if (inputNama.empty()) {
                cout << "Nama tim tidak boleh kosong! Silakan masukkan nama tim.\n";
                continue;
            }
            if (!regex_match(inputNama, validTeamNameRegex)) {
                cout << "Nama tim tidak boleh mengandung simbol! Silakan masukkan nama tim yang valid.\n";
                continue;
            }
            
            // Cek nama tim duplikat
            bool duplikat = false;
            for (int j = 0; j < i; j++) {
                if (inputNama == tim[j].nama) {
                    cout << "Nama tim tidak boleh sama dengan tim lain. Silakan masukkan nama yang berbeda.\n";
                    duplikat = true;
                    break;
                }
            }
            if (duplikat) continue;
            
            tim[i].nama = inputNama;
            break;
        }
    }
}

// Implementasi algoritma round robin Berger
void buatJadwalBerger(bool doubleRound) {
    int n = jumlahTim;
    int rounds = n - 1;
    int matches_per_round = n / 2;
    
    // Jika jumlah tim ganjil, tambahkan tim "dummy"
    bool ada_dummy = false;
    if (n % 2 != 0) {
        n++;
        ada_dummy = true;
        rounds = n - 1;
        matches_per_round = n / 2;
    }
    
    vector<int> teams(n);
    for (int i = 0; i < n; i++) {
        teams[i] = i;
    }
    
    for (int round = 0; round < rounds; round++) {
        for (int match = 0; match < matches_per_round; match++) {
            int home = teams[match];
            int away = teams[n - 1 - match];
            
            // Skip jika ada tim dummy
            if (ada_dummy && (home == n - 1 || away == n - 1)) {
                continue;
            }
            
            // Alternasi kandang dan tandang untuk keseimbangan
            if ((round + match) % 2 == 0) {
                tambahPertandingan(home, away, true);  // tim1 sebagai tuan rumah
            } else {
                tambahPertandingan(away, home, true);  // tim2 sebagai tuan rumah
            }
        }
        
        // Rotasi tim untuk round berikutnya (algoritma Berger)
        // Tim pertama tetap, yang lain dirotasi
        vector<int> new_teams(n);
        new_teams[0] = teams[0];
        new_teams[1] = teams[n - 1];
        for (int i = 2; i < n; i++) {
            new_teams[i] = teams[i - 1];
        }
        teams = new_teams;
    }
    
    // Tambahkan putaran kedua jika Double Round Robin
    if (doubleRound) {
        PertandinganNode* current = jadwal.head;
        int counter = 0;
        while (current != nullptr && counter < jumlahMatch) {
            tambahPertandingan(current->tim2, current->tim1, true);
            current = current->next;
            counter++;
        }
    }
}

// Fungsi untuk membuat jadwal single round robin
void buatJadwalSingleRoundRobin() {
    buatJadwalBerger(false);
}

// Fungsi untuk membuat jadwal double round robin
void buatJadwalDoubleRoundRobin() {
    buatJadwalBerger(true);
}

// Fungsi untuk menampilkan jadwal pertandingan
void tampilkanJadwalPertandingan() {
    if (jadwal.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = ceil((double)jumlahMatch / jumlahHari);
    
    PertandinganNode* current = jadwal.head;
    int idx = 0;
    
    for (int h = 0; h < jumlahHari; h++) {
        cout << "\nHari " << h + 1 << ":\n";
        cout << "---------------------------------\n";
        
        for (int m = 0; m < matchPerHari && current != nullptr; m++) {
            cout << "- " << tim[current->tim1].nama << " vs " << tim[current->tim2].nama;
            
            if (current->sudahDimainkan) {
                cout << " (" << current->skor1 << " - " << current->skor2 << ")";
            }
            
            cout << endl;
            current = current->next;
            idx++;
        }
    }
}

// Fungsi untuk validasi skor berdasarkan jenis olahraga
bool validasiSkor(int jenis, int skor1, int skor2, string& pesanError) {
    if (skor1 < 0 || skor2 < 0) {
        pesanError = "Skor tidak boleh negatif.";
        return false;
    }
    
    if (jenis == 5) { // Voli
        // Validasi untuk voli (pemenang harus mencapai 3 set)
        if (skor1 > 3 || skor2 > 3) {
            pesanError = "Dalam voli, maksimal set yang bisa dimenangkan adalah 3.";
            return false;
        }
        
        if (skor1 < 3 && skor2 < 3) {
            pesanError = "Salah satu tim harus memenangkan minimal 3 set dalam voli.";
            return false;
        }
        
        if (skor1 == 3 && skor2 == 3) {
            pesanError = "Kedua tim tidak bisa memenangkan 3 set dalam satu pertandingan voli.";
            return false;
        }
    }
    
    return true;
}

// Fungsi untuk menemukan hasil head-to-head antara dua tim
void getHeadToHead(int tim1, int tim2, int& menang1, int& menang2, int& seri) {
    menang1 = 0;
    menang2 = 0;
    seri = 0;
    
    PertandinganNode* current = jadwal.head;
    while (current != nullptr) {
        if (current->sudahDimainkan) {
            if ((current->tim1 == tim1 && current->tim2 == tim2) || 
                (current->tim1 == tim2 && current->tim2 == tim1)) {
                
                int skor_tim1 = (current->tim1 == tim1) ? current->skor1 : current->skor2;
                int skor_tim2 = (current->tim1 == tim1) ? current->skor2 : current->skor1;
                
                if (skor_tim1 > skor_tim2) {
                    menang1++;
                } else if (skor_tim1 < skor_tim2) {
                    menang2++;
                } else {
                    seri++;
                }
            }
        }
        current = current->next;
    }
}

// Fungsi untuk memperbarui statistik tim berdasarkan hasil pertandingan
void updateStatistikTim(PertandinganNode* match) {
    int tim1 = match->tim1;
    int tim2 = match->tim2;
    
    tim[tim1].main++;
    tim[tim2].main++;
    
    // Update statistik umum untuk semua olahraga
    if (jenisOlahraga == 1 || jenisOlahraga == 2) { // Sepak Bola / Futsal
        tim[tim1].golCetak += match->skor1;
        tim[tim1].golKebobolan += match->skor2;
        tim[tim2].golCetak += match->skor2;
        tim[tim2].golKebobolan += match->skor1;
        
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
        tim[tim1].golCetak += match->skor1;
        tim[tim1].golKebobolan += match->skor2;
        tim[tim2].golCetak += match->skor2;
        tim[tim2].golKebobolan += match->skor1;
        
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
        
        // Update statistik set
        tim[tim1].setMenang += match->skor1;
        tim[tim1].setKalah += match->skor2;
        tim[tim2].setMenang += match->skor2;
        tim[tim2].setKalah += match->skor1;
    } else if (jenisOlahraga == 5) { // Voli
        // Update statistik set
        tim[tim1].setMenang += match->skor1;
        tim[tim1].setKalah += match->skor2;
        tim[tim2].setMenang += match->skor2;
        tim[tim2].setKalah += match->skor1;
        
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
    
    // Update selisih gol/set untuk semua olahraga
    for (int i = 0; i < jumlahTim; i++) {
        if (jenisOlahraga <= 3) { // Olahraga dengan gol
            tim[i].selisihGol = tim[i].golCetak - tim[i].golKebobolan;
        } else { // Olahraga dengan set
            tim[i].selisihGol = tim[i].setMenang - tim[i].setKalah;
        }
    }
}

// Fungsi untuk input skor pertandingan pada hari tertentu
void inputSkorHari(int hari) {
    if (jadwal.jumlah == 0) {
        cout << "Belum ada jadwal pertandingan.\n";
        return;
    }
    
    int matchPerHari = ceil((double)jumlahMatch / jumlahHari);
    
    // Hitung indeks awal untuk hari tertentu
    int startIdx = (hari - 1) * matchPerHari;
    
    cout << "\n=== Input Skor Pertandingan Hari " << hari << " ===\n";
    cout << "-----------------------------------------\n";
    
    // Navigasi ke pertandingan pertama hari ini
    PertandinganNode* current = jadwal.head;
    for (int i = 0; i < startIdx && current != nullptr; i++) {
        current = current->next;
    }
    
    // Input skor untuk setiap pertandingan hari ini
    for (int i = 0; i < matchPerHari && current != nullptr; i++) {
        int skor1, skor2;
        string pesanError;
        
        cout << "\n" << tim[current->tim1].nama << " vs " << tim[current->tim2].nama << "\n";
        
        bool skorValid = false;
        while (!skorValid) {
            // Input skor tim 1
            while (true) {
                cout << "Skor " << tim[current->tim1].nama << ": ";
                if (!(cin >> skor1)) {
                    cout << "Input tidak valid. Harap masukkan angka.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                break;
            }
            
            // Input skor tim 2
            while (true) {
                cout << "Skor " << tim[current->tim2].nama << ": ";
                if (!(cin >> skor2)) {
                    cout << "Input tidak valid. Harap masukkan angka.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                break;
            }
            
            // Validasi skor berdasarkan jenis olahraga
            skorValid = validasiSkor(jenisOlahraga, skor1, skor2, pesanError);
            if (!skorValid) {
                cout << "Error: " << pesanError << " Silakan input ulang.\n";
            }
        }
        
        current->skor1 = skor1;
        current->skor2 = skor2;
        current->sudahDimainkan = true;
        
        updateStatistikTim(current);
        current = current->next;
    }
    
    // Sesuaikan perhitungan jumlah pertandingan sudah dimainkan
    int playedMatches = 0;
    PertandinganNode* temp = jadwal.head;
    while (temp != nullptr) {
        if (temp->sudahDimainkan) {
            playedMatches++;
        }
        temp = temp->next;
    }
}

// Fungsi untuk membandingkan tim untuk pengurutan klasemen
bool compareTeams(const TimLiga& a, const TimLiga& b) {
    // Prioritas 1: Poin
    if (a.poin != b.poin) {
        return a.poin > b.poin;
    }
    
    // Prioritas 2: Tergantung jenis olahraga
    if (jenisOlahraga <= 3) { // Sepak Bola, Futsal, Handball
        // Selisih gol
        if (a.selisihGol != b.selisihGol) {
            return a.selisihGol > b.selisihGol;
        }
        
        // Jumlah gol yang dicetak
        if (a.golCetak != b.golCetak) {
            return a.golCetak > b.golCetak;
        }
        
        // Jumlah kemenangan
        if (a.menang != b.menang) {
            return a.menang > b.menang;
        }
    } else { // Tenis Meja, Voli, Badminton
        // Jumlah kemenangan
        if (a.menang != b.menang) {
            return a.menang > b.menang;
        }
        
        // Rasio set
        if (a.selisihGol != b.selisihGol) {
            return a.selisihGol > b.selisihGol;
        }
        
        // Jumlah set yang dimenangkan
        if (a.setMenang != b.setMenang) {
            return a.setMenang > b.setMenang;
        }
    }
    
    // Pertimbangan pertemuan langsung (head-to-head)
    int menang_a = 0, menang_b = 0, seri = 0;
    
    // Cari indeks dari tim A dan B
    int idxA = -1, idxB = -1;
    for (int i = 0; i < jumlahTim; i++) {
        if (tim[i].nama == a.nama) idxA = i;
        if (tim[i].nama == b.nama) idxB = i;
    }
    
    if (idxA != -1 && idxB != -1) {
        getHeadToHead(idxA, idxB, menang_a, menang_b, seri);
        
        if (menang_a != menang_b) {
            return menang_a > menang_b;
        }
    }
    
    // Jika masih sama, jumlah seri lebih sedikit (lebih banyak hasil pasti)
    if (a.seri != b.seri) {
        return a.seri < b.seri;
    }
    
    // Jika semua sama, urut berdasarkan nama tim (untuk stabilitas pengurutan)
    return a.nama < b.nama;
}

// Fungsi untuk mengurutkan klasemen
void urutkanKlasemen() {
    vector<TimLiga> timVector(tim, tim + jumlahTim);
    
    // Gunakan algoritma sort dengan fungsi compare custom
    sort(timVector.begin(), timVector.end(), compareTeams);
    
    // Copy kembali ke array tim
    for (int i = 0; i < jumlahTim; i++) {
        tim[i] = timVector[i];
    }
}

// Fungsi untuk menampilkan klasemen dengan tabel yang rapi
void tampilkanKlasemen() {
    urutkanKlasemen();
    
    if (jenisOlahraga <= 3) { // Sepak Bola, Futsal, Handball
        // Menampilkan batas atas tabel
        cout << "\n+-----+---------------+-----+-----+-----+-----+------+-----+-----+------+\n";
        cout << "| No. |      Tim      |  M  |  W  |  D  |  L  | Pts  |  GF |  GA |  GD  |\n";
        cout << "+-----+---------------+-----+-----+-----+-----+------+-----+-----+------+\n";
        
        // Isi tabel
        for (int i = 0; i < jumlahTim; i++) {
            cout << "| " << setw(3) << i + 1 << " | " 
                 << setw(13) << left << tim[i].nama << " | " 
                 << setw(3) << right << tim[i].main << " | " 
                 << setw(3) << tim[i].menang << " | " 
                 << setw(3) << tim[i].seri << " | " 
                 << setw(3) << tim[i].kalah << " | " 
                 << setw(4) << tim[i].poin << " | "
                 << setw(3) << tim[i].golCetak << " | "
                 << setw(3) << tim[i].golKebobolan << " | "
                 << setw(4) << tim[i].selisihGol << " |\n";
        }
        
        // Menampilkan batas bawah tabel
        cout << "+-----+---------------+-----+-----+-----+-----+------+-----+-----+------+\n";
        cout << "M: Main, W: Menang, D: Seri, L: Kalah, Pts: Poin\n";
        cout << "GF: Gol Cetak, GA: Gol Kebobolan, GD: Selisih Gol\n";
    } else { // Tenis Meja, Voli, Badminton
        // Menampilkan batas atas tabel
        cout << "\n+-----+---------------+-----+-----+-----+-----+------+-----+-----+------+\n";
        cout << "| No. |      Tim      |  M  |  W  |  D  |  L  | Pts  |  SW |  SL |  SD  |\n";
        cout << "+-----+---------------+-----+-----+-----+-----+------+-----+-----+------+\n";
        
        // Isi tabel
        for (int i = 0; i < jumlahTim; i++) {
            cout << "| " << setw(3) << i + 1 << " | " 
                 << setw(13) << left << tim[i].nama << " | " 
                 << setw(3) << right << tim[i].main << " | " 
                 << setw(3) << tim[i].menang << " | " 
                 << setw(3) << tim[i].seri << " | " 
                 << setw(3) << tim[i].kalah << " | " 
                 << setw(4) << tim[i].poin << " | "
                 << setw(3) << tim[i].setMenang << " | "
                 << setw(3) << tim[i].setKalah << " | "
                 << setw(4) << tim[i].selisihGol << " |\n";
        }
        
        // Menampilkan batas bawah tabel
        cout << "+-----+---------------+-----+-----+-----+-----+------+-----+-----+------+\n";
        cout << "M: Main, W: Menang, D: Seri, L: Kalah, Pts: Poin\n";
        cout << "SW: Set Menang, SL: Set Kalah, SD: Selisih Set\n";
    }
}

// Fungsi untuk menampilkan pemenang
void tampilkanPemenang() {
    urutkanKlasemen();
    
    // Cek apakah ada tie untuk juara pertama
    bool ada_tie = false;
    if (jumlahTim > 1) {
        // Jika poin dan semua kriteria tie-breaker sama
        if (tim[0].poin == tim[1].poin && tim[0].selisihGol == tim[1].selisihGol && 
            tim[0].menang == tim[1].menang) {
            
            if (jenisOlahraga <= 3 && tim[0].golCetak == tim[1].golCetak) {
                ada_tie = true;
            } else if (jenisOlahraga > 3 && tim[0].setMenang == tim[1].setMenang) {
                ada_tie = true;
            }
        }
    }
    
    cout << "\n****************************************\n";
    if (ada_tie) {
        cout << "JUARA BERSAMA TURNAMEN: " << tim[0].nama << " dan " << tim[1].nama << endl;
        cout << "Total Poin: " << tim[0].poin << endl;
        cout << "Menang: " << tim[0].menang << " | Seri: " << tim[0].seri << " | Kalah: " << tim[0].kalah << endl;
    } else {
        cout << "JUARA TURNAMEN: " << tim[0].nama << endl;
        cout << "Total Poin: " << tim[0].poin << endl;
        cout << "Menang: " << tim[0].menang << " | Seri: " << tim[0].seri << " | Kalah: " << tim[0].kalah << endl;
        
        if (jenisOlahraga <= 3) {
            cout << "Gol Cetak: " << tim[0].golCetak << " | Gol Kebobolan: " << tim[0].golKebobolan << " | Selisih Gol: " << tim[0].selisihGol << endl;
        } else {
            cout << "Set Menang: " << tim[0].setMenang << " | Set Kalah: " << tim[0].setKalah << " | Selisih Set: " << tim[0].selisihGol << endl;
        }
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
            {"sudahDimainkan", current->sudahDimainkan},
            {"kandang", current->kandang}
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
            {"poin", tim[i].poin},
            {"golCetak", tim[i].golCetak},
            {"golKebobolan", tim[i].golKebobolan},
            {"selisihGol", tim[i].selisihGol},
            {"setMenang", tim[i].setMenang},
            {"setKalah", tim[i].setKalah}
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
    
    // Buat jadwal pertandingan sesuai jenis olahraga
    if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
        cout << "\n1. Single Round Robin (Satu kali pertandingan)\n"
             << "2. Double Round Robin (Home-Away)\n";
        int format;

        while (true) {
            cout << "Pilih format pertandingan (1/2): ";
            cin >> format;

            if (cin.fail() || (format != 1 && format != 2)) {
                cout << "Pilihan format tidak valid. Silakan pilih 1 atau 2.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        }

        if (format == 1) buatJadwalSingleRoundRobin();
        else buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 3) {
        cout << "\nFormat Double Round Robin akan digunakan untuk Handball (standar internasional).\n";
        buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 4 || jenisOlahraga == 6) {
        cout << "\nFormat Single Round Robin akan digunakan untuk " 
             << (jenisOlahraga == 4 ? "Tenis Meja" : "Badminton") << ".\n";
        buatJadwalSingleRoundRobin();
    }
    
    // Tampilkan jadwal pertandingan
    cout << "\n=== JADWAL PERTANDINGAN ===\n";
    tampilkanJadwalPertandingan();
    
    // Input skor untuk setiap hari pertandingan
    for (int hari = 1; hari <= jumlahHari; hari++) {
        inputSkorHari(hari);
        cout << "\n=== Klasemen Sementara Setelah Hari " << hari << " ===\n";
        tampilkanKlasemen();
    }
    
    // Tampilkan klasemen akhir dan pemenang
    cout << "\n=== KLASEMEN AKHIR ===\n";
    tampilkanKlasemen();
    tampilkanPemenang();
    
    // Simpan data turnamen
    simpanTurnamen();
    
    // Bersihkan memori
    bersihkanMemori();
}