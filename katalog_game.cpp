// =====================================================
// Sistem Pendataan Game Digital
// =====================================================
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cstdlib>
using namespace std;

// =====================================================
// kolom: [0] ID Game | [1] Nama Game | [2] Genre | [3] Tahun Rilis
// =====================================================
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
    {"110", "Forza Horizon 5", "Racing", "2021"}};
int jumlahData = 10;

// mencari baris di database berdasarkan ID (kalo ngga ada, return -1)
int cariBaris(int id)
{
    string s = to_string(id);
    for (int i = 0; i < jumlahData; i++)
        if (db[i][0] == s)
            return i;
    return -1;
}

void cetakHeader()
{
    cout << left << setw(6) << "ID"
         << setw(20) << "Nama Game"
         << setw(18) << "Genre"
         << "Tahun" << endl;
    cout << string(50, '-') << endl;
}

void cetakBaris(int baris)
{
    cout << left << setw(6) << db[baris][0]
         << setw(20) << db[baris][1]
         << setw(18) << db[baris][2]
         << db[baris][3] << endl;
}

// =====================================================
// NODE POHON (dipakai BST dan AVL)
// Node hanya menyimpan ID sebagai key, data lengkap ada di db
// =====================================================
struct Node
{
    int id;
    Node *kiri;
    Node *kanan;
    int tinggi; // hanya dipakai oleh AVL
};

Node *rootBST = NULL;
Node *rootAVL = NULL;

Node *buatNode(int id)
{
    Node *n = new Node;
    n->id = id;
    n->kiri = n->kanan = NULL;
    n->tinggi = 1;
    return n;
}

Node *nodeTerkecil(Node *n)
{
    while (n->kiri != NULL)
        n = n->kiri;
    return n;
}

int maks(int a, int b) { return (a > b) ? a : b; }

// Search sama untuk BST dan AVL (berdasarkan key)
Node *searchTree(Node *root, int id)
{
    while (root != NULL && root->id != id)
    {
        if (id < root->id) // kalo id lebih besar dari root dia akan cari ke kiri
            root = root->kiri;
        else
            root = root->kanan; // dan sebaliknya
    }
    return root;
}

// =====================================================
// BST
// =====================================================
Node *insertBST(Node *root, int id)
{
    if (root == NULL) // kalo posisi saat kosong, maka buat node baru
        return buatNode(id);
    if (id < root->id) // kalo id saat ini lebih kecil dari root, id yg baru akan masuk ke kiri
        root->kiri = insertBST(root->kiri, id);
    else if (id > root->id) // kalo id lebih besar dari root, id yg baru akan masuk ke kanan
        root->kanan = insertBST(root->kanan, id);
    return root;
}

Node *deleteBST(Node *root, int id)
{
    if (root == NULL) // kalo posisi saat kosong, return NULL
        return NULL;
    if (id < root->id)
        root->kiri = deleteBST(root->kiri, id);
    else if (id > root->id)
        root->kanan = deleteBST(root->kanan, id);
    else
    {
        // kasus 1 & 2: 0 atau 1 anak
        if (root->kiri == NULL)
        {
            Node *tmp = root->kanan;
            delete root;
            return tmp;
        }
        if (root->kanan == NULL)
        {
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

int faktorSeimbang(Node *n)
{
    return (n == NULL) ? 0 : tinggiAVL(n->kiri) - tinggiAVL(n->kanan);
}

Node *rotasiKanan(Node *y)
{
    Node *x = y->kiri;
    Node *t2 = x->kanan;
    x->kanan = y;
    y->kiri = t2;
    y->tinggi = 1 + maks(tinggiAVL(y->kiri), tinggiAVL(y->kanan));
    x->tinggi = 1 + maks(tinggiAVL(x->kiri), tinggiAVL(x->kanan));
    return x;
}

Node *rotasiKiri(Node *x)
{
    Node *y = x->kanan;
    Node *t2 = y->kiri;
    y->kiri = x;
    x->kanan = t2;
    x->tinggi = 1 + maks(tinggiAVL(x->kiri), tinggiAVL(x->kanan));
    y->tinggi = 1 + maks(tinggiAVL(y->kiri), tinggiAVL(y->kanan));
    return y;
}

// menyeimbangkan node (dipakai oleh insert dan delete)
Node *seimbangkan(Node *n)
{
    n->tinggi = 1 + maks(tinggiAVL(n->kiri), tinggiAVL(n->kanan));
    int fs = faktorSeimbang(n);

    if (fs > 1 && faktorSeimbang(n->kiri) >= 0) // Left-Left
        return rotasiKanan(n);
    if (fs > 1 && faktorSeimbang(n->kiri) < 0)
    { // Left-Right
        n->kiri = rotasiKiri(n->kiri);
        return rotasiKanan(n);
    }
    if (fs < -1 && faktorSeimbang(n->kanan) <= 0) // Right-Right
        return rotasiKiri(n);
    if (fs < -1 && faktorSeimbang(n->kanan) > 0)
    { // Right-Left
        n->kanan = rotasiKanan(n->kanan);
        return rotasiKiri(n);
    }
    return n;
}

Node *insertAVL(Node *root, int id)
{
    if (root == NULL)
        return buatNode(id);
    if (id < root->id)
        root->kiri = insertAVL(root->kiri, id);
    else if (id > root->id)
        root->kanan = insertAVL(root->kanan, id);
    else
        return root;
    return seimbangkan(root);
}

Node *deleteAVL(Node *root, int id)
{
    if (root == NULL)
        return NULL;
    if (id < root->id)
        root->kiri = deleteAVL(root->kiri, id);
    else if (id > root->id)
        root->kanan = deleteAVL(root->kanan, id);
    else
    {
        if (root->kiri == NULL)
        {
            Node *tmp = root->kanan;
            delete root;
            return tmp;
        }
        if (root->kanan == NULL)
        {
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
void inorderTabel(Node *root)
{
    if (root == NULL)
        return;
    inorderTabel(root->kiri);
    cetakBaris(cariBaris(root->id));
    inorderTabel(root->kanan);
}

// mengumpulkan ID ke array secara inorder (hasilnya urut naik)
void kumpulkanId(Node *n, vector<int> &arr)
{
    if (n == NULL)
        return;
    kumpulkanId(n->kiri, arr);
    arr.push_back(n->id);
    kumpulkanId(n->kanan, arr);
}

int tinggiPohon(Node *n)
{
    if (n == NULL)
        return 0;
    int a = tinggiPohon(n->kiri), b = tinggiPohon(n->kanan);
    return 1 + maks(a, b);
}

// jumlah digit terpanjang dari semua ID di pohon (untuk lebar kolom)
int labelTerpanjang(Node *n)
{
    if (n == NULL)
        return 0;
    int a = labelTerpanjang(n->kiri), b = labelTerpanjang(n->kanan);
    int s = (int)to_string(n->id).size();
    return maks(s, maks(a, b));
}

const int MARGIN = 2;

// Menggambar pohon ke grid karakter (atas ke bawah).
// Kolom node ditentukan urutan inorder, baris ditentukan kedalaman.
// Mengembalikan kolom tengah node ini.
int gambarNode(Node *n, int kedalaman, int &urutan,
               vector<string> &grid, int lebarSlot)
{
    if (n == NULL)
        return -1;

    int kolKiri = gambarNode(n->kiri, kedalaman + 1, urutan, grid, lebarSlot);
    int pusat = MARGIN + urutan * lebarSlot + lebarSlot / 2;
    urutan++;
    int kolKanan = gambarNode(n->kanan, kedalaman + 1, urutan, grid, lebarSlot);

    // tulis ID (baris node = 1 + 2 * kedalaman, baris 0 untuk tanda ROOT)
    string label = to_string(n->id);
    int baris = 1 + 2 * kedalaman;
    int awal = pusat - (int)label.size() / 2;
    for (size_t i = 0; i < label.size(); i++)
        grid[baris][awal + i] = label[i];

    // garis penghubung ke anak (baris di bawah node)
    if (kolKiri != -1 || kolKanan != -1)
    {
        int br = baris + 1;
        int dari = (kolKiri != -1) ? kolKiri : pusat;
        int sampai = (kolKanan != -1) ? kolKanan : pusat;
        for (int c = dari; c <= sampai; c++)
            grid[br][c] = '-';
        grid[br][pusat] = '+';
        if (kolKiri != -1)
            grid[br][kolKiri] = '+';
        if (kolKanan != -1)
            grid[br][kolKanan] = '+';
    }
    return pusat;
}

// function untuk menampilkan pohon (dipanggil setelah setiap interaksi)
void tampilPohon(const string &nama, Node *root)
{
    vector<int> arr;
    kumpulkanId(root, arr);

    cout << "\n================== TAMPILAN " << nama << " ==================" << endl;
    cout << "Tinggi pohon  : " << tinggiPohon(root) << endl;
    cout << "Array ID Game : [";
    for (size_t i = 0; i < arr.size(); i++)
    {
        if (i > 0)
            cout << ", ";
        cout << arr[i];
    }
    cout << "]" << endl;

    cout << "\nTampilan Tree :" << endl;
    if (root == NULL)
    {
        cout << "(pohon kosong)" << endl;
        return;
    }

    int tinggi = tinggiPohon(root);
    int lebarSlot = labelTerpanjang(root) + 2;
    int lebar = MARGIN + (int)arr.size() * lebarSlot + MARGIN;
    vector<string> grid(2 * tinggi, string(lebar, ' '));

    int urutan = 0;
    int pusat = gambarNode(root, 0, urutan, grid, lebarSlot);

    // tanda root di atas node paling atas
    string tanda = "ROOT";
    for (size_t i = 0; i < tanda.size(); i++)
        grid[0][pusat - 2 + i] = tanda[i];

    cout << endl;
    for (size_t r = 0; r < grid.size(); r++)
    {
        size_t akhir = grid[r].find_last_not_of(' ');
        cout << grid[r].substr(0, akhir + 1) << endl;
    }
}

// =====================================================
// PROSES OPERASI (database dipakai bersama BST & AVL)
// Insert & Delete mengubah database, lalu BST dan AVL
// sama-sama diperbarui supaya isinya selalu sama.
// =====================================================
void prosesInsert(int id, const string &nama, const string &genre, int tahun)
{
    cout << "\n>>> INSERT ID " << id << endl;
    if (cariBaris(id) != -1) // mengecek apakah id baru sudah ada di database
    {
        cout << "ID " << id << " sudah ada, data tidak ditambahkan." << endl;
    }
    else if (jumlahData >= MAKS) // mengecek apakah database penuh (MAKS 100)
    {
        cout << "Database penuh." << endl;
    }
    else // menambahkan data baru ke database
    {
        db[jumlahData][0] = to_string(id);
        db[jumlahData][1] = nama;
        db[jumlahData][2] = genre;
        db[jumlahData][3] = to_string(tahun);
        jumlahData++;
        rootBST = insertBST(rootBST, id);
        rootAVL = insertAVL(rootAVL, id);
        cout << "Data berhasil ditambahkan ke database "
             << "(BST dan AVL sama-sama diperbarui)." << endl;
    }
}

void prosesDelete(int id)
{
    cout << "\n>>> DELETE ID " << id << endl;
    int b = cariBaris(id);
    if (b == -1) // mengecek apakah id yang ingin dihapus ada di database
    {
        cout << "ID " << id << " tidak ditemukan." << endl;
    }
    else // hapus row sesuai id
    {
        string nama = db[b][1];
        // geser baris di bawahnya ke atas supaya tidak ada baris kosong
        for (int i = b; i < jumlahData - 1; i++)
            for (int k = 0; k < 4; k++)
                db[i][k] = db[i + 1][k];
        jumlahData--;
        rootBST = deleteBST(rootBST, id);
        rootAVL = deleteAVL(rootAVL, id);
        cout << "Data " << nama << " (ID " << id << ") dihapus dari database "
             << "(BST dan AVL sama-sama diperbarui)." << endl;
    }
}

// metode: 1 = BST, 2 = AVL (search dilakukan di pohon yang dipilih)
void prosesSearch(int metode, int id)
{
    cout << "\n>>> SEARCH ID " << id << endl;
    Node *hasil = searchTree(metode == 1 ? rootBST : rootAVL, id);
    if (hasil == NULL)
    {
        cout << "ID " << id << " tidak ditemukan." << endl;
    }
    else
    {
        cout << "ID " << id << " ditemukan:" << endl;
        cetakHeader();
        cetakBaris(cariBaris(hasil->id));
    }
}

void prosesView(int metode) // menampilkan tabal
{
    cout << "\n>>> VIEW (inorder traversal "
         << (metode == 1 ? "BST" : "AVL") << ")" << endl;
    cetakHeader();
    inorderTabel(metode == 1 ? rootBST : rootAVL);
}

// =====================================================
// MENU
// =====================================================
int bacaInt(const string &teks)
{
    int x;
    cout << teks;
    while (!(cin >> x))
    {
        if (cin.eof())
            exit(0);
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input harus angka, ulangi: ";
    }
    cin.ignore(1000, '\n');
    return x;
}

// untuk men-delay output supaya tidak langsung muncul semua sekaligus
void tungguEnter(const string &pesan)
{
    cout << "\n[Tekan Enter " << pesan << "] ";
    string s;
    getline(cin, s);
}

// setelah keterangan hasil: Enter -> pohon, Enter -> kembali ke menu
void tampilkanPohonDanKembali(int metode)
{
    string nama = (metode == 1) ? "BST" : "AVL Tree";
    tungguEnter("untuk melihat pohon " + nama);
    tampilPohon(nama, metode == 1 ? rootBST : rootAVL);
    tungguEnter("untuk kembali ke menu");
}

// metode: 1 = BST, 2 = AVL
void menuTree(int metode)
{
    string nama = (metode == 1) ? "BST" : "AVL Tree";
    int pilih;
    do
    {
        cout << "\n==================== MENU " << nama << " ====================\n";
        cout << "1. Insert\n2. Delete\n3. Search\n4. View\n0. Kembali\n";
        pilih = bacaInt("Pilih: ");

        if (pilih == 1) // input 1
        {
            int id = bacaInt("ID Game     : ");
            string namaGame, genre;
            cout << "Nama Game   : ";
            getline(cin, namaGame);
            cout << "Genre       : ";
            getline(cin, genre);
            int tahun = bacaInt("Tahun Rilis : ");
            prosesInsert(id, namaGame, genre, tahun);
            tampilkanPohonDanKembali(metode);
        }
        else if (pilih == 2) // input 2
        {
            prosesDelete(bacaInt("ID yang dihapus: "));
            tampilkanPohonDanKembali(metode);
        }
        else if (pilih == 3) // input 3
        {
            prosesSearch(metode, bacaInt("ID yang dicari: "));
            tampilkanPohonDanKembali(metode);
        }
        else if (pilih == 4) // input 4
        {
            prosesView(metode);
            tampilkanPohonDanKembali(metode);
        }
        else if (pilih != 0) // input tidak ada dalam daftar pilihan
        {
            cout << "Menu tidak ada." << endl;
        }
    } while (pilih != 0);
}

// membangun BST dan AVL dari 10 data awal di database
void bangunPohonDariDatabase()
{
    for (int i = 0; i < jumlahData; i++)
    {
        int id = stoi(db[i][0]);
        rootBST = insertBST(rootBST, id);
        rootAVL = insertAVL(rootAVL, id);
    }
}

int main()
{
    bangunPohonDariDatabase();

    int pilih;
    do
    {
        cout << "\n================ SISTEM PENDATAAN GAME DIGITAL ================\n";
        cout << "1. BST\n2. AVL Tree\n0. Keluar\n";
        pilih = bacaInt("Pilih: ");

        if (pilih == 1 || pilih == 2)
            menuTree(pilih);
        else if (pilih != 0)
            cout << "Menu tidak ada." << endl;
    } while (pilih != 0);

    cout << "Program selesai." << endl;
    return 0;
}
