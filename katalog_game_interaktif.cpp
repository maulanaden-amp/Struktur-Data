// =====================================================
// Sistem Pendataan Game Digital
// BST dan AVL Tree memakai DATABASE YANG SAMA
// Database = array 2 dimensi (seperti tabel), key pohon = ID Game
// Operasi: Insert, Delete, Search, View
// =====================================================
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
using namespace std;

// =====================================================
// DATABASE (array 2 dimensi, 1 database untuk BST & AVL)
// kolom: [0] ID Game | [1] Nama Game | [2] Genre | [3] Tahun Rilis
// =====================================================
const int MAKS = 100;

string db[MAKS][4] = {
    {"101", "Minecraft",       "Sandbox",         "2011"},
    {"102", "GTA V",           "Action",          "2013"},
    {"103", "Valorant",        "FPS",             "2020"},
    {"104", "The Sims 4",      "Simulation",      "2014"},
    {"105", "Elden Ring",      "RPG",             "2022"},
    {"106", "Dota 2",          "MOBA",            "2013"},
    {"107", "Cyberpunk 2077",  "RPG",             "2020"},
    {"108", "Resident Evil 4", "Survival Horror", "2023"},
    {"109", "Stardew Valley",  "Simulation",      "2016"},
    {"110", "Forza Horizon 5", "Racing",          "2021"}
};
int jumlahData = 10;

// mencari baris di database berdasarkan ID (-1 jika tidak ada)
int cariBaris(int id) {
    string s = to_string(id);
    for (int i = 0; i < jumlahData; i++)
        if (db[i][0] == s) return i;
    return -1;
}

void cetakHeader() {
    cout << left << setw(6) << "ID"
         << setw(20) << "Nama Game"
         << setw(18) << "Genre"
         << "Tahun" << endl;
    cout << string(50, '-') << endl;
}

void cetakBaris(int baris) {
    cout << left << setw(6) << db[baris][0]
         << setw(20) << db[baris][1]
         << setw(18) << db[baris][2]
         << db[baris][3] << endl;
}

// =====================================================
// NODE POHON (dipakai BST dan AVL)
// Node hanya menyimpan ID sebagai key, data lengkap ada di db
// =====================================================
struct Node {
    int id;
    Node *kiri;
    Node *kanan;
    int tinggi;   // hanya dipakai oleh AVL
};

Node *rootBST = NULL;
Node *rootAVL = NULL;

Node* buatNode(int id) {
    Node *n = new Node;
    n->id = id;
    n->kiri = n->kanan = NULL;
    n->tinggi = 1;
    return n;
}

Node* nodeTerkecil(Node *n) {
    while (n->kiri != NULL) n = n->kiri;
    return n;
}

// Search sama untuk BST dan AVL (berdasarkan key)
Node* searchTree(Node *root, int id) {
    while (root != NULL && root->id != id) {
        if (id < root->id) root = root->kiri;
        else root = root->kanan;
    }
    return root;
}

// =====================================================
// BST
// =====================================================
Node* insertBST(Node *root, int id) {
    if (root == NULL) return buatNode(id);
    if (id < root->id)      root->kiri  = insertBST(root->kiri, id);
    else if (id > root->id) root->kanan = insertBST(root->kanan, id);
    return root;
}

Node* deleteBST(Node *root, int id) {
    if (root == NULL) return NULL;
    if (id < root->id)
        root->kiri = deleteBST(root->kiri, id);
    else if (id > root->id)
        root->kanan = deleteBST(root->kanan, id);
    else {
        // kasus 1 & 2: 0 atau 1 anak
        if (root->kiri == NULL) {
            Node *tmp = root->kanan;
            delete root;
            return tmp;
        }
        if (root->kanan == NULL) {
            Node *tmp = root->kiri;
            delete root;
            return tmp;
        }
        // kasus 3: 2 anak -> ganti dengan successor (terkecil di kanan)
        Node *succ = nodeTerkecil(root->kanan);
        root->id = succ->id;
        root->kanan = deleteBST(root->kanan, succ->id);
    }
    return root;
}

// =====================================================
// AVL TREE
// =====================================================
int tinggiAVL(Node *n) { return (n == NULL) ? 0 : n->tinggi; }

int maks(int a, int b) { return (a > b) ? a : b; }

int faktorSeimbang(Node *n) {
    return (n == NULL) ? 0 : tinggiAVL(n->kiri) - tinggiAVL(n->kanan);
}

Node* rotasiKanan(Node *y) {
    Node *x = y->kiri;
    Node *t2 = x->kanan;
    x->kanan = y;
    y->kiri = t2;
    y->tinggi = 1 + maks(tinggiAVL(y->kiri), tinggiAVL(y->kanan));
    x->tinggi = 1 + maks(tinggiAVL(x->kiri), tinggiAVL(x->kanan));
    return x;
}

Node* rotasiKiri(Node *x) {
    Node *y = x->kanan;
    Node *t2 = y->kiri;
    y->kiri = x;
    x->kanan = t2;
    x->tinggi = 1 + maks(tinggiAVL(x->kiri), tinggiAVL(x->kanan));
    y->tinggi = 1 + maks(tinggiAVL(y->kiri), tinggiAVL(y->kanan));
    return y;
}

// menyeimbangkan node (dipakai oleh insert dan delete)
Node* seimbangkan(Node *n) {
    n->tinggi = 1 + maks(tinggiAVL(n->kiri), tinggiAVL(n->kanan));
    int fs = faktorSeimbang(n);

    if (fs > 1 && faktorSeimbang(n->kiri) >= 0)      // Left-Left
        return rotasiKanan(n);
    if (fs > 1 && faktorSeimbang(n->kiri) < 0) {     // Left-Right
        n->kiri = rotasiKiri(n->kiri);
        return rotasiKanan(n);
    }
    if (fs < -1 && faktorSeimbang(n->kanan) <= 0)    // Right-Right
        return rotasiKiri(n);
    if (fs < -1 && faktorSeimbang(n->kanan) > 0) {   // Right-Left
        n->kanan = rotasiKanan(n->kanan);
        return rotasiKiri(n);
    }
    return n;
}

Node* insertAVL(Node *root, int id) {
    if (root == NULL) return buatNode(id);
    if (id < root->id)       root->kiri  = insertAVL(root->kiri, id);
    else if (id > root->id)  root->kanan = insertAVL(root->kanan, id);
    else return root;
    return seimbangkan(root);
}

Node* deleteAVL(Node *root, int id) {
    if (root == NULL) return NULL;
    if (id < root->id)
        root->kiri = deleteAVL(root->kiri, id);
    else if (id > root->id)
        root->kanan = deleteAVL(root->kanan, id);
    else {
        if (root->kiri == NULL) {
            Node *tmp = root->kanan;
            delete root;
            return tmp;
        }
        if (root->kanan == NULL) {
            Node *tmp = root->kiri;
            delete root;
            return tmp;
        }
        Node *succ = nodeTerkecil(root->kanan);
        root->id = succ->id;
        root->kanan = deleteAVL(root->kanan, succ->id);
    }
    return seimbangkan(root);
}

// =====================================================
// TAMPILAN (View & Pohon)
// =====================================================
// View: inorder traversal -> data tampil berurutan berdasarkan ID
void inorderTabel(Node *root) {
    if (root == NULL) return;
    inorderTabel(root->kiri);
    cetakBaris(cariBaris(root->id));
    inorderTabel(root->kanan);
}

// mengumpulkan ID ke array secara inorder (hasilnya urut naik)
void kumpulkanId(Node *n, vector<int> &arr) {
    if (n == NULL) return;
    kumpulkanId(n->kiri, arr);
    arr.push_back(n->id);
    kumpulkanId(n->kanan, arr);
}

int tinggiPohon(Node *n) {
    if (n == NULL) return 0;
    int a = tinggiPohon(n->kiri), b = tinggiPohon(n->kanan);
    return 1 + maks(a, b);
}

// =====================================================
// VISUALISASI POHON DARI ATAS KE BAWAH
// Root berada di atas, lalu anak kiri dan kanan ditampilkan di bawahnya.
// =====================================================
void cetakTreeAtasBawah(Node *n, string prefix = "",
                        bool anakTerakhir = true, string posisi = "ROOT") {
    if (n == NULL) return;

    if (posisi == "ROOT")
        cout << n->id << " [ROOT]" << endl;
    else
        cout << prefix << (anakTerakhir ? "\\-- " : "|-- ")
             << n->id << " [" << posisi << "]" << endl;

    // Tentukan prefix untuk level berikutnya.
    string prefixBaru = prefix;
    if (posisi != "ROOT")
        prefixBaru += (anakTerakhir ? "    " : "|   ");

    // Anak kiri dicetak lebih dahulu, kemudian anak kanan.
    if (n->kiri != NULL && n->kanan != NULL) {
        cetakTreeAtasBawah(n->kiri, prefixBaru, false, "KIRI");
        cetakTreeAtasBawah(n->kanan, prefixBaru, true, "KANAN");
    } else if (n->kiri != NULL) {
        cetakTreeAtasBawah(n->kiri, prefixBaru, true, "KIRI");
    } else if (n->kanan != NULL) {
        cetakTreeAtasBawah(n->kanan, prefixBaru, true, "KANAN");
    }
}

void tampilSatuPohon(const string &nama, Node *root) {
    vector<int> arr;
    kumpulkanId(root, arr);

    cout << "\n================ " << nama << " ================" << endl;
    cout << "Tinggi pohon : " << tinggiPohon(root) << endl;
    cout << "Array ID Game : [";
    for (size_t i = 0; i < arr.size(); i++) {
        if (i > 0) cout << ", ";
        cout << arr[i];
    }
    cout << "]" << endl;

    cout << "Visualisasi Tree (root di atas):" << endl;
    cetakTreeAtasBawah(root);
    cout << "==============================================" << endl;
}

// Menampilkan hasil secara bertahap dengan menekan ENTER.
void tampilPohonBertahap() {
    cout << "\nTekan ENTER untuk menampilkan BST...";
    cin.get();
    tampilSatuPohon("BST", rootBST);

    cout << "\nTekan ENTER untuk menampilkan AVL Tree...";
    cin.get();
    tampilSatuPohon("AVL TREE", rootAVL);

    cout << "\nTekan ENTER untuk kembali ke menu utama...";
    cin.get();
}

// =====================================================
// PROSES OPERASI (database dipakai bersama BST & AVL)
// =====================================================
void prosesInsert(int id, const string &nama, const string &genre, int tahun) {
    cout << "\n>>> INSERT ID " << id << endl;
    if (cariBaris(id) != -1) {
        cout << "ID " << id << " sudah ada, data tidak ditambahkan." << endl;
    } else if (jumlahData >= MAKS) {
        cout << "Database penuh." << endl;
    } else {
        db[jumlahData][0] = to_string(id);
        db[jumlahData][1] = nama;
        db[jumlahData][2] = genre;
        db[jumlahData][3] = to_string(tahun);
        jumlahData++;
        rootBST = insertBST(rootBST, id);
        rootAVL = insertAVL(rootAVL, id);
        cout << "Data berhasil ditambahkan (BST dan AVL ikut berubah)." << endl;
    }
    tampilPohonBertahap();
}

void prosesDelete(int id) {
    cout << "\n>>> DELETE ID " << id << endl;
    int b = cariBaris(id);
    if (b == -1) {
        cout << "ID " << id << " tidak ditemukan." << endl;
    } else {
        string nama = db[b][1];
        // geser baris di bawahnya ke atas supaya tidak ada baris kosong
        for (int i = b; i < jumlahData - 1; i++)
            for (int k = 0; k < 4; k++)
                db[i][k] = db[i + 1][k];
        jumlahData--;
        rootBST = deleteBST(rootBST, id);
        rootAVL = deleteAVL(rootAVL, id);
        cout << "Data " << nama << " (ID " << id
             << ") dihapus (BST dan AVL ikut berubah)." << endl;
    }
    tampilPohonBertahap();
}

void prosesSearch(int id) {
    cout << "\n>>> SEARCH ID " << id << endl;
    Node *a = searchTree(rootBST, id);
    Node *b = searchTree(rootAVL, id);
    cout << "[BST] " << (a ? "ditemukan" : "tidak ditemukan") << endl;
    cout << "[AVL] " << (b ? "ditemukan" : "tidak ditemukan") << endl;
    if (a != NULL) {
        cetakHeader();
        cetakBaris(cariBaris(a->id));
    }
    tampilPohonBertahap();
}

// pilihan: 1 = lewat BST, 2 = lewat AVL
void prosesView(int pilihan) {
    cout << "\n>>> VIEW (inorder traversal "
         << (pilihan == 1 ? "BST" : "AVL") << ")" << endl;
    cetakHeader();
    inorderTabel(pilihan == 1 ? rootBST : rootAVL);
    tampilPohonBertahap();
}

// membangun BST dan AVL dari 10 data awal di database
void bangunPohonDariDatabase() {
    for (int i = 0; i < jumlahData; i++) {
        int id = stoi(db[i][0]);
        rootBST = insertBST(rootBST, id);
        rootAVL = insertAVL(rootAVL, id);
    }
}

// =====================================================
// DEMO & MENU
// =====================================================
void demo() {
    cout << "\n######## DEMO OTOMATIS ########";
    prosesInsert(111, "Hollow Knight", "Metroidvania", 2017);
    prosesDelete(103);
    prosesSearch(105);
    prosesView(1);
}

int bacaInt(const string &teks) {
    int x;
    cout << teks;
    while (!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input harus angka, ulangi: ";
    }
    cin.ignore(1000, '\n');
    return x;
}

int main() {
    bangunPohonDariDatabase();

    int pilih;
    do {
        cout << "\n===== SISTEM PENDATAAN GAME DIGITAL =====\n";
        cout << "1. Insert\n2. Delete\n3. Search\n4. View\n";
        cout << "5. Demo otomatis (Insert 111, Delete 103, Search 105, View)\n";
        cout << "0. Keluar\n";
        pilih = bacaInt("Pilih: ");

        if (pilih == 1) {
            int id = bacaInt("ID Game     : ");
            string nama, genre;
            cout << "Nama Game   : "; getline(cin, nama);
            cout << "Genre       : "; getline(cin, genre);
            int tahun = bacaInt("Tahun Rilis : ");
            prosesInsert(id, nama, genre, tahun);
        } else if (pilih == 2) {
            prosesDelete(bacaInt("ID yang dihapus: "));
        } else if (pilih == 3) {
            prosesSearch(bacaInt("ID yang dicari: "));
        } else if (pilih == 4) {
            int p = bacaInt("Lihat lewat (1 = BST, 2 = AVL): ");
            if (p == 1 || p == 2) prosesView(p);
            else cout << "Pilihan tidak valid." << endl;
        } else if (pilih == 5) {
            demo();
        } else if (pilih != 0) {
            cout << "Menu tidak ada." << endl;
        }
    } while (pilih != 0);

    cout << "Program selesai." << endl;
    return 0;
}
