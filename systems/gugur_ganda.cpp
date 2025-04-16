#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

struct Tim
{
    string nama;
    int kekalahan = 0;
};

struct Pertandingan
{
    int hari;
    Tim *tim1;
    Tim *tim2;
    Tim *pemenang = nullptr;
    Tim *kalah = nullptr;
    bool played = false;
    string bracket;
    int skor1 = 0;
    int skor2 = 0;
};

struct NodePertandingan
{
    Pertandingan pertandingan;
    NodePertandingan *selanjutnya;
};

NodePertandingan *jadwalPertandingan = nullptr;

void tambahNodePertandingan(const Pertandingan &p)
{
    NodePertandingan *nodeBaru = new NodePertandingan{p, nullptr};
    if (!jadwalPertandingan)
        jadwalPertandingan = nodeBaru;
    else
    {
        NodePertandingan *temp = jadwalPertandingan;
        while (temp->selanjutnya)
            temp = temp->selanjutnya;
        temp->selanjutnya = nodeBaru;
    }
}

void tampilkanJadwalHari(int hari)
{
    cout << "\n===== Jadwal Hari ke-" << hari << " =====\n";
    bool ditemukan = false;
    for (NodePertandingan *temp = jadwalPertandingan; temp; temp = temp->selanjutnya)
    {
        if (temp->pertandingan.hari == hari)
        {
            cout << "[" << temp->pertandingan.bracket << "] ";
            cout << (temp->pertandingan.tim1 ? temp->pertandingan.tim1->nama : "TBA")
                 << " vs "
                 << (temp->pertandingan.tim2 ? temp->pertandingan.tim2->nama : "TBA");
            if (temp->pertandingan.played)
            {
                cout << " --> Hasil: " << temp->pertandingan.skor1 << " - " << temp->pertandingan.skor2;
                cout << " --> Pemenang: " << temp->pertandingan.pemenang->nama;
            }
            cout << "\n";
            ditemukan = true;
        }
    }
    if (!ditemukan)
        cout << "Tidak ada pertandingan dijadwalkan hari ini.\n";
}

void tampilkanJadwalLengkap()
{
    cout << "\n====== Jadwal Pertandingan Lengkap ======\n";
    for (NodePertandingan *temp = jadwalPertandingan; temp; temp = temp->selanjutnya)
    {
        cout << "Hari " << temp->pertandingan.hari << " [" << temp->pertandingan.bracket << "]: ";
        cout << (temp->pertandingan.tim1 ? temp->pertandingan.tim1->nama : "TBA")
             << " vs "
             << (temp->pertandingan.tim2 ? temp->pertandingan.tim2->nama : "TBA");
        if (temp->pertandingan.played)
        {
            cout << " --> Hasil: " << temp->pertandingan.skor1 << " - " << temp->pertandingan.skor2;
            cout << " --> Pemenang: " << temp->pertandingan.pemenang->nama;
        }
        cout << "\n";
    }
}

string masukkanCabangOlahraga()
{
    cout << "Pilih cabang olahraga:\n";
    cout << "1. Sepak Bola\n2. Futsal\n3. Voli\n4. Badminton\n5. Tenis Meja\n6. Karate\n7. Taekwondo\n";
    int pilihan;
    cout << "Masukkan nomor pilihan: ";
    cin >> pilihan;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (pilihan)
    {
    case 1:
        return "sepak bola";
    case 2:
        return "futsal";
    case 3:
        return "voli";
    case 4:
        return "badminton";
    case 5:
        return "tenis meja";
    case 6:
        return "karate";
    case 7:
        return "taekwondo";
    default:
        return "sepak bola";
    }
}

int masukkanJumlahTim()
{
    int jml;
    while (true)
    {
        cout << "Jumlah tim: ";
        cin >> jml;
        if (jml % 2 != 0)
        {
            cout << "Jangan ganjil, masukkan jumlah tim genap.\n";
        }
        else
        {
            break;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return jml;
}

int masukkanJumlahHari()
{
    int hari;
    cout << "Jumlah hari pertandingan: ";
    cin >> hari;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return hari;
}

vector<Tim> masukkanTim(int jumlah)
{
    vector<Tim> timList;
    for (int i = 0; i < jumlah; i++)
    {
        Tim t;
        cout << "Nama tim ke-" << (i + 1) << ": ";
        getline(cin, t.nama);
        timList.push_back(t);
    }
    return timList;
}

bool olahragaBerbasisSet(const string &olahraga)
{
    return (olahraga == "voli" || olahraga == "badminton" ||
            olahraga == "tenis meja" || olahraga == "taekwondo" || olahraga == "karate");
}

int masukkanSkor(const string &namaTim, bool berbasisSet)
{
    int skor;
    while (true)
    {
        cout << "  Masukkan " << (berbasisSet ? "jumlah set dimenangkan" : "skor")
             << " untuk " << namaTim << ": ";
        cin >> skor;
        if (berbasisSet && (skor < 0 || skor > 3))
        {
            cout << "  Skor set harus antara 0 - 3.\n";
            continue;
        }
        break;
    }
    return skor;
}

vector<Pertandingan> buatPertandingan(const vector<Tim *> &daftarTim, int hari, const string &bracket)
{
    vector<Pertandingan> pertandinganList;
    for (size_t i = 0; i + 1 < daftarTim.size(); i += 2)
    {
        Pertandingan p;
        p.hari = hari;
        p.bracket = bracket;
        p.tim1 = daftarTim[i];
        p.tim2 = daftarTim[i + 1];
        pertandinganList.push_back(p);
    }
    return pertandinganList;
}

void buatPertandinganLL(const vector<Tim *> &daftarTim, int hari, const string &bracket)
{
    vector<Pertandingan> pertandinganList = buatPertandingan(daftarTim, hari, bracket);
    for (auto &p : pertandinganList)
        tambahNodePertandingan(p);
    tampilkanJadwalHari(hari);
}

void prosesPertandingan(Pertandingan &p, const string &olahraga)
{
    bool berbasisSet = olahragaBerbasisSet(olahraga);
    int skor1, skor2;
    while (true)
    {
        cout << "\n[" << p.bracket << "] Hari " << p.hari << " - "
             << p.tim1->nama << " vs " << p.tim2->nama << "\n";
        skor1 = masukkanSkor(p.tim1->nama, berbasisSet);
        skor2 = masukkanSkor(p.tim2->nama, berbasisSet);

        if (skor1 == skor2)
        {
            cout << "  Hasil tidak boleh seri. Ulangi input.\n";
            continue;
        }
        if (berbasisSet && (skor1 != 2 && skor2 != 2))
        {
            cout << "Keterangan tidak valid: Salah satu tim harus memenangkan 2 set.\n";
            continue;
        }
        break;
    }
    p.skor1 = skor1;
    p.skor2 = skor2;
    if (skor1 > skor2)
    {
        p.pemenang = p.tim1;
        p.kalah = p.tim2;
    }
    else
    {
        p.pemenang = p.tim2;
        p.kalah = p.tim1;
    }
    p.kalah->kekalahan++;
    p.played = true;
}

void jalankanTurnamen(const string &olahraga, int totalHari, vector<Tim> &timList)
{
    int hariSaatIni = 1;
    vector<Tim *> wb;
    vector<Tim *> lb;

    for (auto &t : timList)
        wb.push_back(&t);

    while (true)
    {
        if (wb.size() >= 2)
        {
            cout << "\nMemproses WB pada Hari " << hariSaatIni << "...\n";
            buatPertandinganLL(wb, hariSaatIni, "WB");
            for (NodePertandingan *temp = jadwalPertandingan; temp; temp = temp->selanjutnya)
            {
                if (temp->pertandingan.hari == hariSaatIni &&
                    temp->pertandingan.bracket == "WB" && !temp->pertandingan.played)
                    prosesPertandingan(temp->pertandingan, olahraga);
            }
            vector<Tim *> nextWB;
            for (NodePertandingan *temp = jadwalPertandingan; temp; temp = temp->selanjutnya)
            {
                if (temp->pertandingan.hari == hariSaatIni &&
                    temp->pertandingan.bracket == "WB")
                {
                    nextWB.push_back(temp->pertandingan.pemenang);
                    lb.push_back(temp->pertandingan.kalah);
                }
            }
            wb = nextWB;
        }

        if (lb.size() >= 2)
        {
            cout << "\nMemproses LB pada Hari " << hariSaatIni << "...\n";
            buatPertandinganLL(lb, hariSaatIni, "LB");
            for (NodePertandingan *temp = jadwalPertandingan; temp; temp = temp->selanjutnya)
            {
                if (temp->pertandingan.hari == hariSaatIni &&
                    temp->pertandingan.bracket == "LB" && !temp->pertandingan.played)
                    prosesPertandingan(temp->pertandingan, olahraga);
            }
            vector<Tim *> nextLB;
            for (NodePertandingan *temp = jadwalPertandingan; temp; temp = temp->selanjutnya)
            {
                if (temp->pertandingan.hari == hariSaatIni &&
                    temp->pertandingan.bracket == "LB")
                {
                    if (temp->pertandingan.pemenang->kekalahan < 2)
                        nextLB.push_back(temp->pertandingan.pemenang);
                }
            }
            lb = nextLB;
        }

        if (wb.size() == 1 && lb.size() == 1)
        {
            int hariFinal = totalHari;
            Pertandingan finalPertandingan;
            finalPertandingan.hari = hariFinal;
            finalPertandingan.bracket = "FINAL";
            finalPertandingan.tim1 = wb[0];
            finalPertandingan.tim2 = lb[0];
            cout << "\nGrand Final dijadwalkan pada Hari " << hariFinal << ":\n";
            cout << finalPertandingan.tim1->nama << " (WB) vs " << finalPertandingan.tim2->nama << " (LB)\n";
            prosesPertandingan(finalPertandingan, olahraga);
            if (finalPertandingan.tim1 == finalPertandingan.kalah && finalPertandingan.tim1->kekalahan == 1)
            {
                cout << "\nGrand Final Reset diperlukan karena " << finalPertandingan.tim1->nama
                     << " baru kalah satu kali. Main ulang Grand Final.\n";
                Pertandingan finalReset;
                finalReset.hari = hariFinal;
                finalReset.bracket = "FINAL-RESET";
                finalReset.tim1 = finalPertandingan.tim1;
                finalReset.tim2 = finalPertandingan.tim2;
                prosesPertandingan(finalReset, olahraga);
                finalPertandingan = finalReset;
            }
            tambahNodePertandingan(finalPertandingan);
            cout << "\nJUARA TURNAMEN: " << finalPertandingan.pemenang->nama << "\n";
            break;
        }

        hariSaatIni++;
        if (hariSaatIni > totalHari)
            hariSaatIni = totalHari;
    }
    tampilkanJadwalLengkap();
}

int main()
{
    string olahraga = masukkanCabangOlahraga();
    int jumlahTim = masukkanJumlahTim();
    int jumlahHari = masukkanJumlahHari();
    vector<Tim> daftarTim = masukkanTim(jumlahTim);
    jalankanTurnamen(olahraga, jumlahHari, daftarTim);
    return 0;
}
