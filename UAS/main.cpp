#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#include <fstream>
#include <iomanip>

using namespace std;

struct Node {
    string subject;
    string teacher;
    string day;
    string startTime;
    string endTime;
    string kelas;
    Node* next;
};

Node* createNode(string subject, string teacher, string kelas, string day, string startTime, string endTime) {
    Node* newNode = new Node();
    newNode->subject = subject;
    newNode->teacher = teacher;
    newNode->kelas = kelas;
    newNode->day = day;
    newNode->startTime = startTime;
    newNode->endTime = endTime;
    newNode->next = nullptr;
    return newNode;
}

string calculateEndTime(string startTime, int duration) {
    int hour = stoi(startTime.substr(0, 2));
    int minute = stoi(startTime.substr(3, 2));
    hour += duration;
    if (hour >= 24) {
        hour -= 24;
    }
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d.%02d", hour, minute);
    return string(buffer);
}

void removeSubject(Node*& head, string day, string startTime) {
    if (!head) {
        cout << "Jadwal kosong." << endl;
        return;
    }
    if (head->day == day && head->startTime == startTime) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return;
    }
    Node* temp = head;
    Node* prev = nullptr;
    while (temp && !(temp->day == day && temp->startTime == startTime)) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) {
        cout << "Mata pelajaran tidak ditemukan." << endl;
        return;
    }
    prev->next = temp->next;
    delete temp;
}

void searchSubject(Node* head, string subject) {
    Node* temp = head;
    bool found = false;
    while (temp) {
        if (temp->subject == subject) {
            cout << "Mata pelajaran     : " << temp->subject << endl;
            cout << "Guru               : " << temp->teacher << endl;
            cout << "Kelas              : " << temp->kelas << endl;
            cout << "Hari               : " << temp->day << endl;
            cout << "Jam                : " << temp->startTime << " - " << temp->endTime << endl << endl;
            found = true;
        }
        temp = temp->next;
    }
    if (!found) {
        cout << "Mata pelajaran tidak ditemukan." << endl;
    }
}

void displayJadwal(Node* head) {
    if (!head) {
        cout << "Jadwal kosong." << endl;
        return;
    }
    Node* temp = head;
    int no = 1;  // Mulai nomor dari 1
    cout << "+----+--------------------+----------------+--------+-----------------+-----------------------+" << endl;
    cout << "| No |   Mata Pelajaran   |      Guru      | Kelas  |       Hari      |        Waktu          |" << endl;
    cout << "+----+--------------------+----------------+--------+-----------------+-----------------------+" << endl;
    while (temp) {
        cout << "| " << setw(2) << no << " | " << setw(18) << left << temp->subject
            << " | " << setw(14) << temp->teacher
            << " | " << setw(6) << temp->kelas
            << " | " << setw(15) << temp->day
            << " | " << setw(5) << temp->startTime << " - " << setw(13) << temp->endTime << " |" << endl;
        no++;
        temp = temp->next;
    }
    cout << "+----+--------------------+----------------+--------+-----------------+-----------------------+" << endl;
}

void printJadwalToHTML(Node* head, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: cannot create file" << endl;
        return;
    }

    // Write HTML headers
    file << "<!DOCTYPE html>\n<html>\n<head>\n<title>Jadwal Mata Pelajaran</title>\n";
    file << "<style>\n";
    file << "table { border-collapse: collapse; width: 100%; }\n";
    file << "th, td { border: 1px solid black; padding: 8px; text-align: left; }\n";
    file << "th { background-color: #f2f2f2; }\n";
    file << "</style>\n";
    file << "</head>\n<body>\n";

    // Write table headers
    file << "<h2>Jadwal Mata Pelajaran</h2>\n";
    file << "<table>\n";
    file << "<tr>\n";
    file << "<th>No</th>\n";
    file << "<th>Mata Pelajaran</th>\n";
    file << "<th>Guru</th>\n";
    file << "<th>Kelas</th>\n";
    file << "<th>Hari</th>\n";
    file << "<th>Waktu</th>\n";
    file << "</tr>\n";

    // Write table data
    Node* temp = head;
    int no = 1;
    while (temp) {
        file << "<tr>\n";
        file << "<td>" << no << "</td>\n";
        file << "<td>" << temp->subject << "</td>\n";
        file << "<td>" << temp->teacher << "</td>\n";
        file << "<td>" << temp->kelas << "</td>\n";
        file << "<td>" << temp->day << "</td>\n";
        file << "<td>" << temp->startTime << " - " << temp->endTime << "</td>\n";
        file << "</tr>\n";
        temp = temp->next;
        no++;
    }

    // Write HTML footers
    file << "</table>\n</body>\n</html>";

    file.close();
    cout << "Jadwal telah dicetak ke " << filename << endl;

#ifdef _WIN32
    system(("start " + filename).c_str());
#elif __APPLE__
    system(("open " + filename).c_str());
#else
    system(("xdg-open " + filename).c_str());
#endif
}

int dayToNumber(string day) {
    string days[] = { "senin", "selasa", "rabu", "kamis", "jumat", "sabtu" };
    for (int i = 0; i < 6; ++i) {
        if (day == days[i]) return i + 1;
    }
    return 7; // Jika hari tidak valid
}

bool compareTime(string time1, string time2) {
    int hour1 = stoi(time1.substr(0, 2));
    int minute1 = stoi(time1.substr(3, 2));
    int hour2 = stoi(time2.substr(0, 2));
    int minute2 = stoi(time2.substr(3, 2));
    if (hour1 != hour2) {
        return hour1 < hour2;
    }
    return minute1 < minute2;
}

void sortJadwal(Node*& head) {
    if (!head || !head->next) return;
    Node* i = head;
    Node* j = nullptr;
    while (i) {
        j = i->next;
        while (j) {
            if (dayToNumber(i->day) > dayToNumber(j->day) ||
                (dayToNumber(i->day) == dayToNumber(j->day) && !compareTime(i->startTime, j->startTime))) {
                swap(i->subject, j->subject);
                swap(i->teacher, j->teacher);
                swap(i->kelas, j->kelas);
                swap(i->day, j->day);
                swap(i->startTime, j->startTime);
                swap(i->endTime, j->endTime);
            }
            j = j->next;
        }
        i = i->next;
    }
}

void Undo(Node*& head, stack<Node*>& undoStack) {
    if (undoStack.empty()) {
        cout << "Tidak ada aksi yang bisa dihapus." << endl;
        return;
    }
    Node* lastAction = undoStack.top();
    undoStack.pop();
    if (head == lastAction) {
        head = head->next;
        delete lastAction;
        return;
    }
    Node* temp = head;
    while (temp->next != lastAction) {
        temp = temp->next;
    }
    temp->next = lastAction->next;
    delete lastAction;
}

void addSubject(Node*& head, stack<Node*>& undoStack, string subject, string teacher, string kelas, string day, string startTime, int duration) {
    string endTime = calculateEndTime(startTime, duration);
    Node* newNode = createNode(subject, teacher, kelas, day, startTime, endTime);
    if (!head) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    undoStack.push(newNode);
}

void adminMenu(Node*& jadwal, stack<Node*>& undoStack) {
    int pilihanMenu;
    string subject, teacher, day, startTime, kelas;
    int duration;
    char ulang;

    do {
        cout << "+-----------------------------------+" << endl;
        cout << "| SISTEM PENJADWALAN MATA PELAJARAN |" << endl;
        cout << "+---+-------------------------------+" << endl;
        cout << "|Menu :                             |" << endl;
        cout << "+---+-------------------------------+" << endl;
        cout << "|1. | Menambah Mata Pelajaran       |" << endl;
        cout << "|2. | Menghapus Mata Pelajaran      |" << endl;
        cout << "|3. | Mencari Mata Pelajaran        |" << endl;
        cout << "|4. | Menampilkan Jadwal            |" << endl;
        cout << "|5. | Mengurutkan Jadwal            |" << endl;
        cout << "|6. | Undo Aksi Terakhir            |" << endl;
        cout << "|7. | Cetak Jadwal                  |" << endl;
        cout << "|8. | Selesai                       |" << endl;
        cout << "+---+-------------------------------+" << endl << endl;

        cout << "Masukkan pilihan anda : ";
        cin >> pilihanMenu;
        system("cls");
        cout << endl;

        switch (pilihanMenu) {
        case 1:
            cout << "Masukkan nama mata pelajaran : ";
            cin.ignore();
            getline(cin, subject);
            cout << "Masukkan nama guru           : ";
            getline(cin, teacher);
            cout << "Masukkan kelas               : ";
            getline(cin, kelas);
            cout << "Masukkan hari                : ";
            getline(cin, day);
            cout << "Masukkan jam mulai (HH.MM)   : ";
            getline(cin, startTime);
            cout << "Masukkan durasi (dalam jam)  : ";
            cin >> duration;
            addSubject(jadwal, undoStack, subject, teacher, kelas, day, startTime, duration);
            break;

        case 2:
            cout << "Masukkan hari mata pelajaran yang ingin dihapus : ";
            cin.ignore();
            getline(cin, day);
            cout << "Masukkan jam mulai mata pelajaran yang ingin dihapus  : ";
            getline(cin, startTime);
            removeSubject(jadwal, day, startTime);
            break;

        case 3:
            cout << "Masukkan nama mata pelajaran yang ingin dicari : ";
            cin.ignore();
            getline(cin, subject);
            searchSubject(jadwal, subject);
            break;

        case 4:
            displayJadwal(jadwal);
            break;

        case 5:
            sortJadwal(jadwal);
            cout << "Jadwal telah diurutkan." << endl;
            break;

        case 6:
            Undo(jadwal, undoStack);
            break;

        case 7:
            printJadwalToHTML(jadwal, "jadwal.html");
            break;

        case 8:
            return;  // Kembali ke menu login

        default:
            cout << "Menu yang anda pilih tidak terdaftar" << endl << endl;
        }

        cout << "\nKembali ke menu? (y/n): ";
        cin >> ulang;
        system("cls");
    } while (ulang == 'y' || ulang == 'Y');
}

bool validateStudentLogin() {
    const string studentUsername = "murid";
    const string studentPassword = "password123";
    string username, password;

    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;
    system("cls");
    return (username == studentUsername && password == studentPassword);
}

void studentMenu(Node* jadwal) {
    char ulang;
    do {
        cout << "+-----------------------------------+" << endl;
        cout << "|       SISTEM PENJADWALAN          |" << endl;
        cout << "+---+-------------------------------+" << endl;
        cout << "|Menu :                             |" << endl;
        cout << "+---+-------------------------------+" << endl;
        cout << "|1. | Cetak Jadwal                  |" << endl;
        cout << "|2. | Selesai                       |" << endl;
        cout << "+---+-------------------------------+" << endl << endl;

        cout << "Masukkan pilihan anda : ";
        int pilihanMenu;
        cin >> pilihanMenu;
        system("cls");
        cout << endl;

        switch (pilihanMenu) {
        case 1:
            printJadwalToHTML(jadwal, "jadwal.html");
            break;

        case 2:
            return;  // Kembali ke menu login

        default:
            cout << "Menu yang anda pilih tidak terdaftar" << endl << endl;
        }

        cout << "\nKembali ke menu? (y/n): ";
        cin >> ulang;
        system("cls");
    } while (ulang == 'y' || ulang == 'Y');
}

bool validateAdminLogin() {
    const string adminUsername = "admin";
    const string adminPassword = "password123";
    string username, password;

    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;
    system("cls");
    return (username == adminUsername && password == adminPassword);
}

int main() {
    Node* jadwal = nullptr;
    stack<Node*> undoStack;
    int loginChoice;
    bool running = true;

    while (running) {
        cout << "Pilih Login: " << endl;
        cout << "1. Admin" << endl;
        cout << "2. Murid" << endl;
        cout << "3. Selesai" << endl;
        cout << "Masukkan pilihan anda: ";
        cin >> loginChoice;
        system("cls");

        switch (loginChoice) {
        case 1:
            if (validateAdminLogin()) {
                adminMenu(jadwal, undoStack);
            }
            else {
                cout << "Login gagal! Username atau password salah." << endl;
            }
            break;
        case 2:
            if (validateStudentLogin()) {
                studentMenu(jadwal);
            }
            else {
                cout << "Login gagal! Username atau password salah." << endl;
            }
            break;
        case 3:
            cout << "Selesai." << endl;
            return 0;
        default:
            cout << "Pilihan tidak valid!" << endl;
        }
        system("cls");
    }

    return 0;
}
