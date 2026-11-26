#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

// DATABASE GAME
const int MAKS = 100;
string db[MAKS][4] = {
    {"101", "Minecraft", "Sandbox", "2011"},
    {"102", "GTA V", "Action", "2013"},
    {"103", "Valorant", "FPS", "2020"},
    {"104", "The Sims 4", "Simulation", "2014"},
    {"105", "Elden Ring", "RPG", "2022"},
    {"106", "Dota 2", "MOBA", "2013"},
    {"107", "Cyberpunk 2077", "RPG", "2020"},
    {"108", "Resident Evil 4", "Survival Horror", "2023"},
    {"109", "Stardew Valley", "Simulation", "2016"},
    {"110", "Forza Horizon 5", "Racing", "2021"}
};
int jumlahData = 10;

// STRUKTUR NODE UNTUK BST DAN AVL
struct Node {
    int id;
    int tinggi;
    Node *kiri;
    Node *kanan;
};

Node *rootBST = NULL;
Node *rootAVL = NULL;

Node* buatNode(int id) {
    Node *baru = new Node;
    baru->id = id;
    baru->tinggi = 1;
    baru->kiri = NULL;
    baru->kanan = NULL;
    return baru;
}

// MENCARI DATA DI DATABASE
int cariBaris(int id) {
    for (int i = 0; i < jumlahData; i++) {
        if (db[i][0] == to_string(id)) return i;
    }
    return -1;
}

void cetakHeader() {
    cout << left << setw(6) << "ID"
         << setw(20) << "Nama Game"
         << setw(18) << "Genre" << "Tahun\n";
    cout << string(50, '-') << endl;
}

void cetakBaris(int baris) {
    cout << left << setw(6) << db[baris][0]
         << setw(20) << db[baris][1]
         << setw(18) << db[baris][2]
         << db[baris][3] << endl;
}

// MENCARI NODE DI DALAM POHON
Node* searchTree(Node *root, int id) {
    if (root == NULL || root->id == id) return root;
    if (id < root->id) return searchTree(root->kiri, id);
    return searchTree(root->kanan, id);
}

// ==================== BST ====================
Node* insertBST(Node *root, int id) {
    if (root == NULL) return buatNode(id);

    if (id < root->id)
        root->kiri = insertBST(root->kiri, id);
    else if (id > root->id)
        root->kanan = insertBST(root->kanan, id);

    return root;
}

Node* nodeTerkecil(Node *root) {
    while (root->kiri != NULL) root = root->kiri;
    return root;
}

Node* deleteBST(Node *root, int id) {
    if (root == NULL) return NULL;

    if (id < root->id) {
        root->kiri = deleteBST(root->kiri, id);
    } else if (id > root->id) {
        root->kanan = deleteBST(root->kanan, id);
    } else {
        if (root->kiri == NULL) {
            Node *sementara = root->kanan;
            delete root;
            return sementara;
        }
        if (root->kanan == NULL) {
            Node *sementara = root->kiri;
            delete root;
            return sementara;
        }

        Node *pengganti = nodeTerkecil(root->kanan);
        root->id = pengganti->id;
        root->kanan = deleteBST(root->kanan, pengganti->id);
    }
    return root;
}

// ==================== AVL TREE ====================
int tinggi(Node *root) {
    if (root == NULL) return 0;
    return root->tinggi;
}

int terbesar(int a, int b) {
    if (a > b) return a;
    return b;
}

int faktorBalance(Node *root) {
    if (root == NULL) return 0;
    return tinggi(root->kiri) - tinggi(root->kanan);
}

// Rotasi digunakan untuk menyeimbangkan AVL
Node* rotasiKanan(Node *y) {
    Node *x = y->kiri;
    Node *tengah = x->kanan;

    x->kanan = y;
    y->kiri = tengah;

    y->tinggi = 1 + terbesar(tinggi(y->kiri), tinggi(y->kanan));
    x->tinggi = 1 + terbesar(tinggi(x->kiri), tinggi(x->kanan));
    return x;
}

Node* rotasiKiri(Node *x) {
    Node *y = x->kanan;
    Node *tengah = y->kiri;

    y->kiri = x;
    x->kanan = tengah;

    x->tinggi = 1 + terbesar(tinggi(x->kiri), tinggi(x->kanan));
    y->tinggi = 1 + terbesar(tinggi(y->kiri), tinggi(y->kanan));
    return y;
}

Node* seimbangkan(Node *root) {
    if (root == NULL) return root;

    root->tinggi = 1 + terbesar(tinggi(root->kiri), tinggi(root->kanan));
    int balance = faktorBalance(root);

    // Kondisi Left Left
    if (balance > 1 && faktorBalance(root->kiri) >= 0)
        return rotasiKanan(root);

    // Kondisi Left Right
    if (balance > 1 && faktorBalance(root->kiri) < 0) {
        root->kiri = rotasiKiri(root->kiri);
        return rotasiKanan(root);
    }

    // Kondisi Right Right
    if (balance < -1 && faktorBalance(root->kanan) <= 0)
        return rotasiKiri(root);

    // Kondisi Right Left
    if (balance < -1 && faktorBalance(root->kanan) > 0) {
        root->kanan = rotasiKanan(root->kanan);
        return rotasiKiri(root);
    }

    return root;
}

Node* insertAVL(Node *root, int id) {
    if (root == NULL) return buatNode(id);

    if (id < root->id)
        root->kiri = insertAVL(root->kiri, id);
    else if (id > root->id)
        root->kanan = insertAVL(root->kanan, id);
    else
        return root;

    return seimbangkan(root);
}

Node* deleteAVL(Node *root, int id) {
    if (root == NULL) return NULL;

    if (id < root->id) {
        root->kiri = deleteAVL(root->kiri, id);
    } else if (id > root->id) {
        root->kanan = deleteAVL(root->kanan, id);
    } else {
        if (root->kiri == NULL) {
            Node *sementara = root->kanan;
            delete root;
            return sementara;
        }
        if (root->kanan == NULL) {
            Node *sementara = root->kiri;
            delete root;
            return sementara;
        }

        Node *pengganti = nodeTerkecil(root->kanan);
        root->id = pengganti->id;
        root->kanan = deleteAVL(root->kanan, pengganti->id);
    }

    return seimbangkan(root);
}

// ==================== TAMPILAN POHON ====================
int tinggiPohon(Node *root) {
    if (root == NULL) return 0;
    return 1 + terbesar(tinggiPohon(root->kiri), tinggiPohon(root->kanan));
}

// Menampilkan ID secara berurutan dari kecil ke besar
void tampilArray(Node *root, bool &pertama) {
    if (root == NULL) return;

    tampilArray(root->kiri, pertama);
    if (!pertama) cout << ", ";
    cout << root->id;
    pertama = false;
    tampilArray(root->kanan, pertama);
}

// Menampilkan pohon dari atas ke bawah
void cetakPohon(Node *root, string jarak, string posisi) {
    if (root == NULL) return;

    cout << jarak << posisi << root->id << endl;
    cetakPohon(root->kiri, jarak + "   ", "|-- Kiri : ");
    cetakPohon(root->kanan, jarak + "   ", "|-- Kanan: ");
}

void tampilSatuPohon(string nama, Node *root) {
    cout << "\n========== " << nama << " TREE ==========\n";
    cout << "Tinggi pohon: " << tinggiPohon(root) << endl;

    cout << "Array ID Game: [";
    bool pertama = true;
    tampilArray(root, pertama);
    cout << "]\n";

    if (root == NULL) {
        cout << "Pohon masih kosong.\n";
    } else {
        cout << "Struktur pohon (root berada di atas):\n";
        cetakPohon(root, "", "ROOT: ");
    }
}

// Menunggu ENTER, lalu menampilkan BST dan AVL satu per satu
void tungguEnter() {
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}

void tampilBertahap() {
    tungguEnter();
    tampilSatuPohon("BST", rootBST);

    tungguEnter();
    tampilSatuPohon("AVL", rootAVL);

    tungguEnter();
}

// ==================== OPERASI PROGRAM ====================
void prosesInsert(int id, string nama, string genre, int tahun) {
    tungguEnter();
    cout << "\n--- HASIL INSERT ---\n";

    if (cariBaris(id) != -1) {
        cout << "ID tersebut sudah ada. Data tidak ditambahkan.\n";
    } else if (jumlahData >= MAKS) {
        cout << "Database penuh.\n";
    } else {
        db[jumlahData][0] = to_string(id);
        db[jumlahData][1] = nama;
        db[jumlahData][2] = genre;
        db[jumlahData][3] = to_string(tahun);
        jumlahData++;

        rootBST = insertBST(rootBST, id);
        rootAVL = insertAVL(rootAVL, id);
        cout << "Data berhasil ditambahkan ke BST dan AVL.\n";
    }
    tampilBertahap();
}

void prosesDelete(int id) {
    tungguEnter();
    cout << "\n--- HASIL DELETE ---\n";
    int baris = cariBaris(id);

    if (baris == -1) {
        cout << "ID tidak ditemukan.\n";
    } else {
        cout << "Game " << db[baris][1] << " berhasil dihapus.\n";

        for (int i = baris; i < jumlahData - 1; i++) {
            for (int j = 0; j < 4; j++) {
                db[i][j] = db[i + 1][j];
            }
        }
        jumlahData--;

        rootBST = deleteBST(rootBST, id);
        rootAVL = deleteAVL(rootAVL, id);
    }
    tampilBertahap();
}

void prosesSearch(int id) {
    tungguEnter();
    cout << "\n--- HASIL SEARCH ---\n";
    Node *hasil = searchTree(rootBST, id);

    if (hasil == NULL) {
        cout << "Game dengan ID " << id << " tidak ditemukan.\n";
    } else {
        cout << "Game ditemukan!\n";
        cetakHeader();
        cetakBaris(cariBaris(id));
    }
    tampilBertahap();
}

void inorderTabel(Node *root) {
    if (root == NULL) return;
    inorderTabel(root->kiri);
    int baris = cariBaris(root->id);
    if (baris != -1) cetakBaris(baris);
    inorderTabel(root->kanan);
}

void prosesView(int pilihan) {
    tungguEnter();
    cout << "\n--- HASIL VIEW ---\n";
    cetakHeader();

    if (pilihan == 1) {
        cout << "Data dari BST:\n";
        inorderTabel(rootBST);
    } else {
        cout << "Data dari AVL:\n";
        inorderTabel(rootAVL);
    }
    tampilBertahap();
}

// Membuat BST dan AVL dari data awal
void bangunPohon() {
    for (int i = 0; i < jumlahData; i++) {
        int id = stoi(db[i][0]);
        rootBST = insertBST(rootBST, id);
        rootAVL = insertAVL(rootAVL, id);
    }
}

int bacaAngka(string pesan) {
    int angka;
    cout << pesan;
    while (!(cin >> angka)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Masukkan angka yang benar: ";
    }
    cin.ignore(1000, '\n');
    return angka;
}

// DEMO MENJALANKAN BEBERAPA OPERASI
void demo() {
    cout << "\n===== DEMO OTOMATIS =====\n";
    prosesInsert(111, "Hollow Knight", "Metroidvania", 2017);
    prosesDelete(103);
    prosesSearch(105);
    prosesView(1);
}

int main() {
    bangunPohon();
    int pilih;

    do {
        cout << "\n===== SISTEM PENDATAAN GAME DIGITAL =====\n";
        cout << "1. Insert\n";
        cout << "2. Delete\n";
        cout << "3. Search\n";
        cout << "4. View\n";
        cout << "5. Demo otomatis\n";
        cout << "0. Keluar\n";

        pilih = bacaAngka("Pilih menu: ");

        if (pilih == 1) {
            int id = bacaAngka("ID Game: ");
            string nama, genre;
            cout << "Nama Game: ";
            getline(cin, nama);
            cout << "Genre: ";
            getline(cin, genre);
            int tahun = bacaAngka("Tahun Rilis: ");
            prosesInsert(id, nama, genre, tahun);
        } else if (pilih == 2) {
            int id = bacaAngka("ID yang ingin dihapus: ");
            prosesDelete(id);
        } else if (pilih == 3) {
            int id = bacaAngka("ID yang ingin dicari: ");
            prosesSearch(id);
        } else if (pilih == 4) {
            int p = bacaAngka("Lihat melalui (1 = BST, 2 = AVL): ");
            if (p == 1 || p == 2) prosesView(p);
            else cout << "Pilihan tidak valid.\n";
        } else if (pilih == 5) {
            demo();
        } else if (pilih != 0) {
            cout << "Menu tidak tersedia.\n";
        }

    } while (pilih != 0);

    cout << "Program selesai.\n";
    return 0;
}
