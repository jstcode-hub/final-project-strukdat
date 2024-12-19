#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

// Struktur untuk data Kamar
typedef struct Kamar
{
    int nomorKamar;
    char jenisKamar[20];
    int tersedia; // 1 untuk tersedia, 0 untuk tidak tersedia
    float harga;  // Harga kamar
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

typedef struct History
{
    char nama[50];
    int nomorKamar;
    struct History *next;
} History;

// Fungsi untuk menambah data kamar ke dalam linked list kamar
void tambahKamar(Kamar **head, int nomorKamar, const char *jenisKamar, int tersedia, float harga)
{
    Kamar *baru = (Kamar *)malloc(sizeof(Kamar));
    baru->nomorKamar = nomorKamar;
    strcpy(baru->jenisKamar, jenisKamar);
    baru->tersedia = tersedia;
    baru->harga = harga;
    baru->next = *head;
    *head = baru;
}

// Fungsi untuk menampilkan daftar kamar
void tampilkanKamar(Kamar *head)
{
    Kamar *temp = head;
    printf("\n%-15s%-20s%-15s%-10s\n", "Nomor Kamar", "Jenis Kamar", "Status", "Harga");
    printf("---------------------------------------------------------\n");

    while (temp != NULL)
    {
        printf("%-15d%-20s%-15sRp. %.2f\n", temp->nomorKamar, temp->jenisKamar,
               (temp->tersedia ? "Tersedia" : "Tidak Tersedia"), temp->harga);
        temp = temp->next;
    }
}

// Fungsi untuk menambah data orang yang melakukan reservasi ke dalam double linked list
void tambahReservasi(Orang **head, Kamar *kamarHead, Queue *q, const char *nama, int nomorKamar)
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

            // Tambahkan ke dalam queue
            baru->next = NULL;
            if (q->rear == NULL)
            {
                q->front = q->rear = baru;
            }
            else
            {
                q->rear->next = baru;
                q->rear = baru;
            }

            printf("Reservasi berhasil untuk %s pada kamar nomor %d\n", nama, nomorKamar);
            return;
        }
        kamar = kamar->next;
    }
    printf("Kamar nomor %d tidak tersedia.\n", nomorKamar);
}

// Fungsi untuk menampilkan data orang yang menunggu persetujuan dari queue
void tampilkanAntrianReservasi(Queue *q)
{
    Orang *temp = q->front;
    printf("\n%-30s%-15s%-15s\n", "Nama", "Nomor Kamar", "Status Reservasi");
    printf("-----------------------------------------------\n");

    while (temp != NULL)
    {
        printf("%-30s%-15d%-15s\n", temp->nama, temp->nomorKamarReservasi,
               (temp->statusReservasi == 0 ? "Belum Disetujui" : "Disetujui"));
        temp = temp->next;
    }
}

// Fungsi untuk menyetujui reservasi berdasarkan nama dan nomor kamar
void setujuiReservasiManual(Queue *q, History **historyHead, const char *nama, int nomorKamar)
{
    Orang *current = q->front;
    Orang *prev = NULL;

    while (current != NULL)
    {
        if (strcmp(current->nama, nama) == 0 && current->nomorKamarReservasi == nomorKamar)
        {
            printf("Menyetujui reservasi untuk %s pada kamar nomor %d\n", current->nama, current->nomorKamarReservasi);

            // Tambahkan ke dalam history
            tambahHistory(historyHead, current->nama, current->nomorKamarReservasi);

            // Hapus dari queue
            if (prev == NULL) // Jika orang yang disetujui adalah orang pertama
            {
                q->front = current->next;
                if (q->front == NULL) // Jika antrian kosong
                {
                    q->rear = NULL;
                }
            }
            else
            {
                prev->next = current->next;
                if (current == q->rear) // Jika orang yang disetujui adalah orang terakhir
                {
                    q->rear = prev;
                }
            }

            free(current);
            return;
        }

        prev = current;
        current = current->next;
    }

    printf("Reservasi dengan nama %s dan kamar nomor %d tidak ditemukan dalam antrian.\n", nama, nomorKamar);
}

// Fungsi untuk menambahkan riwayat pemesanan ke dalam linked list history
void tambahHistory(History **head, const char *nama, int nomorKamar)
{
    History *baru = (History *)malloc(sizeof(History));
    strcpy(baru->nama, nama);
    baru->nomorKamar = nomorKamar;
    baru->next = *head;
    *head = baru;
}

// Fungsi untuk menampilkan riwayat pemesanan
void tampilkanHistory(History *head)
{
    if (head == NULL)
    {
        printf("Belum ada riwayat pemesanan.\n");
        return;
    }

    History *temp = head;
    printf("\n%-15s%-15s\n", "Nama", "Nomor Kamar");
    printf("-----------------------------\n");

    while (temp != NULL)
    {
        printf("%-15s%-15d\n", temp->nama, temp->nomorKamar);
        temp = temp->next;
    }
}

// Fungsi untuk mengeluarkan orang pertama dari queue dan menyetujuinya
void dequeue(Queue *q, History **historyHead)
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

    orangDisetujui->statusReservasi = 1; // Ubah status menjadi disetujui
    printf("Menyetujui reservasi untuk %s pada kamar nomor %d\n", orangDisetujui->nama, orangDisetujui->nomorKamarReservasi);

    // Tambahkan ke dalam history
    tambahHistory(historyHead, orangDisetujui->nama, orangDisetujui->nomorKamarReservasi);

    free(orangDisetujui); // Bebaskan memori orang yang sudah disetujui
}

// Fungsi untuk menghapus semua reservasi
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

// Fungsi untuk autentikasi sandi
int autentikasiSandi()
{
    const char sandiBenar[] = "admin123";
    char sandiInput[20];

    printf("Masukkan sandi untuk mengakses riwayat pemesanan: ");
    scanf("%s", sandiInput);

    if (strcmp(sandiInput, sandiBenar) == 0)
    {
        printf("Sandi benar. Mengakses riwayat pemesanan...\n");
        return 1;
    }
    else
    {
        printf("Sandi salah. Akses ditolak.\n");
        return 0;
    }
}

// Fungsi Menu
void menu(Kamar **kamarHead, Orang **reservasiHead, Queue *q, History **historyHead)
{
    int pilihan, nomorKamar, status;
    float harga;
    char nama[50], jenisKamar[20];

    do
    {
        system("cls");
        printf("\n--- Menu ---\n");
        printf("1. Tambah Kamar\n");
        printf("2. Tampilkan Daftar Kamar\n");
        printf("3. Tambah Reservasi\n");
        printf("4. Tampilkan Antrian Reservasi\n");
        printf("5. Setujui Reservasi (Antri)\n");
        printf("6. Tampilkan Riwayat Pemesanan\n");
        printf("7. Hapus Semua Kamar\n");
        printf("8. Hapus Semua Reservasi\n");
        printf("9. Keluar\n");
        printf("Pilih menu (1-9): ");
        fflush(stdin);
        scanf("%d", &pilihan);

        switch (pilihan)
        {
        case 1:
            system("cls");
            printf("Masukkan nomor kamar: ");
            scanf("%d", &nomorKamar);
            printf("Masukkan jenis kamar: ");
            scanf("%s", jenisKamar);
            printf("Masukkan status kamar (1 untuk tersedia, 0 untuk tidak tersedia): ");
            scanf("%d", &status);
            printf("Masukkan harga kamar: ");
            scanf("%f", &harga);
            tambahKamar(kamarHead, nomorKamar, jenisKamar, status, harga);
            break;
        case 2:
            system("cls");
            tampilkanKamar(*kamarHead);
            getchar();
            getchar();
            break;
        case 3:
            system("cls");
            printf("Masukkan nama: ");
            scanf("%s", nama);
            tampilkanKamar(*kamarHead);
            printf("Masukkan nomor kamar yang ingin dipesan: ");
            scanf("%d", &nomorKamar);
            tambahReservasi(reservasiHead, *kamarHead, q, nama, nomorKamar);
            getchar();
            getchar();
            break;
        case 4:
            system("cls");
            tampilkanAntrianReservasi(q);
            getchar();
            getchar();
            break;
        case 5:
            system("cls");
            char namaManual[50];
            int nomorKamarManual;

            printf("Antrian Reservasi:\n");
            tampilkanAntrianReservasi(q);

            printf("\nMasukkan nama: ");
            scanf("%s", namaManual);
            printf("Masukkan nomor kamar: ");
            scanf("%d", &nomorKamarManual);

            setujuiReservasiManual(q, historyHead, namaManual, nomorKamarManual);

            getchar();
            getchar();
            break;
                case 6:
                    system("cls");
                    if (autentikasiSandi())
                    {
                        tampilkanHistory(*historyHead);
                    }
            getchar();
            getchar();
            break;
        case 7:
            system("cls");
            hapusSemuaKamar(kamarHead);
            printf("Semua kamar telah dihapus.\n");
            getchar();
            getchar();
            break;
        case 8:
            system("cls");
            hapusSemuaReservasi(reservasiHead);
            printf("Semua reservasi telah dihapus.\n");
            getchar();
            getchar();
            break;
        case 9:
            printf("Keluar dari program...\n");
            break;
        default:
            printf("Pilihan tidak valid, coba lagi.\n");
        }
    } while (pilihan != 9);
}

int main()
{
    Kamar *kamarHead = NULL;
    Orang *reservasiHead = NULL;
    Queue q = {NULL, NULL};      // Queue kosong pada awalnya
    History *historyHead = NULL; // Linked list history kosong

    menu(&kamarHead, &reservasiHead, &q, &historyHead);

    return 0;
}
