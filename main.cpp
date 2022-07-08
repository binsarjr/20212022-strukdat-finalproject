/*

Sistem Informasi Sederhana untuk Menyeleksi para calon Tentara
berdasarkan tinggi badannya.


@author 21.11.3988 Agus Nurwahid
@author 21.11.4000 Lia Ayu Lestari
@author 21.11.4001 Safitri
@author 21.11.4002 Binsar Dwi Jasuma
@author 21.11.4003 Iska Rahmawati
@author 21.11.4028 Aisyah Shafira

*/

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define OS_Windows 1
#else
#define OS_Windows 0
#endif

/**
Library external dari https://github.com/p-ranav/tabulate
untuk print table
*/
#include "./Tabulate.h"
using namespace tabulate;
// Untuk Memudahkan dalam mengelola array, dalam code ini digunakan hanya untuk
// mengosongkan element array
#include <array>
#include <iostream>
// Digunakan untuk cin.ignore supaya lebih aman dalam penghapus buffer
#include <limits>
// Digunakan untuk getline
#include <iomanip>
#include <sstream>
#include <string>

// https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

using namespace std;

struct CalonTentara {
    string kode;
    string nama;
    float tinggi;
    string nilai()
    {
        if (tinggi >= 180)
            return "A";
        else if (tinggi >= 170)
            return "B";
        else if (tinggi >= 165)
            return "C";
        else
            return "D";
    }
    bool isLulus() { return (nilai() != "D"); }

    string lulus() { return isLulus() ? "Lulus" : "Tidak Lulus"; }

    void print(bool kelulusan = false)
    {
        cout << "KODE        : " << kode << endl;
        cout << "NAMA        : " << nama << endl;
        cout << "TINGGI BADAN: " << tinggi << endl;
        if (kelulusan) {
            cout << "NILAI       : " << nilai() << endl;
            cout << "Lulus       : " << lulus() << endl;
        }
    }
};

void clear()
{
    if (OS_Windows) {
        system("cls");
    }
    else {
        system("clear");
    }
}

const int MAXIMUM_CALON_TENTARA = 20;
int totalCalonTentara = 0;
array<CalonTentara, MAXIMUM_CALON_TENTARA> calonTentara;

bool isEmpty();
bool isFull();
void tambah();
void edit();
void tampilkan(bool = false);
void hapus();
void cari();
void sorting();
void hasilSeleksi();

Table createTable(bool kelulusan = false)
{
    Table table;
    if (kelulusan) {
        table.add_row({"Kode", "Nama", "Tinggi Badan", "Nilai", "Hasil"});
    }
    else {
        table.add_row({"Kode", "Nama", "Tinggi Badan"});
    }
    return table;
}

void printTable(Table *table, bool kelulusan = false)
{
    if (kelulusan) (*table).column(3).format().font_align(FontAlign::center);
    (*table).column(2).format().font_align(FontAlign::right);
    cout << *table << endl;
}

void addRowTable(int row, CalonTentara calonTentara, Table *table,
                 bool kelulusan = false)
{
    if (kelulusan) {
        string nilai = calonTentara.nilai();
        (*table).add_row({calonTentara.kode, calonTentara.nama,
                          to_string_with_precision(calonTentara.tinggi, 2),
                          nilai, calonTentara.lulus()});
        if (nilai == "A")
            (*table)[row][3]
                .format()
                .font_color(Color::green)
                .font_style({FontStyle::bold});
        if (nilai == "B")
            (*table)[row][3]
                .format()
                .font_color(Color::blue)
                .font_style({FontStyle::bold});
        if (nilai == "C")
            (*table)[row][3]
                .format()
                .font_color(Color::yellow)
                .font_style({FontStyle::bold});
        if (nilai == "D")
            (*table)[row][3]
                .format()
                .font_color(Color::red)
                .font_style({FontStyle::bold});

        if (calonTentara.isLulus()) {
            (*table)[row][4]
                .format()
                .font_align(FontAlign::center)
                .font_color(Color::grey)
                .font_background_color(Color::green);
        }
        else {
            (*table)[row][4]
                .format()
                .font_align(FontAlign::center)
                .font_color(Color::grey)
                .font_background_color(Color::red);
        }
    }
    else {
        (*table).add_row({calonTentara.kode, calonTentara.nama,
                          to_string_with_precision(calonTentara.tinggi, 2)});
    }
}

void dataDummy()
{
    for (int i = 0; i <= MAXIMUM_CALON_TENTARA; i++) {
        if (isFull()) {
            return;
        };
        calonTentara[totalCalonTentara].nama = "Asep" + to_string(i);
        calonTentara[totalCalonTentara].kode = "CT" + to_string(i);

        int tinggi = 165.0;
        if (i % 2 == 0) tinggi = 150.0;
        if (i % 3 == 0) tinggi = 160.0;
        if (i % 4 == 0) tinggi = 170.0;
        if (i % 5 == 0) tinggi = 180.0;
        calonTentara[totalCalonTentara].tinggi = tinggi + i;

        cout << "Berhasil injek data ke-" << (totalCalonTentara++) + 1 << endl;
    }
    cout << "\n\nData Dummy sudah di inject\n\n";
}

void hapusDataDummy()
{
    calonTentara.fill({});
    totalCalonTentara = 0;
    cout << "\n\nData dummy sudah dihapus\n\n";
}

Table createMenu()
{
    Table table;

    table.add_row({"1. ", "Tambah Data Calon Tentera"});
    table.add_row({"2. ", "Edit Data Calon Tentera"});
    table.add_row({"3. ", "Tampilkan Data Calon Tentera"});
    table.add_row({"4. ", "Hapus Data Calon Tentera"});
    table.add_row({"5. ", "Tampilkan Hasil Seleksi"});
    table.add_row({"6. ", "Pencarian Data Calon Tentera"});

    table.add_row({"", ""});
    table.add_row({"97.", "Gunakan Data Dummy"});
    table.add_row({"98.", "Hapus Data Dummy"});
    table.add_row({"99.", "Exit"});
    table.format()
        .border_top("")
        .border_bottom("")
        .border_left(" ")
        .border_right(" ")
        .corner("");

    Table tableLuar;
    tableLuar.add_row({"Menu Seleksi Calon Tentara"});
    string columnTotal =
        "Total Calon Tentara saat ini " + to_string(totalCalonTentara);
    if (totalCalonTentara == MAXIMUM_CALON_TENTARA) columnTotal += " (FULL)";
    tableLuar.add_row({columnTotal});

    tableLuar.format()
        .multi_byte_characters(true)
        // Font styling
        .font_style({FontStyle::bold, FontStyle::dark})
        // Corners
        .corner_top_left("ᛰ")
        .corner_top_right("ᛯ")
        .corner_bottom_left("ᛮ")
        .corner_bottom_right("ᛸ")
        .corner_top_left_color(Color::cyan)
        .corner_top_right_color(Color::yellow)
        .corner_bottom_left_color(Color::green)
        .corner_bottom_right_color(Color::red)
        // Borders
        .border_top("ᛜ")
        .border_bottom("ᛜ")
        .border_left("ᚿ")
        .border_right("ᛆ")
        .border_left_color(Color::yellow)
        .border_right_color(Color::green)
        .border_top_color(Color::cyan)
        .border_bottom_color(Color::red);
    tableLuar[0][0].format().font_align(FontAlign::center);
    tableLuar[1][0].format().font_align(FontAlign::center);

    if (totalCalonTentara == MAXIMUM_CALON_TENTARA) {
        tableLuar[1][0].format().font_color(Color::red);
    }
    else {
        tableLuar[1][0].format().font_color(Color::green);
    }

    tableLuar.add_row({table});
    return tableLuar;
}

int main()
{
    int pilih;
    clear();

    do {
        // Menampilkan Menu
        cout << createMenu() << endl;
        cout << "Pilih: ";

        while (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "\n[!] Mohon Masukkan Angka\n";
            cout << "Pilih: ";
        }

        switch (pilih) {
            case 1:
                tambah();
                break;
            case 2:
                edit();
                break;
            case 3:
                tampilkan();
                break;
            case 4:
                hapus();
                break;
            case 5:
                hasilSeleksi();
                break;
            case 6:
                cari();
                break;
            case 97:
                dataDummy();
                break;
            case 98:
                hapusDataDummy();
                break;
        }
    } while (pilih != 99);
    cout << endl << "Selamat tinggal" << endl << endl;
}

bool isEmpty()
{
    bool empty = totalCalonTentara == 0;
    if (empty) {
        cout << "\n\nData tidak tersedia.\nSilakan input data terlebih "
                "dahulu\n\n"
             << endl;
    }
    return empty;
}

bool isFull()
{
    bool full = (totalCalonTentara == MAXIMUM_CALON_TENTARA);
    if (full) cout << "\n\nPendaftaran sudah penuh\n\n" << endl;
    return full;
}
void tambah()
{
    if (isFull()) return;
    cout << "KODE        : ";
    getline(cin >> ws, calonTentara[totalCalonTentara].kode);
    cout << "Nama        : ";
    getline(cin >> ws, calonTentara[totalCalonTentara].nama);
    cout << "TINGGI BADAN: ";
    cin >> calonTentara[totalCalonTentara].tinggi;
    cout << endl << "Data Telah Ditambahkan" << endl << endl;
    totalCalonTentara++;
    cin.get();
    return;
}

void edit()
{
    if (isEmpty()) return;
    tampilkan();
    string kode;
    cout << "Masukkan data kode : ";
    getline(cin >> ws, kode);
    
    bool found{false};

    for (int i = 0; i < totalCalonTentara; i++) {
        if (calonTentara[i].kode == kode) {
            found=true;
            calonTentara[i].print();
            cout << "Masukkan Data yg akan di edit" << endl;
            cout << "Nama        : ";

            getline(cin >> ws, calonTentara[i].nama);
            cout << "TINGGI BADAN: ";
            cin >> calonTentara[i].tinggi;
            cout << endl << "Data Telah di Edit" << endl << endl;
        }
    }
    if(!found) cout << "\n\nData tidak ditemukan\n\n";
}

void hapus()
{
    if (isEmpty()) return;
    tampilkan();
    string kode;
    string yes;
    cout << "Masukkan data kode : ";
    getline(cin >> ws, kode);

    bool deleted = false;
    bool found{false};

    for (int i = 0; i < totalCalonTentara; i++) {
        if (calonTentara[i].kode == kode) {
            found=true;
            cout << "[?] Apakah Anda Yakin Untuk Menghapus Data Dengan Kode "
                 << kode << " (y/n) ";
            getline(cin >> ws, yes);
            if (yes == "y" || yes == "Y") {
                calonTentara[i].kode = "";
                calonTentara[i].nama = "";
                calonTentara[i].tinggi = 0;
                totalCalonTentara--;
                cout << "\n\nData Telah Dihapus\n\n";
                deleted = true;
            }
            else {
                cout << "\n\nBatal Hapus\n" << endl;
            }
        }
    }
    if(!found) cout << "\n\nData tidak ditemukan\n\n";

    if (deleted) {
        for (int i = 0; i < totalCalonTentara; i++) {
            if (calonTentara[i].kode == "") {
                calonTentara[i] = calonTentara[i + 1];
                calonTentara[i + 1].kode = "";
            }
        }
    }
}

void tampilkan(bool kelulusan)
{
    if (isEmpty()) return;
    Table table = createTable(kelulusan);
    clear();
    cout << "\n\n";
    for (int i = 0; i < totalCalonTentara; i++) {
        addRowTable(i + 1, calonTentara[i], &table, kelulusan);
    }
    printTable(&table, kelulusan);
    cout << "\n\n";
}

void cari()
{
    if (isEmpty()) return;
    clear();
    string data;
    cout << "Masukan kode atau nama untuk mencari data anda: ";
    getline(cin >> ws, data);

    bool found{false};

    for (int i = 0; i < totalCalonTentara; i++) {
        if (calonTentara[i].kode == data ||
            calonTentara[i].nama.find(data) != string::npos) {
            found=true;
            calonTentara[i].print();
            cout << "\n\n";
        };
    }
    if (found) {
        cout << endl << "Data sudah ditemuakan.." << endl << endl;
    } else {
        cout << "\n\nData tidak ditemukan\n\n";
    }
}

void tukarData(int a, int b)
{
    CalonTentara temp = calonTentara[b];
    calonTentara[b] = calonTentara[a];
    calonTentara[a] = temp;
}

void sorting()
{
    for (int i = 1; i < totalCalonTentara; i++) {
        for (int j = totalCalonTentara - 1; j >= i; j--) {
            if (calonTentara[j].tinggi > calonTentara[j - 1].tinggi)
                tukarData(j, j - 1);
        }
    }
}

void hasilSeleksi()
{
    if (isEmpty()) return;
    clear();
    sorting();
    tampilkan(true);
}
