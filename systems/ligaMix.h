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
struct TimLiga {
    string nama;
    int main = 0;
    int menang = 0;
    int seri = 0;
    int kalah = 0;
    int poin = 0;
};

// Node untuk linked list pertandingan
struct PertandinganNode {
    int tim1, tim2;
    int skor1, skor2;
    bool sudahDimainkan = false;
    PertandinganNode* next; // Pointer ke node berikutnya untuk linked list
};

// Struktur untuk mengelola linked list pertandingan
struct JadwalPertandingan {
    PertandinganNode* head;
    PertandinganNode* tail;
    int jumlah;

    // Constructor
    JadwalPertandingan() : head(nullptr), tail(nullptr), jumlah(0) {}
};

// Fungsi-fungsi untuk sistem liga
void sistemLiga();

#endif