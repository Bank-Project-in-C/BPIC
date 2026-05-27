#include <stdio.h>
#include <string.h>
#include<stdlib.h>

struct user {
typedef struct {
    char type[20];
    double amount;
    char date[20];
} Transaction;

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
};
    char id[10];
    double balance;
    Transaction history[MAX_TRANS];
    int trans_count;
} Account;

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
        printf("\n=== BANK MENYU ===\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Balans\n");
        printf("4. Tarixce\n");
        printf("0. Cix\n");
        printf("Secim: ");
        scanf("%d", &choice);

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
