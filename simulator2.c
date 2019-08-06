#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define ARR_SIZE 8
#define CAR_NUM 50
#define PARK_NUM 5

//Time struct
typedef struct {
    int hour;
    int minute;
}Time;

//Car struct
typedef struct {
    char *plateNumber;  // a plateNumber for a car
    char permit;        // '1' for has a permit, '0' for no permit.
    Time enteringTime;  // the time that the car entered the lot
    int lotParkedIn;    // thenumber of the lot that the car is parked in.
}Car;

//ParkingLot struct
typedef struct {
    int lotNumber;      //unique ID of the parking lot
    double hourlyRate;  //the park lot fee (per hour)
    double maxCharge;   // maximum amount of a lot for the day
    int capacity;       //the maximum number of cars that can park in that lot
    int currentCarCount;//number of cars currently parked in the lot
    double revenue;     // keeps track of the amount of money taken in by that lot so far.
}ParkingLot;

// Sets the hours and minutes amount for the given time t based
// on the specified hours h.  (e.g., 1.25 hours would be 1 hour
// and 15 minutes)
void setHours(Time *t, double h) {
    t->hour = (int)h;
    t->minute = (int)(60*(h-(int)h));
//    printf("%.2f hours = %2d:%02d\n",h,t->hour,t->minute);
}

// Takes two Time objects (not pointers) and computes the difference
// in time from t1 to t2 and then stores that difference in the diff
// Time (which must be a pointer)
void difference(Time t1, Time t2, Time *diff) {
    double time1 = t1.hour + ((double)t1.minute/60);
    double time2 = t2.hour + ((double)t2.minute/60);
    double time3 = time2 - time1;
    setHours(diff,time3);

}

// Initialize the car pointed to by c to have the given plate and
// hasPermit status.  The car should have it’s lotParkedIn set to
// 0 and enteringTime to be -1 hours and -1 minutes.
void initializeCar(Car *c, char *plate, int hasPermit) {
    c->plateNumber = plate;
    c->permit = '0' + hasPermit;
    c->lotParkedIn = 0;
    c->enteringTime.hour = -1;
    c->enteringTime.minute = -1;
}


// Initialize the lot pointed to by p to have the given number,
// capacity, hourly rate and max charge values.  The currentCarCount
// and revenue should be at 0.
void initializeLot(ParkingLot *p, int num, int cap, double rate, double max) {
    p->lotNumber = num;
    p->capacity = cap;
    p->hourlyRate = rate;
    p->maxCharge = max;
    p->currentCarCount = 0;
    p->revenue = 0;
}


// Print out the parking lot parameters so that is displays as
// follows:   Parking Lot #2 - rate = $3.00, capacity 6, current cars 5
void printLotInfo(ParkingLot p) {
    printf("Parking Lot #%d - rate = $%.2f, capacity %d, current cars %d\n",p.lotNumber,p.hourlyRate,p.capacity,p.currentCarCount);
}

// Simulate a car entering the parking lot
// ...
void carEnters(ParkingLot *p, Car *c,int h, int m){
    // determine the park lot is not full
    if(p->capacity > p->currentCarCount){
        p->currentCarCount++;
        c->lotParkedIn = p->lotNumber;
        c->enteringTime.hour =h;
        c->enteringTime.minute = m;
        printf("Car %s enters Lot %d at %2d:%02d.\n",c->plateNumber,c->lotParkedIn,c->enteringTime.hour,c->enteringTime.minute);
    }else{
        printf("Car %s arrives at Lot %d at %2d:%02d, but the lot is full.\n",c->plateNumber,p->lotNumber,h,m);
        printf("Car %s cannot get in.\n",c->plateNumber);
    }
}

// Simulate a car leaving the parking lot
// ...
void carLeaves(ParkingLot *p, Car *c, int h, int m){
    if(c->lotParkedIn == p->lotNumber){
        double price = 0;
        Time time1,time2,time3;
        time1 = c->enteringTime;
        time2.hour = h;
        time2.minute = m;
        time3.hour = 0;
        time3.minute = 0;
        difference(time1,time2, &time3);
        int parkHours = time3.hour;
        if(time3.minute > 0){
            parkHours++;
        }
    
        if(c->permit == '0'){
            double amount = p -> hourlyRate;
            price = amount * parkHours;
            if(price > p->maxCharge){
                price = p->maxCharge;
            }
            printf("Car %s leaves Lot %d at %2d:%02d paid $%.2f.\n",
                   c->plateNumber,c->lotParkedIn,h,m,price);
            }
       if(c->permit == '1'){
            printf("Car %s leaves Lot %d at %2d:%02d.\n",
                   c->plateNumber,c->lotParkedIn,h,m);
            c->permit = 0;
        }

        p->revenue += price;
        p->currentCarCount--;
        c->lotParkedIn = 0;
        c->enteringTime.hour =0;
        c->enteringTime.minute =0;
    }

}
//The function should generate a random license plate
//with the format "XXX ###" where X is a random character from A to Z
//and # is a random digit from 0 to 9. T
//function should generate a random license plate with the format "XXX ###" where X is a
//random character from A to Z and # is a random digit from 0 to 9. The random string must be
//dynamically-allocated and a pointer to this string should be returned.

char* randomPlate(){
    char *plateNumber = malloc(ARR_SIZE * sizeof(char));
    int i = 0;
    while (i < 7){
        if (i < 3 ) plateNumber[i] = 'A' + (rand()%26);
        if (i == 3) plateNumber[i] = ' ';
        if (i > 3 ) plateNumber[i] = '0'+ (rand()%10);
//                printf("%c",plateNumber[i]);//for debug
        i++;
    }
    plateNumber[i]='\0';
//    printf("\n%s\n",plateNumber);//for debug
    return plateNumber;
}

Car* randomCar(){
    Car *c = malloc(sizeof(Car));
    int permit = rand() % 2;
    initializeCar(c,randomPlate(),permit);
    return c;

}

void printCarInfo(Car *c){
    printf("Car %s ",c->plateNumber);
    if(c->lotParkedIn != -1 ){
        printf("Parking in Lot %d at %2d:%02d",c->lotParkedIn,c->enteringTime.hour,c->enteringTime.minute);
    }
    if(c->permit == '1'){
        printf("with permit.\n");
    }else
        printf("without permit.\n");
}

int main() {
    srand(time(NULL));
    int i,j;
    //1. Make an array to hold pointers to 50 Cars and an array to hold pointers to 5 ParkingLots
//    int carNum = 50, parkNum = 5;
    Car *cars[CAR_NUM];//an array to hold pointers to 50 Cars
    ParkingLot *parks[PARK_NUM];//an array to hold pointers to 5 parkingLots
    //2. Set up 50 cars
    for(i = 0;i < CAR_NUM; ++i){
        cars[i] = randomCar();
    }
    printf("\n");

    
    // display 50 cars
    for(i = 0;i < CAR_NUM; ++i)
    {
        printf("Car %s with permit %c\n",
               cars[i]->plateNumber,cars[i]->permit);
    
    }
    printf("\n");
    
    // 3. Set up 5 parking lots
    for(j = 0;j < PARK_NUM; ++j){
        parks[j] = malloc(sizeof(ParkingLot));
        int num = j+1;
        int cap = 5 * (j+1);
        double rate = (double)(4+j);
        double max = (double)(2*(6+j));
        initializeLot(parks[j],num,cap,rate,max);
//        printf("inited\n");
    }
    
    //4. Display the lot info for all parkingLot.
    for(j = 0;j < PARK_NUM; ++j){// Set up 5 parking lots
        printLotInfo(*parks[j]);
    }
    printf("\n");
    

    // 5. Simulate cars entering the lots
    Time enterTime;//starting at 6am.
    enterTime.hour = 6;
    enterTime.minute = 0;
    
    for(i = 0; i < CAR_NUM; ++i)
    {
        //enter a randomly-chosen lot
        j = rand() % 5;
        carEnters(parks[j],cars[i],enterTime.hour,enterTime.minute);
        enterTime.minute += 5;//every 5 minutes
        if(enterTime.minute >= 60){
            enterTime.hour++;
            enterTime.minute -= 60;
        }
    }
    printf("\n");
    
    //6.Display the lot info again for all parking lots.
    for(i = 0;i<PARK_NUM;++i)
    {
        printLotInfo(*parks[i]);
    }
    printf("\n");

    //7. Simulate all parked cars trying to leave the lots
    Time leaveTime;//starting at 11am.
    leaveTime.hour = 11;
    leaveTime.minute = 0;
    
    for(i = 0; i < CAR_NUM ; ++i)
    {
//        printCarInfo(cars[i]);
        if(cars[i]->lotParkedIn != 0){
            j = cars[i]->lotParkedIn - 1;
            carLeaves(parks[j],cars[i],leaveTime.hour,leaveTime.minute);
            leaveTime.minute += 5;//every 5 minutes
            if(leaveTime.minute >= 60){
                leaveTime.hour++;
                leaveTime.minute -= 60;
            }
            
        }
    }
    printf("\n");
    
    //8. Display the lot info again for all parking lots.
    for(i = 0;i < PARK_NUM;++i)
    {
        printLotInfo(*parks[i]);
    }
    printf("\n");
    
    //9. Display the revenue for all parking lots.
    for(j = 0 ; j<PARK_NUM;++j){
        printf("Total revenue of Lot %d is $%4.2f\n",j+1, parks[j]->revenue);
    }
    printf("\n");

    //10. Free up ALL memory that you had dynamically allocated

    for(i = 0; i < CAR_NUM; ++i ){
        free(cars[i]->plateNumber);
        free(cars[i]);
    }
    for(j = 0; j < PARK_NUM; ++j ){
        free(parks[j]);
    }

    return 0;
}



