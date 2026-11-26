
// =====================================================
// Sistem Pendataan Game Digital
// Implementasi BST dan AVL Tree (key = ID Game)
// Operasi: Insert, Delete, Search, View
// =====================================================
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Game
{
    int id;
    string nama;
    string genre;
    int tahun;
};

void cetakGame(const Game &g)
{
    cout << left << setw(6) << g.id
         << setw(20) << g.nama
         << setw(18) << g.genre
         << g.tahun << endl;
}

void cetakHeader()
{
    cout << left << setw(6) << "ID"
         << setw(20) << "Nama Game"
         << setw(18) << "Genre"
         << "Tahun" << endl;
    cout << string(50, '-') << endl;
}

// =====================================================
// BAGIAN 1 : BINARY SEARCH TREE (BST)
// =====================================================
struct NodeBST
{
    Game data;
    NodeBST *kiri;
    NodeBST *kanan;
};

NodeBST *buatNodeBST(Game g)
{
    NodeBST *n = new NodeBST;
    n->data = g;
    n->kiri = n->kanan = NULL;
    return n;
}

NodeBST *insertBST(NodeBST *root, Game g)
{
    if (root == NULL)
        return buatNodeBST(g);
    if (g.id < root->data.id)
        root->kiri = insertBST(root->kiri, g);
    else if (g.id > root->data.id)
        root->kanan = insertBST(root->kanan, g);
    else
        cout << "ID " << g.id << " sudah ada, data tidak ditambahkan.\n";
    return root;
}

NodeBST *nodeTerkecilBST(NodeBST *n)
{
    while (n->kiri != NULL)
        n = n->kiri;
    return n;
}

NodeBST *deleteBST(NodeBST *root, int id)
{
    if (root == NULL)
        return NULL;
    if (id < root->data.id)
        root->kiri = deleteBST(root->kiri, id);
    else if (id > root->data.id)
        root->kanan = deleteBST(root->kanan, id);
    else
    {
        // kasus 1 & 2: 0 atau 1 anak
        if (root->kiri == NULL)
        {
            NodeBST *tmp = root->kanan;
            delete root;
            return tmp;
        }
        if (root->kanan == NULL)
        {
            NodeBST *tmp = root->kiri;
            delete root;
            return tmp;
        }
        // kasus 3: 2 anak -> ganti dengan successor (terkecil di kanan)
        NodeBST *succ = nodeTerkecilBST(root->kanan);
        root->data = succ->data;
        root->kanan = deleteBST(root->kanan, succ->data.id);
    }
    return root;
}

NodeBST *searchBST(NodeBST *root, int id)
{
    while (root != NULL && root->data.id != id)
    {
        if (id < root->data.id)
            root = root->kiri;
        else
            root = root->kanan;
    }
    return root;
}

void inorderBST(NodeBST *root)
{
    if (root == NULL)
        return;
    inorderBST(root->kiri);
    cetakGame(root->data);
    inorderBST(root->kanan);
}

int tinggiBST(NodeBST *n)
{
    if (n == NULL)
        return 0;
    int a = tinggiBST(n->kiri), b = tinggiBST(n->kanan);
    return 1 + (a > b ? a : b);
}

// menampilkan bentuk pohon (miring ke kiri: kanan di atas, kiri di bawah)
void tampilPohonBST(NodeBST *root, int spasi = 0)
{
    if (root == NULL)
        return;
    tampilPohonBST(root->kanan, spasi + 6);
    cout << string(spasi, ' ') << root->data.id << endl;
    tampilPohonBST(root->kiri, spasi + 6);
}

// =====================================================
// BAGIAN 2 : AVL TREE
// =====================================================
struct NodeAVL
{
    Game data;
    NodeAVL *kiri;
    NodeAVL *kanan;
    int tinggi;
};

int tinggiAVL(NodeAVL *n) { return (n == NULL) ? 0 : n->tinggi; }

int maks(int a, int b) { return (a > b) ? a : b; }

int faktorSeimbang(NodeAVL *n)
{
    return (n == NULL) ? 0 : tinggiAVL(n->kiri) - tinggiAVL(n->kanan);
}

NodeAVL *buatNodeAVL(Game g)
{
    NodeAVL *n = new NodeAVL;
    n->data = g;
    n->kiri = n->kanan = NULL;
    n->tinggi = 1;
    return n;
}

NodeAVL *rotasiKanan(NodeAVL *y)
{
    NodeAVL *x = y->kiri;
    NodeAVL *t2 = x->kanan;
    x->kanan = y;
    y->kiri = t2;
    y->tinggi = 1 + maks(tinggiAVL(y->kiri), tinggiAVL(y->kanan));
    x->tinggi = 1 + maks(tinggiAVL(x->kiri), tinggiAVL(x->kanan));
    return x;
}

NodeAVL *rotasiKiri(NodeAVL *x)
{
    NodeAVL *y = x->kanan;
    NodeAVL *t2 = y->kiri;
    y->kiri = x;
    x->kanan = t2;
    x->tinggi = 1 + maks(tinggiAVL(x->kiri), tinggiAVL(x->kanan));
    y->tinggi = 1 + maks(tinggiAVL(y->kiri), tinggiAVL(y->kanan));
    return y;
}

// menyeimbangkan node (dipakai oleh insert dan delete)
NodeAVL *seimbangkan(NodeAVL *n)
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

NodeAVL *insertAVL(NodeAVL *root, Game g)
{
    if (root == NULL)
        return buatNodeAVL(g);
    if (g.id < root->data.id)
        root->kiri = insertAVL(root->kiri, g);
    else if (g.id > root->data.id)
        root->kanan = insertAVL(root->kanan, g);
    else
    {
        cout << "ID " << g.id << " sudah ada, data tidak ditambahkan.\n";
        return root;
    }
    return seimbangkan(root);
}

NodeAVL *nodeTerkecilAVL(NodeAVL *n)
{
    while (n->kiri != NULL)
        n = n->kiri;
    return n;
}

NodeAVL *deleteAVL(NodeAVL *root, int id)
{
    if (root == NULL)
        return NULL;
    if (id < root->data.id)
        root->kiri = deleteAVL(root->kiri, id);
    else if (id > root->data.id)
        root->kanan = deleteAVL(root->kanan, id);
    else
    {
        if (root->kiri == NULL)
        {
            NodeAVL *tmp = root->kanan;
            delete root;
            return tmp;
        }
        if (root->kanan == NULL)
        {
            NodeAVL *tmp = root->kiri;
            delete root;
            return tmp;
        }
        NodeAVL *succ = nodeTerkecilAVL(root->kanan);
        root->data = succ->data;
        root->kanan = deleteAVL(root->kanan, succ->data.id);
    }
    return seimbangkan(root);
}

NodeAVL *searchAVL(NodeAVL *root, int id)
{
    while (root != NULL && root->data.id != id)
    {
        if (id < root->data.id)
            root = root->kiri;
        else
            root = root->kanan;
    }
    return root;
}

void inorderAVL(NodeAVL *root)
{
    if (root == NULL)
        return;
    inorderAVL(root->kiri);
    cetakGame(root->data);
    inorderAVL(root->kanan);
}

void tampilPohonAVL(NodeAVL *root, int spasi = 0)
{
    if (root == NULL)
        return;
    tampilPohonAVL(root->kanan, spasi + 6);
    cout << string(spasi, ' ') << root->data.id << endl;
    tampilPohonAVL(root->kiri, spasi + 6);
}

// =====================================================
// DATA AWAL (10 data, dipakai BST dan AVL)
// =====================================================
const int JUMLAH_AWAL = 10;
Game dataAwal[JUMLAH_AWAL] = {
    {101, "Minecraft", "Sandbox", 2011},
    {102, "GTA V", "Action", 2013},
    {103, "Valorant", "FPS", 2020},
    {104, "The Sims 4", "Simulation", 2014},
    {105, "Elden Ring", "RPG", 2022},
    {106, "Dota 2", "MOBA", 2013},
    {107, "Cyberpunk 2077", "RPG", 2020},
    {108, "Resident Evil 4", "Survival Horror", 2023},
    {109, "Stardew Valley", "Simulation", 2016},
    {110, "Forza Horizon 5", "Racing", 2021}};

NodeBST *rootBST = NULL;
NodeAVL *rootAVL = NULL;

void isiDataAwal()
{
    for (int i = 0; i < JUMLAH_AWAL; i++)
    {
        rootBST = insertBST(rootBST, dataAwal[i]);
        rootAVL = insertAVL(rootAVL, dataAwal[i]);
    }
}

// =====================================================
// DEMO : contoh implementasi dari soal (111, 103, 105)
// =====================================================
void tampilKeduaPohon(const string &judul)
{
    cout << "\n=== " << judul << " ===\n";
    cout << "\n[BST] tinggi = " << tinggiBST(rootBST) << endl;
    tampilPohonBST(rootBST);
    cout << "\n[AVL] tinggi = " << tinggiAVL(rootAVL) << endl;
    tampilPohonAVL(rootAVL);
}

void demo()
{
    tampilKeduaPohon("Setelah 10 data awal");

    Game baru = {111, "Hollow Knight", "Metroidvania", 2017};
    rootBST = insertBST(rootBST, baru);
    rootAVL = insertAVL(rootAVL, baru);
    tampilKeduaPohon("Setelah INSERT ID 111");

    rootBST = deleteBST(rootBST, 103);
    rootAVL = deleteAVL(rootAVL, 103);
    tampilKeduaPohon("Setelah DELETE ID 103 (Valorant)");

    cout << "\n=== SEARCH ID 105 ===\n";
    NodeBST *a = searchBST(rootBST, 105);
    NodeAVL *b = searchAVL(rootAVL, 105);
    cetakHeader();
    if (a)
    {
        cout << "[BST] ";
        cetakGame(a->data);
    }
    if (b)
    {
        cout << "[AVL] ";
        cetakGame(b->data);
    }

    cout << "\n=== VIEW (inorder) ===\n";
    cetakHeader();
    inorderBST(rootBST);
}

// =====================================================
// MENU
// =====================================================
Game inputGame()
{
    Game g;
    cout << "ID Game     : ";
    cin >> g.id;
    cin.ignore();
    cout << "Nama Game   : ";
    getline(cin, g.nama);
    cout << "Genre       : ";
    getline(cin, g.genre);
    cout << "Tahun Rilis : ";
    cin >> g.tahun;
    return g;
}

void menuTree(bool pakaiAVL)
{
    int pilih;
    do
    {
        cout << "\n--- MENU " << (pakaiAVL ? "AVL TREE" : "BST") << " ---\n";
        cout << "1. Insert\n2. Delete\n3. Search\n4. View (inorder)\n";
        cout << "5. Tampilkan bentuk pohon\n0. Kembali\nPilih: ";
        cin >> pilih;

        if (pilih == 1)
        {
            Game g = inputGame();
            if (pakaiAVL)
                rootAVL = insertAVL(rootAVL, g);
            else
                rootBST = insertBST(rootBST, g);
            cout << "Proses insert selesai.\n";
        }
        else if (pilih == 2)
        {
            int id;
            cout << "ID yang dihapus: ";
            cin >> id;
            bool ada = pakaiAVL ? (searchAVL(rootAVL, id) != NULL)
                                : (searchBST(rootBST, id) != NULL);
            if (!ada)
                cout << "ID " << id << " tidak ditemukan.\n";
            else
            {
                if (pakaiAVL)
                    rootAVL = deleteAVL(rootAVL, id);
                else
                    rootBST = deleteBST(rootBST, id);
                cout << "Data berhasil dihapus.\n";
            }
        }
        else if (pilih == 3)
        {
            int id;
            cout << "ID yang dicari: ";
            cin >> id;
            if (pakaiAVL)
            {
                NodeAVL *h = searchAVL(rootAVL, id);
                if (h)
                {
                    cetakHeader();
                    cetakGame(h->data);
                }
                else
                    cout << "ID " << id << " tidak ditemukan.\n";
            }
            else
            {
                NodeBST *h = searchBST(rootBST, id);
                if (h)
                {
                    cetakHeader();
                    cetakGame(h->data);
                }
                else
                    cout << "ID " << id << " tidak ditemukan.\n";
            }
        }
        else if (pilih == 4)
        {
            cetakHeader();
            if (pakaiAVL)
                inorderAVL(rootAVL);
            else
                inorderBST(rootBST);
        }
        else if (pilih == 5)
        {
            if (pakaiAVL)
            {
                cout << "Tinggi AVL = " << tinggiAVL(rootAVL) << endl;
                tampilPohonAVL(rootAVL);
            }
            else
            {
                cout << "Tinggi BST = " << tinggiBST(rootBST) << endl;
                tampilPohonBST(rootBST);
            }
        }
    } while (pilih != 0);
}

int main()
{
    isiDataAwal();
    int pilih;
    do
    {
        cout << "\n===== SISTEM PENDATAAN GAME DIGITAL =====\n";
        cout << "1. Menu BST\n2. Menu AVL Tree\n";
        cout << "3. Demo otomatis (Insert 111, Delete 103, Search 105, View)\n";
        cout << "0. Keluar\nPilih: ";
        cin >> pilih;
        if (pilih == 1)
            menuTree(false);
        else if (pilih == 2)
            menuTree(true);
        else if (pilih == 3)
            demo();
    } while (pilih != 0);

    cout << "Program selesai.\n";
    return 0;
}
