#ifndef LIGA_MIX_H
#define LIGA_MIX_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// Struktur data untuk tim
struct TimLiga {
    string nama;
    int main = 0;
    int menang = 0;
    int seri = 0;
    int kalah = 0;
    int poin = 0;
    int golCetak = 0;
    int golKebobolan = 0;
    int selisihGol = 0;
    int setMenang = 0;
    int setKalah = 0;
    int poinCetak = 0;
    int poinKebobolan = 0;
};

// Node untuk linked list pertandingan
struct PertandinganNode {
    int tim1, tim2;
    int skor1, skor2;
    bool sudahDimainkan = false;
    bool kandang = true;  // true jika tim1 bermain di kandang
    PertandinganNode* next = nullptr;
};

// Struktur untuk mengelola linked list pertandingan
struct JadwalPertandingan {
    PertandinganNode* head;
    PertandinganNode* tail;
    int jumlah;

    // Constructor
    JadwalPertandingan() : head(nullptr), tail(nullptr), jumlah(0) {}
};

// Fungsi eksternal
json loadTurnamen();
void saveTurnamen(const json& data);

// Fungsi untuk sistem liga
bool loadTurnamenData();
void tambahPertandingan(int tim1, int tim2, bool kandang = true);
void inputDataTim();
void buatJadwalBerger(bool doubleRound = false);
void buatJadwalSingleRoundRobin();
void buatJadwalDoubleRoundRobin();
void tampilkanJadwalPertandingan();
bool validasiSkor(int jenis, int skor1, int skor2, string& pesanError);
void getHeadToHead(int tim1, int tim2, int& menang1, int& menang2, int& seri);
void updateStatistikTim(PertandinganNode* match);
void inputSkorHari(int hari);
bool compareTeams(const TimLiga& a, const TimLiga& b);
void urutkanKlasemen();
void tampilkanKlasemen();
void tampilkanPemenang();
json jadwalToJson();
void simpanTurnamen();
void bersihkanMemori();
void sistemLiga();

#endif