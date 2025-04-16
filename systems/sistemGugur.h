#ifndef SISTEM_GUGUR_H
#define SISTEM_GUGUR_H

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// Struktur data untuk tim dalam sistem gugur
struct TimGugur {
    string nama;
    bool aktif = true;  // Status tim masih dalam turnamen
    int skor = 0;       // Skor dalam pertandingan terakhir
};

// Struktur data untuk pertandingan dalam sistem gugur
struct PertandinganGugur {
    int tim1, tim2;     // Index tim yang bertanding (-1 jika belum ditentukan)
    int skor1, skor2;
    bool sudahDimainkan = false;
    int babak;          // Babak pertandingan (1 = final, 2 = semifinal, dst)
    int nomorMatch;     // Nomor pertandingan dalam babak
};

// Fungsi-fungsi untuk sistem gugur
bool loadTurnamenDataGugur();
void inputDataTimGugur();
void buatBaganGugur();
void tampilkanBaganGugur();
void jalankanPertandingan();
void updateTimPemenang(int idxMatch, int idxPemenang);
void tampilkanPemenangGugur();
void simpanTurnamenGugur();
void sistemGugur();

#endif // SISTEM_GUGUR_H