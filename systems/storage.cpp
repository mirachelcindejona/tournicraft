#include "systems/storage.h"

const string TURNAMEN_FILE = "data/turnamen.json";

// Fungsi untuk memuat data turnamen dari file JSON
json loadTurnamen() {
    ifstream inFile(TURNAMEN_FILE);
    if (!inFile) return json::array(); // jika file tidak ada, return array kosong
    json data;
    inFile >> data;
    return data;
}

// Fungsi untuk menyimpan data turnamen ke file JSON
void saveTurnamen(const json& data) {
    ofstream outFile(TURNAMEN_FILE);
    outFile << data.dump(4);
}
