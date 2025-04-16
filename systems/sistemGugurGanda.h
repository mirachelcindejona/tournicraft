#ifndef SISTEM_GUGUR_GANDA_H
#define SISTEM_GUGUR_GANDA_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <cmath>

using namespace std;
using json = nlohmann::json;

// Struktur untuk tim dalam sistem gugur ganda
struct TimGugurGanda {
    string nama;
    int skor;
    bool isEliminated;
    int kekalahan; // dalam gugur ganda, tim dieliminasi setelah kalah 2 kali
};

// Struktur untuk pertandingan dalam sistem gugur ganda
struct PertandinganGugurGanda {
    TimGugurGanda tim1;
    TimGugurGanda tim2;
    string deskripsi;
    bool sudahDimainkan;
    int skor1;
    int skor2;
    int babak;
    bool isLoserBracket; // true jika pertandingan di bracket pecundang
    PertandinganGugurGanda* next; // Pointer untuk linked list
};

// Struktur untuk mengelola jadwal pertandingan
struct JadwalGugurGanda {
    PertandinganGugurGanda* head;
    PertandinganGugurGanda* tail;
    int jumlah;
    
    // Constructor
    JadwalGugurGanda() : head(nullptr), tail(nullptr), jumlah(0) {}
};

// Fungsi utama sistem gugur ganda
void sistemGugurGanda();

#endif // SISTEM_GUGUR_GANDA_H