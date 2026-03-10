#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAMA_LENGKAP 200
#define MIN_HP_LENGTH 13
#define MAX_KTP_LENGTH 20
#define MAX_JALUR_KODE 5
#define MAX_KURSI 5

// Struktur data untuk informasi tiket
typedef struct {
    char kode_jalur[4]; // Misal "001"
    char nama_jalur[20];
    long harga_ekonomi;
    long harga_eksklusif; // Harga ekonomi + 35.000
} JalurTiket;

// Struktur data untuk informasi pelanggan
typedef struct {
    char nama_lengkap[MAX_NAMA_LENGKAP + 1];
    char no_ktp[MAX_KTP_LENGTH + 1];
    char no_hp[MIN_HP_LENGTH + 1];
    char jenis_pembayaran[10]; // "CASH" atau "TRANSFER"
    char tanggal_keberangkatan[20]; // Misal: "27 Maret 2026"
    char kode_jalur_terpilih[4];
    int jumlah_kursi;
    int tipe_tiket; // 1: Ekonomi, 2: Eksklusif
    long total_biaya;
} DataPelanggan;

// Array data jalur tiket
JalurTiket daftar_jalur[MAX_JALUR_KODE] = {
    {"001", "Siantar", 120000, 155000}, // 120000 + 35000
    {"002", "Medan", 150000, 185000},   // 150000 + 35000
    {"003", "Parapat", 30000, 65000},   // 30000 + 35000
    {"004", "Sidikalang", 140000, 175000},// 140000 + 35000
    {"005", "Siborong-borong", 75000, 110000} // 75000 + 35000
};

// Fungsi untuk membersihkan buffer input setelah scanf
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fungsi validasi input string (hanya huruf dan spasi) untuk nama
int isValidName(const char *str) {
    if (strlen(str) == 0) return 0; // Tidak boleh kosong
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Fungsi validasi input numerik string (hanya digit) untuk KTP/HP
int isValidNumericString(const char *str) {
    if (strlen(str) == 0) return 0; // Tidak boleh kosong
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Fungsi untuk mendapatkan input string dengan validasi panjang dan tipe
void getInputString(const char *prompt, char *buffer, int max_len, int min_len, int type_validation_flag) {
    while (1) {
        printf("%s", prompt);
        fgets(buffer, max_len + 2, stdin); // +2 for newline and null terminator
        buffer[strcspn(buffer, "\n")] = '\0'; // Hapus newline

        if (strlen(buffer) == 0) {
            printf("Input tidak boleh kosong. Mohon coba lagi.\n");
            continue;
        }

        if (strlen(buffer) > max_len) {
            printf("Panjang input melebihi maksimum %d karakter. Mohon coba lagi.\n", max_len);
            continue;
        }
        
        if (strlen(buffer) < min_len) {
            printf("Panjang input kurang dari minimum %d karakter. Mohon coba lagi.\n", min_len);
            continue;
        }

        if (type_validation_flag == 1) { // 1 for numeric only (KTP/HP)
            if (!isValidNumericString(buffer)) {
                printf("Input harus berupa angka. Mohon coba lagi.\n");
                continue;
            }
        } else if (type_validation_flag == 2) { // 2 for name (alpha and space only)
            if (!isValidName(buffer)) {
                printf("Nama Lengkap hanya boleh mengandung huruf dan spasi. Mohon coba lagi.\n");
                continue;
            }
        }
        break; // Input valid
    }
}


// Fungsi untuk mendapatkan input pilihan jalur
int pilihJalur(char *kode_jalur_output) {
    int pilihan_valid = 0;
    int idx_terpilih = -1;
    while (!pilihan_valid) {
        printf("\n--- Pilihan Jalur KBT ---\n");
        printf("Kode | Tujuan           | Ekonomi      | Eksklusif\n");
        printf("-----|------------------|--------------|-------------\n");
        for (int i = 0; i < MAX_JALUR_KODE; i++) {
            printf("%-4s | %-16s | Rp%-10ld | Rp%-10ld\n", 
                   daftar_jalur[i].kode_jalur, 
                   daftar_jalur[i].nama_jalur, 
                   daftar_jalur[i].harga_ekonomi, 
                   daftar_jalur[i].harga_eksklusif);
        }
        printf("--------------------------------------------------\n");
        
        char input_kode[10];
        getInputString("Masukkan kode jalur yang Anda inginkan (misal: 001): ", input_kode, 3, 3, 1); // 1 = numeric only

        for (int i = 0; i < MAX_JALUR_KODE; i++) {
            if (strcmp(input_kode, daftar_jalur[i].kode_jalur) == 0) {
                strcpy(kode_jalur_output, input_kode);
                pilihan_valid = 1;
                idx_terpilih = i;
                break;
            }
        }
        if (!pilihan_valid) {
            printf("Kode jalur '%s' tidak ditemukan. Mohon coba lagi.\n", input_kode);
        }
    }
    return idx_terpilih; // Mengembalikan indeks jalur dalam array
}

// Fungsi untuk mendapatkan input tipe tiket
int pilihTipeTiket() {
    int tipe_valid = 0;
    int pilihan = 0;
    while (!tipe_valid) {
        printf("\nPilih tipe tiket:\n");
        printf("1. Ekonomi\n");
        printf("2. Eksklusif (+Rp35.000 + Snack Roti)\n");
        printf("Masukkan pilihan Anda (1/2): ");
        if (scanf("%d", &pilihan) == 1 && (pilihan == 1 || pilihan == 2)) {
            tipe_valid = 1;
        } else {
            printf("Pilihan tidak valid. Masukkan 1 atau 2. Mohon coba lagi.\n");
        }
        clearInputBuffer(); // Membersihkan buffer setelah scanf
    }
    return pilihan;
}

// Fungsi untuk mendapatkan input jumlah kursi
int getJumlahKursi() {
    int jumlah_kursi = 0;
    while (1) {
        printf("Masukkan jumlah kursi (minimal 1, maksimal %d): ", MAX_KURSI);
        if (scanf("%d", &jumlah_kursi) == 1 && jumlah_kursi >= 1 && jumlah_kursi <= MAX_KURSI) {
            break;
        } else {
            printf("Jumlah kursi tidak valid. Masukkan angka antara 1 sampai %d. Mohon coba lagi.\n", MAX_KURSI);
        }
        clearInputBuffer(); // Membersihkan buffer setelah scanf
    }
    clearInputBuffer(); // Membersihkan buffer setelah scanf terakhir
    return jumlah_kursi;
}

// Fungsi untuk mendapatkan input jenis pembayaran
void getJenisPembayaran(char *jenis_pembayaran) {
    int pilihan_valid = 0;
    while (!pilihan_valid) {
        printf("Pilih jenis pembayaran (CASH/TRANSFER): ");
        fgets(jenis_pembayaran, 10, stdin);
        jenis_pembayaran[strcspn(jenis_pembayaran, "\n")] = '\0';
        for (int i = 0; jenis_pembayaran[i]; i++) {
            jenis_pembayaran[i] = toupper(jenis_pembayaran[i]); // Ubah ke huruf besar
        }
        if (strcmp(jenis_pembayaran, "CASH") == 0 || strcmp(jenis_pembayaran, "TRANSFER") == 0) {
            pilihan_valid = 1;
        } else {
            printf("Pilihan pembayaran tidak valid. Masukkan 'CASH' atau 'TRANSFER'. Mohon coba lagi.\n");
        }
    }
}

// Fungsi untuk menghitung total biaya
long hitungTotalBiaya(int idx_jalur, int tipe_tiket, int jumlah_kursi) {
    long harga_per_kursi;
    if (tipe_tiket == 1) { // Ekonomi
        harga_per_kursi = daftar_jalur[idx_jalur].harga_ekonomi;
    } else { // Eksklusif
        harga_per_kursi = daftar_jalur[idx_jalur].harga_eksklusif;
    }

    long total_sementara = harga_per_kursi * jumlah_kursi;

    // Potongan 5% untuk eksklusif jika >1 tiket
    if (tipe_tiket == 2 && jumlah_kursi > 1) {
        total_sementara = total_sementara - (total_sementara * 5 / 100);
    }
    return total_sementara;
}

// Fungsi untuk menampilkan ringkasan pesanan
void tampilkanRingkasan(const DataPelanggan *data, const JalurTiket *jalur) {
    printf("\n--------------------------------------------------\n");
    printf("           RINGKASAN PEMESANAN TIKET KBT          \n");
    printf("--------------------------------------------------\n");
    printf("Nama Lengkap       : %s\n", data->nama_lengkap);
    printf("Nomor KTP          : %s\n", data->no_ktp);
    printf("Nomor HP           : %s\n", data->no_hp);
    printf("Tanggal Berangkat  : %s\n", data->tanggal_keberangkatan);
    printf("Jalur              : %s - %s\n", jalur->kode_jalur, jalur->nama_jalur);
    printf("Tipe Tiket         : %s\n", (data->tipe_tiket == 1) ? "Ekonomi" : "Eksklusif");
    printf("Jumlah Kursi       : %d\n", data->jumlah_kursi);
    if (data->tipe_tiket == 2) {
        printf("Fasilitas Tambahan : %d Buah Roti\n", data->jumlah_kursi);
    }
    printf("Jenis Pembayaran   : %s\n", data->jenis_pembayaran);
    printf("TOTAL BIAYA        : Rp%ld\n", data->total_biaya);
    printf("--------------------------------------------------\n");
}

int main() {
    DataPelanggan pelanggan;
    int idx_jalur_terpilih = -1;

    printf("Selamat datang di Sistem Tiketing Transportasi Umum KBT!\n");

    // Input Nama Lengkap
    getInputString("Masukkan Nama Lengkap Anda (maks 200 karakter): ", pelanggan.nama_lengkap, MAX_NAMA_LENGKAP, 1, 2); // 2 = name validation

    // Input Nomor KTP
    getInputString("Masukkan Nomor KTP Anda (maks 20 digit): ", pelanggan.no_ktp, MAX_KTP_LENGTH, 1, 1); // 1 = numeric only

    // Input Nomor HP
    getInputString("Masukkan Nomor HP Anda (min 13 digit): ", pelanggan.no_hp, MIN_HP_LENGTH + 5, MIN_HP_LENGTH, 1); // 1 = numeric only
    
    // Input Tanggal Keberangkatan
    getInputString("Masukkan tanggal keberangkatan (contoh: 27 Maret 2026): ", pelanggan.tanggal_keberangkatan, 19, 5, 0); // Max length for "DD Month YYYY" or similar, 0 = no specific type validation

    idx_jalur_terpilih = pilihJalur(pelanggan.kode_jalur_terpilih); // Fungsi ini akan meminta input sampai valid
    
    pelanggan.tipe_tiket = pilihTipeTiket();
    pelanggan.jumlah_kursi = getJumlahKursi();
    getJenisPembayaran(pelanggan.jenis_pembayaran);

    pelanggan.total_biaya = hitungTotalBiaya(idx_jalur_terpilih, pelanggan.tipe_tiket, pelanggan.jumlah_kursi);

    // Tampilkan semua data yang dimasukkan user dan total biaya
    tampilkanRingkasan(&pelanggan, &daftar_jalur[idx_jalur_terpilih]);

    printf("\nTerima kasih telah menggunakan layanan KBT!\n");

    return 0;
}
