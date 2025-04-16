#include "systems/sistemGugur.h"
#include <fstream>
#include <sstream>
#include <random>
#include "systems/storage.h"

vector<TimGugur> timGugur;
vector<PertandinganGugur> jadwalGugur;
int jumlahTimGugur, jumlahHariGugur, jumlahMatchGugur = 0;
int jenisOlahragaGugur;
string namaTurnamenGugur;
int jumlahBabak = 0;

bool loadTurnamenDataGugur() {
    json turnamenData = loadTurnamen();
    
    if (turnamenData.empty()) {
        cout << "Tidak ada data turnamen yang tersimpan.\n";
        return false;
    }
    
    // Ambil turnamen terakhir yang dibuat
    json lastTurnamen = turnamenData[turnamenData.size() - 1];
    
    // Periksa apakah sistem pertandingan adalah "Knockout"
    if (lastTurnamen.contains("sistem") && lastTurnamen["sistem"] != "Knockout") {
        cout << "Sistem pertandingan bukan Knockout. Program tidak dapat dijalankan.\n";
        return false;
    }
    
    // Load data turnamen
    namaTurnamenGugur = lastTurnamen["nama"];
    jenisOlahragaGugur = lastTurnamen["jenisOlahraga"];
    jumlahTimGugur = lastTurnamen["jumlahTim"];
    jumlahHariGugur = lastTurnamen["jumlahHari"];
    
    cout << "Turnamen " << namaTurnamenGugur << " berhasil dimuat!\n";
    cout << "Cabang Olahraga: " << jenisOlahragaGugur << "\n";
    cout << "Jumlah Tim: " << jumlahTimGugur << "\n";
    cout << "Jumlah Hari: " << jumlahHariGugur << "\n";
    
    return true;
}

void inputDataTimGugur() {
    timGugur.resize(jumlahTimGugur);
    cin.ignore(); // Bersihkan newline sebelum getline
    
    cout << "\n=== INPUT DATA TIM ===\n";
    for (int i = 0; i < jumlahTimGugur; i++) {
        cout << "Nama tim ke-" << i + 1 << ": ";
        getline(cin, timGugur[i].nama);
    }
}

// Cek apakah jumlah tim adalah pangkat 2 (2, 4, 8, 16, dst)
bool isPowerOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

void buatBaganGugur() {
    // Hitung jumlah babak yang diperlukan
    jumlahBabak = ceil(log2(jumlahTimGugur));
    int totalMatch = (1 << jumlahBabak) - 1; // 2^jumlahBabak - 1
    
    cout << "\nTotal babak: " << jumlahBabak << endl;
    cout << "Total pertandingan: " << totalMatch << endl;
    
    // Acak urutan tim
    vector<int> urutanTim;
    for (int i = 0; i < jumlahTimGugur; i++) {
        urutanTim.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(urutanTim.begin(), urutanTim.end(), g);
    
    // Inisialisasi jadwal pertandingan
    jadwalGugur.clear();
    
    // Buat bagan mulai dari babak pertama (babak terbanyak)
    int timPerBabak = (1 << (jumlahBabak - 1)); // 2^(jumlahBabak-1)
    int matchPerBabak = timPerBabak / 2;
    
    // Babak pertama
    for (int i = 0; i < matchPerBabak; i++) {
        PertandinganGugur match;
        match.babak = jumlahBabak;
        match.nomorMatch = i + 1;
        
        if (2*i < jumlahTimGugur) {
            match.tim1 = urutanTim[2*i];
        } else {
            match.tim1 = -1; // Bye
        }
        
        if (2*i+1 < jumlahTimGugur) {
            match.tim2 = urutanTim[2*i+1];
        } else {
            match.tim2 = -1; // Bye
        }
        
        jadwalGugur.push_back(match);
        jumlahMatchGugur++;
    }
    
    // Babak-babak berikutnya
    for (int babak = jumlahBabak - 1; babak >= 1; babak--) {
        matchPerBabak = (1 << (babak - 1)); // 2^(babak-1)
        for (int i = 0; i < matchPerBabak; i++) {
            PertandinganGugur match;
            match.babak = babak;
            match.nomorMatch = i + 1;
            match.tim1 = -1; // Belum ditentukan
            match.tim2 = -1; // Belum ditentukan
            jadwalGugur.push_back(match);
            jumlahMatchGugur++;
        }
    }
}

string getNamaBabak(int babak, int totalBabak) {
    if (babak == 1) return "Final";
    if (babak == 2) return "Semifinal";
    if (babak == 3) return "Perempat Final";
    
    int jumlahTim = 1 << (totalBabak - babak + 1);
    return "Babak " + to_string(jumlahTim) + " Besar";
}

void tampilkanBaganGugur() {
    cout << "\n=== BAGAN PERTANDINGAN ===\n";
    
    // Tampilkan dari babak awal ke final
    for (int babak = jumlahBabak; babak >= 1; babak--) {
        cout << "\n" << getNamaBabak(babak, jumlahBabak) << ":\n";
        
        for (const auto& match : jadwalGugur) {
            if (match.babak == babak) {
                string tim1Name = (match.tim1 != -1) ? timGugur[match.tim1].nama : "TBD";
                string tim2Name = (match.tim2 != -1) ? timGugur[match.tim2].nama : "TBD";
                
                cout << "Match " << match.nomorMatch << ": " << tim1Name << " vs " << tim2Name;
                
                if (match.sudahDimainkan) {
                    cout << " (" << match.skor1 << "-" << match.skor2 << ")";
                }
                
                cout << endl;
            }
        }
    }
}

// Update tim yang maju ke babak berikutnya
void updateTimPemenang(int idxMatch, int idxPemenang) {
    int babak = jadwalGugur[idxMatch].babak;
    int nomorMatch = jadwalGugur[idxMatch].nomorMatch;
    
    // Jika bukan final, update tim untuk babak berikutnya
    if (babak > 1) {
        // Cari match di babak selanjutnya
        int nextMatchIdx = -1;
        int nextBabak = babak - 1;
        int nextNomorMatch = (nomorMatch + 1) / 2;
        
        for (int i = 0; i < jadwalGugur.size(); i++) {
            if (jadwalGugur[i].babak == nextBabak && jadwalGugur[i].nomorMatch == nextNomorMatch) {
                nextMatchIdx = i;
                break;
            }
        }
        
        if (nextMatchIdx != -1) {
            // Update tim1 atau tim2 berdasarkan nomor match saat ini
            if (nomorMatch % 2 == 1) {
                jadwalGugur[nextMatchIdx].tim1 = idxPemenang;
            } else {
                jadwalGugur[nextMatchIdx].tim2 = idxPemenang;
            }
        }
    }
}

void jalankanPertandingan() {
    cout << "\n=== JALANKAN PERTANDINGAN ===\n";
    
    // Distribusikan match per hari
    int matchPerHari = ceil((double)jumlahMatchGugur / jumlahHariGugur);
    int matchIndex = 0;
    
    for (int hari = 1; hari <= jumlahHariGugur && matchIndex < jumlahMatchGugur; hari++) {
        cout << "\n=== HARI " << hari << " ===\n";
        int matchHariIni = min(matchPerHari, jumlahMatchGugur - matchIndex);
        
        for (int m = 0; m < matchHariIni; m++) {
            int idxMatch = matchIndex + m;
            
            // Jika salah satu tim belum ditentukan, coba tentukan dulu
            if (jadwalGugur[idxMatch].tim1 == -1 || jadwalGugur[idxMatch].tim2 == -1) {
                cout << "Pertandingan belum bisa dilaksanakan karena tim belum lengkap.\n";
                continue;
            }
            
            int idxTim1 = jadwalGugur[idxMatch].tim1;
            int idxTim2 = jadwalGugur[idxMatch].tim2;
            string namaBabak = getNamaBabak(jadwalGugur[idxMatch].babak, jumlahBabak);
            
            cout << "\n" << namaBabak << " - Match " << jadwalGugur[idxMatch].nomorMatch << endl;
            cout << timGugur[idxTim1].nama << " vs " << timGugur[idxTim2].nama << endl;
            
            cout << "Skor " << timGugur[idxTim1].nama << ": ";
            cin >> jadwalGugur[idxMatch].skor1;
            
            cout << "Skor " << timGugur[idxTim2].nama << ": ";
            cin >> jadwalGugur[idxMatch].skor2;
            
            jadwalGugur[idxMatch].sudahDimainkan = true;
            
            // Tentukan pemenang
            int idxPemenang;
            if (jadwalGugur[idxMatch].skor1 > jadwalGugur[idxMatch].skor2) {
                idxPemenang = idxTim1;
                cout << timGugur[idxTim1].nama << " MENANG!\n";
            } else if (jadwalGugur[idxMatch].skor1 < jadwalGugur[idxMatch].skor2) {
                idxPemenang = idxTim2;
                cout << timGugur[idxTim2].nama << " MENANG!\n";
            } else {
                // Jika seri, tanyakan pemenang berdasarkan aturan
                cout << "Skor seri! Tentukan pemenang berdasarkan aturan:\n";
                cout << "1. " << timGugur[idxTim1].nama << endl;
                cout << "2. " << timGugur[idxTim2].nama << endl;
                
                int pilihan;
                do {
                    cout << "Pilih pemenang (1/2): ";
                    cin >> pilihan;
                } while (pilihan != 1 && pilihan != 2);
                
                if (pilihan == 1) {
                    idxPemenang = idxTim1;
                    cout << timGugur[idxTim1].nama << " MENANG!\n";
                } else {
                    idxPemenang = idxTim2;
                    cout << timGugur[idxTim2].nama << " MENANG!\n";
                }
            }
            
            // Update tim pemenang ke babak berikutnya
            updateTimPemenang(idxMatch, idxPemenang);
            
            // Set tim yang kalah menjadi tidak aktif
            if (idxPemenang == idxTim1) {
                timGugur[idxTim2].aktif = false;
            } else {
                timGugur[idxTim1].aktif = false;
            }
        }
        
        matchIndex += matchHariIni;
        
        // Tampilkan bagan setelah pertandingan hari ini
        tampilkanBaganGugur();
    }
}

void tampilkanPemenangGugur() {
    // Cari pemenang dari final (babak 1)
    int finalIdx = -1;
    for (int i = 0; i < jadwalGugur.size(); i++) {
        if (jadwalGugur[i].babak == 1) {
            finalIdx = i;
            break;
        }
    }
    
    if (finalIdx != -1 && jadwalGugur[finalIdx].sudahDimainkan) {
        int idxTim1 = jadwalGugur[finalIdx].tim1;
        int idxTim2 = jadwalGugur[finalIdx].tim2;
        int skor1 = jadwalGugur[finalIdx].skor1;
        int skor2 = jadwalGugur[finalIdx].skor2;
        
        cout << "\n****************************************\n";
        cout << "JUARA TURNAMEN " << namaTurnamenGugur << ":\n";
        
        if (skor1 > skor2) {
            cout << timGugur[idxTim1].nama << " (Skor Final: " << skor1 << "-" << skor2 << ")\n";
        } else {
            cout << timGugur[idxTim2].nama << " (Skor Final: " << skor2 << "-" << skor1 << ")\n";
        }
        cout << "****************************************\n";
    } else {
        cout << "\nFinal belum dimainkan. Belum ada pemenang.\n";
    }
}

void simpanTurnamenGugur() {
    json turnamenData = loadTurnamen();
    
    // Buat data tim
    json timData = json::array();
    for (int i = 0; i < jumlahTimGugur; i++) {
        json timObj = {
            {"nama", timGugur[i].nama},
            {"aktif", timGugur[i].aktif},
            {"skor", timGugur[i].skor}
        };
        timData.push_back(timObj);
    }
    
    // Buat data pertandingan
    json jadwalData = json::array();
    for (int i = 0; i < jumlahMatchGugur; i++) {
        json matchObj = {
            {"tim1", jadwalGugur[i].tim1},
            {"tim2", jadwalGugur[i].tim2},
            {"skor1", jadwalGugur[i].skor1},
            {"skor2", jadwalGugur[i].skor2},
            {"sudahDimainkan", jadwalGugur[i].sudahDimainkan},
            {"babak", jadwalGugur[i].babak},
            {"nomorMatch", jadwalGugur[i].nomorMatch}
        };
        jadwalData.push_back(matchObj);
    }
    
    // Update turnamen yang sudah ada
    for (auto& turnamen : turnamenData) {
        if (turnamen["nama"] == namaTurnamenGugur) {
            turnamen["tim"] = timData;
            turnamen["jadwal"] = jadwalData;
            turnamen["jumlahMatch"] = jumlahMatchGugur;
            turnamen["jumlahBabak"] = jumlahBabak;
            saveTurnamen(turnamenData);
            cout << "\nTurnamen " << namaTurnamenGugur << " berhasil diperbarui!\n";
            return;
        }
    }
    
    // Jika tidak ditemukan, buat turnamen baru
    json turnamenObj = {
        {"nama", namaTurnamenGugur},
        {"jenisOlahraga", jenisOlahragaGugur},
        {"jumlahTim", jumlahTimGugur},
        {"jumlahHari", jumlahHariGugur},
        {"jumlahMatch", jumlahMatchGugur},
        {"jumlahBabak", jumlahBabak},
        {"sistem", "Knockout"},
        {"tim", timData},
        {"jadwal", jadwalData}
    };
    
    turnamenData.push_back(turnamenObj);
    saveTurnamen(turnamenData);
    
    cout << "\nTurnamen " << namaTurnamenGugur << " berhasil disimpan!\n";
}

void sistemGugur() {
    cout << "\n=== SISTEM KNOCKOUT ===\n";
    
    // Load data turnamen dari file
    if (!loadTurnamenDataGugur()) {
        return; // Keluar jika tidak ada data turnamen atau bukan sistem Knockout
    }
    
    // Verifikasi jenis olahraga yang valid untuk sistem gugur
    // Contoh: Cabang olahraga 7 (Tenis), 9 (Basket), 11 (Catur), 12 (Panahan), 13 (Gulat)
    vector<int> validOlahraga = {7, 9, 11, 12, 13};
    bool isValid = false;
    for (int olahraga : validOlahraga) {
        if (jenisOlahragaGugur == olahraga) {
            isValid = true;
            break;
        }
    }
    
    if (!isValid) {
        cout << "Peringatan: Cabang olahraga ini tidak umum menggunakan sistem knockout.\n";
        cout << "Lanjutkan? (y/n): ";
        char lanjut;
        cin >> lanjut;
        if (tolower(lanjut) != 'y') return;
    }
    
    inputDataTimGugur();
    buatBaganGugur();
    tampilkanBaganGugur();
    
    jalankanPertandingan();
    tampilkanPemenangGugur();
    
    // Simpan data turnamen ke JSON
    simpanTurnamenGugur();
}