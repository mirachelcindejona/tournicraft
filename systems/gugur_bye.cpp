#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

struct Tim {
    string nama;
    int skor;
    bool isBye;
};

struct Pertandingan {
    Tim tim1;
    Tim tim2;
    bool isBye;
    string deskripsi;
};

// Fungsi untuk menghitung power of two terdekat (lebih besar dan lebih kecil)
int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

int previousPowerOfTwo(int n) {
    int power = 1;
    while (power * 2 < n) {
        power *= 2;
    }
    return power;
}

int main() {
    // Pilihan olahraga
    cout << "=== Sistem Gugur Bye ===\n";
    cout << "Pilih jenis pertandingan:\n";
    cout << "1. Sepak Bola\n";
    cout << "2. Futsal\n";
    cout << "3. Badminton\n";
    cout << "4. Tennis\n";
    cout << "5. Volly\n";
    cout << "6. Taekwondo\n";
    cout << "7. Basket\n";
    cout << "Pilihan: ";
    
    int pilihanOlahraga;
    cin >> pilihanOlahraga;
    
    string namaOlahraga;
    bool isPeserta = false;
    
    switch(pilihanOlahraga) {
        case 1:
            namaOlahraga = "Sepak Bola";
            break;
        case 2:
            namaOlahraga = "Futsal";
            break;
        case 3:
            namaOlahraga = "Badminton";
            break;
        case 4:
            namaOlahraga = "Tennis";
            break;
        case 5:
            namaOlahraga = "Volly";
            break;
        case 6:
            namaOlahraga = "Taekwondo";
            isPeserta = true;
            break;
        case 7:
            namaOlahraga = "Basket";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
            return 0;
    }
    
    cout << "\nAnda memilih olahraga: " << namaOlahraga << endl;
    
    int jumlahTim;
    string label = isPeserta ? "peserta" : "tim";
    
    cout << "Masukkan jumlah " << label << " (minimal 3): ";
    cin >> jumlahTim;

    if (jumlahTim < 3) {
        cout << "Jumlah " << label << " minimal harus 3.\n";
        return 0;
    }

    vector<Tim> tim(jumlahTim);
    for (int i = 0; i < jumlahTim; i++) {
        cout << "Nama " << label << " ke-" << i + 1 << ": ";
        cin >> tim[i].nama;
        tim[i].skor = 0;
        tim[i].isBye = false;
    }

    // Input jumlah hari pertandingan
    int hari;
    cout << "\nMasukkan jumlah hari pertandingan: ";
    cin >> hari;

    if (hari <= 0) {
        cout << "Jumlah hari tidak valid.\n";
        return 0;
    }

    // Cek apakah jumlah tim ganjil, jika ya maka ada tim yang mendapat BYE
    bool adaBye = (jumlahTim % 2 != 0);
    int timBye = -1;
    
    if (adaBye) {
        // Tim terakhir mendapatkan BYE
        timBye = jumlahTim - 1;
        tim[timBye].isBye = true;
    }

    // Buat jadwal pertandingan
    vector<vector<Pertandingan>> jadwalHari(hari);
    
    // Menyimpan tim dalam dua antrian - antrian pertandingan reguler dan antrian bye
    queue<Tim> antrianPertandingan;
    queue<Tim> antrianBye;
    
    // Memasukkan semua tim ke antrian sesuai status BYE
    for (int i = 0; i < jumlahTim; i++) {
        if (tim[i].isBye) {
            antrianBye.push(tim[i]);
        } else {
            antrianPertandingan.push(tim[i]);
        }
    }

    // Hitung total pertandingan yang mungkin
    int totalPertandingan = 0;
    if (adaBye) {
        totalPertandingan = jumlahTim - 1; // Dengan satu BYE
    } else {
        totalPertandingan = jumlahTim / 2 + (jumlahTim / 2) - 1; // Tanpa BYE
    }

    // Distribusikan pertandingan per hari
    int pertandinganPerHari = totalPertandingan / hari;
    int sisaPertandingan = totalPertandingan % hari;

    // Membuat jadwal awal
    cout << "\n=== Jadwal Pertandingan " << namaOlahraga << " ===\n";
    
    // Buat semua pertandingan reguler terlebih dahulu
    vector<Pertandingan> semuaPertandingan;
    
    // Pertandingan babak pertama
    while (antrianPertandingan.size() >= 2) {
        Pertandingan p;
        p.tim1 = antrianPertandingan.front();
        antrianPertandingan.pop();
        p.tim2 = antrianPertandingan.front();
        antrianPertandingan.pop();
        p.isBye = false;
        p.deskripsi = p.tim1.nama + " vs " + p.tim2.nama;
        semuaPertandingan.push_back(p);
    }

    // Jika ada sisa tim tanpa lawan (tidak termasuk tim BYE)
    if (antrianPertandingan.size() == 1) {
        Pertandingan p;
        p.tim1 = antrianPertandingan.front();
        antrianPertandingan.pop();
        p.tim2 = Tim{"-", 0, false}; // Placeholder
        p.isBye = true;
        p.deskripsi = p.tim1.nama + " mendapatkan BYE tambahan";
        semuaPertandingan.push_back(p);
    }

    // Pertandingan dengan tim BYE
    if (!antrianBye.empty()) {
        // Simpan pertandingan "pemenang babak pertama vs tim BYE" untuk nanti
        // Akan dimasukkan setelah menentukan pemenang pertandingan pertama
        cout << "\nTim yang mendapat BYE: " << antrianBye.front().nama << endl;
    }

    // Distribusikan semua pertandingan ke dalam hari-hari
    int hariIdx = 0;
    int pertandinganCounter = 0;

    for (const Pertandingan& p : semuaPertandingan) {
        // Jika sudah mencapai batas pertandingan per hari, pindah ke hari berikutnya
        if (pertandinganCounter >= pertandinganPerHari + (hariIdx < sisaPertandingan ? 1 : 0)) {
            hariIdx++;
            pertandinganCounter = 0;
            
            // Jika sudah melebihi jumlah hari yang diinput, berhenti
            if (hariIdx >= hari) {
                break;
            }
        }
        
        jadwalHari[hariIdx].push_back(p);
        pertandinganCounter++;
    }

    // Tampilkan jadwal per hari
    for (int i = 0; i < hari; i++) {
        cout << "\nHari " << (i + 1) << ":\n";
        if (jadwalHari[i].empty()) {
            cout << "Tidak ada pertandingan.\n";
        } else {
            for (const Pertandingan& p : jadwalHari[i]) {
                cout << p.deskripsi << endl;
            }
        }
    }

    // Reset antrian untuk simulasi pertandingan
    antrianPertandingan = queue<Tim>();
    antrianBye = queue<Tim>();
    
    // Memasukkan semua tim ke antrian sesuai status BYE
    for (int i = 0; i < jumlahTim; i++) {
        if (tim[i].isBye) {
            antrianBye.push(tim[i]);
        } else {
            antrianPertandingan.push(tim[i]);
        }
    }

    // Simulasi pertandingan babak per babak
    int babak = 1;
    bool pertandinganDenganBye = false;
    queue<Tim> antrianBerikutnya;
    
    cout << "\n=== Simulasi Pertandingan ===\n";
    cout << "\n== Babak " << babak << " ==\n";
    
    // Jalankan pertandingan hingga ada satu pemenang
    while (antrianPertandingan.size() > 0 || antrianBye.size() > 0 || antrianBerikutnya.size() > 1) {
        // Reset antrian jika perlu pindah ke babak berikutnya
        if (antrianPertandingan.empty() && !pertandinganDenganBye) {
            if (!antrianBerikutnya.empty()) {
                // Pindahkan tim dari antrianBerikutnya ke antrianPertandingan untuk babak berikutnya
                while (!antrianBerikutnya.empty()) {
                    antrianPertandingan.push(antrianBerikutnya.front());
                    antrianBerikutnya.pop();
                }
                babak++;
                cout << "\n== Babak " << babak << " ==\n";
            }
        }
        
        // Pertandingan reguler antara dua tim
        if (antrianPertandingan.size() >= 2) {
            Tim tim1 = antrianPertandingan.front();
            antrianPertandingan.pop();
            Tim tim2 = antrianPertandingan.front();
            antrianPertandingan.pop();
            
            cout << "Pertandingan: " << tim1.nama << " vs " << tim2.nama << endl;
            int skor1, skor2;
            cout << "Masukkan skor " << tim1.nama << ": ";
            cin >> skor1;
            cout << "Masukkan skor " << tim2.nama << ": ";
            cin >> skor2;
            
            // Tentukan pemenang
            Tim pemenang;
            if (skor1 > skor2) {
                pemenang = tim1;
                pemenang.skor = skor1;
                cout << tim1.nama << " menang dengan skor " << skor1 << "-" << skor2 << endl;
            } else {
                pemenang = tim2;
                pemenang.skor = skor2;
                cout << tim2.nama << " menang dengan skor " << skor2 << "-" << skor1 << endl;
            }
            
            // Jika ini pertandingan pertama dan ada tim BYE, pemenang melawan tim BYE
            if (!antrianBye.empty() && !pertandinganDenganBye) {
                pertandinganDenganBye = true;
                cout << "\nPemenang " << pemenang.nama << " akan melawan tim BYE " << antrianBye.front().nama << endl;
                antrianBerikutnya.push(pemenang);
            } else {
                // Jika tidak ada BYE atau sudah melewati pertandingan dengan BYE, masukkan ke antrian berikutnya
                antrianBerikutnya.push(pemenang);
            }
        } 
        // Pertandingan dengan tim BYE
        else if (!antrianBye.empty() && pertandinganDenganBye && !antrianBerikutnya.empty()) {
            Tim timDenganBye = antrianBye.front();
            antrianBye.pop();
            Tim lawan = antrianBerikutnya.front();
            antrianBerikutnya.pop();
            
            cout << "\nPertandingan: " << lawan.nama << " vs " << timDenganBye.nama << " (BYE)" << endl;
            int skor1, skor2;
            cout << "Masukkan skor " << lawan.nama << ": ";
            cin >> skor1;
            cout << "Masukkan skor " << timDenganBye.nama << " (BYE): ";
            cin >> skor2;
            
            // Tentukan pemenang
            Tim pemenang;
            if (skor1 > skor2) {
                pemenang = lawan;
                pemenang.skor = skor1;
                cout << lawan.nama << " menang dengan skor " << skor1 << "-" << skor2 << endl;
            } else {
                pemenang = timDenganBye;
                pemenang.skor = skor2;
                cout << timDenganBye.nama << " menang dengan skor " << skor2 << "-" << skor1 << endl;
            }
            
            // Pemenang bertanding selanjutnya
            pertandinganDenganBye = false;
            antrianBerikutnya.push(pemenang);
        }
        // Jika hanya tersisa satu tim di pertandingan reguler, pindahkan ke antrian berikutnya
        else if (antrianPertandingan.size() == 1) {
            antrianBerikutnya.push(antrianPertandingan.front());
            cout << antrianPertandingan.front().nama << " melangkah ke babak berikutnya tanpa lawan.\n";
            antrianPertandingan.pop();
        }
        
        // Jika semua antrian kosong kecuali antrianBerikutnya dengan satu tim, berarti sudah ada pemenang
        if (antrianPertandingan.empty() && antrianBye.empty() && antrianBerikutnya.size() == 1) {
            break;
        }
    }

    cout << "\n=== JUARA " << namaOlahraga << " ===\n";
    if (!antrianBerikutnya.empty()) {
        if (isPeserta) {
            cout << "Pemenang: " << antrianBerikutnya.front().nama << endl;
        } else {
            cout << "Tim pemenang: " << antrianBerikutnya.front().nama << endl;
        }
    }

    return 0;
}