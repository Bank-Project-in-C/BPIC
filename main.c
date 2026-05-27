#include <stdio.h>
#include <string.h>
#define MAX_CARDS 5
#define MAX_ACCOUNTS 5
#include<stdlib.h>

//leyla ZAKIYEVA
struct user {
typedef struct {
    char type[20];
    double amount;
    char date[20];
} Transaction;

struct Card {
    char card_number[17];
    int is_blocked;
};

struct BankAccount {
    char account_number[10];
typedef struct {
    char name[50];
    char surname[50];
    char birth[50];
    char FIN[50];
    char phone[50];
    char email[50];
    char account_id[50];
    char password[50];
    float balance;
    struct Card cards[MAX_CARDS];
    int card_count;
};
    char id[10];
    double balance;
    Transaction history[MAX_TRANS];
    int trans_count;
} Account;

void clear_buffer() {
    while (getchar() != '\n');
}

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
    char temp_account_number[10];
    printf("\n           --- Hesabin yaradilmasi ---         \n");
    printf("Yeni hesab nomresini daxil edin: ");
    scanf("%9s", temp_account_number);
    clear_buffer();

    if (find_account(accounts, *account_count, temp_account_number) != -1) {
        printf("\n[ Xeta bas verdi! ] %s nomreli hesab artiq movcuddur!\n", temp_account_number);
        return;
    }

void show_balance(Account *acc) {
    printf("\n=== BALANS ===\n");
    printf("Ad    : %s\n", acc->name);
    printf("ID    : %s\n", acc->id);
    printf("Balans: %.2f AZN\n", acc->balance);
    printf("==============\n");
}
    int index = *account_count;
    strcpy(accounts[index].account_number, temp_account_number);
    accounts[index].balance = 0.0f;
    accounts[index].card_count = 0;

    printf("[ Ugurlu emeliyyat ] %s nomreli hesab yaradildi.\n", accounts[index].account_number);
    (*account_count)++;
}

void show_history(Account *acc) {
    printf("\n=== EMELIYYAT TARIXCESI ===\n");
void create_card(struct BankAccount accounts[], int account_count) {
    if (account_count == 0) {
        printf("\n[ Xeta bas verdi! ] Sistemde hec bir hesab yoxdur! Hesab yaradin.\n");
        return;
    }

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
    char search_acc[10];
    printf("\nHansi hesab ucun kart yaratmaq isteyirsiniz? Nomreni daxil edin: ");
    scanf("%9s", search_acc);
    clear_buffer();

    int acc_idx = find_account(accounts, account_count, search_acc);
    if (acc_idx == -1) {
        printf("[ Xeta bas verdi! ] Bele bir hesab tapilmadi.\n");
        return;
    }

void deposit(Account *acc, double amount) {
    acc->balance += amount;
    save_transaction(acc, "Deposit", amount);
    printf("%.2f AZN elave edildi.\n", amount);
}
    struct BankAccount *acc = &accounts[acc_idx];

void withdraw(Account *acc, double amount) {
    if (amount > acc->balance) {
        printf("Balans kifayet deyil!\n");
    if (acc->card_count >= MAX_CARDS) {
        printf("\n[ Xeta bas verdi! ] Bu hesab ucun maksimum kart limitine catdiniz (%d).\n", MAX_CARDS);
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
    acc->balance -= amount;
    save_transaction(acc, "Withdrawal", amount);
    printf("%.2f AZN cixarildi.\n", amount);
}

int main() {
    struct user usr;
    FILE *fp;
    char entered_pass[50];
    char phone[50];
    int opt;
    Account acc;
    strcpy(acc.name, "Eli Memmedov");
    strcpy(acc.id, "AZ001");
    acc.balance = 0.0;
    acc.trans_count = 0;

    printf("\nWhat do you want to do?");
    printf("\n\n1. Register an account");
    printf("\n2. Login");
    printf("\n\n Your choice:\t");
    scanf("%d",&opt);
    struct BankAccount bank_accounts[MAX_ACCOUNTS];
    int total_accounts = 0;
    int choice;
    double amount;

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
            }
            else {
                printf("\nInvalid password");
            }
        }
    return 0;
    while (1) {
        printf("\n_ _ _..._ _ _ Bank (Aktiv hesablar: %d) _ _ _..._ _ _\n", total_accounts);
        printf("1. Create Bank Account\n");
        printf("2. Create Card\n");
        printf("3. Block Card\n");
        printf("4. Cixis\n");
        printf("Secim edin: ");
        printf("\n=== BANK MENYU ===\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Balans\n");
        printf("4. Tarixce\n");
        printf("0. Cix\n");
        printf("Secim: ");
        scanf("%d", &choice);

        if (scanf("%d", &choice) != 1) {
            printf("Yanlis daxiletme! 1 2 3 4 -den birini daxil edin.\n");
            clear_buffer();
            continue;
        }

        if (choice == 1) {
            create_bank_account(bank_accounts, &total_accounts);
        }
        else if (choice == 2) {
            create_card(bank_accounts, total_accounts);
        }
        else if (choice == 3) {
            block_card(bank_accounts, total_accounts);
        }
        else if (choice == 4) {
            printf("BYE!\n");
            break;
        }
        else {
            printf("Yanlis secim!\n");
        }
    }
    return 0;
        switch (choice) {
            case 1:
                printf("Mebleg: ");
                scanf("%lf", &amount);
                deposit(&acc, amount);
                break;
            case 2:
                printf("Mebleg: ");
                scanf("%lf", &amount);
                withdraw(&acc, amount);
                break;
            case 3:
                show_balance(&acc);
                break;
            case 4:
                show_history(&acc);
                break;
            case 0:
                printf("Cixis...\n");
                return 0;
            default:
                printf("Yanlis secim!\n");
        }
    }
}
