#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CARDS_NEW 100
#define MAX_CARDS_OLD 5
#define MAX_ACCOUNTS 5
#define MAX_TRANS 100
#define MAX_CUSTOMERS 100

// ============================================================================
// 1. STRUKTURLAR (Bütün Modulların Strukturları Qorunub)
// ============================================================================

// --- Əlinin Modulunun Strukturları ---
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

// --- Yeni Kart Modulunun Strukturları ---
struct Card {
    int id;
    char card_number[17];
    char owner_name[50];
    double balance;
    char card_type[20];
    int is_active;
};

struct CardList {
    struct Card cards[MAX_CARDS_NEW];
    int count;
    int next_id;
};

// --- Qeydiyyat/Giriş Modulunun Strukturu ---
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

// --- Leylanın Modulunun Strukturları ---
struct OldCard {
    char card_number[17];
    int is_blocked;
};

struct BankAccount {
    char account_number[10];
    float balance;
    struct OldCard cards[MAX_CARDS_OLD];
    int card_count;
};

// --- Yeni Müştəri Modulunun Strukturları ---
struct Customer {
    int id;
    char full_name[60];
    char phone[15];
    char email[50];
    char address[100];
    int is_active;
};

struct CustomerList {
    struct Customer customers[MAX_CUSTOMERS];
    int count;
    int next_id;
};


// ============================================================================
// 2. KÖMƏKÇİ VƏ FAYL FUNKSİYALARI
// ============================================================================

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_date(char *buf) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(buf, 20, "%d.%m.%Y", tm);
}

// Yeni Kart Modulunun Fayl Funksiyaları
void save_to_file(struct CardList *list) {
    FILE *file = fopen("cards.dat", "wb");
    if (file != NULL) {
        fwrite(&list->count, sizeof(int), 1, file);
        fwrite(&list->next_id, sizeof(int), 1, file);
        fwrite(list->cards, sizeof(struct Card), list->count, file);
        fclose(file);
    }
}

void load_from_file(struct CardList *list) {
    FILE *file = fopen("cards.dat", "rb");
    if (file == NULL) {
        list->count = 0;
        list->next_id = 1;
        struct Card test_card = {list->next_id++, "1234567812345678", "Ali Asadzada", 1250.50, "VISA", 1};
        list->cards[0] = test_card;
        list->count = 1;
        return;
    }
    fread(&list->count, sizeof(int), 1, file);
    fread(&list->next_id, sizeof(int), 1, file);
    fread(list->cards, sizeof(struct Card), list->count, file);
    fclose(file);
}

// Müştəri Modulunun Fayl Funksiyaları
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


// ============================================================================
// 3. YENİ GƏTİRDİYİN KART MODULUNUN FUNKSİYALARI
// ============================================================================

void add_card(struct CardList *list) {
    if (list->count >= MAX_CARDS_NEW) {
        printf("[X] Card list is full! Cannot add more cards.\n");
        return;
    }

    struct Card new_card;
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
        struct Card c = list->cards[i];
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
            struct Card c = list->cards[i];
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
            printf("[OK] Card status is now: %s\n",
                   list->cards[i].is_active ? "Active" : "Blocked");
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

            int type;
            double amount;
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


// ============================================================================
// 4. LEYLANIN HESAB MODULUNUN FUNKSİYALARI
// ============================================================================

int find_account(struct BankAccount accounts[], int account_count, const char *num) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_number, num) == 0) {
            return i;
        }
    }
    return -1;
}

void create_bank_account(struct BankAccount accounts[], int *account_count) {
    if (*account_count >= MAX_ACCOUNTS) {
        printf("\n[ Xeta bas verdi! ] Bankda maksimum hesab limitine catildi (%d).\n", MAX_ACCOUNTS);
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

    if (acc->card_count >= MAX_CARDS_OLD) {
        printf("\n[ Xeta bas verdi! ] Bu hesab ucun maksimum kart limitine catdiniz (%d).\n", MAX_CARDS_OLD);
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


// ============================================================================
// 5. ƏLİNİN BALANS VƏ TARİXÇƏ MODULUNUN FUNKSİYALARI
// ============================================================================

void save_transaction(Account *acc, char *type, double amount) {
    if (acc->trans_count >= MAX_TRANS) return;
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


// ============================================================================
// 6. YENİ GƏTİRDİYİN MÜŞTƏRİ MODULUNUN FUNKSİYALARI
// ============================================================================

void add_customer(struct CustomerList *list) {
    if (list->count >= MAX_CUSTOMERS) {
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
    if (found == 0) {
        printf("[X] No customer found with that name.\n");
    }
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
            printf("[OK] Account status is now: %s\n",
                   list->customers[i].is_active ? "Active" : "Closed");
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


// ============================================================================
// 7. YENİLƏNMİŞ ANA MENYÜ (5 SİSTEM BİR YERDƏ)
// ============================================================================

int main() {
    // Bütün modulların məlumat bazaları fayllardan yüklənir
    struct CardList card_list;
    load_from_file(&card_list);

    struct CustomerList cust_list;
    load_customers_from_file(&cust_list);

    struct user usr;
    FILE *fp;

    struct BankAccount bank_accounts[MAX_ACCOUNTS];
    int total_accounts = 0;

    Account eli_acc;
    strcpy(eli_acc.name, "Eli Memmedov");
    strcpy(eli_acc.id, "AZ001");
    eli_acc.balance = 0.0;
    eli_acc.trans_count = 0;

    int main_choice;
    int opt;
    double amount;

    while (1) {
        printf("\n==============================================\n");
        printf("         SUPREME BANKING INTEGRATED SUITE      \n");
        printf("==============================================\n");
        printf("1. USER AUTHENTICATION SYSTEM (Register/Login)\n");
        printf("2. ACCOUNT MANAGEMENT SYSTEM (Leyla's Module)\n");
        printf("3. ADVANCED CARD SYSTEM (New Card Module)\n");
        printf("4. GLOBAL ACCOUNT OPERATION SYSTEM (Eli's Module)\n");
        printf("5. CUSTOMER INFORMATION SYSTEM (New Customer Module)\n");
        printf("0. SAVE & EXIT SYSTEM\n");
        printf("==============================================\n");
        printf("Ana menyudan idareetme sistemini secin: ");

        if (scanf("%d", &main_choice) != 1) {
            printf("[X] Yanlis secim! Reqem daxil edin.\n");
            clear_buffer();
            continue;
        }
        clear_buffer();

        if (main_choice == 0) {
            // Hər iki datanı fayla qeyd edib çıxırıq
            save_to_file(&card_list);
            save_customers_to_file(&cust_list);
            printf("Butun melumatlar ('cards.dat' ve 'customers.dat') yadda saxlanildi. Ugurlar!\n");
            break;
        }

        switch (main_choice) {
            case 1: // --- Qeydiyyat / Giriş Modulu ---
                printf("\n--- AUTHENTICATION SUB-MENU ---");
                printf("\n1. Register an account");
                printf("\n2. Login");
                printf("\nYour choice:\t");
                if (scanf("%d", &opt) != 1) { clear_buffer(); break; }
                clear_buffer();

                if(opt == 1) {
                    system("cls");
                    fflush(stdout);
                    printf("Enter your name:\t");  scanf("%49s", usr.name);
                    printf("Enter your surname:\t"); scanf("%49s", usr.surname);
                    printf("Enter your birth:\t");   scanf("%49s", usr.birth);
                    printf("Enter your FIN:\t");     scanf("%49s", usr.FIN);
                    printf("Enter your phone number:\t"); scanf("%49s", usr.phone);
                    printf("Enter your email:\t");    scanf("%49s", usr.email);
                    printf("Enter new password:\t"); scanf("%49s", usr.password);
                    usr.balance = 0;
                    strcpy(usr.name, usr.phone);

                    fp = fopen(strcat(usr.name, ".dat"), "w");
                    if (fp != NULL) {
                        fwrite(&usr, sizeof(struct user), 1, fp);
                        printf("\n\nAccount successfully registered");
                        fclose(fp);
                    } else {
                        printf("\n\nAccount not registered");
                    }
                }
                else if (opt == 2) {
                    system("cls");
                    char filename[50];
                    char entered_pass[50];
                    printf("\nEnter your phone number/email/ID:\t");
                    scanf("%49s", filename);
                    printf("Password:\t");
                    scanf("%49s", entered_pass);
                    clear_buffer();

                    strcat(filename, ".dat");
                    fp = fopen(filename, "r");
                    if (fp == NULL) {
                        printf("\n\nAccount not found");
                    } else {
                        fread(&usr, sizeof(struct user), 1, fp);
                        fclose(fp);
                        if (strcmp(entered_pass, usr.password) == 0) {
                            printf("\nPassword matched");
                        } else {
                            printf("\nInvalid password");
                        }
                    }
                }
                break;

            case 2: // --- Leylanın Hesab Modulu ---
                printf("\n--- ACCOUNT MANAGEMENT SUB-MENU (Aktiv: %d) ---\n", total_accounts);
                printf("1. Create Bank Account\n");
                printf("2. Create Card\n");
                printf("3. Block Card\n");
                printf("Secim edin: ");
                int leyla_choice;
                if (scanf("%d", &leyla_choice) != 1) { clear_buffer(); break; }
                clear_buffer();

                if (leyla_choice == 1) create_bank_account(bank_accounts, &total_accounts);
                else if (leyla_choice == 2) create_card(bank_accounts, total_accounts);
                else if (leyla_choice == 3) block_card(bank_accounts, total_accounts);
                else printf("Yanlis alt-secim!\n");
                break;

            case 3: // --- Advanced Card Modulu ---
                printf("\n--- ADVANCED CARD SUB-MENU ---\n");
                printf("1. Add Card\n");
                printf("2. Display All Cards\n");
                printf("3. Search Card\n");
                printf("4. Block / Unblock Card\n");
                printf("5. Delete Card\n");
                printf("6. Deposit / Withdraw Money\n");
                printf("Your choice: ");
                int card_choice;
                if (scanf("%d", &card_choice) != 1) { clear_buffer(); break; }
                clear_buffer();

                switch (card_choice) {
                    case 1: add_card(&card_list); break;
                    case 2: display_all_cards(&card_list); break;
                    case 3: search_card(&card_list); break;
                    case 4: toggle_card_status(&card_list); break;
                    case 5: delete_card(&card_list); break;
                    case 6: manage_balance(&card_list); break;
                    default: printf("[X] Invalid sub-choice.\n");
                }
                break;

            case 4: // --- Əlinin Balans/Tarixçə Modulu ---
                printf("\n--- GLOBAL ACCOUNT TRANSACTION SUB-MENU ---\n");
                printf("1. Deposit\n");
                printf("2. Withdraw\n");
                printf("3. Balans\n");
                printf("4. Tarixce\n");
                printf("Secim: ");
                int eli_choice;
                if (scanf("%d", &eli_choice) != 1) { clear_buffer(); break; }
                clear_buffer();

                switch (eli_choice) {
                    case 1:
                        printf("Mebleg: ");
                        scanf("%lf", &amount);
                        deposit(&eli_acc, amount);
                        break;
                    case 2:
                        printf("Mebleg: ");
                        scanf("%lf", &amount);
                        withdraw(&eli_acc, amount);
                        break;
                    case 3: show_balance(&eli_acc); break;
                    case 4: show_history(&eli_acc); break;
                    default: printf("Yanlis alt-secim!\n");
                }
                break;

            case 5: // --- Yeni Müştəri Modulu ---
                printf("\n--- CUSTOMER INFORMATION SUB-MENU ---\n");
                printf("1. Add Customer\n");
                printf("2. Display All Customers\n");
                printf("3. Search by ID\n");
                printf("4. Search by Name\n");
                printf("5. Update Customer Info\n");
                printf("6. Close / Reopen Account\n");
                printf("7. Delete Customer\n");
                printf("Your choice: ");
                int cust_choice;
                if (scanf("%d", &cust_choice) != 1) { clear_buffer(); break; }
                clear_buffer();

                switch (cust_choice) {
                    case 1: add_customer(&cust_list); break;
                    case 2: display_all_customers(&cust_list); break;
                    case 3: search_customer_by_id(&cust_list); break;
                    case 4: search_customer_by_name(&cust_list); break;
                    case 5: update_customer(&cust_list); break;
                    case 6: toggle_customer_status(&cust_list); break;
                    case 7: delete_customer(&cust_list); break;
                    default: printf("[X] Invalid sub-choice.\n");
                }
                break;

            default:
                printf("[X] Yanlis ana secim! Siyahidaki reqemleri daxil edin.\n");
        }
    }
    return 0;
}
