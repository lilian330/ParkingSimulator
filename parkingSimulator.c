#include <stdio.h>
#include <math.h>
#include <string.h>

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
//    printf("%.2f hours = %2d:%02d\n",h,t->hour,t->minute);//for debug
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
    c->permit = '0'+ hasPermit;
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
    if(p->lotNumber == c->lotParkedIn){
        Time time1 = c->enteringTime;
        Time time2;
        time2.hour = h;
        time2.minute = m;
        Time time3;
        time3.hour=0;
        time3.minute=0;
        difference(time1,time2, &time3);
        
        int parkHours = time3.hour;
        if(time3.minute > 0){
            parkHours++;
        }
        double price = 0;
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
            c->permit = '0';
        }
        
        p->revenue += price;
        p->currentCarCount--;
    }

}


int main() {
    Car  car1, car2, car3, car4, car5, car6, car7, car8, car9;
    ParkingLot p1, p2;
    
    // Set up 9 cars
    initializeCar(&car1, "ABC 123", 0);
    initializeCar(&car2, "ABC 124", 0);
    initializeCar(&car3, "ABD 314", 0);
    initializeCar(&car4, "ADE 901", 0);
    initializeCar(&car5, "AFR 304", 0);
    initializeCar(&car6, "AGD 888", 0);
    initializeCar(&car7, "AAA 111", 0);
    initializeCar(&car8, "ABB 001", 0);
    initializeCar(&car9, "XYZ 678", 1);
    
    // Set up two parking lots
    initializeLot(&p1, 1, 4, 5.5, 20.0);
    initializeLot(&p2, 2, 6, 3.0, 12.0);
    
    printLotInfo(p1);
    printLotInfo(p2);
    printf("\n");
    
    // Simulate cars entering the lots
    carEnters(&p1, &car1, 7, 15);
    carEnters(&p1, &car2, 7, 25);
    carEnters(&p2, &car3, 8,  0);
    carEnters(&p2, &car4, 8, 10);
    carEnters(&p1, &car5, 8, 15);
    carEnters(&p1, &car6, 8, 20);
    carEnters(&p1, &car7, 8, 30);
    carEnters(&p2, &car7, 8, 32);
    carEnters(&p2, &car8, 8, 50);
    carEnters(&p2, &car9, 8, 55);
    
    printf("\n");
    printLotInfo(p1);
    printLotInfo(p2);
    printf("\n");
    
    // Simulate cars leaving the lots
    carLeaves(&p2, &car4, 9, 0);
    carLeaves(&p1, &car2, 9, 5);
    carLeaves(&p1, &car6, 10, 0);
    carLeaves(&p1, &car1, 10, 30);
    carLeaves(&p2, &car8, 13, 0);
    carLeaves(&p2, &car9, 15, 15);
    carEnters(&p1, &car8, 17, 10);
    carLeaves(&p1, &car5, 17, 50);
    carLeaves(&p2, &car7, 18, 0);
    carLeaves(&p2, &car3, 18, 15);
    carLeaves(&p1, &car8, 20, 55);
    
    printf("\n");
    printLotInfo(p1);
    printLotInfo(p2);
    printf("\n");
    
    // Display the total revenue
    printf("Total revenue of Lot 1 is $%4.2f\n", p1.revenue);
    printf("Total revenue of Lot 2 is $%4.2f\n", p2.revenue);
    
    return 0;
}

