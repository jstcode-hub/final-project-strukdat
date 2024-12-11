#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

// Struktur data untuk doubly linked list
struct Reservasi {
    int id;
    string nama;
    string tipeKamar;
    Reservasi* prev;
    Reservasi* next;

    Reservasi(int id, string nama, string tipeKamar) : id(id), nama(nama), tipeKamar(tipeKamar), prev(NULL), next(NULL) {}
};

class SistemManajemenHotel {
private:
    Reservasi* head;
    Reservasi* tail;
    queue<Reservasi*> waitingList; // Queue untuk pelanggan yang menunggu check-in
    stack<Reservasi*> checkoutHistory; // Stack untuk mencatat riwayat checkout
    int idCounter;

public:
    SistemManajemenHotel() : head(NULL), tail(NULL), idCounter(1) {}

    // Tambahkan reservasi baru ke dalam doubly linked list
    void tambahReservasi(string nama, string tipeKamar) {
        Reservasi* reservasiBaru = new Reservasi(idCounter++, nama, tipeKamar);

        if (!head) {
            head = tail = reservasiBaru;
        } else {
            tail->next = reservasiBaru;
            reservasiBaru->prev = tail;
            tail = reservasiBaru;
        }

        cout << "Reservasi untuk " << nama << " telah ditambahkan dengan ID: " << reservasiBaru->id << endl;
    }

    // Tambahkan pelanggan ke dalam antrian tunggu
    void tambahKeAntrianTunggu(string nama, string tipeKamar) {
        Reservasi* reservasiTunggu = new Reservasi(idCounter++, nama, tipeKamar);
        waitingList.push(reservasiTunggu);
        cout << "Pelanggan " << nama << " telah ditambahkan ke dalam daftar tunggu." << endl;
    }

    // Check-in dari daftar tunggu
    void checkInDariAntrian() {
        if (waitingList.empty()) {
            cout << "Tidak ada pelanggan dalam daftar tunggu." << endl;
            return;
        }

        Reservasi* reservasi = waitingList.front();
        waitingList.pop();
        tambahReservasi(reservasi->nama, reservasi->tipeKamar);
        delete reservasi; // Menghapus data sementara dari daftar tunggu
    }

    // Checkout dan simpan ke dalam stack
    void checkout(int id) {
        Reservasi* current = head;

        while (current) {
            if (current->id == id) {
                // Simpan ke dalam riwayat checkout
                checkoutHistory.push(current);

                // Hapus dari doubly linked list
                if (current->prev) {
                    current->prev->next = current->next;
                } else {
                    head = current->next;
                }

                if (current->next) {
                    current->next->prev = current->prev;
                } else {
                    tail = current->prev;
                }

                cout << "Reservasi dengan ID " << id << " telah checkout." << endl;
                return;
            }
            current = current->next;
        }

        cout << "Reservasi dengan ID " << id << " tidak ditemukan." << endl;
    }

    // Tampilkan semua reservasi
    void tampilkanSemuaReservasi() {
        if (!head) {
            cout << "Tidak ada reservasi saat ini." << endl;
            return;
        }

        Reservasi* current = head;
        while (current) {
            cout << "ID: " << current->id << ", Nama: " << current->nama << ", Tipe Kamar: " << current->tipeKamar << endl;
            current = current->next;
        }
    }

    // Tampilkan riwayat checkout
    void tampilkanRiwayatCheckout() {
        if (checkoutHistory.empty()) {
            cout << "Belum ada riwayat checkout." << endl;
            return;
        }

        stack<Reservasi*> tempStack = checkoutHistory; // Salin untuk ditampilkan
        while (!tempStack.empty()) {
            Reservasi* reservasi = tempStack.top();
            tempStack.pop();
            cout << "ID: " << reservasi->id << ", Nama: " << reservasi->nama << ", Tipe Kamar: " << reservasi->tipeKamar << endl;
        }
    }

    // Menu untuk mengelola sistem
    void menu() {
        int pilihan;
        do {
            cout << "\n===== Sistem Manajemen Hotel =====\n";
            cout << "1. Tambah Reservasi\n";
            cout << "2. Tambah ke Antrian Tunggu\n";
            cout << "3. Check-in dari Antrian\n";
            cout << "4. Checkout\n";
            cout << "5. Tampilkan Semua Reservasi\n";
            cout << "6. Tampilkan Riwayat Checkout\n";
            cout << "7. Keluar\n";
            cout << "Pilih opsi: ";
            cin >> pilihan;

            switch (pilihan) {
                case 1: {
                    string nama, tipeKamar;
                    cout << "Masukkan nama: ";
                    cin.ignore();
                    getline(cin, nama);
                    cout << "Masukkan tipe kamar: ";
                    getline(cin, tipeKamar);
                    tambahReservasi(nama, tipeKamar);
                    break;
                }
                case 2: {
                    string nama, tipeKamar;
                    cout << "Masukkan nama: ";
                    cin.ignore();
                    getline(cin, nama);
                    cout << "Masukkan tipe kamar: ";
                    getline(cin, tipeKamar);
                    tambahKeAntrianTunggu(nama, tipeKamar);
                    break;
                }
                case 3:
                    checkInDariAntrian();
                    break;
                case 4: {
                    int id;
                    cout << "Masukkan ID reservasi untuk checkout: ";
                    cin >> id;
                    checkout(id);
                    break;
                }
                case 5:
                    tampilkanSemuaReservasi();
                    break;
                case 6:
                    tampilkanRiwayatCheckout();
                    break;
                case 7:
                    cout << "Keluar dari program.\n";
                    break;
                default:
                    cout << "Pilihan tidak valid.\n";
            }
        } while (pilihan != 7);
    }
};

int main() {
    SistemManajemenHotel hotel;
    hotel.menu();
    return 0;
}
