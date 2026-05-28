#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_TRANS 100
#define MAX_CARDS_LEYLA 5
#define MAX_ACCOUNTS_LEYLA 5

#define MIN_CEKILME_MEBLEGI     1.0
#define MAX_CEKILME_MEBLEGI  1000.0
#define MIN_KOCURME_MEBLEGI     0.01
#define MAX_KOCURME_MEBLEGI  5000.0
#define MAX_MEBLAQ_ARTIRMA   10000.0
#define KART_NOMRESI_UZUNLUGU  17
#define MAX_KOD_UZUNLUGU       20
#define XIDMET_SAYI             5

#define MAX_CARDS_ALI 100
#define MAX_CUSTOMERS_ALI 100



// AYGÜL
struct user {
    char name[50];
    char surname[50];
    char birth[50];
    char FIN[50];
    char phone[50];
    char email[50];
    char account_id[50];
    char password[50];
    float balance;
};

// LƏMAN
typedef struct {
    char type[20];
    double amount;
    char date[20];
} Transaction;

typedef struct {
    char name[50];
    char id[10];
    double balance;
    Transaction history[MAX_TRANS];
    int trans_count;
} Account;

// LEYLA
struct LeylaCard {
    char card_number[17];
    int is_blocked;
};

struct BankAccount {
    char account_number[10];
    float balance;
    struct LeylaCard cards[MAX_CARDS_LEYLA];
    int card_count;
};

// ABDULLAH
typedef struct {
    char   kart_nomresi[KART_NOMRESI_UZUNLUGU]; 
    double balans;                               
    char   sahib_adi[50];                        
} AbdullahKart;

typedef struct {
    int  kod;         
    char ad[30];      
    double min_odenis; 
} AbdullahXidmet;

// ƏLİ
struct AliCard {
    int id;                
    char card_number[17];   
    char owner_name[50];    
    double balance;       
    char card_type[20];   
    int is_active;         
};

struct CardList {
    struct AliCard cards[MAX_CARDS_ALI];
    int count;              
    int next_id;          
};

// ƏLİ
struct Customer {
    int id;               
    char full_name[60];   
    char phone[15];     
    char email[50];      
    char address[100];   
    int is_active;       
};

struct CustomerList {
    struct Customer customers[MAX_CUSTOMERS_ALI];
    int count;      
    int next_id;   
};


// Aygul
void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



// LƏMAN
void get_date(char *buf) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buf, 20, "%d.%m.%Y", tm);
}

void save_transaction(Account *acc, char *type, double amount) {
    Transaction *t = &acc->history[acc->trans_count];
    strcpy(t->type, type);
    t->amount = amount;
    get_date(t->date);
    acc->trans_count++;
}

void show_balance(Account *acc) {
    printf("\n=== BALANS ===\n");
    printf("Ad    : %s\n", acc->name);
    printf("ID    : %s\n", acc->id);
    printf("Balans: %.2f AZN\n", acc->balance);
    printf("==============\n");
}

void show_history(Account *acc) {
    printf("\n=== EMELIYYAT TARIXCESI ===\n");
    if (acc->trans_count == 0) {
        printf("Hec bir emeliyyat yoxdur.\n");
        return;
    }
    for (int i = 0; i < acc->trans_count; i++) {
        printf("%d. %s | %.2f AZN | %s\n",
            i + 1,
            acc->history[i].type,
            acc->history[i].amount,
            acc->history[i].date);
    }
    printf("===========================\n");
    printf("Umumi: %d emeliyyat\n", acc->trans_count);
}

void deposit(Account *acc, double amount) {
    acc->balance += amount;
    save_transaction(acc, "Deposit", amount);
    printf("%.2f AZN elave edildi.\n", amount);
}

void withdraw(Account *acc, double amount) {
    if (amount > acc->balance) {
        printf("Balans kifayet deyil!\n");
        return;
    }
    acc->balance -= amount;
    save_transaction(acc, "Withdrawal", amount);
    printf("%.2f AZN cixarildi.\n", amount);
}


// LEYLA
int find_account(struct BankAccount accounts[], int account_count, const char *num) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_number, num) == 0) {
            return i;
        }
    }
    return -1;
}

void create_bank_account(struct BankAccount accounts[], int *account_count) {
    if (*account_count >= MAX_ACCOUNTS_LEYLA) {
        printf("\n[ Xeta bas verdi! ] Bankda maksimum hesab limitine catildi (%d).\n", MAX_ACCOUNTS_LEYLA);
        return;
    }

    char temp_account_number[10];
    printf("\n           --- Hesabin yaradilmasi ---         \n");
    printf("Yeni hesab nomresini daxil edin: ");
    scanf("%9s", temp_account_number);
    clear_buffer();

    if (find_account(accounts, *account_count, temp_account_number) != -1) {
        printf("\n[ Xeta bas verdi! ] %s nomreli hesab artiq movcuddur!\n", temp_account_number);
        return;
    }

    int index = *account_count;
    strcpy(accounts[index].account_number, temp_account_number);
    accounts[index].balance = 0.0f;
    accounts[index].card_count = 0;

    printf("[ Ugurlu emeliyyat ] %s nomreli hesab yaradildi.\n", accounts[index].account_number);
    (*account_count)++;
}

void create_card(struct BankAccount accounts[], int account_count) {
    if (account_count == 0) {
        printf("\n[ Xeta bas verdi! ] Sistemde hec bir hesab yoxdur! Hesab yaradin.\n");
        return;
    }

    char search_acc[10];
    printf("\nHansi hesab ucun kart yaratmaq isteyirsiniz? Nomreni daxil edin: ");
    scanf("%9s", search_acc);
    clear_buffer();

    int acc_idx = find_account(accounts, account_count, search_acc);
    if (acc_idx == -1) {
        printf("[ Xeta bas verdi! ] Bele bir hesab tapilmadi.\n");
        return;
    }

    struct BankAccount *acc = &accounts[acc_idx];

    if (acc->card_count >= MAX_CARDS_LEYLA) {
        printf("\n[ Xeta bas verdi! ] Bu hesab ucun maksimum kart limitine catdiniz (%d).\n", MAX_CARDS_LEYLA);
        return;
    }
   
    char temp_card_number[17];
    printf("\n *** Kart nomresi *** \n");
    printf("Kartin 16 reqemli nomresini daxil edin: ");
    scanf("%16s", temp_card_number);
    clear_buffer();

    for (int i = 0; i < account_count; i++) {
        for (int j = 0; j < accounts[i].card_count; j++) {
            if (strcmp(accounts[i].cards[j].card_number, temp_card_number) == 0) {
                printf("[ Xeta bas verdi! ] Kart artıq sistemde movcuddur!\n");
                return;
            }
        }
    }
   
    strcpy(acc->cards[acc->card_count].card_number, temp_card_number);
    acc->cards[acc->card_count].is_blocked = 0;
   
    printf("[ Ugurlu emeliyyat ] %s karti yaradildi (Hesab: %s).\n", temp_card_number, acc->account_number);
    acc->card_count++;
}

void block_card(struct BankAccount accounts[], int account_count) {
    if (account_count == 0) {
        printf("\n[ Xeta bas verdi! ] Sistemde hec bir hesab ve kart yoxdur.\n");
        return;
    }

    char search_card[17];
    printf("\nBloklamaq istediyiniz kartin 16 reqemli nomresini daxil edin: ");
    scanf("%16s", search_card);
    clear_buffer();

    for (int i = 0; i < account_count; i++) {
        for (int j = 0; j < accounts[i].card_count; j++) {
            if (strcmp(accounts[i].cards[j].card_number, search_card) == 0) {
                if (accounts[i].cards[j].is_blocked == 1) {
                    printf("[ i ] %s karti ARTIQ bloklanmisdir.\n", search_card);
                    return;
                }
                accounts[i].cards[j].is_blocked = 1;
                printf("[ Ugurlu emeliyyat ] %s karti BLOKLANIB (Hesab: %s).\n", search_card, accounts[i].account_number);
                return;
            }
        }
    }

    printf("[ Xeta bas verdi! ] Bele bir kart tapilmadi.\n");
}


// ABDULLAH
static void xett_ciz(void) {
    printf("--------------------------------------------------\n");
}

static double double_oxu(const char *mesaj) {
    double deger;
    int oxuma_sayi;
    while (1) {
        printf("%s", mesaj);
        oxuma_sayi = scanf("%lf", &deger);
        clear_buffer(); 
        if (oxuma_sayi == 1) return deger;
        printf("  [XETA] Zehmet olmasa düzgün rəqəm daxil edin.\n");
    }
}

static void kart_nomresi_oxu(char *hedef) {
    char temp_buf[KART_NOMRESI_UZUNLUGU + 1]; 
    while (1) {
        printf("  Kart nomresi (16 reqem): ");
        if (fgets(temp_buf, sizeof(temp_buf), stdin) != NULL) {
            size_t uzunlug = strlen(temp_buf);
            if (uzunlug > 0 && temp_buf[uzunlug - 1] == '\n') {
                temp_buf[uzunlug - 1] = '\0';
                uzunlug--;
            } else {
                clear_buffer();
            }
            if (uzunlug == 16) {
                strncpy(hedef, temp_buf, KART_NOMRESI_UZUNLUGU);
                hedef[KART_NOMRESI_UZUNLUGU - 1] = '\0'; 
                return;
            }
            printf("  [XETA] Kart nomresi tam 16 reqemden ibaret olmalidir.\n");
        }
    }
}

void ode_kommunal(AbdullahKart *kart) {
    const AbdullahXidmet xidmetler[XIDMET_SAYI] = {
        {1, "Ishiq (Azerenerji)",   1.00},
        {2, "Qaz (Azerigas)",       1.00},
        {3, "Su (Azersu)",          0.50},
        {4, "Internet / TV",        5.00},
        {5, "Mobil telefon",        1.00}
    };
    int     secim;          
    char    musteri_kodu[MAX_KOD_UZUNLUGU]; 
    double  odenis_meblegi;  
    int     secim_gosterici; 

    xett_ciz();
    printf("       KOMMUNAL / XİDMƏT ODENİSİ\n");
    xett_ciz();
    printf("  Xidmet seçin:\n");
    for (int i = 0; i < XIDMET_SAYI; i++) {
        printf("    %d. %s\n", xidmetler[i].kod, xidmetler[i].ad);
    }
    printf("\n");

    secim_gosterici = 0;
    while (!secim_gosterici) {
        printf("  Secimiz (1-%d): ", XIDMET_SAYI);
        if (scanf("%d", &secim) == 1 && secim >= 1 && secim <= XIDMET_SAYI) {
            secim_gosterici = 1; 
        } else {
            printf("  [XETA] Zehmet olmasa 1-%d arasinda secim edin.\n", XIDMET_SAYI);
        }
        clear_buffer();
    }

    printf("  Musteri / abune kodunuzu daxil edin: ");
    if (fgets(musteri_kodu, MAX_KOD_UZUNLUGU, stdin) != NULL) {
        size_t uz = strlen(musteri_kodu);
        if (uz > 0 && musteri_kodu[uz - 1] == '\n')
            musteri_kodu[uz - 1] = '\0';
    }

    odenis_meblegi = double_oxu("  Odenis meblegi (AZN): ");

    if (odenis_meblegi <= 0.0) {
        printf("\n  [XETA] Odenis meblegi musbat olmalidir.\n");
        return;
    }
    if (odenis_meblegi < xidmetler[secim - 1].min_odenis) {
        printf("\n  [XETA] Bu xidmet ucun minimum odenis: %.2f AZN\n", xidmetler[secim - 1].min_odenis);
        return;
    }
    if (kart->balans < odenis_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans : %.2f AZN\n", kart->balans);
        printf("  Lazim olan  : %.2f AZN\n", odenis_meblegi);
        return;
    }

    kart->balans -= odenis_meblegi;
    xett_ciz();
    printf("          ODENİS QEBZİ\n");
    xett_ciz();
    printf("  Xidmet       : %s\n",    xidmetler[secim - 1].ad);
    printf("  Musteri kodu : %s\n",    musteri_kodu);
    printf("  Odenis       : %.2f AZN\n", odenis_meblegi);
    printf("  Yeni balans  : %.2f AZN\n", kart->balans);
    xett_ciz();
    printf("  ✓ Odenis ugurla tamamlandi!\n");
    xett_ciz();
}

void koc_pulu(AbdullahKart *gonderici, AbdullahKart *alici) {
    char   alici_kart_no[KART_NOMRESI_UZUNLUGU]; 
    double kocurme_meblegi;                       

    xett_ciz();
    printf("         KART-DAN KARTA KOCURME\n");
    xett_ciz();
    kart_nomresi_oxu(alici_kart_no);

    if (strcmp(alici_kart_no, gonderici->kart_nomresi) == 0) {
        printf("\n  [XETA] Oz kartiniza kocurme ede bilmezsiniz.\n");
        return;
    }

    kocurme_meblegi = double_oxu("  Kocurme meblegi (AZN): ");

    if (kocurme_meblegi < MIN_KOCURME_MEBLEGI) {
        printf("\n  [XETA] Minimum kocurme meblegi: %.2f AZN\n", MIN_KOCURME_MEBLEGI);
        return;
    }
    if (kocurme_meblegi > MAX_KOCURME_MEBLEGI) {
        printf("\n  [XETA] Maksimum kocurme meblegi: %.2f AZN\n", MAX_KOCURME_MEBLEGI);
        return;
    }
    if (gonderici->balans < kocurme_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans   : %.2f AZN\n", gonderici->balans);
        printf("  Kocurme ucun  : %.2f AZN lazimdir\n", kocurme_meblegi);
        return;
    }

    gonderici->balans -= kocurme_meblegi;  
    alici->balans     += kocurme_meblegi;  

    xett_ciz();
    printf("         KOCURME QEBZİ\n");
    xett_ciz();
    printf("  Gonderici kart : %s\n",    gonderici->kart_nomresi);
    printf("  Alici kart     : %s\n",    alici_kart_no);
    printf("  Kocurulen      : %.2f AZN\n", kocurme_meblegi);
    printf("  Sizin yeni bal.: %.2f AZN\n", gonderici->balans);
    xett_ciz();
    printf("  ✓ Kocurme ugurla tamamlandi!\n");
    xett_ciz();
}

void cekil_naqd(AbdullahKart *kart) {
    double cekilme_meblegi; 

    xett_ciz();
    printf("          NAĞDLAŞDIRMA (PUL CEKME)\n");
    xett_ciz();
    printf("  Cari balans : %.2f AZN\n", kart->balans);
    printf("  Limit       : %.2f - %.2f AZN\n", MIN_CEKILME_MEBLEGI, MAX_CEKILME_MEBLEGI);
    xett_ciz();

    cekilme_meblegi = double_oxu("  Cekilecek mebleg (AZN): ");

    if (cekilme_meblegi < MIN_CEKILME_MEBLEGI) {
        printf("\n  [XETA] Minimum cekilme meblegi %.2f AZN-dir.\n", MIN_CEKILME_MEBLEGI);
        return;
    }
    if (cekilme_meblegi > MAX_CEKILME_MEBLEGI) {
        printf("\n  [XETA] Maksimum cekilme meblegi %.2f AZN-dir.\n", MAX_CEKILME_MEBLEGI);
        return;
    }
    if (kart->balans < cekilme_meblegi) {
        printf("\n  [XETA] Balans kifayet etmir!\n");
        printf("  Cari balans  : %.2f AZN\n", kart->balans);
        printf("  Cekmek istey.: %.2f AZN\n", cekilme_meblegi);
        printf("  Catismazliq  : %.2f AZN\n", cekilme_meblegi - kart->balans);
        return;
    }

    kart->balans -= cekilme_meblegi;
    xett_ciz();
    printf("      NAĞDLAŞDIRMA QEBZİ\n");
    xett_ciz();
    printf("  Kart sahibi  : %s\n",    kart->sahib_adi);
    printf("  Verildi      : %.2f AZN\n", cekilme_meblegi);
    printf("  Yeni balans  : %.2f AZN\n", kart->balans);
    xett_ciz();
    printf("  ✓ Nağdlaşdirma ugurla tamamlandi!\n");
    xett_ciz();
}

void artir_balans(AbdullahKart *kart) {
    double artirma_meblegi; 

    xett_ciz();
    printf("           BALANS ARTIRMA (MEDAXIL)\n");
    xett_ciz();
    printf("  Cari balans : %.2f AZN\n", kart->balans);
    xett_ciz();

    artirma_meblegi = double_oxu("  Elave edilecek mebleg (AZN): ");

    if (artirma_meblegi <= 0.0) {
        printf("\n  [XETA] Mebleg musbat olmalidir. Menfi ve ya sifir daxil etmek olmaz.\n");
        return;
    }
    if (artirma_meblegi > MAX_MEBLAQ_ARTIRMA) {
        printf("\n  [XETA] Bir dəfəlik maksimum artirma: %.2f AZN\n", MAX_MEBLAQ_ARTIRMA);
        return;
    }

    kart->balans += artirma_meblegi;
    xett_ciz();
    printf("          MEDAXIL QEBZİ\n");
    xett_ciz();
    printf("  Kart sahibi  : %s\n",    kart->sahib_adi);
    printf("  Elave edildi : %.2f AZN\n", artirma_meblegi);
    printf("  Yeni balans  : %.2f AZN\n", kart->balans);
    xett_ciz();
    printf("  ✓ Balans ugurla artırildi!\n");
    xett_ciz();
}


// ƏLİ
void save_to_file(struct CardList *list) {
    FILE *file = fopen("cards.dat", "wb");
    if (file != NULL) {
        fwrite(&list->count, sizeof(int), 1, file);
        fwrite(&list->next_id, sizeof(int), 1, file);
        fwrite(list->cards, sizeof(struct AliCard), list->count, file);
        fclose(file);
    }
}

void load_from_file(struct CardList *list) {
    FILE *file = fopen("cards.dat", "rb");
    if (file == NULL) {
        list->count = 0;
        list->next_id = 1;
        struct AliCard test_card = {list->next_id++, "1234567812345678", "Ali Asadzada", 1250.50, "VISA", 1};
        list->cards[0] = test_card;
        list->count = 1;
        return;
    }
    fread(&list->count, sizeof(int), 1, file);
    fread(&list->next_id, sizeof(int), 1, file);
    fread(list->cards, sizeof(struct AliCard), list->count, file);
    fclose(file);
}

void add_card(struct CardList *list) {
    if (list->count >= MAX_CARDS_ALI) {
        printf("[X] Card list is full! Cannot add more cards.\n");
        return;
    }
    struct AliCard new_card;
    new_card.id = list->next_id++; 
    printf("\n--- Adding Card (Internal ID: %d) ---\n", new_card.id);
    printf("Enter card number (16 digits): ");
    scanf("%16s", new_card.card_number);
    clear_buffer(); 
    printf("Enter owner name            : ");
    fgets(new_card.owner_name, sizeof(new_card.owner_name), stdin);
    new_card.owner_name[strcspn(new_card.owner_name, "\n")] = 0; 
    printf("Enter balance (AZN)         : ");
    if (scanf("%lf", &new_card.balance) != 1) {
        printf("[X] Invalid balance! Setting to 0.00\n");
        new_card.balance = 0.0;
    }
    clear_buffer();
    printf("Enter card type (VISA/MasterCard): ");
    scanf("%19s", new_card.card_type);
    clear_buffer();
    new_card.is_active = 1; 
    list->cards[list->count] = new_card;
    list->count++;
    printf("[OK] Card added successfully! Assigned ID: %d\n", new_card.id);
}

void display_all_cards(struct CardList *list) {
    if (list->count == 0) {
        printf("\nNo cards in the list yet.\n");
        return;
    }
    printf("\n========== ALL CARDS ==========\n");
    for (int i = 0; i < list->count; i++) {
        struct AliCard c = list->cards[i];
        printf("\n--- Internal ID: %d ---\n", c.id);
        printf("Card Number : %s\n", c.card_number);
        printf("Owner Name  : %s\n", c.owner_name);
        printf("Balance     : %.2f AZN\n", c.balance);
        printf("Card Type   : %s\n", c.card_type);
        printf("Status      : %s\n", c.is_active ? "Active" : "Blocked");
    }
    printf("\n================================\n");
}

void search_card(struct CardList *list) {
    char search_number[17];
    printf("Enter card number to search: ");
    scanf("%16s", search_number);
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->cards[i].card_number, search_number) == 0) {
            struct AliCard c = list->cards[i];
            printf("\n--- Card Found (Internal ID: %d) ---\n", c.id);
            printf("Card Number : %s\n", c.card_number);
            printf("Owner Name  : %s\n", c.owner_name);
            printf("Balance     : %.2f AZN\n", c.balance);
            printf("Card Type   : %s\n", c.card_type);
            printf("Status      : %s\n", c.is_active ? "Active" : "Blocked");
            return;
        }
    }
    printf("[X] Card not found!\n");
}

void toggle_card_status(struct CardList *list) {
    char search_number[17];
    printf("Enter card number to block/unblock: ");
    scanf("%16s", search_number);
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->cards[i].card_number, search_number) == 0) {
            list->cards[i].is_active = !list->cards[i].is_active;
            printf("[OK] Card status is now: %s\n", list->cards[i].is_active ? "Active" : "Blocked");
            return;
        }
    }
    printf("[X] Card not found!\n");
}

void delete_card(struct CardList *list) {
    char search_number[17];
    printf("Enter card number to delete: ");
    scanf("%16s", search_number);
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->cards[i].card_number, search_number) == 0) {
            for (int j = i; j < list->count - 1; j++) {
                list->cards[j] = list->cards[j + 1];
            }
            list->count--;
            printf("[OK] Card deleted successfully!\n");
            return;
        }
    }
    printf("[X] Card not found!\n");
}

void manage_balance(struct CardList *list) {
    char search_number[17];
    printf("Enter card number for transaction: ");
    scanf("%16s", search_number);
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->cards[i].card_number, search_number) == 0) {
            if (!list->cards[i].is_active) {
                printf("[X] Transaction failed! This card is BLOCKED.\n");
                return;
            }
            int type; double amount;
            printf("Select operation (1 = Deposit, 2 = Withdraw): ");
            if (scanf("%d", &type) != 1) {
                printf("[X] Invalid choice!\n");
                clear_buffer();
                return;
            }
            clear_buffer();
            if (type == 1) {
                printf("Enter amount to deposit: ");
                if (scanf("%lf", &amount) == 1 && amount > 0) {
                    list->cards[i].balance += amount;
                    printf("[OK] Deposited %.2f AZN. New Balance: %.2f AZN\n", amount, list->cards[i].balance);
                } else {
                    printf("[X] Invalid amount!\n");
                }
                clear_buffer();
            } else if (type == 2) {
                printf("Enter amount to withdraw: ");
                if (scanf("%lf", &amount) == 1 && amount > 0 && amount <= list->cards[i].balance) {
                    list->cards[i].balance -= amount;
                    printf("[OK] Withdrew %.2f AZN. New Balance: %.2f AZN\n", amount, list->cards[i].balance);
                } else {
                    printf("[X] Invalid amount or Insufficient funds!\n");
                }
                clear_buffer();
            } else {
                printf("[X] Invalid operation type!\n");
            }
            return;
        }
    }
    printf("[X] Card not found!\n");
}

void show_ali_card_menu() {
    printf("\n===== BANK CARD SYSTEM =====\n");
    printf("1. Add Card\n");
    printf("2. Display All Cards\n");
    printf("3. Search Card\n");
    printf("4. Block / Unblock Card\n");
    printf("5. Delete Card\n");
    printf("6. Deposit / Withdraw Money\n");
    printf("7. Exit Card System\n");
    printf("Your choice: ");
}



// ƏLİ
void save_customers_to_file(struct CustomerList *list) {
    FILE *file = fopen("customers.dat", "wb");
    if (file != NULL) {
        fwrite(&list->count, sizeof(int), 1, file);
        fwrite(&list->next_id, sizeof(int), 1, file);
        fwrite(list->customers, sizeof(struct Customer), list->count, file);
        fclose(file);
    }
}

void load_customers_from_file(struct CustomerList *list) {
    FILE *file = fopen("customers.dat", "rb");
    if (file == NULL) {
        list->count   = 0;
        list->next_id = 1;
        struct Customer test_cust = {list->next_id++, "Ali Asadzada", "+994554740818", "eliesedzade21e@mail.com", "Baku, Azerbaijan", 1};
        list->customers[0] = test_cust;
        list->count = 1;
        return;
    }
    fread(&list->count, sizeof(int), 1, file);
    fread(&list->next_id, sizeof(int), 1, file);
    fread(list->customers, sizeof(struct Customer), list->count, file);
    fclose(file);
}

void add_customer(struct CustomerList *list) {
    if (list->count >= MAX_CUSTOMERS_ALI) {
        printf("[X] Customer list is full! Cannot add more customers.\n");
        return;
    }
    struct Customer new_customer;
    new_customer.id = list->next_id++;
    printf("\n--- Adding Customer (ID: %d) ---\n", new_customer.id);
    printf("Enter full name   : ");
    fgets(new_customer.full_name, sizeof(new_customer.full_name), stdin);
    new_customer.full_name[strcspn(new_customer.full_name, "\n")] = 0; 
    printf("Enter phone number: ");
    scanf("%14s", new_customer.phone);
    clear_buffer();
    printf("Enter email       : ");
    scanf("%49s", new_customer.email);
    clear_buffer();
    printf("Enter address     : ");
    fgets(new_customer.address, sizeof(new_customer.address), stdin);
    new_customer.address[strcspn(new_customer.address, "\n")] = 0;
    new_customer.is_active = 1;   
    list->customers[list->count] = new_customer;
    list->count++;
    printf("[OK] Customer added successfully! (ID: %d)\n", new_customer.id);
}

void display_all_customers(struct CustomerList *list) {
    if (list->count == 0) {
        printf("\nNo customers in the list yet.\n");
        return;
    }
    printf("\n========== CUSTOMER LIST ==========\n");
    for (int i = 0; i < list->count; i++) {
        struct Customer c = list->customers[i];
        printf("\n--- Customer %d ---\n",  i + 1);
        printf("ID       : %d\n",          c.id);
        printf("Name     : %s\n",          c.full_name);
        printf("Phone    : %s\n",          c.phone);
        printf("Email    : %s\n",          c.email);
        printf("Address  : %s\n",          c.address);
        printf("Status   : %s\n",          c.is_active ? "Active" : "Closed");
    }
    printf("\n===================================\n");
}

void search_customer_by_id(struct CustomerList *list) {
    int search_id;
    printf("Enter customer ID to search: ");
    if (scanf("%d", &search_id) != 1) {
        printf("[X] Invalid ID format!\n");
        clear_buffer();
        return;
    }
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (list->customers[i].id == search_id) {
            struct Customer c = list->customers[i];
            printf("\n--- Customer Found ---\n");
            printf("ID       : %d\n",  c.id);
            printf("Name     : %s\n",  c.full_name);
            printf("Phone    : %s\n",  c.phone);
            printf("Email    : %s\n",  c.email);
            printf("Address  : %s\n",  c.address);
            printf("Status   : %s\n",  c.is_active ? "Active" : "Closed");
            return;
        }
    }
    printf("[X] Customer with ID %d not found!\n", search_id);
}

void search_customer_by_name(struct CustomerList *list) {
    char search_name[60];
    printf("Enter name to search: ");
    fgets(search_name, sizeof(search_name), stdin);
    search_name[strcspn(search_name, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < list->count; i++) {
        if (strstr(list->customers[i].full_name, search_name) != NULL) {
            struct Customer c = list->customers[i];
            printf("\n--- Match Found ---\n");
            printf("ID       : %d\n",  c.id);
            printf("Name     : %s\n",  c.full_name);
            printf("Phone    : %s\n",  c.phone);
            printf("Email    : %s\n",  c.email);
            printf("Address  : %s\n",  c.address);
            printf("Status   : %s\n",  c.is_active ? "Active" : "Closed");
            found = 1;
        }
    }
    if (found == 0) printf("[X] No customer found with that name.\n");
}

void update_customer(struct CustomerList *list) {
    int search_id;
    printf("Enter customer ID to update: ");
    if (scanf("%d", &search_id) != 1) {
        printf("[X] Invalid ID format!\n");
        clear_buffer();
        return;
    }
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (list->customers[i].id == search_id) {
            printf("\nWhat do you want to update?\n");
            printf("1. Phone number\n");
            printf("2. Email\n");
            printf("3. Address\n");
            printf("Your choice: ");
            int choice;
            if (scanf("%d", &choice) != 1) {
                printf("[X] Invalid choice.\n");
                clear_buffer();
                return;
            }
            clear_buffer();
            if (choice == 1) {
                printf("Enter new phone number: ");
                scanf("%14s", list->customers[i].phone);
                clear_buffer();
                printf("[OK] Phone updated!\n");
            } else if (choice == 2) {
                printf("Enter new email: ");
                scanf("%49s", list->customers[i].email);
                clear_buffer();
                printf("[OK] Email updated!\n");
            } else if (choice == 3) {
                printf("Enter new address: ");
                fgets(list->customers[i].address, sizeof(list->customers[i].address), stdin);
                list->customers[i].address[strcspn(list->customers[i].address, "\n")] = 0;
                printf("[OK] Address updated!\n");
            } else {
                printf("[X] Invalid choice.\n");
            }
            return;
        }
    }
    printf("[X] Customer with ID %d not found!\n", search_id);
}

void toggle_customer_status(struct CustomerList *list) {
    int search_id;
    printf("Enter customer ID to close/reopen account: ");
    if (scanf("%d", &search_id) != 1) {
        printf("[X] Invalid ID format!\n");
        clear_buffer();
        return;
    }
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (list->customers[i].id == search_id) {
            list->customers[i].is_active = !list->customers[i].is_active;
            printf("[OK] Account status is now: %s\n", list->customers[i].is_active ? "Active" : "Closed");
            return;
        }
    }
    printf("[X] Customer with ID %d not found!\n", search_id);
}

void delete_customer(struct CustomerList *list) {
    int search_id;
    printf("Enter customer ID to delete: ");
    if (scanf("%d", &search_id) != 1) {
        printf("[X] Invalid ID format!\n");
        clear_buffer();
        return;
    }
    clear_buffer();
    for (int i = 0; i < list->count; i++) {
        if (list->customers[i].id == search_id) {
            for (int j = i; j < list->count - 1; j++) {
                list->customers[j] = list->customers[j + 1];
            }
            list->count--;
            printf("[OK] Customer deleted successfully!\n");
            return;
        }
    }
    printf("[X] Customer with ID %d not found!\n", search_id);
}

void show_ali_customer_menu() {
    printf("\n===== BANK CUSTOMER SYSTEM =====\n");
    printf("1. Add Customer\n");
    printf("2. Display All Customers\n");
    printf("3. Search by ID\n");
    printf("4. Search by Name\n");
    printf("5. Update Customer Info\n");
    printf("6. Close / Reopen Account\n");
    printf("7. Delete Customer\n");
    printf("8. Exit Customer System\n");
    printf("Your choice: ");
}


//Integrated
int main() {
   
    struct CardList ali_card_list;
    load_from_file(&ali_card_list); 

    struct CustomerList ali_cust_list;
    load_customers_from_file(&ali_cust_list);

    struct user usr;
    FILE *fp;
    int opt;

    Account leman_acc;
    strcpy(leman_acc.name, "Eli Memmedov");
    strcpy(leman_acc.id, "AZ001");
    leman_acc.balance = 0.0;
    leman_acc.trans_count = 0;
    int leman_choice;
    double leman_amount;

    struct BankAccount leyla_accounts[MAX_ACCOUNTS_LEYLA];
    int leyla_total_accounts = 0;
    int leyla_choice;

    AbdullahKart abdullah_kart1 = { .kart_nomresi = "1234567890123456", .balans = 500.00, .sahib_adi = "Əli Həsənov" };
    AbdullahKart abdullah_kart2 = { .kart_nomresi = "9876543210654321", .balans = 100.00, .sahib_adi = "Nigar Muradova" };
    int abdullah_secim;

    int master_choice;

    // Integrated system-Aygul
    while (1) {
        printf("\n==================================================\n");
        printf("       SUPREME BANKING INTEGRATED SYSTEM v4       \n");
        printf("==================================================\n");
        printf("1. USER AUTHENTICATION SYSTEM \n");
        printf("2. GLOBAL ACCOUNT OPERATIONS \n");
        printf("3. ACCOUNT CREATION & LOCKING \n");
        printf("4. TERMINAL & ATM INTERFACE \n");
        printf("5. ADVANCED CARD DATABASE \n");
        printf("6. CUSTOMER RELATIONS ENGINE \n");
        printf("0. SAVE ALL DATABASES & POWER OFF\n");
        printf("==================================================\n");
        printf("Sistemi secin: ");

        if (scanf("%d", &master_choice) != 1) {
            printf("[X] Reqem daxil edin!\n");
            clear_buffer();
            continue;
        }
        clear_buffer();

        if (master_choice == 0) {
            save_to_file(&ali_card_list);
            save_customers_to_file(&ali_cust_list);
            printf("\n[OK] Butun datalar fayllara ugurla yazildi. Sistem sondurulur. Sag olun!\n");
            break;
        }

        switch (master_choice) {
            
            case 1: 
                    // AYGÜL
                printf("\nWhat do you want to do?");
                printf("\n\n1. Register an account");
                printf("\n2. Login");
                printf("\n\n Your choice:\t");
                scanf("%d",&opt);

                if(opt==1) {
                    system("cls");
                    fflush(stdout);
                    printf("Enter your name:\t");
                    scanf("%s",usr.name);
                    printf("Enter your surname:\t");
                    scanf("%s",usr.surname);
                    printf("Enter your birth:\t");
                    scanf("%s",usr.birth);
                    printf("Enter your FIN:\t");
                    scanf("%s",usr.FIN);
                    printf("Enter your phone number:\t");
                    scanf("%s",usr.phone);
                    printf("Enter your email:\t");
                    scanf("%s",usr.email);
                    printf("Enter new password:\t");
                    scanf("%s",usr.password);
                    usr.balance=0;
                    strcpy(usr.name,usr.phone);
                    fp=fopen(strcat(usr.name,".dat"),"w");
                    fwrite(&usr,sizeof(struct user),1,fp);
                    if (fwrite!=0){
                        printf("\n\nAccount successfully registered");
                        fclose(fp);
                    }
                    else {
                        printf("\n\nAccount not registered");
                    }
                }
                if (opt==2) {
                    system("cls");
                    while (getchar()!='\n');
                    char filename[50];
                    char entered_pass[50];
                    printf("\nEnter your phone number/email/ID:\t");
                    scanf("%s",filename);
                    printf("Password:\t");
                    scanf("%s",entered_pass);
                    strcat(filename,".dat");
                    fp=fopen(filename,"r");
                    if (fp==NULL) {
                        printf("\n\nAccount not found");
                    }
                    else {
                        fread(&usr,sizeof(struct user),1,fp);
                        fclose(fp);
                    }
                    if (!strcmp(entered_pass,usr.password)) {
                        printf("\nPassword matched");
                    }
                    else {
                        printf("\nInvalid password");
                    }
                }
                break;

            case 2: 
                    // LƏMAN
                while (1) {
                    printf("\n=== BANK MENYU ===\n");
                    printf("1. Deposit\n");
                    printf("2. Withdraw\n");
                    printf("3. Balans\n");
                    printf("4. Tarixce\n");
                    printf("0. Cix\n");
                    printf("Secim: ");
                    scanf("%d", &leman_choice);

                    if (leman_choice == 1) {
                        printf("Mebleg: ");
                        scanf("%lf", &leman_amount);
                        deposit(&leman_acc, leman_amount);
                    } else if (leman_choice == 2) {
                        printf("Mebleg: ");
                        scanf("%lf", &leman_amount);
                        withdraw(&leman_acc, leman_amount);
                    } else if (leman_choice == 3) {
                        show_balance(&leman_acc);
                    } else if (leman_choice == 4) {
                        show_history(&leman_acc);
                    } else if (leman_choice == 0) {
                        printf("Cixis...\n");
                        break;
                    } else {
                        printf("Yanlis secim!\n");
                    }
                }
                break;

            case 3:
                    // LEYLA
                while (1) {
                    printf("\n... Bank (Aktiv hesablar: %d) ...\n", leyla_total_accounts);
                    printf("1. Create Bank Account\n");
                    printf("2. Create Card\n");
                    printf("3. Block Card\n");
                    printf("4. Cixis\n");
                    printf("Secim edin: ");
                   
                    if (scanf("%d", &leyla_choice) != 1) {
                        printf("Yanlis daxiletme! 1 2 3 4 -den birini daxil edin.\n");
                        clear_buffer();
                        continue;
                    }

                    if (leyla_choice == 1) {
                        create_bank_account(leyla_accounts, &leyla_total_accounts);
                    }
                    else if (leyla_choice == 2) {
                        create_card(leyla_accounts, leyla_total_accounts);
                    }
                    else if (leyla_choice == 3) {
                        block_card(leyla_accounts, leyla_total_accounts);
                    }
                    else if (leyla_choice == 4) {
                        printf("BYE!\n");
                        break;
                    }
                    else {
                        printf("Yanlis secim!\n");
                    }
                }
                break;

            case 4:
                    // ABDULLAH
                while (1) {
                    printf("\n");
                    xett_ciz();
                    printf("         BANK SİSTEMİ - ANA MENYU\n");
                    xett_ciz();
                    printf("  Kart sahibi : %s\n", abdullah_kart1.sahib_adi);
                    printf("  Balans      : %.2f AZN\n\n", abdullah_kart1.balans);
                    printf("  1. Kommunal / Xidmet Odenisi\n");
                    printf("  2. Kartdan Karta Kocurme\n");
                    printf("  3. Nağdlaşdırma (Pul Cekme)\n");
                    printf("  4. Balans Artirma (Medaxil)\n");
                    printf("  0. Cixis\n");
                    xett_ciz();
                    printf("  Seciminiz: ");

                    if (scanf("%d", &abdullah_secim) != 1) {
                        clear_buffer();
                        continue; 
                    }
                    clear_buffer();

                    if (abdullah_secim == 1) {
                        ode_kommunal(&abdullah_kart1);
                    } else if (abdullah_secim == 2) {
                        koc_pulu(&abdullah_kart1, &abdullah_kart2);
                        printf("  [TEST] %s-in yeni balansi: %.2f AZN\n", abdullah_kart2.sahib_adi, abdullah_kart2.balans);
                    } else if (abdullah_secim == 3) {
                        cekil_naqd(&abdullah_kart1);
                    } else if (abdullah_secim == 4) {
                        artir_balans(&abdullah_kart1);
                    } else if (abdullah_secim == 0) {
                        printf("\n  Sistemden cixildi. Ugurlar!\n\n");
                        break;
                    } else {
                        printf("\n  [XETA] Yalniz 0-4 arasinda secim edin.\n");
                    }
                }
                break;

            case 5: 
                    // ƏLİ
                while (1) {
                    show_ali_card_menu();
                    int ali_card_choice;
                    if (scanf("%d", &ali_card_choice) != 1) {
                        printf("[X] Invalid choice. Please enter a number.\n");
                        clear_buffer();
                        continue;
                    }
                    clear_buffer();

                    if (ali_card_choice == 1) add_card(&ali_card_list);
                    else if (ali_card_choice == 2) display_all_cards(&ali_card_list);
                    else if (ali_card_choice == 3) search_card(&ali_card_list);
                    else if (ali_card_choice == 4) toggle_card_status(&ali_card_list);
                    else if (ali_card_choice == 5) delete_card(&ali_card_list);
                    else if (ali_card_choice == 6) manage_balance(&ali_card_list);
                    else if (ali_card_choice == 7) {
                        save_to_file(&ali_card_list);
                        printf("Exiting Card System...\n");
                        break;
                    } else {
                        printf("[X] Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 6:
                    // ƏLİ
                while (1) {
                    show_ali_customer_menu();
                    int ali_cust_choice;
                    if (scanf("%d", &ali_cust_choice) != 1) {
                        printf("[X] Invalid choice. Please enter a number.\n");
                        clear_buffer();
                        continue;
                    }
                    clear_buffer();

                    if (ali_cust_choice == 1) add_customer(&ali_cust_list);
                    else if (ali_cust_choice == 2) display_all_customers(&ali_cust_list);
                    else if (ali_cust_choice == 3) search_customer_by_id(&ali_cust_list);
                    else if (ali_cust_choice == 4) search_customer_by_name(&ali_cust_list);
                    else if (ali_cust_choice == 5) update_customer(&ali_cust_list);
                    else if (ali_cust_choice == 6) toggle_customer_status(&ali_cust_list);
                    else if (ali_cust_choice == 7) delete_customer(&ali_cust_list);
                    else if (ali_cust_choice == 8) {
                        save_customers_to_file(&ali_cust_list);
                        printf("Exiting Customer System...\n");
                        break;
                    } else {
                        printf("[X] Invalid choice. Please try again.\n");
                    }
                }
                break;

            default:
                printf("[X] Yanlis sistem secimi! Ana siyahidan secim edin.\n");
        }
    }
    return 0;
}
