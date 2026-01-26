#include <stdio.h> // Standard I/O library
#include <stdlib.h> // Standard library for memory allocation, process control, conversions, etc.

/* Function to book a ticket */
void bookTicket() {
    FILE *fp;
    int ticketNo, seatNo;
    char name[50], train[50];

    fp = fopen("tickets.txt", "a");
    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Ticket Number: ");
    scanf("%d", &ticketNo);

    printf("Enter Passenger Name: ");
    scanf("%s", name);

    printf("Enter Train Name: ");
    scanf("%s", train);

    printf("Enter Seat Number: ");
    scanf("%d", &seatNo);

    fprintf(fp, "%d %s %s %d\n", ticketNo, name, train, seatNo);
    fclose(fp);

    printf("\n✅ Ticket Booked Successfully!\n");
}

/* Function to view all tickets */
void viewTickets() {
    FILE *fp;
    int ticketNo, seatNo;
    char name[50], train[50];

    fp = fopen("tickets.txt", "r");
    if (fp == NULL) {
        printf("\nNo tickets found.\n");
        return;
    }

    printf("\n🎫 BOOKED TICKETS\n");
    printf("----------------------------\n");

    while (fscanf(fp, "%d %s %s %d",
                  &ticketNo, name, train, &seatNo) != EOF) {
        printf("Ticket No : %d\n", ticketNo);
        printf("Name      : %s\n", name);
        printf("Train     : %s\n", train);
        printf("Seat No   : %d\n", seatNo);
        printf("----------------------------\n");
    }

    fclose(fp);
}

/* Function to cancel a ticket */
void cancelTicket() {
    FILE *fp, *temp;
    int ticketNo, seatNo, cancelNo, found = 0;
    char name[50], train[50];

    fp = fopen("tickets.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL) {
        printf("\nNo tickets to cancel.\n");
        return;
    }

    printf("Enter Ticket Number to Cancel: ");
    scanf("%d", &cancelNo);

    while (fscanf(fp, "%d %s %s %d",
                  &ticketNo, name, train, &seatNo) != EOF) {
        if (ticketNo == cancelNo) {
            found = 1;
        } else {
            fprintf(temp, "%d %s %s %d\n",
                    ticketNo, name, train, seatNo);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("tickets.txt");
    rename("temp.txt", "tickets.txt");

    if (found)
        printf("\n❌ Ticket Cancelled Successfully!\n");
    else
        printf("\nTicket not found!\n");
}

/* Main function */
int main() {
    int choice;

    do {
        printf("\n RAILWAY MANAGEMENT SYSTEM\n");
        printf("1. Book Ticket\n");
        printf("2. View Tickets\n");
        printf("3. Cancel Ticket\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                bookTicket();
                break;
            case 2:
                viewTickets();
                break;
            case 3:
                cancelTicket();
                break;
            case 4:
                printf("👋 Exiting Program...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 4);

    return 0;
}
