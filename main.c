#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RECORDS 100
#define MAX_FIELD_LENGTH 1024

// Structure to represent an account
struct Account {
    char name[1024];
    char surname[1024];
    char IBAN[34];
    char coin[4];
    int amount;
};

// Function to read data from a CSV file
void readFromCSV(const char *filePath, struct Account accounts[], int *count) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[200 * 4]; // Adjusted buffer size
    fgets(line, sizeof(line), file); // Read header line (if any), assuming the first line contains headers

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < 1024) {
        // Split the line into fields
        char *token = strtok(line, ",");
        if (token != NULL) {
            strcpy(accounts[i].name, token);
            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(accounts[i].surname, token);
                token = strtok(NULL, ",");
                if (token != NULL) {
                    strcpy(accounts[i].IBAN, token);
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        strcpy(accounts[i].coin, token);
                        token = strtok(NULL, ",");
                        if (token != NULL) {
                            accounts[i].amount = atoi(token);
                            i++;
                        }
                    }
                }
            }
        }
    }
    *count = i;

    fclose(file);
}

// Function to write data to a CSV file
void writeToCSV(const char *filePath, struct Account accounts[], int count) {
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write header line
    fprintf(file, "Name,Surname,IBAN,Coin,Amount\n");

    // Write data
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s,%d\n", accounts[i].name, accounts[i].surname, accounts[i].IBAN, accounts[i].coin, accounts[i].amount);
    }

    fclose(file);
}

// Function to convert string to uppercase
void toUpper(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

int isOnlyAlpha(const char *argmnt){
    while (*argmnt) {
        if (!isalpha(*argmnt)) {
            return 0; // Not a letter
        }
        argmnt++;
    }
    return 1; // All characters are letters
}
int login(char *name, char *surname) {
    // Convert username and password to uppercase for case-insensitive comparison
    toUpper(name);
    toUpper(surname);
    if(isOnlyAlpha(name) && isOnlyAlpha(surname)){
        return 1;
    }else{
        return 0;
    }
}

void getAccounts(struct Account accounts[], int count, char name[], char surname[], int positions[], int *numMatches) {
    *numMatches = 0; // Initialize the number of matches

    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].name, name) == 0 && strcmp(accounts[i].surname, surname) == 0) {
            // Account found with matching name and surname
            positions[*numMatches] = i; // Store the position
            (*numMatches)++; // Increment the count of matches
        }
    }
}

void displayAccounts(struct Account accounts[], int numMatches, int positions[]) {
    printf("Your accounts:\n");
    for (int i = 0; i < numMatches; i++) {
        int pos = positions[i];
        printf("Account %d:", i + 1);
        printf(" Name: %s %s", accounts[pos].name, accounts[pos].surname);
        printf(" IBAN: %s", accounts[pos].IBAN);
        printf(" Coin: %s", accounts[pos].coin);
        printf(" Amount: %d", accounts[pos].amount);
        printf("\n");
    }
}
// Function to edit an account
void editAccount(struct Account accounts[], int count,int numMatches, int positions[]) {
    char IBAN[34];
    printf("Enter the IBAN of the account you want to edit: ");
    scanf("%s", IBAN);

    for (int i = 0; i < numMatches; i++) {
        int pos = positions[i];
        if (strcmp(IBAN, accounts[pos].IBAN) == 0) {
            // Account found, allow editing
            char temp_IBAN[34];
            printf("Enter new IBAN: ");
            scanf("%s",&temp_IBAN);
            toUpper(temp_IBAN);
            int ok = 1;
            for(int i = 0; i < count; i++){
                if(strcmp(temp_IBAN,accounts[i].IBAN) == 0){
                    ok = 0;
                    break;
                }
            }
            if(ok == 1){
                strcpy(accounts[pos].IBAN,temp_IBAN);
            }else{
                printf("IBAN could not be changed!\n");
            }
            printf("Enter new amount: ");
            scanf("%d", &accounts[pos].amount);
            printf("Enter new coin: ");
            scanf("%s", accounts[pos].coin);
            toUpper(accounts[pos].coin);
            printf("Account edited successfully.\n");
            return;
        }
    }
    printf("Account not found.\n");
}

// Function to delete an account
void deleteAccount(struct Account accounts[], int *count) {
    char IBAN[34];
    printf("Enter the IBAN of the account you want to delete: ");
    scanf("%s", IBAN);

    for (int i = 0; i < *count; i++) {
        if (strcmp(IBAN, accounts[i].IBAN) == 0) {
            // Account found, remove it
            for (int j = i; j < *count - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            (*count)--;
            printf("Account deleted successfully.\n");
            return;
        }
    }
    printf("Account not found.\n");
}

void createAccount(struct Account accounts[], int *count,int *numMatches,int positions[]) {
    if (*count >= 100) {
        printf("Maximum number of accounts reached.\n");
        return;
    }

    printf("Enter name: ");
    scanf("%s", accounts[*count].name);
    toUpper(accounts[*count].name);

    printf("Enter surname: ");
    scanf("%s", accounts[*count].surname);
    toUpper(accounts[*count].surname);

    char temp_IBAN[34];
    printf("Enter IBAN: ");
    scanf("%s", &temp_IBAN);
    toUpper(temp_IBAN);
    for(int i = 0; i < *count; i++){
        if(strcmp(temp_IBAN,accounts[i].IBAN) == 0){
            printf("Invalid IBAN! Couldn't create new account!");
            return;
        }
    }
    strcpy(accounts[*count].IBAN,temp_IBAN);

    printf("Enter coin: ");
    scanf("%s", accounts[*count].coin);
    toUpper(accounts[*count].coin);

    printf("Enter amount: ");
    scanf("%d", &accounts[*count].amount);
    positions[*numMatches] = *count;
    (*count)++;
    (*numMatches)++;
}

// Function to view account data
void viewAccount(struct Account accounts[], int count, int numMatches, int positions[]) {
    char IBAN[34];
    printf("Enter the IBAN of the account you want to view: ");
    scanf("%s", IBAN);
    toUpper(IBAN);
    for (int i = 0; i < numMatches; i++) {
        int pos = positions[i];
        if (strcmp(IBAN, accounts[pos].IBAN) == 0) {
            // Account found, display its data
            printf("Name: %s %s", accounts[pos].name, accounts[pos].surname);
            printf(" IBAN: %s", accounts[pos].IBAN);
            printf(" Coin: %s", accounts[pos].coin);
            printf(" Amount: %d\n", accounts[pos].amount);
            return;
        }
    }
    printf("Account not found.\n");
}

void convertCurrency(char sourceCoin[],char destCoin[], int *amount){
    if(strcmp(sourceCoin,"LEI") == 0 && strcmp(destCoin,"EUR") == 0){
        *amount = *amount / 5;
    }
    if(strcmp(sourceCoin,"LEI") == 0 && strcmp(destCoin,"DOL") == 0){
        *amount = *amount / 5;
    }
    if(strcmp(sourceCoin,"LEI") == 0 && strcmp(destCoin,"GBP") == 0){
        *amount = *amount / 6;
    }
    if(strcmp(sourceCoin,"DOL") == 0 && strcmp(destCoin,"LEI") == 0){
        *amount = *amount * 5;
    }
    if(strcmp(sourceCoin,"GBP") == 0 && strcmp(destCoin,"LEI") == 0){
        *amount = *amount * 6;
    }
}

// Function to perform transactions
void performTransactions(struct Account accounts[], int count,int numMathces,int positions[]) {
    char sourceIBAN[34], destIBAN[34];
    int sourceIndex, destIndex;
    int amount;

    printf("Enter source IBAN: ");
    scanf("%s", sourceIBAN);
    toUpper(sourceIBAN);
    int ok = 0;
    for(int i = 0; i < numMathces; i++){
        int pos = positions[i];
        if(strcmp(sourceIBAN,accounts[pos].IBAN) == 0){
            ok = 1;
            sourceIndex = pos;
            break;
        }
    }
    if(ok == 0){
        printf("You do not have access to this account!");
        return;
    }
    printf("Enter destination IBAN: ");
    scanf("%s", destIBAN);
    toUpper(destIBAN);
    ok = 0;
    for(int i = 0; i < count; i++){
        if(strcmp(destIBAN,accounts[i].IBAN) == 0){
            ok = 1;
            destIndex = i;
            break;
        }
    }
    if(ok == 0){
        printf("This account does not exist!");
        return;
    }
    printf("Enter amount: ");
    scanf("%d", &amount);

    // Check if the source account has enough balance
    if (accounts[sourceIndex].amount < amount) {
        printf("Insufficient balance in the source account.\n");
        return;
    }

    // Perform transaction
    accounts[sourceIndex].amount -= amount;
    convertCurrency(accounts[sourceIndex].coin,accounts[destIndex].coin,&amount);
    accounts[destIndex].amount += amount;

    printf("Transaction successful.\n");
}

// Function to display the main menu
void displayMenu() {
    printf("\n\t\t:: Bank Management System ::\n");
    printf("\t1. Edit Account\n");
    printf("\t2. Delete Account\n");
    printf("\t3. Create Account\n");
    printf("\t4. View Account\n");
    printf("\t5. Perform Transactions\n");
    printf("\t6. Exit\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s login [NAME] [SURNAME]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "login") != 0) {
        printf("Invalid command\n");
        return 1;
    }


    if (!login(argv[2], argv[3])) {
        printf("Invalid username\n");
        return 1;
    }

    struct Account accounts[MAX_RECORDS];
    int numAccounts = 0;
    readFromCSV("accounts.csv", accounts, &numAccounts);
    int positions[100], numMatches = 0;
    getAccounts(accounts,numAccounts,argv[2],argv[3],positions,&numMatches);
    int choice;
    do {
        displayMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAccounts(accounts,numMatches,positions);
                editAccount(accounts, numAccounts,numMatches,positions);
                break;
            case 2:
                displayAccounts(accounts,numMatches,positions);
                deleteAccount(accounts, &numAccounts);
                break;
            case 3:
                createAccount(accounts, &numAccounts,&numMatches,positions);
                break;
            case 4:
                viewAccount(accounts, numAccounts,numMatches,positions);
                break;
            case 5:
                performTransactions(accounts, numAccounts,numMatches,positions);
                break;
            case 6:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice\n");
        }

    } while (choice != 6);

    writeToCSV("accounts.csv", accounts, numAccounts);

    return 0;
}
