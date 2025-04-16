#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct Player {
    string name;
};

struct MatchNode {
    Player* p1;
    Player* p2;
    int score1;
    int score2;
    string tanggal;
    string waktu;
    Player* winner;
    MatchNode* next;
};

// Fungsi buat tanggal berdasarkan hari dan input tanggal mulai
string generateTanggal(int hari, int startTanggal, int startBulan, int startTahun) {
    int dd = startTanggal + hari - 1;
    int mm = startBulan;
    int yy = startTahun;
    if (dd > 30) {
        dd -= 30;
        mm += 1;
    }
    ostringstream oss;
    oss << setfill('0') << setw(2) << dd << "/"
        << setw(2) << mm << "/"
        << yy;
    return oss.str();
}

// Buat pertandingan dengan penjadwalan otomatis
MatchNode* createMatches(Player players[], int count, int totalHari, int tgl, int bln, int thn) {
    MatchNode* head = nullptr;
    MatchNode* tail = nullptr;

    vector<string> waktuList = {"14:00", "16:00", "18:00"};
    int pertandinganKe = 0;

    for (int i = 0; i < count; i += 2) {
        MatchNode* newNode = new MatchNode;
        newNode->p1 = &players[i];
        newNode->p2 = &players[i + 1];
        newNode->score1 = 0;
        newNode->score2 = 0;

        int hari = (pertandinganKe % totalHari) + 1;
        string tanggal = generateTanggal(hari, tgl, bln, thn);
        string waktu = waktuList[pertandinganKe % waktuList.size()];

        newNode->tanggal = tanggal;
        newNode->waktu = waktu;
        newNode->winner = nullptr;
        newNode->next = nullptr;

        if (!head) head = tail = newNode;
        else {
            tail->next = newNode;
            tail = newNode;
        }
        pertandinganKe++;
    }

    return head;
}

// Tampilkan jadwal pertandingan
void tampilkanJadwal(MatchNode* head, int round, const string& turnamen) {
    MatchNode* current = head;
    int no = 1;
    cout << "\n📅 Jadwal Babak " << round << " - " << turnamen << ":\n";
    while (current) {
        cout << "Pertandingan " << no++ << ": " << current->p1->name
             << " vs " << current->p2->name
             << " | Tanggal: " << current->tanggal
             << " | Waktu: " << current->waktu << "\n";
        current = current->next;
    }
}

// Input skor tiap pertandingan
void inputSkor(MatchNode* head) {
    MatchNode* curr = head;
    while (curr) {
        cout << "\n" << curr->p1->name << " vs " << curr->p2->name << "\n";
        cout << "Skor " << curr->p1->name << ": "; cin >> curr->score1;
        cout << "Skor " << curr->p2->name << ": "; cin >> curr->score2;

        if (curr->score1 > curr->score2)
            curr->winner = curr->p1;
        else if (curr->score2 > curr->score1)
            curr->winner = curr->p2;
        else {
            int pilih;
            cout << "Skor seri! Pilih pemenang (1/" << curr->p1->name << ", 2/" << curr->p2->name << "): ";
            cin >> pilih;
            curr->winner = (pilih == 1) ? curr->p1 : curr->p2;
        }
        curr = curr->next;
    }
}

// Buat babak selanjutnya dari pemenang
MatchNode* babakBerikutnya(MatchNode* head, int hari, int totalHari, int tgl, int bln, int thn) {
    vector<Player> nextPlayers;
    MatchNode* curr = head;
    while (curr) {
        nextPlayers.push_back(*curr->winner);
        curr = curr->next;
    }
    return createMatches(&nextPlayers[0], nextPlayers.size(), totalHari, tgl, bln, thn);
}

// Tampilkan juara
void tampilkanJuara(MatchNode* lastRound, const string& turnamen) {
    if (lastRound && lastRound->winner)
        cout << "\n🏆 JUARA " << turnamen << ": " << lastRound->winner->name << " 🏆\n";
}

int main() {
    int n, hariMax, tgl, bln, thn;
    string turnamen;

    cout << "Nama turnamen: ";
    getline(cin, turnamen);

    cout << "Tanggal mulai (dd): "; cin >> tgl;
    cout << "Bulan mulai (mm): "; cin >> bln;
    cout << "Tahun mulai (yyyy): "; cin >> thn;

    cout << "Jumlah hari pertandingan: "; cin >> hariMax;
    cout << "Jumlah pemain (genap): "; cin >> n;

    if (n % 2 != 0 || n < 2) {
        cout << "Jumlah pemain harus genap dan minimal 2.\n";
        return 1;
    }

    cin.ignore(); // flush newline
    Player* players = new Player[n];
    for (int i = 0; i < n; ++i) {
        cout << "Nama pemain ke-" << (i + 1) << ": ";
        getline(cin, players[i].name);
    }

    MatchNode* round = createMatches(players, n, hariMax, tgl, bln, thn);
    int babak = 1;

    while (round && round->next) {
        tampilkanJadwal(round, babak, turnamen);
        inputSkor(round);
        round = babakBerikutnya(round, babak + 1, hariMax, tgl, bln, thn);
        babak++;
    }

    tampilkanJadwal(round, babak, turnamen);
    inputSkor(round);
    tampilkanJuara(round, turnamen);

    delete[] players;
    return 0;
}
