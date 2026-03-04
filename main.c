#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEATS 50
#define TICKET_FILE "tickets.txt"

struct Train {
    int id;
    char name[50];
    char from[30];
    char to[30];
    int total_seats;
    float fare;
    char departure[10];
};

struct Ticket {
    int booking_id;
    int train_id;
    char passenger_name[50];
    int age;
    char gender;
    int seat_number;
    char status[15];
    char emergency_contact[15];
};

char seat_map[MAX_SEATS + 1];

void clearScreen();
void loadSeatMap(int train_id);
int generateBookingID();
int isWindowSeat(int seat);
int isSandwiched(int seat);
int allocateSeat(char gender, int age);
void viewTrains();
struct Train* findTrain(int id);
void bookTicket();
void viewBookings();
void cancelTicket();
void searchTrain();
void showSeatMap();

struct Train trains[5] = {
    {101,"Rajdhani Express","Delhi","Mumbai",50,850.0,"06:00"},
    {102,"Shatabdi Express","Chennai","Bangalore",50,450.0,"07:30"},
    {103,"Duronto Express","Kolkata","Delhi",50,700.0,"22:00"},
    {104,"Garib Rath","Mumbai","Pune",50,120.0,"09:15"},
    {105,"Jan Shatabdi","Delhi","Agra",50,200.0,"08:00"}
};

int trainCount = 5;

int main() {
    int choice;

    do {
        printf("\n=============================================\n");
        printf("      RAILWAY MANAGEMENT SYSTEM\n");
        printf("=============================================\n");
        printf("1. View All Trains\n");
        printf("2. Search Train\n");
        printf("3. Book Ticket\n");
        printf("4. View Bookings\n");
        printf("5. Cancel Ticket\n");
        printf("6. View Seat Map\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice) {
            case 1: viewTrains(); break;
            case 2: searchTrain(); break;
            case 3: bookTicket(); break;
            case 4: viewBookings(); break;
            case 5: cancelTicket(); break;
            case 6: showSeatMap(); break;
            case 7: printf("\nThank you.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while(choice != 7);

    return 0;
}

void clearScreen() {
    printf("\n--------------------------------------------------\n");
}

void loadSeatMap(int train_id) {
    int i;
    for(i=1;i<=MAX_SEATS;i++) seat_map[i]='0';

    FILE *fp=fopen(TICKET_FILE,"r");
    if(fp==NULL) return;

    struct Ticket t;
    while(fscanf(fp,"%d %d %s %d %c %d %s %s",
        &t.booking_id,&t.train_id,t.passenger_name,
        &t.age,&t.gender,&t.seat_number,
        t.status,t.emergency_contact)==8) {

        if(t.train_id==train_id &&
           strcmp(t.status,"CONFIRMED")==0 &&
           t.seat_number>=1 && t.seat_number<=MAX_SEATS) {
            seat_map[t.seat_number]=t.gender;
        }
    }
    fclose(fp);
}

int generateBookingID() {
    int count=1001;
    FILE *fp=fopen(TICKET_FILE,"r");
    if(fp==NULL) return count;
    char line[200];
    while(fgets(line,sizeof(line),fp)) count++;
    fclose(fp);
    return count;
}

int isWindowSeat(int seat) {
    return (seat%3==1);
}

int isSandwiched(int seat) {
    if(seat<=1 || seat>=MAX_SEATS) return 0;
    char left=seat_map[seat-1];
    char right=seat_map[seat+1];
    int leftF=(left=='F'||left=='C');
    int rightF=(right=='F'||right=='C');
    return(leftF && rightF);
}

int allocateSeat(char gender,int age) {
    int seat;

    if(age<12) {
        for(seat=1;seat<=10;seat++)
            if(seat_map[seat]=='0') return seat;
    }

    if(gender=='F') {
        for(seat=1;seat<=MAX_SEATS;seat++)
            if(seat_map[seat]=='0') return seat;
    }

    if(gender=='M') {
        for(seat=1;seat<=MAX_SEATS;seat++)
            if(seat_map[seat]=='0' && !isSandwiched(seat)) return seat;
    }

    return -1;
}

void viewTrains() {
    int i;
    clearScreen();
    printf("ID   Name                 From       To         Fare   Dep\n");
    for(i=0;i<trainCount;i++) {
        printf("%-4d %-20s %-10s %-10s %-6.0f %s\n",
            trains[i].id,trains[i].name,
            trains[i].from,trains[i].to,
            trains[i].fare,trains[i].departure);
    }
}

struct Train* findTrain(int id) {
    int i;
    for(i=0;i<trainCount;i++)
        if(trains[i].id==id) return &trains[i];
    return NULL;
}

void bookTicket() {
    int train_id;
    struct Ticket t;

    viewTrains();
    printf("\nEnter Train ID: ");
    scanf("%d",&train_id);

    struct Train *tr=findTrain(train_id);
    if(tr==NULL) {
        printf("Train not found.\n");
        return;
    }

    t.train_id=train_id;
    t.booking_id=generateBookingID();
    strcpy(t.status,"CONFIRMED");

    printf("Passenger Name: ");
    scanf("%s",t.passenger_name);

    printf("Age: ");
    scanf("%d",&t.age);

    if(t.age<12) {
        t.gender='C';
        printf("Emergency Contact: ");
        scanf("%s",t.emergency_contact);
    } else {
        printf("Gender (M/F): ");
        scanf(" %c",&t.gender);
        strcpy(t.emergency_contact,"N/A");
    }

    loadSeatMap(train_id);
    t.seat_number=allocateSeat(t.gender,t.age);

    if(t.seat_number==-1) {
        printf("No seats available.\n");
        return;
    }

    FILE *fp=fopen(TICKET_FILE,"a");
    fprintf(fp,"%d %d %s %d %c %d %s %s\n",
        t.booking_id,t.train_id,t.passenger_name,
        t.age,t.gender,t.seat_number,
        t.status,t.emergency_contact);
    fclose(fp);

    printf("\nTicket Confirmed. Seat No: %d\n",t.seat_number);
}

void viewBookings() {
    FILE *fp=fopen(TICKET_FILE,"r");
    if(fp==NULL) {
        printf("No bookings found.\n");
        return;
    }

    struct Ticket t;
    while(fscanf(fp,"%d %d %s %d %c %d %s %s",
        &t.booking_id,&t.train_id,t.passenger_name,
        &t.age,&t.gender,&t.seat_number,
        t.status,t.emergency_contact)==8) {

        printf("%d | %s | Train:%d | Seat:%d | %s\n",
            t.booking_id,t.passenger_name,
            t.train_id,t.seat_number,t.status);
    }
    fclose(fp);
}

void cancelTicket() {
    int cancel_id,found=0,i,count=0;
    struct Ticket all[200];

    printf("Enter Booking ID: ");
    scanf("%d",&cancel_id);

    FILE *fp=fopen(TICKET_FILE,"r");
    if(fp==NULL) return;

    while(fscanf(fp,"%d %d %s %d %c %d %s %s",
        &all[count].booking_id,&all[count].train_id,
        all[count].passenger_name,&all[count].age,
        &all[count].gender,&all[count].seat_number,
        all[count].status,all[count].emergency_contact)==8) {

        if(all[count].booking_id==cancel_id) {
            strcpy(all[count].status,"CANCELLED");
            found=1;
        }
        count++;
    }
    fclose(fp);

    fp=fopen(TICKET_FILE,"w");
    for(i=0;i<count;i++) {
        fprintf(fp,"%d %d %s %d %c %d %s %s\n",
            all[i].booking_id,all[i].train_id,
            all[i].passenger_name,all[i].age,
            all[i].gender,all[i].seat_number,
            all[i].status,all[i].emergency_contact);
    }
    fclose(fp);

    if(found) printf("Ticket Cancelled.\n");
    else printf("Booking ID not found.\n");
}

void searchTrain() {
    char from[30],to[30];
    int i,found=0;

    printf("From: ");
    scanf("%s",from);
    printf("To: ");
    scanf("%s",to);

    for(i=0;i<trainCount;i++) {
        if(strcmp(trains[i].from,from)==0 &&
           strcmp(trains[i].to,to)==0) {
            printf("Train Found: %d %s Fare: %.0f\n",
                trains[i].id,trains[i].name,trains[i].fare);
            found=1;
        }
    }

    if(!found) printf("No train found.\n");
}

void showSeatMap() {
    int tid,i;
    printf("Enter Train ID: ");
    scanf("%d",&tid);

    loadSeatMap(tid);

    printf("\nSeat Map (Horizontal View)\n\n");

    for(i=1;i<=MAX_SEATS;i++) {
        if(seat_map[i]=='0')
            printf("[%2d:  ] ",i);
        else
            printf("[%2d:%c ] ",i,seat_map[i]);

        if(i%10==0) printf("\n");
    }
    printf("\n");
}