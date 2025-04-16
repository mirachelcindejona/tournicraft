#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
using namespace std;

// Struktur Team: menyimpan nama tim dan jumlah kekalahan.
struct Team
{
    string name;
    int losses = 0;
};

// Struktur Match: menyimpan informasi pertandingan.
struct Match
{
    int day; // Hari pertandingan dijadwalkan.
    Team *team1;
    Team *team2;
    Team *winner = nullptr;
    Team *loser = nullptr;
    bool played = false;
    string bracket; // "WB", "LB", "FINAL", atau "FINAL-RESET".
    int score1 = 0; // Skor/set tim1.
    int score2 = 0; // Skor/set tim2.
};

// Node untuk linked list jadwal pertandingan.
struct MatchNode
{
    Match match;
    MatchNode *next;
};

// Global pointer untuk head linked list jadwal.
MatchNode *scheduleHead = nullptr;

// Tambahkan node pertandingan ke akhir linked list.
void appendMatchNode(const Match &m)
{
    MatchNode *newNode = new MatchNode{m, nullptr};
    if (!scheduleHead)
        scheduleHead = newNode;
    else
    {
        MatchNode *temp = scheduleHead;
        while (temp->next)
            temp = temp->next;
        temp->next = newNode;
    }
}

// Tampilkan jadwal pertandingan untuk hari tertentu beserta hasil jika sudah dimainkan.
void displayScheduleForDay(int day)
{
    cout << "\n===== Jadwal Hari ke-" << day << " =====\n";
    bool found = false;
    for (MatchNode *temp = scheduleHead; temp; temp = temp->next)
    {
        if (temp->match.day == day)
        {
            cout << "[" << temp->match.bracket << "] ";
            cout << (temp->match.team1 ? temp->match.team1->name : "TBA")
                 << " vs "
                 << (temp->match.team2 ? temp->match.team2->name : "TBA");
            if (temp->match.played)
            {
                cout << " --> Hasil: " << temp->match.score1 << " - " << temp->match.score2;
                cout << " --> Pemenang: " << temp->match.winner->name;
            }
            cout << "\n";
            found = true;
        }
    }
    if (!found)
        cout << "Tidak ada pertandingan dijadwalkan hari ini.\n";
}

// Tampilkan jadwal pertandingan lengkap.
void displayFullSchedule()
{
    cout << "\n====== Jadwal Pertandingan Lengkap ======\n";
    for (MatchNode *temp = scheduleHead; temp; temp = temp->next)
    {
        cout << "Hari " << temp->match.day << " [" << temp->match.bracket << "]: ";
        cout << (temp->match.team1 ? temp->match.team1->name : "TBA")
             << " vs "
             << (temp->match.team2 ? temp->match.team2->name : "TBA");
        if (temp->match.played)
        {
            cout << " --> Hasil: " << temp->match.score1 << " - " << temp->match.score2;
            cout << " --> Pemenang: " << temp->match.winner->name;
        }
        cout << "\n";
    }
}

// Pemilihan cabang olahraga menggunakan nomor.
string inputSport()
{
    cout << "Pilih cabang olahraga:\n";
    cout << "1. Sepak Bola\n2. Futsal\n3. Voli\n4. Badminton\n5. Tenis Meja\n6. Karate\n7. Taekwondo\n";
    int choice;
    cout << "Masukkan nomor pilihan: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (choice)
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

int inputNumTeams()
{
    int num;
    cout << "Jumlah tim: ";
    cin >> num;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return num;
}

int inputNumDays()
{
    int days;
    cout << "Jumlah hari pertandingan: ";
    cin >> days;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return days;
}

vector<Team> inputTeams(int num)
{
    vector<Team> teams;
    for (int i = 0; i < num; i++)
    {
        Team t;
        cout << "Nama tim ke-" << (i + 1) << ": ";
        getline(cin, t.name);
        teams.push_back(t);
    }
    return teams;
}

// Tentukan apakah olahraga menggunakan sistem set.
bool isSetBasedSport(const string &sport)
{
    return (sport == "voli" || sport == "badminton" || sport == "tenis meja" ||
            sport == "taekwondo" || sport == "karate");
}

// Input skor atau set sesuai jenis olahraga.
int inputScore(const string &teamName, bool isSet)
{
    int score;
    while (true)
    {
        cout << "  Masukkan " << (isSet ? "jumlah set dimenangkan" : "skor")
             << " untuk " << teamName << ": ";
        cin >> score;
        if (isSet && (score < 0 || score > 3))
        {
            cout << "  Skor set harus antara 0 - 3.\n";
            continue;
        }
        break;
    }
    return score;
}

// Generate pertandingan dari vector pointer tim untuk suatu bracket pada hari tertentu.
vector<Match> generateMatches(const vector<Team *> &teamList, int day, const string &bracket)
{
    vector<Match> matches;
    for (size_t i = 0; i + 1 < teamList.size(); i += 2)
    {
        Match m;
        m.day = day;
        m.bracket = bracket;
        m.team1 = teamList[i];
        m.team2 = teamList[i + 1];
        matches.push_back(m);
    }
    return matches;
}

// Bungkus generateMatches() dan simpan ke linked list.
void generateMatchesLL(const vector<Team *> &teamList, int day, const string &bracket)
{
    vector<Match> matches = generateMatches(teamList, day, bracket);
    for (auto &m : matches)
        appendMatchNode(m);
    displayScheduleForDay(day);
}

void processMatch(Match &m, const string &sport)
{
    bool isSet = isSetBasedSport(sport);
    int score1, score2;
    while (true)
    {
        cout << "\n[" << m.bracket << "] Hari " << m.day << " - "
             << m.team1->name << " vs " << m.team2->name << "\n";
        score1 = inputScore(m.team1->name, isSet);
        score2 = inputScore(m.team2->name, isSet);
        if (score1 == score2)
        {
            cout << "  Hasil tidak boleh seri. Ulangi input.\n";
        }
        else
        {
            break;
        }
    }
    m.score1 = score1;
    m.score2 = score2;
    if (score1 > score2)
    {
        m.winner = m.team1;
        m.loser = m.team2;
    }
    else
    {
        m.winner = m.team2;
        m.loser = m.team1;
    }
    m.loser->losses++; // Tambahkan kekalahan untuk tim yang kalah.
    m.played = true;
}

void runTournament(const string &sport, int totalDays, vector<Team> &teams)
{
    int currentDay = 1;
    vector<Team *> wb; // Winner Bracket
    vector<Team *> lb; // Loser Bracket

    for (auto &t : teams)
        wb.push_back(&t);

    while (true)
    {
        // --- Proses WB ---
        if (wb.size() >= 2)
        {
            cout << "\nMemproses WB pada Hari " << currentDay << "...\n";
            generateMatchesLL(wb, currentDay, "WB");
            for (MatchNode *temp = scheduleHead; temp; temp = temp->next)
            {
                if (temp->match.day == currentDay && temp->match.bracket == "WB" && !temp->match.played)
                    processMatch(temp->match, sport);
            }
            vector<Team *> nextWB;
            for (MatchNode *temp = scheduleHead; temp; temp = temp->next)
            {
                if (temp->match.day == currentDay && temp->match.bracket == "WB")
                {
                    nextWB.push_back(temp->match.winner);
                    lb.push_back(temp->match.loser);
                }
            }
            wb = nextWB;
        }
        // --- Proses LB ---
        if (lb.size() >= 2)
        {
            cout << "\nMemproses LB pada Hari " << currentDay << "...\n";
            generateMatchesLL(lb, currentDay, "LB");
            for (MatchNode *temp = scheduleHead; temp; temp = temp->next)
            {
                if (temp->match.day == currentDay && temp->match.bracket == "LB" && !temp->match.played)
                    processMatch(temp->match, sport);
            }
            vector<Team *> nextLB;
            for (MatchNode *temp = scheduleHead; temp; temp = temp->next)
            {
                if (temp->match.day == currentDay && temp->match.bracket == "LB")
                {
                    if (temp->match.winner->losses < 2)
                        nextLB.push_back(temp->match.winner);
                }
            }
            lb = nextLB;
        }

        // --- Cek Kondisi Final ---
        if (wb.size() == 1 && lb.size() == 1)
        {
            int finalDay = totalDays;
            Match gf;
            gf.day = finalDay;
            gf.bracket = "FINAL";
            gf.team1 = wb[0];
            gf.team2 = lb[0];
            cout << "\nGrand Final dijadwalkan pada Hari " << finalDay << ":\n";
            cout << gf.team1->name << " (WB) vs " << gf.team2->name << " (LB)\n";
            processMatch(gf, sport);
            // Grand Final Reset: jika finalis WB kalah dan baru mendapat 1 kekalahan.
            if (gf.team1 == gf.loser && gf.team1->losses == 1)
            {
                cout << "\nGrand Final Reset diperlukan karena " << gf.team1->name
                     << " baru kalah satu kali. Main ulang Grand Final.\n";
                Match gfReset;
                gfReset.day = finalDay;
                gfReset.bracket = "FINAL-RESET";
                gfReset.team1 = gf.team1;
                gfReset.team2 = gf.team2;
                processMatch(gfReset, sport);
                gf = gfReset;
            }
            appendMatchNode(gf);
            cout << "\nJUARA TURNAMEN: " << gf.winner->name << " 🏆\n";
            break;
        }
        // Lanjut ke hari berikutnya.
        currentDay++;
        if (currentDay > totalDays)
            currentDay = totalDays;
    }
    displayFullSchedule();
}

int main()
{
    string sport = inputSport();
    int numTeams = inputNumTeams();
    int days = inputNumDays();
    vector<Team> teamList = inputTeams(numTeams);
    runTournament(sport, days, teamList);
    return 0;
}
