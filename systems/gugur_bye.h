#ifndef GUGUR_BYE_H
#define GUGUR_BYE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

// Struktur untuk tim
struct TimBye {
    string nama;
    int skor;
    bool isBye;
};

// Struktur untuk pertandingan
struct PertandinganBye {
    TimBye tim1;
    TimBye tim2;
    bool isBye;
    string deskripsi;
    bool sudahDimainkan;
    int skor1;
    int skor2;
    PertandinganBye* next; // Pointer untuk linked list
};

// Struktur untuk mengelola jadwal pertandingan
struct JadwalBye {
    PertandinganBye* head;
    PertandinganBye* tail;
    int jumlah;
    
    // Constructor
    JadwalBye() : head(nullptr), tail(nullptr), jumlah(0) {}
};

// Fungsi untuk menghitung power of two terdekat
int nextPowerOfTwo(int n);
int previousPowerOfTwo(int n);

// Fungsi utama sistem gugur bye
int gugurBye();

#endif