#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <windows.h>
#include <time.h>
//these will be whithin the queue

typedef struct car{
    int car_id; //unique ID for the car
    int entry_time; //time it entered
    int exit_time;//tiem it leaves
    struct car* next_car_ptr; //car in front of it in queue
}car;

typedef struct light{
    char name[15];
    int state;//for red or green, 1 = green 0 = red
    int interval;//amt of passes needed for the state to change -- Could also be renamed to service rate like we have in the street but I dont like resuing variable names.
}light;

//queue structure
typedef struct street{
    char streetname[15];
    car *rear_ptr; //newest car in queue
    car *front_ptr; //car about to leave
    int maxcarcount; //most cars that can be in queue
    int service_rate; // Amount of cycles needed to move the car in queue
    int curcarcount; //I added this one wasnt in the ulearn post but it will be good to track how many cars are in the queue
}street;

car *createcar(int i); //for creating and returing a cars pointe
void movequeue(street *p,street *p2); //for moving the queues
void enterqueue(street *p,int i,car *new); //going to take the struct for street and int for the car's id
void updatelight();//updates the lights to red or green
int full(street *p); // checks if full
int empty(street *p);// checks if empty
light* initlight(char name[15],int state, int interval);//for creating the lights
street* initstreet(char streetname[15],int maxcars, int rate); //for creating the streets
float calcpercent(float number,float size); //calculates % full
void updatepercent();//updates the percent (bar graph)
void converttime(car *p);//prints time spent in queue

//when you add more intersections change the colomn, rows will stay as 2

light* lightmatrix[4][2]; //setting the matrix of lights
street* streetarray[5]; //update with the amount of streets

//first value is the intersection, second is the two accompanying lights


int main() {
    SetConsoleOutputCP(CP_UTF8); //Without setting the typeset to utf8 we can't display the percent bars properly.
    int i;
    int cycles = 1;
    //intersection 1
    light* pinechestnut = initlight("Pine",1,3);
    light* chestnutpine = initlight("Chestnut",0,3);
    //intersection 2
    light* richmondpine = initlight("Richmond",0,3);
    light* pinerichmond = initlight("Pine2",1,3);
    //intersection 3
    light* friendship = initlight("Friendship",1,3);
    light* chestnutfriendship = initlight("Chestnut 2",0,3);
    //intersection 4
    light* broadempire = initlight("Broad",0,3);
    light* empirebroad= initlight("Empire",1,3);

    //create an intersection of lights




    //add a street
    //streets
    street *pine = initstreet("Pine",44,3);
    street *chestnut = initstreet("Chestnut",50,1);
    street *friendships = initstreet("Friendship",30,7);
    street *richmond = initstreet("Richmond",34,8);
    street *broad = initstreet("Broad",36,5);
    street *empire = initstreet("Empire",32,6);
    street *weybossett = initstreet("Weybossett",47,4);


    //this will not be in our array for holding cars that leave
    street *depart = initstreet("departures",999,0);//we will clear the cars from here
    //




    //add the street to the array (oyu need to change the array size above.

    //assigning streets to array
    streetarray[0] = pine;
    streetarray[1] = chestnut;
    streetarray[2] = friendships;
    streetarray[3] = richmond;
    streetarray[4] = broad;
    streetarray[5] = empire;
    streetarray[6] = weybossett;



    //add the intersection to the light matrix (need to change the size as well)

    //assigning the lights to the matrix
    lightmatrix[0][0] = pinechestnut;
    lightmatrix[0][1] = chestnutpine;
    lightmatrix[1][0] = richmondpine;
    lightmatrix[1][1] = pinerichmond;
    lightmatrix[2][0] = friendship;
    lightmatrix[2][1] = chestnutfriendship;
    lightmatrix[3][0] = broadempire;
    lightmatrix[3][1] = empirebroad;


    //engine stuff here


//adding some cars to the queues.
    for(i = 0; i <20; i++){
        enterqueue(pine,i,NULL);
    }

    for(i = 19; i <35; i++){
        enterqueue(friendships,i,NULL);
    }
    for(i = 34; i <45; i++){
        enterqueue(weybossett,i,NULL);
    }

    updatelight();//updating the lights before cycle starts
//engine stuff
    while(cycles < 30) {
        printf("Cycle: %d\n", cycles); //displays what cycle it is.

        //Deciding if cars can enter
        srand(time(0)); //seeding random using current system time
        int randomint = rand() % 10; //assigning a random int 1-10
        updatepercent();//updates the graph
        printf("\n"); //new line for formatting



        if(cycles % pine->service_rate ==0 && lightmatrix[0][0]->state == 1){ // if the cycles % the service rate  and light is green
            if(!empty(pine) && randomint >=4){ //if random int is >=4 turn left
                if(!empty(pine) && !full(chestnut)){ //checking if you can turn left
                    printf("\nCar #%d has turned left to Chestnut Street",pine->front_ptr->car_id);
                    movequeue(pine,chestnut); //moves the car to chestnuts queue
                }
            }else{
                if(!empty(pine) && !full(richmond)){ //check if richmond is full and if pine is empty
                    printf("\nCar #%d has turned left to Richmond Street",pine->front_ptr->car_id);
                    movequeue(pine,richmond); //move car to richmond
                }
            }
        }

        //chestnut to friendship. left turn only.
        if(!empty(chestnut)){
            if(cycles %chestnut->service_rate ==0 && lightmatrix[2][1]->state == 1 && randomint > 1){ // if the cycles % the service rate  and light is green
                if(!empty(chestnut) && !full(friendships)){ // if chestnuts not empty and rand is > 1 turn to friendship
                    printf("\nCar #%d has turned left to Friendship Street",chestnut->front_ptr->car_id);
                    movequeue(chestnut,friendships); //move queues
                }else{
                    printf("\nCar #%d has departed, time spent in queue: ",chestnut->front_ptr->car_id); //otherwise the car departs from the simu;ation
                    converttime(chestnut->front_ptr); //converts the time to print out how long it has been in the queue
                    movequeue(chestnut,depart); //move to the depart queue
                }
            }
        }


        //friendship to richmond (no light here)
        if(!empty(friendships)) { //if friendship isnt empty
            if(randomint > 2 && !full(richmond)) { // if the cycles % the service rate  and light is green
                printf("\nCar #%d has turned left Richmond Street",friendships->front_ptr->car_id);
                movequeue(friendships,richmond); //move queues
            }else {
                printf("\nCar #%d has departed, time spent in queue: ",friendships->front_ptr->car_id);
                converttime(friendships->front_ptr); //conver the time to print how long it was in queue
                movequeue(friendships,depart);//move to depature queue
            }
        }

        //richmond to pine (turn left again)
        if(!empty(richmond) && lightmatrix[1][0]->state == 1) { // if the cycles % the service rate  and light is green
            if(randomint < 5 && !full(pine)) {//randomint < 5 we turn to pine
                printf("\nCar #%d has turned left Pine Street",richmond->front_ptr->car_id);
                movequeue(richmond,pine);
            }else{
                printf("\nCar #%d has turned left to Weybosset Street",richmond->front_ptr->car_id); //otherwise turn to weybosset
                movequeue(richmond,weybossett);
            }
        }

        if(!empty(weybossett)){ //if weybossett isnt empty
            if(cycles % weybossett->service_rate == 0){// if the cycles % the service rate  and light is green
                if(randomint >=7 && !full(empire)){ //70% chance to turn right to empire
                    printf("\nCar #%d has turned right to Empire Street",weybossett->front_ptr->car_id);
                    movequeue(weybossett,empire);
                }else{
                    if(!full(chestnut)){ //otherwise it turns left to chestnut
                        printf("\nCar #%d has turned left to Chestnut Street",weybossett->front_ptr->car_id);
                        movequeue(weybossett,chestnut);
                    }
                }
            }
        }



        //broad to straight towards (weybossett) i think
        if(!empty(broad)){ //if broad isnt empty
            if(cycles %broad->service_rate == 0  && lightmatrix[3][0]->state == 1){// if the cycles % the service rate  and light is green
                if(randomint < 1 && !full(weybossett)) { //90% chance to turn to weybosset
                    printf("\nCar #%d has turned left Pine Street",broad->front_ptr->car_id);
                    movequeue(broad,weybossett); //moves queues
                }else{
                    printf("\nCar #%d has departed, time spent in queue: ",broad->front_ptr->car_id); //otherwise it departs
                    converttime(broad->front_ptr);//prints the time it was in queue
                    movequeue(broad,depart);//moves to depatures
                }
            }
        }

        //broad to empire (left only)
        if(!empty(broad)){//broad not empty
            if(cycles % broad->service_rate == 0  && lightmatrix[3][0]->state == 1){// if the cycles % the service rate  and light is green
                if(randomint < 1 && !full(empire)) { //90% chance to turn to pine
                    printf("\nCar #%d has turned left Pine Street",broad->front_ptr->car_id);
                    movequeue(broad,empire);//moves queues
                }else{
                    printf("\nCar #%d has departed, time spent in queue: ",broad->front_ptr->car_id);//otherwise leaves again
                    converttime(broad->front_ptr);//prints how long it was in queue
                    movequeue(broad,depart);//moves to depatures
                }
            }
        }

        //empire to broad
        if(!empty(empire)){//empire not empty
            if(cycles %empire->service_rate == 0  && lightmatrix[3][1]->state == 1){// if the cycles % the service rate  and light is green
                if(!empty(empire) && randomint >=9){ //10% chance to leave queue
                    printf("\nCar #%d has departed, time spent in queue: ",empire->front_ptr->car_id);
                    converttime(empire->front_ptr);//prints time spent
                    movequeue(empire,depart);//leaves queue
                }
            }else{
                if(!full(broad)){ //otherwise if broad not full, moves to broad
                    printf("\nCar #%d has turned left to Broad Street",empire->front_ptr->car_id);
                    movequeue(empire,broad);//moves queue
                }
            }
        }

        sleep(1); //sleeping the program for a second
        cycles++;//incrementing the cycle
        if(cycles% lightmatrix[0][0]->interval == 0){//if its been 3 cycles (all lights are on the same interval;
            updatelight();//update the lights
        }
        system("cls");//clear the screen
    } //loops again

    return 0;
}


void converttime(car *p){
    long sec,h,m,s; //needed for time conversion
    p->exit_time = time(0); //we set the cars exit time to when we call convert time
    sec = ( p->exit_time- p->entry_time); //our total time spent in queue is the exit time - the entry time (exit time will be greater obviously thats why we subtract from it)
    h = (sec/3600); //amount of hours would be the time spent in queue /3600
    m = (sec-(3600*h))/60;//minutes would be 3600*h/60
    s = (sec -(3600*h)-(m*60)); //seconds would be 3600*h - m*60
    printf("%ld:%ld",m,s); //prints out minutes and seconds it's been in queue
}
void updatepercent() {
    int i,j,k;//for our loops
    for (i = 0; i < 7; i++) {//we have 6 streets.
        int count = 0; //count starts at 0 so we can print the empty boxes
        float val = calcpercent((float)streetarray[i]->curcarcount,(float)streetarray[i]->maxcarcount);//our value is our calculated percent full
        int amt = floor(val/10);//we find out how many solid squares we need to print using floor division
        printf("\n%s Street: ",streetarray[i]->streetname);//prints the street name
        for(j = 0; j <amt; j++) {  //loop for printing the solid boxes
            printf("■");
            count++;//increment the count
        }
        for(k = count; k <10; k++) { //starting at the current count until 10 we print the empty boxes if applicable
            printf("□");
        }
        printf(" %d%% full",(int)val);//printing what % full the street is
    }
}
float calcpercent(float number, float size){
    return (number/size*100); //simple enough, just finding what % full the street is
}
void updatelight(){
    //will loop through matrix to update each light
    int i;
    for(i = 0; i <4; i++){
        (lightmatrix[i][0]->state == 1) ? (lightmatrix[i][0]->state = 0,lightmatrix[i][1]->state = 1): (lightmatrix[i][1]->state = 0,lightmatrix[i][0]->state = 1); //ternary operator here instead of if/else, makes it a 1-liner basically just looping through the i amount of lights and changing them to red or green
    }
}
void enterqueue(street *p,int i,car* new){
    if( new == NULL){
        new = createcar(i);
    }
    //if something at the beginning of the queue connect it
    if(p->rear_ptr != NULL){
        p->rear_ptr->next_car_ptr = new;
    }
    //makes rue we track where the front pointer is as a queue is FIFO
    if (p->front_ptr == NULL){
        p->front_ptr = new;
    }
    p->rear_ptr = new; // inserts it at the end of the queue
    p->curcarcount++; //update count
}
void movequeue(street *p,street *p2){
    if(p->front_ptr == NULL){//if the queues empty we just return
        return;
    }
    enterqueue(p2,0,p->front_ptr);//putting the front pointer of the car into the second street;
    p->front_ptr = p->front_ptr->next_car_ptr; //making sure the queue we just moved from still makes sense
    p->curcarcount--;//decrementing the queue's car count
}
light* initlight(char name[15],int state, int interval){
    light *p =(light*)malloc(sizeof (light)); //this way we dont need to make a huge mess with variables usuing an init function is nicer
    strcpy(p->name,name); //stores the name of the light
    p->state = state; //state (red or green represented by 1 or 0)
    p->interval = interval;//interval (using 3 for 30 seoncds);


    return p; //returns the pointer
}
street* initstreet(char streetname[15],int maxcars, int rate){
    street *p = (street*) malloc(sizeof (street)); //init for each street as well to be cleaner
    strcpy(p->streetname,streetname); //sets the street name
    p->rear_ptr = NULL; //both front and rear pointer are null due to it being empty
    p->front_ptr = NULL;
    p->maxcarcount = maxcars; //define the max amount it can hold
    p->curcarcount = 0; //car count is 0 sicne its empty
    p->service_rate = rate;// service rate (how many loops until something happens)
    return p; //returns the pointer
}
int full(street *p){//checking if street is full
    return (p->curcarcount == p->maxcarcount); //will return if the street is full
}
int empty(street *p){//checking if empty
    return (p->curcarcount == 0); //will return if empty
}
car *createcar(int i){ //for creating a new car
    car *carptr = (car*) malloc(sizeof(car)); //mallocs
    carptr->car_id = i ; //makes the id = to whatever is passed in
    carptr->entry_time = time(0); //Current time is entry time
    carptr->exit_time = 0; //exit time doesnt exist yet
    carptr->next_car_ptr = NULL; //next car doesnt exist either
    return carptr; //retunrs the pointer for the car
}