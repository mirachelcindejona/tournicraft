#ifndef SISTEM_GUGUR_H
#define SISTEM_GUGUR_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <cmath>

using namespace std;
using json = nlohmann::json;

// Struktur untuk tim dalam sistem gugur
struct TimGugur {
    string nama;
    int skor;
    bool isEliminated;
};

// Struktur untuk pertandingan dalam sistem gugur
struct PertandinganGugur {
    TimGugur tim1;
    TimGugur tim2;
    string deskripsi;
    bool sudahDimainkan;
    int skor1;
    int skor2;
    int babak;
    PertandinganGugur* next; // Pointer untuk linked list
};

// Struktur untuk mengelola jadwal pertandingan
struct JadwalGugur {
    PertandinganGugur* head;
    PertandinganGugur* tail;
    int jumlah;
    
    // Constructor
    JadwalGugur() : head(nullptr), tail(nullptr), jumlah(0) {}
};

// Fungsi utama sistem gugur
void sistemGugur();

#endif