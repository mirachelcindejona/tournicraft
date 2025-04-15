#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

const int MAX_TIM = 20;

struct Tim {
    string nama;
    int main = 0;
    int menang = 0;
    int seri = 0;
    int kalah = 0;
    int poin = 0;
};

struct Pertandingan {
    int tim1, tim2;
    int skor1, skor2;
    bool sudahDimainkan = false;
};

Tim tim[MAX_TIM];
Pertandingan jadwal[MAX_TIM * (MAX_TIM - 1)];
int jumlahTim, jumlahHari, jumlahMatch = 0;
int jenisOlahraga;

void inputDataTim() {
    do {
        cout << "Masukkan jumlah tim (maks " << MAX_TIM << "): ";
        cin >> jumlahTim;
        if (jumlahTim < 2 || jumlahTim > MAX_TIM) {
            cout << "Jumlah tim harus antara 2 sampai " << MAX_TIM << "!\n";
        }
    } while (jumlahTim < 2 || jumlahTim > MAX_TIM);

    cin.ignore(); // Bersihkan newline sebelum getline
    for (int i = 0; i < jumlahTim; i++) {
        cout << "Nama tim ke-" << i + 1 << ": ";
        getline(cin, tim[i].nama);
    }
}

void buatJadwalSingleRoundRobin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = i + 1; j < jumlahTim; j++) {
            jadwal[jumlahMatch++] = {i, j, 0, 0, false};
        }
    }
}

void buatJadwalDoubleRoundRobin() {
    for (int i = 0; i < jumlahTim - 1; i++) {
        for (int j = i + 1; j < jumlahTim; j++) {
            jadwal[jumlahMatch++] = {i, j, 0, 0, false};
            jadwal[jumlahMatch++] = {j, i, 0, 0, false};
        }
    }
}

void tampilkanJadwalPertandingan() {
    int matchPerHari = jumlahMatch / jumlahHari;
    int sisa = jumlahMatch % jumlahHari;
    int idx = 0;
    for (int h = 0; h < jumlahHari; h++) {
        int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
        if (sisa > 0) sisa--;
        cout << "\nHari " << h + 1 << ":\n";
        for (int m = 0; m < matchHariIni && idx < jumlahMatch; m++, idx++) {
            cout << "- " << tim[jadwal[idx].tim1].nama << " vs " << tim[jadwal[idx].tim2].nama << endl;
        }
    }
}

void inputSkorHari(int hari) {
    int matchPerHari = jumlahMatch / jumlahHari;
    int sisa = jumlahMatch % jumlahHari;
    
    int startIdx = 0;
    for (int h = 0; h < hari - 1; h++) {
        int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
        if (sisa > 0) sisa--;
        startIdx += matchHariIni;
    }
    
    int matchHariIni = matchPerHari + (sisa > 0 ? 1 : 0);
    if (sisa > 0) sisa--;
    
    cout << "\n=== Input Skor Pertandingan Hari " << hari << " ===\n";
    for (int i = startIdx; i < startIdx + matchHariIni && i < jumlahMatch; i++) {
        cout << tim[jadwal[i].tim1].nama << " vs " << tim[jadwal[i].tim2].nama << "\n";
        cout << "Skor " << tim[jadwal[i].tim1].nama << ": ";
        cin >> jadwal[i].skor1;
        cout << "Skor " << tim[jadwal[i].tim2].nama << ": ";
        cin >> jadwal[i].skor2;
        jadwal[i].sudahDimainkan = true;

        tim[jadwal[i].tim1].main++;
        tim[jadwal[i].tim2].main++;

        if (jenisOlahraga == 1 || jenisOlahraga == 2) { // Sepak Bola / Futsal
            if (jadwal[i].skor1 > jadwal[i].skor2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 3;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 < jadwal[i].skor2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 3;
                tim[jadwal[i].tim1].kalah++;
            } else {
                tim[jadwal[i].tim1].seri++;
                tim[jadwal[i].tim2].seri++;
                tim[jadwal[i].tim1].poin++;
                tim[jadwal[i].tim2].poin++;
            }
        } else if (jenisOlahraga == 3) { // Handball
            if (jadwal[i].skor1 > jadwal[i].skor2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 2;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 < jadwal[i].skor2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 2;
                tim[jadwal[i].tim1].kalah++;
            } else {
                tim[jadwal[i].tim1].seri++;
                tim[jadwal[i].tim2].seri++;
                tim[jadwal[i].tim1].poin++;
                tim[jadwal[i].tim2].poin++;
            }
        } else if (jenisOlahraga == 4 || jenisOlahraga == 6) { // Tenis Meja / Badminton
            if (jadwal[i].skor1 > jadwal[i].skor2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 2;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 < jadwal[i].skor2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 2;
                tim[jadwal[i].tim1].kalah++;
            }
        } else if (jenisOlahraga == 5) { // Voli
            if (jadwal[i].skor1 == 3 && jadwal[i].skor2 <= 1) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 3;
                tim[jadwal[i].tim2].kalah++;
            } else if (jadwal[i].skor1 == 3 && jadwal[i].skor2 == 2) {
                tim[jadwal[i].tim1].menang++;
                tim[jadwal[i].tim1].poin += 2;
                tim[jadwal[i].tim2].kalah++;
                tim[jadwal[i].tim2].poin += 1;
            } else if (jadwal[i].skor2 == 3 && jadwal[i].skor1 <= 1) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 3;
                tim[jadwal[i].tim1].kalah++;
            } else if (jadwal[i].skor2 == 3 && jadwal[i].skor1 == 2) {
                tim[jadwal[i].tim2].menang++;
                tim[jadwal[i].tim2].poin += 2;
                tim[jadwal[i].tim1].kalah++;
                tim[jadwal[i].tim1].poin += 1;
            }
        }
    }
}

void urutkanKlasemen() {
    sort(tim, tim + jumlahTim, [](const Tim &a, const Tim &b) {
        if (a.poin != b.poin)
            return a.poin > b.poin;
        else
            return a.menang > b.menang;
    });
}

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

int main() {
    cout << "Pilih jenis olahraga:\n";
    cout << "1. Sepak Bola\n2. Futsal\n3. Handball\n4. Tenis Meja\n5. Voli\n6. Badminton\n";
    cout << "Masukkan pilihan (1-6): ";
    cin >> jenisOlahraga;

    inputDataTim();

    cout << "Masukkan jumlah hari pertandingan: ";
    cin >> jumlahHari;

    if (jenisOlahraga == 1 || jenisOlahraga == 2 || jenisOlahraga == 5) {
        cout << "Pilih format pertandingan:\n";
        cout << "1. Single Round Robin\n2. Double Round Robin\n";
        int format;
        cin >> format;
        if (format == 1) buatJadwalSingleRoundRobin();
        else buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 3) {
        buatJadwalDoubleRoundRobin();
    } else if (jenisOlahraga == 4 || jenisOlahraga == 6) {
        buatJadwalSingleRoundRobin();
    }

    tampilkanJadwalPertandingan();
    
    char inputSkor;
    cout << "\nApakah ingin menginput skor pertandingan? (y/n): ";
    cin >> inputSkor;
    
    if (inputSkor == 'y' || inputSkor == 'Y') {
        for (int hari = 1; hari <= jumlahHari; hari++) {
            inputSkorHari(hari);
            cout << "\n=== Klasemen Sementara ===\n";
            tampilkanKlasemen();
            
            if (hari < jumlahHari) {
                char lanjut;
                cout << "\nApakah ingin menginput skor hari ke-" << hari + 1 << "? (y/n): ";
                cin >> lanjut;
                if (lanjut != 'y' && lanjut != 'Y') {
                    break;
                }
            }
        }
    }

    cout << "\n=== Klasemen Akhir ===\n";
    tampilkanKlasemen();
    tampilkanPemenang();

    return 0;
}