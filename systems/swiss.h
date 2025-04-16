#ifndef SISTEM_SWISS_H
#define SISTEM_SWISS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "nlohmann/json.hpp"
#include "systems/storage.h"

using namespace std;
using json = nlohmann::json;

// Struktur untuk tim dalam sistem Swiss
struct TimSwiss {
    string nama;
    int poin = 0;
    int matchPlayed = 0;
    bool byeRound = false;
    vector<int> lawan; // Menyimpan indeks lawan yang sudah dihadapi
};

// Struktur untuk pertandingan dalam sistem Swiss
struct PertandinganSwiss {
    TimSwiss tim1;
    TimSwiss tim2;
    bool isBye; // Apakah pertandingan ini adalah bye (tim lawan kosong)
    int skor1;
    int skor2;
    bool sudahDimainkan;
    string deskripsi;
    PertandinganSwiss* next; // Pointer untuk linked list
};

// Struktur untuk mengelola jadwal pertandingan sebagai linked list
struct JadwalSwiss {
    PertandinganSwiss* head;
    PertandinganSwiss* tail;
    int jumlah;
    
    // Constructor
    JadwalSwiss() : head(nullptr), tail(nullptr), jumlah(0) {}
};

// Fungsi utama sistem Swiss
void sistemSwiss();
int swiss(); // Backward compatibility function

#endif // SISTEM_SWISS_H