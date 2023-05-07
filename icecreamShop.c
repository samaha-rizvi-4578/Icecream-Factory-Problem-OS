// 21k-4578 21k-4897 21k-3301
// Ice-Cream Factory Problem
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

// defined prices are in dollars
#define SMALL 5.05
#define MEDIUM 6.55
#define LARGE 8.75
#define CHOCOLATE 2.00
#define VANILLA 2.00
#define STRAWBERRY 2.00
#define CHOCOLATECHIP 1.58
#define SPRINKLES 0.98
#define CUP 2.24
#define CONE 1.12

// struct to store details
typedef struct
{
    int id;
    char *flav;
    char *top;
    char *size;
    char *choice;
    double price;
} icecream;

// quantity of resources
int tickets = 50;
int sizes[3] = {25, 20, 15};
int flavors[3] = {31, 28, 24};
int toppings[2] = {26, 28};
int choices[2] = {23, 30};
double revenue = 0.0;
int cust_count = 0;
int count = 0;

// semaphore declaration
sem_t ticket, size, small, medium, large, flavor, choc, van, straw, topping, chocChip, sprin, ch, cup, cone, payment;

// functions prototypes
void displayMenu();
void *takeOrders(void *arg);
void deliverIcecream(icecream *ID, long int tid);
void *processOrders(void *arg);
void customerData(icecream *ID);
void invoice();

//-----------------------------------------------------------------------

// main driver function
int main()
{
    srand(time(NULL));
    int N;
    int choice;
    printf("********WELCOME TO SLS ICE-CREAM PARLOUR********\n\n");
    do
    {
        choice = 0;
        printf("Enter Number of Cones you want [1 - %d]:  ", tickets);
        scanf("%d", &N);
        fflush(stdin);

        // condition to check valid input
        if (N > tickets || N <= 0)
        {
            printf("\nWarning: Invalid input\nDo you want to enter again? (1 for yes): ");
            fflush(stdin);
            scanf("%d", &choice);
            fflush(stdin);
        }

    } while (choice == 1);

    // display menu
    displayMenu();

    // declare the struct of size N
    icecream c[N];

    // get the flavors and toppings of customers choice
    // multi threading to take the customization  order from customer
    pthread_t th[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&th[i], NULL, &takeOrders, (void *)&c[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(th[i], NULL);
    }

    // semaphore initialization
    sem_init(&ticket, 0, 1); // restrict the acces of ticket to 1 person at a time
    sem_init(&size, 0, 3);   // restrict to 3 sizes at atime
    sem_init(&small, 0, 1);  // access for small
    sem_init(&medium, 0, 1); // access for medium
    sem_init(&large, 0, 1);  // access for large

    sem_init(&flavor, 0, 3); // restrict to 3 flavors at a time
    sem_init(&choc, 0, 1);   // access of chocolate flavor
    sem_init(&van, 0, 1);    // access for vanilla flavor
    sem_init(&straw, 0, 1);  // access for strawberry flavor

    sem_init(&topping, 0, 2);  // restrict to 2 toppings at a time
    sem_init(&chocChip, 0, 1); // access for chocolate chip topping
    sem_init(&sprin, 0, 1);    // access for sprinkles

    sem_init(&ch, 0, 2);   // restrict to 2 choices at a time
    sem_init(&cup, 0, 1);  // access for cup
    sem_init(&cone, 0, 1); // access for cone

    sem_init(&payment, 0, 1); // restrict the access of total sales

    printf("*****Ice-Cream Making Process Started*********\n\n");
    // multithreading to proccess order
    pthread_t cust[N];
    for (int i = 0; i < N; i++)
    {
        pthread_create(&cust[i], NULL, &processOrders, (void *)&c[i]);
        sleep(1);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(cust[i], NULL);
    }

    // Print shop journal at closing
    invoice();

    // destroy semaphores
    sem_destroy(&ticket);

    sem_destroy(&size);
    sem_destroy(&small);
    sem_destroy(&medium);
    sem_destroy(&large);

    sem_destroy(&flavor);
    sem_destroy(&choc);
    sem_destroy(&van);
    sem_destroy(&straw);

    sem_destroy(&topping);
    sem_destroy(&chocChip);
    sem_destroy(&sprin);

    sem_destroy(&ch);
    sem_destroy(&cup);
    sem_destroy(&cone);

    sem_destroy(&payment);
    return 0;
}
//----------------------------------------------------------

// fucntion to display menu
void displayMenu()
{
    printf("\n\n********MENU**********\n\n\n");
    printf("SIZE      || FLAVOURS          || TOPPINGS         || CHOICES\n");
    printf("__________________________________________________________________\n\n");
    printf("1.Small   || 1.Chocolate       || 1.chocolateChips || 1.cup\n");
    printf("2.Medium  || 2.Vanilla         || 2.sprinkles      || 2.cone\n");
    printf("3.Large   || 3.Strawberry\n\n\n");
}
// take orders
//-----------------------------------------------------------
void *takeOrders(void *arg)
{
    int a;
    icecream *c = (icecream *)arg;
    long int threadID = pthread_self();
    cust_count++;
    c->id = cust_count;
    fflush(stdin);
    printf("\n---Worker Thread %ld taking order---\n", threadID);
    fflush(stdin);
    printf("choose size for icecream %d: ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->size = "small";
    else if (a == 2)
        c->size = "medium";
    else if (a == 3)
        c->size = "large";
    fflush(stdin);
    printf("choose Flavor for icecream %d: ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->flav = "chocolate";
    else if (a == 2)
        c->flav = "vanilla";
    else if (a == 3)
        c->flav = "strawberry";
    fflush(stdin);
    printf("choose toppings for icecream %d: ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->top = "chocolateChip";
    else if (a == 2)
        c->top = "sprinkles";
    fflush(stdin);
    printf("choose choice for icecream %d: ", c->id);
    scanf("%d", &a);
    if (a == 1)
        c->choice = "cup";
    else if (a == 2)
        c->choice = "cone";
    fflush(stdin);
    c->price = 0.0;
    printf("\n---Customer [%d] entered into Order Queue\n", c->id);
    printf("\n\n");
    fflush(stdin);
    pthread_exit(NULL);
    sleep(2);
    return NULL;
}
//-------------------------------------------------------------------
//--------------------------------------------------------------

// fucntion to deliver order
void deliverIcecream(icecream *ID, long int tid)
{
    // Simulate delivery with a random time between 01 and 20 minutes
    int time = rand() % 16 + 5;
    // payments counter - ENTER
    printf("\n****Delivering Order with Worker Thread %ld \n\n", tid);
    printf("Delivering %s %s ice cream with %s topping in a %s...\n", ID->size, ID->flav, ID->top, ID->choice);
    printf("Customer %d Billed: $ %.2f.\n", ID->id, ID->price);

    if (time <= 10)
    {
        printf("Order delivered on time!\n");
    }
    else
    {
        printf("Order delivered [lATE BY] %d minutes\n", time);
    }
    count++;
    // payments counter - EXIT
}

// fucntion for icecream making process
void *processOrders(void *arg)
{
    long int threadID = pthread_self();
    icecream *ID = (icecream *)arg;
    int race_1 = 0; // check race condition

    // enter ticket counter
    sem_wait(&ticket);

    if (tickets <= 0)
    {
        printf("\nCustomer [%d] Leaving! [ALERT]: Tickets Finished\n", ID->id);
        syscall(335, "Leaving! [ALERT]: Tickets Finished\n", ID->id);
        sleep(1);
        pthread_exit(NULL);
    }
    // reduce tikcet count
    tickets--;
    printf("\nCustomer [%d]: Ticket Received\n\n", ID->id);
    syscall(335, "Ticket Recieved\n", ID->id);

    sem_post(&ticket);
    // exit ticket counter
    //-----------------------------------------------------------
    sleep(3);

    // enter size counter
    sem_wait(&size);
    if (sizes[0] <= 0 && sizes[1] <= 0 && sizes[2] <= 0)
    {
        printf("\nCustomer [%d] Leaving! [ALERT]: Size Finished\n", ID->id);
        syscall(335, "Leaving! [ALERT]: Size Finished\n", ID->id);
        sleep(1);
        pthread_exit(NULL);
    }
    else
    {
        // check small size availability
        if (ID->size == "small")
        {
            sem_wait(&small);

            if (sizes[0] > 0)
            {
                sizes[0]--;
                race_1++;
                ID->price += SMALL;
                printf("Customer [%d]: Got Size [SMALL]\n", ID->id);
                syscall(335, "Got Size [SMALL].\n", ID->id);
                sleep(1);
            }

            sem_post(&small);
        }
        else if (ID->size == "medium")
        {
            sem_wait(&medium);
            // check availability of medium size
            if (sizes[1] > 0)
            {
                sizes[1]--;
                race_1++;
                ID->price += MEDIUM;
                printf("Customer [%d]: Got Size [MEDIUM]\n", ID->id);
                syscall(335, "Got Size [MEDIUM]\n", ID->id);
                sleep(1);
            }

            sem_post(&medium);
        }
        else if (ID->size == "large")
        {
            sem_wait(&large);
            // check availability of large size
            if (sizes[2] > 0)
            {
                sizes[2]--;
                race_1++;
                ID->price += LARGE;
                printf("Customer [%d]: Got Size [LARGE]\n", ID->id);
                syscall(335, "Got Size [LARGE]\n", ID->id);
                sleep(1);
            }

            sem_post(&large);
        }
        else
        {
            printf("Customer [%d] Leaving! [ALERT]: Wrong Size\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Wrong Size\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }
        // If no flavour is given to thread it will check race condition i.e race_1 and exit
        if (race_1 == 0)
        {
            printf("Customer [%d] Leaving! [ALERT]: Size Finished\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Size Finished\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }
    }
    printf("Customer [%d] Leaving Size Counter: [SUCCESFUL]\n\n", ID->id);
    syscall(335, "Leaving Size Counter [SUCCESFUL]\n", ID->id);
    sem_post(&size);

    // exit size counter
    //----------------------------------------------------------

    race_1 = 0;
    // enter flavour counter
    sem_wait(&flavor);
    if (flavors[0] <= 0 && flavors[1] <= 0 && flavors[2] <= 0)
    {
        printf("\nCustomer [%d] Leaving! [ALERT]: Flavours Finished\n", ID->id);
        syscall(335, "Leaving! [ALert]: Flavours Finished\n", ID->id);
        sleep(1);
        pthread_exit(NULL);
    }
    else
    {
        // check availability of chocolate flavour
        if (ID->flav == "chocolate")
        {
            sem_wait(&choc);

            if (flavors[0] > 0)
            {
                flavors[0]--;
                race_1++;
                ID->price += CHOCOLATE;
                printf("Customer [%d]: Got Flavour [CHOCOLATE]\n", ID->id);
                syscall(335, "Got Flavour [CHOCOLATE]\n", ID->id);
                sleep(1);
            }

            sem_post(&choc);
        }
        else if (ID->flav == "vanilla")
        {
            sem_wait(&van);
            // check availability of vanilla flavour
            if (flavors[1] > 0)
            {
                flavors[1]--;
                race_1++;
                ID->price += VANILLA;
                printf("Customer [%d]: Got Flavour [VANILLA]\n", ID->id);
                syscall(335, "Got Flavour [VANILLA]\n", ID->id);
                sleep(1);
            }

            sem_post(&van);
        }
        else if (ID->flav == "strawberry")
        {
            sem_wait(&straw);
            // check availability of strawberry flavour
            if (flavors[2] > 0)
            {
                flavors[2]--;
                race_1++;
                ID->price += STRAWBERRY;
                printf("Customer [%d]: Got Flavour [STRAWBERRY]\n", ID->id);
                syscall(335, "Got Flavour [STRAWBERRY]\n", ID->id);
                sleep(1);
            }

            sem_post(&straw);
        }
        else
        {
            printf("Customer [%d] Leaving! [ALERT]: Wrong Flavour\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Wrong Flavour\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }
        if (race_1 == 0)
        {
            printf("Customer [%d] Leaving! [ALERT]: Flavour Finished\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Flavours Finished\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }
    }
    printf("Customer [%d] Leaving Flavour Counter!: [SUCCESFUL]\n\n", ID->id);
    syscall(335, "Leaving Flavor Counter [SUCCESSFUL]\n", ID->id);
    sem_post(&flavor);
    // exit flavor counter
    //-------------------------------------
    sleep(2);
    race_1 = 0; // reset race condition checker

    // enter toppings counter
    sem_wait(&topping);
    if (toppings[0] <= 0 && toppings[1] <= 0)
    {
        printf("\nCustomer [%d] Leaving! [ALERT]: Toppings Finished\n", ID->id);
        syscall(335, "Leaving! [ALert]: Toppings Finished\n", ID->id);
        sleep(1);
        pthread_exit(NULL);
    }
    else
    {

        if (ID->top == "chocolateChip")
        {
            sem_wait(&chocChip);
            // check availability of chocolate chip topping
            if (toppings[0] > 0)
            {
                toppings[0]--;
                race_1++;
                ID->price += CHOCOLATECHIP;
                printf("Customer [%d]: Got Toppings [CHOCOLATE CHIPS]\n", ID->id);
                syscall(335, "Got Topping [CHOCOLATE CHIPS]\n", ID->id);
                sleep(1);
            }

            sem_post(&chocChip);
        }
        else if (ID->top == "sprinkles")
        {
            sem_wait(&sprin);
            // check availability of sprinkles topping
            if (toppings[1] > 0)
            {
                toppings[1]--;
                race_1++;
                ID->price += SPRINKLES;
                printf("Customer [%d]: Got Toppings [SPRINKLES]\n", ID->id);
                syscall(335, "Got Topping [SPRINKLES]\n", ID->id);
                sleep(1);
            }

            sem_post(&sprin);
        }
        else
        {
            printf("Customer [%d] Leaving! [ALERT]: Wrong Topping\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Wrong Topping\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }

        if (race_1 == 0)
        {

            printf("Customer [%d] Leaving! [ALERT]: Topping Finished\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Topping Finished\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }
    }
    printf("Customer [%d] Leaving Topping Counter! [SUCCESFUL]\n\n", ID->id);
    syscall(335, "Leaving Topping Counter [SUCCESFULL]\n", ID->id);
    sem_post(&topping);
    // toppings counter - EXIT
    //----------------------------------------------
    sleep(2);

    race_1 = 0;
    // choices counter enter

    sem_wait(&ch);
    if (choices[0] <= 0 && choices[1] <= 0)
    {
        printf("\nCustomer [%d] Leaving! [ALERT]: Choices Finished\n", ID->id);
        syscall(335, "Leaving! [ALERT]: choices Finished\n", ID->id);
        sleep(1);
        pthread_exit(NULL);
    }
    else
    {

        if (ID->choice == "cup")
        {
            sem_wait(&cup);
            // check availability of cup
            if (choices[0] > 0)
            {
                choices[0]--;
                race_1++;
                ID->price += CUP;

                printf("Customer [%d]: Got Choice [CUP]\n", ID->id);
                syscall(335, "Got Choice [CUP].\n", ID->id);
                sleep(1);
            }

            sem_post(&cup);
        }
        else if (ID->choice == "cone")
        {
            sem_wait(&cone);
            // check availability of cone
            if (choices[1] > 0)
            {
                choices[1]--;
                race_1++;
                ID->price += CONE;

                printf("Customer [%d]: Got Choice [CONE]\n", ID->id);
                syscall(335, "Got Choice [CONE] \n", ID->id);
                sleep(1);
            }

            sem_post(&cone);
        }
        else
        {
            printf("Customer [%d] Leaving! [ALERT]: Wrong Choice\n", ID->id);
            syscall(335, "Leaving! [ALERT]: Wrong choice\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }

        if (race_1 == 0)
        {

            printf("Customer [%d] Leaving! [ALERT]: Choice Finished\n", ID->id);
            syscall(335, "Leaving! [ALERT]: choices Finished\n", ID->id);
            sleep(1);
            pthread_exit(NULL);
        }
    }
    printf("Customer [%d] Leaving Chocie Counter! [SUCCESFUL]\n\n", ID->id);
    syscall(335, "Leaving choice Counter! [SUCCESFUL]\n", ID->id);
    sem_post(&ch);
    // choices counter exit
    //------------------------------------------------------------------
    sleep(2);
    //-------------------------------------------------------
    // payment counter enter
    sem_wait(&payment);
    revenue += ID->price;
    syscall(335, "GENERATING BILL! \n\n", ID->id);
    deliverIcecream(ID, threadID);
    printf("\n\nGenerating Bill For Customer [%d]\n", ID->id);
    sem_post(&payment);
    // payment counter exit
    //----------------------------------
    sleep(2);
    printf("\nStoring Data of Customer [%d]\n", ID->id);
    syscall(335, "STORING CUSTOMER DATA.\n\n", ID->id);
    customerData(ID);
    sleep(2);
    char *a;
    a = "Leaving Ice-Cream Shop [SUCCESFULL]\n\n";
    printf("\nCustomer [%d] Leaving Queue! [SUCCESFUL]\n", ID->id);
    printf("\nCustomer [%d] Leaving SLS ICECREAM PARLOUR! [SUCCESFUL]\n\n", ID->id);
    // syscall(335, "Leaving Ice-Cream Shop [SUCCESFULL]\n\n", ID->id);
    syscall(335, a, ID->id);
    pthread_exit(NULL);
    return NULL;
}
//---------------------------------------------
// function to store customer data into a txt file
void customerData(icecream *ID)
{
    time_t t;
    time(&t);
    FILE *fptr;
    fptr = fopen("/home/samaha-rizvi/os_project/data.txt", "a");
    if (fptr == NULL)
    {
        printf("[Error] exit");
        exit(1);
    }
    fprintf(fptr, "\nDate: %s\t", ctime(&t));
    fprintf(fptr, "ID: %d ; SIZE: %s ; FLAVOR: %s ; TOPPINGS: %s ; CHOICE: %s ; BILL: %.2lf ;\n", ID->id, ID->size, ID->flav, ID->top, ID->choice, ID->price);
    fclose(fptr);
}
//------------------------
void invoice()
{
    time_t t;
    time(&t);
    // print the final sales report
    printf("\n\n***********SALES REPORT**********\n\t\t %s \n\n", ctime(&t));
    printf("___________________________________________\n");
    printf("Total Customers:        | %d\n", cust_count);
    printf("Succesful Customers:    | %d\n", count);
    printf("Total Revenue:          | $%.2lf\n", revenue);
    printf("___________________________________________\n");
    printf("Total Cup left:         | %d\n", choices[0]);
    printf("Total Cone left:        | %d\n", choices[1]);
    printf("Small size left:        | %d\n", sizes[0]);
    printf("Medium size left:       | %d\n", sizes[1]);
    printf("Large size left:        | %d\n", sizes[2]);
    printf("Chocolate flavor left:  | %d\n", flavors[0]);
    printf("Vanilla flavor left:    | %d\n", flavors[1]);
    printf("Strawberry flavor left: | %d\n", flavors[2]);
    printf("Chocolate Chip left:    | %d\n", toppings[0]);
    printf("Sprinkles left:         | %d\n", toppings[1]);
    printf("___________________________________________\n");
    printf("\nTickets Destroyed:      | %d", cust_count - count);
    printf("\nTickets Left:           | %d\n", tickets);
}
// 21k-4578 21k-4897 21k-3301
