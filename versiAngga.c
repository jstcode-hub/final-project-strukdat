#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk data Kamar
typedef struct Kamar
{
    int nomorKamar;
    char jenisKamar[20];
    int tersedia; // 1 untuk tersedia, 0 untuk tidak tersedia
    struct Kamar *next;
} Kamar;

// Struktur untuk data Orang yang melakukan Reservasi (double linked list)
typedef struct Orang
{
    char nama[50];
    int nomorKamarReservasi;
    int statusReservasi; // 0 untuk belum disetujui, 1 untuk disetujui
    struct Orang *prev;
    struct Orang *next;
} Orang;

// Struktur untuk Queue (single linked list) yang menyimpan orang yang menunggu persetujuan
typedef struct Queue
{
    Orang *front;
    Orang *rear;
} Queue;

// Fungsi untuk menambah data kamar ke dalam linked list kamar
void tambahKamar(Kamar **head, int nomorKamar, const char *jenisKamar, int tersedia)
{
    Kamar *baru = (Kamar *)malloc(sizeof(Kamar));
    baru->nomorKamar = nomorKamar;
    strcpy(baru->jenisKamar, jenisKamar);
    baru->tersedia = tersedia;
    baru->next = *head;
    *head = baru;
}

// Fungsi untuk menampilkan daftar kamar
void tampilkanKamar(Kamar *head)
{
    Kamar *temp = head;
    printf("\n%-15s%-20s%-15s\n", "Nomor Kamar", "Jenis Kamar", "Status");
    printf("----------------------------------------------\n");

    while (temp != NULL)
    {
        printf("%-15d%-20s%-15s\n", temp->nomorKamar, temp->jenisKamar, (temp->tersedia ? "Tersedia" : "Tidak Tersedia"));
        temp = temp->next;
    }
}

// Fungsi untuk menambah data orang yang melakukan reservasi ke dalam double linked list
void tambahReservasi(Orang **head, Kamar *kamarHead, const char *nama, int nomorKamar)
{
    Kamar *kamar = kamarHead;
    while (kamar != NULL)
    {
        if (kamar->nomorKamar == nomorKamar && kamar->tersedia == 1)
        { // Jika kamar tersedia
            Orang *baru = (Orang *)malloc(sizeof(Orang));
            strcpy(baru->nama, nama);
            baru->nomorKamarReservasi = nomorKamar;
            baru->statusReservasi = 0; // Status awal = belum disetujui
            baru->prev = NULL;
            baru->next = *head;
            if (*head != NULL)
            {
                (*head)->prev = baru;
            }
            *head = baru;

            kamar->tersedia = 0; // Mengubah status kamar menjadi tidak tersedia
            printf("Reservasi berhasil untuk %s pada kamar nomor %d\n", nama, nomorKamar);
            return;
        }
        kamar = kamar->next;
    }
    printf("Kamar nomor %d tidak tersedia.\n", nomorKamar);
}

// Fungsi untuk menampilkan data orang yang menunggu persetujuan
void tampilkanAntrianReservasi(Orang *head)
{
    Orang *temp = head;
    printf("\n%-30s%-15s%-15s\n", "Nama", "Nomor Kamar", "Status Reservasi");
    printf("-----------------------------------------------\n");

    while (temp != NULL)
    {
        printf("%-30s%-15d%-15s\n", temp->nama, temp->nomorKamarReservasi,
               (temp->statusReservasi == 0 ? "Belum Disetujui" : "Disetujui"));
        temp = temp->next;
    }
}

// Fungsi untuk menambah orang ke dalam queue (menunggu persetujuan)
void enqueue(Queue *q, Orang *orang)
{
    if (q->rear == NULL)
    {
        q->front = q->rear = orang;
        return;
    }
    q->rear->next = orang;
    q->rear = orang;
}

// Fungsi untuk mengeluarkan orang pertama dari queue dan menyetujuinya
void dequeue(Queue *q)
{
    if (q->front == NULL)
    {
        printf("Tidak ada reservasi dalam antrian.\n");
        return;
    }
    Orang *orangDisetujui = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    orangDisetujui->statusReservasi = 1; // Mengubah status menjadi disetujui
    printf("Menyetujui reservasi untuk %s pada kamar nomor %d\n", orangDisetujui->nama, orangDisetujui->nomorKamarReservasi);
    free(orangDisetujui); // Bebaskan memori orang yang sudah disetujui
}

// Fungsi untuk menghapus semua reservasi dalam antrian
void hapusSemuaReservasi(Orang **head)
{
    Orang *current = *head;
    Orang *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

// Fungsi untuk menghapus semua kamar
void hapusSemuaKamar(Kamar **head)
{
    Kamar *current = *head;
    Kamar *temp;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

// Fungsi Menu
void menu(Kamar **kamarHead, Orang **reservasiHead, Queue *q)
{
    int pilihan, nomorKamar, status;
    char nama[50], jenisKamar[20];

    do
    {
        printf("\n--- Menu ---\n");
        printf("1. Tambah Kamar\n");
        printf("2. Tampilkan Daftar Kamar\n");
        printf("3. Tambah Reservasi\n");
        printf("4. Tampilkan Antrian Reservasi\n");
        printf("5. Setujui Reservasi (Antri)\n");
        printf("6. Hapus Semua Kamar\n");
        printf("7. Hapus Semua Reservasi\n");
        printf("8. Keluar\n");
        printf("Pilih menu (1-8): ");
        scanf("%d", &pilihan);

        switch (pilihan)
        {
        case 1:
            printf("Masukkan nomor kamar: ");
            scanf("%d", &nomorKamar);
            printf("Masukkan jenis kamar: ");
            scanf("%s", jenisKamar);
            printf("Masukkan status kamar (1 untuk tersedia, 0 untuk tidak tersedia): ");
            scanf("%d", &status);
            tambahKamar(kamarHead, nomorKamar, jenisKamar, status);
            break;
        case 2:
            tampilkanKamar(*kamarHead);
            break;
        case 3:
            printf("Masukkan nama: ");
            scanf("%s", nama);
            printf("Masukkan nomor kamar yang ingin dipesan: ");
            scanf("%d", &nomorKamar);
            tambahReservasi(reservasiHead, *kamarHead, nama, nomorKamar);
            break;
        case 4:
            tampilkanAntrianReservasi(*reservasiHead);
            break;
        case 5:
            // Pindahkan orang pertama dari double linked list ke queue
            if (*reservasiHead != NULL)
            {
                Orang *orang = *reservasiHead;
                *reservasiHead = orang->next;
                if (*reservasiHead != NULL)
                {
                    (*reservasiHead)->prev = NULL;
                }
                orang->next = NULL;
                enqueue(q, orang); // Masukkan ke dalam queue untuk disetujui
            }
            dequeue(q); // Proses penyetujuan orang pertama dalam queue
            break;
        case 6:
            hapusSemuaKamar(kamarHead);
            printf("Semua kamar telah dihapus.\n");
            break;
        case 7:
            hapusSemuaReservasi(reservasiHead);
            printf("Semua reservasi telah dihapus.\n");
            break;
        case 8:
            printf("Keluar dari program...\n");
            break;
        default:
            printf("Pilihan tidak valid, coba lagi.\n");
        }
    } while (pilihan != 8);
}

int main()
{
    Kamar *kamarHead = NULL;
    Orang *reservasiHead = NULL;
    Queue q = {NULL, NULL}; // Queue kosong pada awalnya

    menu(&kamarHead, &reservasiHead, &q);

    return 0;
}
