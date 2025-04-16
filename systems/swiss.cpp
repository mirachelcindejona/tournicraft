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


// Fungsi sistemSwissRonde:
// Parameter:
//  - jumlahTim : jumlah tim yang berpartisipasi (digunakan untuk input nama tim)
//  - totalHari : jumlah hari pertandingan

//  Proses dalam fungsi ini:
//  1. Meminta input jumlah ronde (total ronde turnamen).
//  2. Meminta input nama tim sebanyak jumlahTim.
//  3. Menghitung pembagian ronde per hari secara setara (jika sisa, hari awal mendapat tambahan 1 ronde).
//  4. Untuk setiap hari:
//     b. Pada setiap ronde:
//          i. Mengurutkan tim berdasarkan skor.
//         ii. Jika jumlah tim ganjil, memberikan bye pada tim dengan skor terendah yang belum mendapat bye.
//        iii. Membuat pairing pertandingan dan menampilkan jadwal.
//         iv. Meminta input hasil pertandingan dan mengupdate skor.
//          v. Menampilkan skor terkini setelah ronde.
//     c. Jika bukan hari terakhir, menanyakan apakah akan melanjutkan ke hari berikutnya.
//  5. Setelah selesai (atau dihentikan), menampilkan skor akhir (atau skor terakhir).

void sistemSwissRonde(int jumlahTim, int totalHari) {
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
    
    // Hitung pembagian ronde per hari secara setara
    int rondePerHari = jumlahRonde / totalHari;
    int sisaRonde = jumlahRonde % totalHari;
    int rondeGlobal = 1;
    
    bool turnamenDijalankanSampaiAkhir = true;
    
    for (int hari = 1; hari <= totalHari; hari++) {
        cout << "\n========================\n";
        cout << "Hari Pertandingan ke-" << hari << "\n";
        cout << "========================\n";
        
        int rondeHariIni = rondePerHari + ((sisaRonde > 0) ? 1 : 0);
        if (sisaRonde > 0)
            sisaRonde--;
        
        for (int r = 0; r < rondeHariIni; r++) {
            cout << "\n=== Ronde " << rondeGlobal << " ===" << endl;
            
            // Urutkan tim berdasarkan skor secara menurun
            sort(tims.begin(), tims.end(), bandingTim);
            int jumlahTims = tims.size();
            int indeksBye = -1;
            
            // Jika jumlah tim ganjil, cari tim dengan skor terendah yang belum mendapat bye
            if (jumlahTims % 2 != 0) {
                for (int i = jumlahTims - 1; i >= 0; i--) {
                    if (!tims[i].mendapatkanBye) {
                        indeksBye = i;
                        tims[i].mendapatkanBye = true;  // Tandai bahwa tim sudah mendapat bye
                        tims[i].skor++;                // Berikan poin kemenangan dari bye
                        cout << "Tim \"" << tims[i].nama << "\" mendapat bye (otomatis menang) pada ronde " << rondeGlobal << endl;
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
            cout << "\nJadwal Pertandingan Ronde " << rondeGlobal << ":\n";
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
            cout << "\nSkor Terkini setelah Ronde " << rondeGlobal << ":\n";
            for (auto &tm : tims) {
                cout << "Tim \"" << tm.nama << "\": " << tm.skor << "\n";
            }
            
            rondeGlobal++;
        }
        
        if (hari != totalHari) {
            char lanjut;
            cout << "\nApakah Anda ingin melanjutkan ke hari pertandingan berikutnya? (y/n): ";
            cin >> lanjut;
            if (lanjut != 'y' && lanjut != 'Y') {
                cout << "\nTurnamen dihentikan oleh pengguna.\n";
                turnamenDijalankanSampaiAkhir = false;
                break;
            }
        }
    }
    
    // Tampilkan skor akhir atau skor terakhir setelah turnamen selesai atau dihentikan
    sort(tims.begin(), tims.end(), bandingTim);
    if (turnamenDijalankanSampaiAkhir)
        cout << "\nTurnamen selesai! Skor Akhir Turnamen:\n";
    else
        cout << "\nTurnamen dihentikan. Skor Terakhir:\n";
    
    for (auto &tm : tims) {
        cout << "Tim \"" << tm.nama << "\": " << tm.skor << "\n";
    }
}

int sistemSwiss(){
    int pilihanMenu;
    do {
        cout << "\n=== Menu Utama Sistem Pertandingan ===\n";
        cout << "1. Mulai Turnamen\n";
        cout << "2. Keluar\n";
        cout << "Masukkan pilihan: ";
        cin >> pilihanMenu;
        
        if (pilihanMenu == 1) {
            // Input dari main
            string cabangOlahraga;
            cout << "\nMasukkan cabang olahraga: ";
            cin.ignore(); // Membersihkan buffer
            getline(cin, cabangOlahraga);
            
            int jumlahTim;
            cout << "Masukkan jumlah tim: ";
            cin >> jumlahTim;
            
            int jumlahHari;
            cout << "Masukkan jumlah hari pertandingan: ";
            cin >> jumlahHari;
            
            cout << "\nCabang Olahraga: " << cabangOlahraga << "\n";
            
            // Jalankan turnamen Swiss System (fungsi ini juga mengurus input jumlah ronde dan nama tim)
            sistemSwissRonde(jumlahTim, jumlahHari);
        }
    } while (pilihanMenu != 2);
    
    cout << "\nTerima kasih telah menggunakan sistem pertandingan ini.\n";
    return 0;
}