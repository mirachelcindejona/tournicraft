#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Struktur data untuk tim
struct Tim {
    int id;
    string nama;
    int skor;
    bool mendapatkanBye; // Menandakan apakah tim sudah pernah mendapat bye

    Tim(int id, const string &nama) : id(id), nama(nama), skor(0), mendapatkanBye(false) {}
};

// Fungsi pembanding untuk mengurutkan tim berdasarkan skor (descending).
// Jika skor sama, gunakan id sebagai tie-breaker (ascending)
bool bandingTim(const Tim &a, const Tim &b) {
    if (a.skor == b.skor)
        return a.id < b.id;
    return a.skor > b.skor;
}

/*
 * Fungsi sistemSwissRonde:
 * Parameter:
 *   - tims    : vector berisi data tim yang berpartisipasi
 *   - rondeKe : nomor ronde global saat ini
 *
 * Proses dalam fungsi ini:
 * 1. Mengurutkan tim berdasarkan skor.
 * 2. Jika jumlah tim ganjil, memberikan bye kepada tim dengan skor terendah yang belum pernah mendapat bye.
 * 3. Melakukan pairing pertandingan dari tim yang sudah diurutkan.
 * 4. Menampilkan jadwal pertandingan (nama tim) untuk ronde tersebut.
 * 5. Menanyakan apakah pengguna ingin memasukkan hasil pertandingan.
 *    - Jika ya, sistem meminta input pemenang tiap pertandingan dan mengupdate skor.
 * 6. Menampilkan skor terkini setelah ronde.
 */
void sistemSwissRonde(vector<Tim>& tims, int rondeKe) {
    cout << "\n=== Ronde " << rondeKe << " ===" << endl;
    
    // Urutkan tim berdasarkan skor secara menurun
    sort(tims.begin(), tims.end(), bandingTim);
    
    int jumlahTims = tims.size();
    int indeksBye = -1;
    
    // Jika jumlah tim ganjil, cari tim dengan skor terendah (dari bawah) yang belum mendapat bye
    if (jumlahTims % 2 != 0) {
        for (int i = jumlahTims - 1; i >= 0; i--) {
            if (!tims[i].mendapatkanBye) {
                indeksBye = i;
                tims[i].mendapatkanBye = true;  // Tandai bahwa tim sudah mendapat bye
                tims[i].skor++;                // Berikan poin kemenangan dari bye
                cout << "Tim \"" << tims[i].nama << "\" mendapat bye (otomatis menang) pada ronde " << rondeKe << endl;
                break;
            }
        }
    }
    
    // Buat pairing pertandingan
    vector<pair<string, string>> pertandingan;   // Menyimpan pasangan pertandingan (nama tim)
    vector<bool> sudahDipasangkan(jumlahTims, false); // Menandai tim yang sudah dipasangkan
    
    if (indeksBye != -1)
        sudahDipasangkan[indeksBye] = true;  // Tim yang mendapat bye tidak ikut dipasangkan
    
    // Pairing: ambil tim secara berurutan dari daftar yang telah diurutkan
    for (int i = 0; i < jumlahTims; i++) {
        if (sudahDipasangkan[i])
            continue;
        int j = i + 1;
        while (j < jumlahTims && sudahDipasangkan[j])
            j++;
        if (j < jumlahTims) {
            pertandingan.push_back({tims[i].nama, tims[j].nama});
            sudahDipasangkan[i] = sudahDipasangkan[j] = true;
        }
    }
    
    // Tampilkan jadwal pertandingan pada ronde ini
    cout << "\nJadwal Pertandingan Ronde " << rondeKe << ":\n";
    for (auto& pasang : pertandingan) {
        cout << pasang.first << " vs " << pasang.second << "\n";
    }
    
    // Menanyakan apakah pengguna ingin memasukkan hasil pertandingan
    char inputHasil;
    cout << "\nApakah Anda ingin memasukkan hasil pertandingan pada ronde ini? (y/n): ";
    cin >> inputHasil;
    
    if (inputHasil == 'y' || inputHasil == 'Y') {
        // Proses input hasil pertandingan untuk setiap pasangan
        for (auto& pasang : pertandingan) {
            cout << "\nPertandingan: " << pasang.first << " vs " << pasang.second << "\n";
            cout << "Masukkan nomor pemenang (1 untuk \"" << pasang.first 
                 << "\", 2 untuk \"" << pasang.second << "\"): ";
            int pilihanPemenang;
            cin >> pilihanPemenang;
            if (pilihanPemenang == 1) {
                for (auto &tm : tims) {
                    if (tm.nama == pasang.first) {
                        tm.skor++;
                        break;
                    }
                }
            } else if (pilihanPemenang == 2) {
                for (auto &tm : tims) {
                    if (tm.nama == pasang.second) {
                        tm.skor++;
                        break;
                    }
                }
            } else {
                cout << "Input tidak valid. Skor tidak diupdate untuk pertandingan ini.\n";
            }
        }
    }
    
    // Tampilkan skor terkini setelah ronde selesai
    cout << "\nSkor Terkini setelah Ronde " << rondeKe << ":\n";
    for (auto &tm : tims) {
        cout << "Tim \"" << tm.nama << "\": " << tm.skor << "\n";
    }
}

/*
 * Fungsi sistemSwiss:
 * Fungsi ini menangani:
 *   - Input cabang olahraga, jumlah tim, jumlah hari pertandingan, jumlah ronde,
 *     dan nama tim.
 *   - Pembagian jumlah ronde yang akan dilaksanakan per hari secara setara.
 *   - Pelaksanaan turnamen dengan algoritma Swiss System.
 */
void sistemSwiss() {
    cout << "\n=== Sistem Swiss Turnamen ===\n";
    
    string cabangOlahraga;
    cout << "Masukkan cabang olahraga: ";
    cin.ignore(); // Membersihkan buffer
    getline(cin, cabangOlahraga);
    
    int jumlahTim;
    cout << "Masukkan jumlah tim: ";
    cin >> jumlahTim;
    
    int jumlahHari;
    cout << "Masukkan jumlah hari pertandingan: ";
    cin >> jumlahHari;
    
    int jumlahRonde;
    cout << "Masukkan jumlah ronde (berapa kali setiap tim bertanding): ";
    cin >> jumlahRonde;
    
    // Input nama tim
    vector<Tim> tims;
    string namaTim;
    for (int i = 0; i < jumlahTim; i++) {
        cout << "Masukkan nama tim ke-" << i + 1 << ": ";
        cin >> namaTim;
        tims.push_back(Tim(i + 1, namaTim));
    }
    
    cout << "\nCabang Olahraga: " << cabangOlahraga << endl;
    
    // Hitung pembagian ronde per hari
    // Jika jumlah ronde tidak habis dibagi jumlah hari, kelebihan ronde akan dibagikan ke hari awal
    int rondePerHari = jumlahRonde / jumlahHari;
    int sisaRonde = jumlahRonde % jumlahHari;
    
    int rondeGlobal = 1;
    // Pelaksanaan pertandingan per hari
    for (int hari = 1; hari <= jumlahHari; hari++) {
        cout << "\n========================\n";
        cout << "Hari Pertandingan ke-" << hari << "\n";
        cout << "========================\n";
        
        int rondeHariIni = rondePerHari + ((sisaRonde > 0) ? 1 : 0);
        if (sisaRonde > 0)
            sisaRonde--;
        
        // Jalankan ronde-ronde pada hari ini
        for (int r = 0; r < rondeHariIni; r++) {
            sistemSwissRonde(tims, rondeGlobal);
            rondeGlobal++;
        }
        
        // Pertanyaan apakah melanjutkan ke hari pertandingan berikutnya (jika belum hari terakhir)
        if (hari != jumlahHari) {
            char lanjut;
            cout << "\nApakah Anda ingin melanjutkan ke hari pertandingan berikutnya? (y/n): ";
            cin >> lanjut;
            if (lanjut != 'y' && lanjut != 'Y') {
                cout << "\nKembali ke Menu Utama...\n";
                break;
            }
        }
    }
}

int main(){
    int pilihanMenu;
    do {
        cout << "\n=== Menu Utama Sistem Pertandingan ===\n";
        cout << "1. Mulai Turnamen\n";
        cout << "2. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihanMenu;
        
        if (pilihanMenu == 1) {
            sistemSwiss();
        }
    } while (pilihanMenu != 2);
    
    cout << "\nTerima kasih telah menggunakan sistem pertandingan ini.\n";
    return 0;
}
