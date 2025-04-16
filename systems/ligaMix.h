#ifndef LIGA_MIX_H
#define LIGA_MIX_H

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// Struktur data untuk tim
struct Tim {
    string nama;
    int main = 0;
    int menang = 0;
    int seri = 0;
    int kalah = 0;
    int poin = 0;
};

// Struktur data untuk pertandingan
struct Pertandingan {
    int tim1, tim2;
    int skor1, skor2;
    bool sudahDimainkan = false;
};

// Fungsi-fungsi untuk sistem liga
json loadTurnamen();
void saveTurnamen(const json& data);
bool loadTurnamenData();
void inputDataTim();
void buatJadwalSingleRoundRobin();
void buatJadwalDoubleRoundRobin();
void tampilkanJadwalPertandingan();
void inputSkorHari(int hari);
void urutkanKlasemen();
void tampilkanKlasemen();
void tampilkanPemenang();
void simpanTurnamen();
void sistemLiga();

#endif // LIGA_MIX_H
