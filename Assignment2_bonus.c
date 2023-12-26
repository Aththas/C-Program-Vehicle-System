#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>
#include<windows.h>

typedef struct Vehicle
{
    int Vehicle_Number;
    char Vehicle_Manufacturer[20];
    char Vehicle_Name[10];
    int Date_Manufactured;
    float Top_Speed;
    int Number_Of_Seats;
    float Mass;

} vehicle_t;
vehicle_t  Vehicle_info[50];

typedef struct Obstacle {
int Obstacle_number;
char Obstacle_name[32];
char Obstacle_texture[20];
float Obstacle_mass;
} obstacle_t;
obstacle_t Obstacle_info[50];

typedef struct Vehicle_simulation {
int Test_ID;
int Vehicle_ID;
int Obstacle_ID;
int Vehicle_age; //  >=0
float Impact_velocity; //  >=0.0
int Number_of_passengers; // >=0
int Number_of_seatbelt_wearing_passengers; //  >=0
float Safety_rate;
float Damage_rate;
float Passenger_survival_rate;
} simulaion_t;
simulaion_t simulaion_info[50];

int menu();
void vehicle_search_byName(vehicle_t *v,int lines);
void displayVehicleDetails(int j,vehicle_t *v);
void obstacle_search_byName(obstacle_t *obs,int lines);
void displayObstacleDetails(int j,obstacle_t *obs);
int updateVehicle(vehicle_t *v, int lines);
void addVehicle(vehicle_t *v, int availableSlot);
int updateObstacle(obstacle_t *obs, int lines);
void addObstacle(obstacle_t *obs, int availableSlot);
void vehicleTestSimulation(vehicle_t *v, obstacle_t *obs,simulaion_t *s, int test_id);
int searchVehicleNumber(vehicle_t *v, int vehicle_no);
int searchObstacleNumber(obstacle_t *obs, int obstacle_no);
float calculate_survivalRate(float survival_rate, int x);
float calculate_damageRate(float safety_rate);
float calculate_safetyRate(float factor, float impactForce);
float calculate_impactForce(float velocity, float stopping_distance, float mass );
float convertVelocity(float velocity);
float calculate_ageFactor(int age);
void display_simulationResults(simulaion_t *s,int total_results,vehicle_t *v);
void displaySimulation(simulaion_t *s, int i);
void store_simulationData(simulaion_t *s,int totaltest);
void store_vehicleData(vehicle_t *v, int total_vehicles);
void store_obstacleData(obstacle_t *obs, int total_obstacles);

int main()
{
    FILE *vehicle = fopen("vehicle_c.txt","r");

    int total_lines_vehicle=1;////will be used to dynamically allocate memory for vehicle_info array (array of structs)
    int line;//I figured that fgetc returns an integer value
    while ((line=fgetc(vehicle))!=EOF)//get a character until EOF
        {
            if (line=='\n')
            {            //if it's a newline character then we need to allocate more memory for vehicle_info array
                    total_lines_vehicle++;
            }
        }
    fclose(vehicle);
    vehicle_t *Vehicle_info = malloc(sizeof(vehicle_t) * (total_lines_vehicle));

    //load data
    vehicle = fopen("vehicle_c.txt","r");
    int i=0;
    //the loop below gets data from .txt file line by line until EOF and stores it in vehicle_info array
    while (fscanf(vehicle,"%d %s %s %d %f %d %f", &Vehicle_info[i].Vehicle_Number, &Vehicle_info[i].Vehicle_Manufacturer, &Vehicle_info[i].Vehicle_Name, &Vehicle_info[i].Date_Manufactured, &Vehicle_info[i].Top_Speed, &Vehicle_info[i].Number_Of_Seats, &Vehicle_info[i].Mass) != EOF)
    {
            i++;
    }
    fclose(vehicle);

    //display data
    printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|                                                        Vehicles Listing                                                        |\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("Vehicle Number\tVehicle Manufacturer\tVehicle Name\tDate Manufactured\tTop Speed(km/h)\t\tNo. of Seats\tMass(kg)\n");
    printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");

    int j;
    for (j=0;j<total_lines_vehicle;j++){//for every element in vehicle_info, print the variables stored
        displayVehicleDetails(j,Vehicle_info);
    }
    printf("+--------------------------------------------------------------------------------------------------------------------------------+\n\n\n");

    FILE *obstacle = fopen("obstacle_c.txt","r");

    int total_lines_obstacle=1; //will be used to dynamically allocate memory for obstacle_info array (array of structs)
    int line2;   // I figured that fgetc returns an integer value
    while ((line2=fgetc(obstacle))!=EOF){  //get a character until EOF
            if (line2=='\n')
            {               //if it's a newline character then we need to allocate more memory for obstacle_info array
                    total_lines_obstacle++;
            }
    }
    fclose(obstacle); // close the file as it's job is done
    obstacle_t *Obstacle_info = malloc(sizeof(obstacle_t) * (total_lines_obstacle));

    obstacle = fopen("obstacle_c.txt","r");
    int k=0;
    //the loop below gets data from .txt file line by line until EOF and stores it in obstacle_info array
    while (fscanf(obstacle,"%d %s %s %f", &Obstacle_info[k].Obstacle_number, &Obstacle_info[k].Obstacle_name, &Obstacle_info[k].Obstacle_texture, &Obstacle_info[k].Obstacle_mass) != EOF)
    {
            k++;
    }
    fclose(obstacle);

    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("|                                       Obstacles Listing                                        |\n");
    printf("+------------------------------------------------------------------------------------------------+\n");
    printf("Obstacle Number\t\tObstacle Name\t\tObstacle Texture\tObstacle Mass(kg)\n");
    printf("+------------------------------------------------------------------------------------------------+\n");
    int l;
    for (l=0;l<total_lines_obstacle;l++){//for every element in obstacle_info, print the variables stored
        displayObstacleDetails(l,Obstacle_info);
    }
    printf("+------------------------------------------------------------------------------------------------+\n");

    int menu_options,test_id=0,update;
      do{
    menu_options =  menu();

        if(menu_options == 1)
            vehicle_search_byName(Vehicle_info,total_lines_vehicle);
        else if(menu_options == 2)
            obstacle_search_byName(Obstacle_info,total_lines_obstacle);
        else if(menu_options == 3)
        {
            update = updateVehicle(Vehicle_info,total_lines_vehicle);
            if(update == 0)
            {
                total_lines_vehicle++;
            }
        }
        else if(menu_options == 4)
        {
            update = updateObstacle(Obstacle_info,total_lines_obstacle);
            if(update == 0)
            {
                total_lines_obstacle++;
            }
        }
        else if(menu_options == 5)
        {
            vehicleTestSimulation(Vehicle_info, Obstacle_info,simulaion_info,test_id);
            test_id++;

        }
        else if(menu_options == 6)
            display_simulationResults(simulaion_info,test_id,Vehicle_info);
        else if(menu_options == 7)
            store_simulationData(simulaion_info,test_id);
        else if(menu_options == 8)
            {
                store_vehicleData(Vehicle_info, total_lines_vehicle);
                store_obstacleData(Obstacle_info, total_lines_obstacle);
            }
        else if(menu_options == 9)
        {
                int yesno ;
                printf("\nExit the program? \n1.Yes\n2.No: ");
                scanf("%d", &yesno);
                        if(yesno == 1)
                        {
                            printf("\nterminated\n"); menu_options=-1;
                        }
                        else if(yesno = 2)
                        {

                        }
                        else{printf("Invalid input");}
        }

    }while((menu_options>=1) && (menu_options<=9));
}

int menu()
{
    printf("\n\n Menu \n");
    printf("1. Search a vehicle\n");
    printf("2. Search an obstacle\n");
    printf("3. Input/update vehicle information\n");
    printf("4. Input/update obstacle information\n");
    printf("5. Simulate a vehicle crash against an obstacle\n");
    printf("6. Display simulation result\n");
    printf("7. Save all simulation results\n");
    printf("8. Save vehicle/obstacle information\n");
    printf("9. Exit\n");

    int menu;
    printf("Select a menu: ");
    scanf("%d",&menu);
    while(!(1 <= menu && menu <= 9))
    {
        printf("*** OUT OF RANGE *** <Enter a number between %d and %d>: ",1 ,9);
    }
    return menu;
}


void vehicle_search_byName(vehicle_t *v,int lines)
{
    char v_name[10];
    int i,j,search_number = -1;
    printf("Enter the Vehicle name: ");
    scanf("%10[^\n]%*c",&v_name);
    int value = strcmp("all", v_name);
    if(value == 0)
    {
        //display data
        printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|                                                        Vehicles Listing                                                        |\n");
        printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("Vehicle Number\tVehicle Manufacturer\tVehicle Name\tDate Manufactured\tTop Speed(km/h)\t\tNo. of Seats\tMass(kg)\n");
        printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
        for(j=0;j<lines;j++)
        {
            displayVehicleDetails(j,v);
        }
        printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
    }
    else
    {
        for(i=0;i<lines;i++)
        {
            int value = strcmp(v[i].Vehicle_Name, v_name);
            if(value == 0)
            {
                search_number = i;
            }
        }

        if(search_number != -1)
        {
            //display data
            printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
            printf("|                                                        Vehicles Listing                                                        |\n");
            printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
            printf("Vehicle Number\tVehicle Manufacturer\tVehicle Name\tDate Manufactured\tTop Speed(km/h)\t\tNo. of Seats\tMass(kg)\n");
            printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");
            displayVehicleDetails(search_number,v);
            printf("+--------------------------------------------------------------------------------------------------------------------------------+\n");

        }
        else
            printf("*** VEHICLE NOT FOUND ***\n");

    }


}

void displayVehicleDetails(int j,vehicle_t *v)
{

    printf("\t%d\t\t%s\t\t\t%s\t\t%d\t\t\t%.2f\t\t\t%d\t\t%.2f \n", v[j].Vehicle_Number, v[j].Vehicle_Manufacturer, v[j].Vehicle_Name, v[j].Date_Manufactured, v[j].Top_Speed, v[j].Number_Of_Seats, v[j].Mass);

}

void obstacle_search_byName(obstacle_t *obs,int lines)
{
    char obs_name[10];
    int i,j,search_number = -1;
    printf("Enter the Obstacle name: ");
    scanf("%20[^\n]%*c",&obs_name);
    int value = strcmp("all", obs_name);
    if(value == 0)
    {
        //display data
        printf("\t+------------------------------------------------------------------------------------------------+\n");
        printf("\t|                                       Obstacles Listing                                        |\n");
        printf("\t+------------------------------------------------------------------------------------------------+\n");
        printf("\tObstacle Number\t\tObstacle Name\t\tObstacle Texture\tObstacle Mass(kg)\n");
        printf("\t+------------------------------------------------------------------------------------------------+\n");
        for(j=0;j<lines;j++)
        {
            displayObstacleDetails(j,obs);
        }
        printf("\t+------------------------------------------------------------------------------------------------+\n");
    }
    else
    {
        for(i=0;i<50;i++)
        {
            int value = strcmp(obs[i].Obstacle_name, obs_name);
            if(value == 0)
            {
                search_number = i;
            }
        }

        if(search_number != -1)
        {
            //display data
            printf("\t+------------------------------------------------------------------------------------------------+\n");
            printf("\t|                                       Obstacles Listing                                        |\n");
            printf("\t+------------------------------------------------------------------------------------------------+\n");
            printf("\tObstacle Number\t\tObstacle Name\t\tObstacle Texture\tObstacle Mass(kg)\n");
            printf("\t+------------------------------------------------------------------------------------------------+\n");
            displayObstacleDetails(search_number,obs);
            printf("\t+------------------------------------------------------------------------------------------------+\n");
            }
        else
            printf("*** OBSTACLE NOT FOUND ***\n");
    }

}

void displayObstacleDetails(int j,obstacle_t *obs)
{
    printf("\t%d\t\t\t%s\t\t\t%s\t\t\t%.2f \n", obs[j].Obstacle_number, obs[j].Obstacle_name, obs[j].Obstacle_texture, obs[j].Obstacle_mass);
}

int updateVehicle(vehicle_t *v, int lines)
{

    int v_number,search_result = -1,update;
    printf("Enter a Vehicle number: ");
    scanf("%d", &v_number);

    int i,num;
    for(i=0;i<lines;i++)
    {
                num = v[i].Vehicle_Number;
                if(num == v_number)
                {
                    search_result = i;
                }

    }

    if(search_result != -1)
    {
        printf("\nThe vehicle exist\n");
        printf("\nUpdate the vehicle information\n1.Yes\n2.No\n");
        printf("Select an option: ");
            int yesno ;
            scanf("%d", &yesno);
            if(yesno == 1)
            {

                printf("\nUpdate Vehicle number:");
                scanf("%d", &v[search_result].Vehicle_Number);
                printf("Update Vehicle manufacturer:");
                scanf("%20[^\n]%*c", &v[search_result].Vehicle_Manufacturer);
                printf("Update Vehicle name:");
                scanf("%10[^\n]%*c", &v[search_result].Vehicle_Name);
                printf("Update Manufactured date:");
                scanf("%d", &v[search_result].Date_Manufactured);
                printf("Update Top Speed(km/h):");
                scanf("%f", &v[search_result].Top_Speed);
                printf("Update No. of seats:");
                scanf("%d", &v[search_result].Number_Of_Seats);
                printf("Update Mass(kg):");
                scanf("%f", &v[search_result].Mass);
                printf("\nVehicle updated\n\n");
                update=1;
            }
            else if(yesno = 2)
            {

            }
            else
                printf("Invalid input");
    }
    else
    {
        printf("\nAdd vehicle information\n1.Yes\n2.No\n");
        printf("Select an option: ");
        int yesno ;
        scanf("%d", &yesno);
        if(yesno == 1)
        {
            update=0;
            addVehicle(v,lines);
}
        else if(yesno = 2)
        {

        }
        else
            printf("Invalid input");
    }
    return update;
}

void addVehicle(vehicle_t *v, int availableSlot)
{
    if(availableSlot<50)
    {
        printf("\nEnter Vehicle number:");
        scanf("%d", &v[availableSlot].Vehicle_Number);
        printf("Enter Vehicle manufacturer:");
        scanf("%20[^\n]%*c", &v[availableSlot].Vehicle_Manufacturer);
        printf("Enter Vehicle name:");
        scanf("%10[^\n]%*c", &v[availableSlot].Vehicle_Name);
        printf("Enter Manufactured date:");
        scanf("%d", &v[availableSlot].Date_Manufactured);
        printf("Enter Top Speed(km/h):");
        scanf("%f", &v[availableSlot].Top_Speed);
        printf("Enter No. of seats:");
        scanf("%d", &v[availableSlot].Number_Of_Seats);
        printf("Enter Mass(kg):");
        scanf("%f", &v[availableSlot].Mass);

        printf("\nVehicle added\n\n");
    }
    else
    {
        printf("\nCannot add more vehicles \n\n");
    }
}


int updateObstacle(obstacle_t *obs, int lines)
{

    int obs_number,search_result = -1,update;
    printf("Enter a Obstacle number: ");
    scanf("%d", &obs_number);

    int i,num;
    for(i=0;i<lines;i++)
    {
                num = obs[i].Obstacle_number;
                if(num == obs_number)
                {
                    search_result = i;
                }
    }

    if(search_result != -1)
    {
        printf("\nThe obstacle exist\n");
        printf("\nUpdate the obstacle information\n1.Yes\n2.No\n");
        printf("Select an option: ");
            int yesno ;
            scanf("%d", &yesno);
            if(yesno == 1)
            {
                printf("\nUpdate Obstacle number:");
                scanf("%d", &obs[search_result].Obstacle_number);
                printf("Update Obstacle name(20 characters):");
                scanf("%20[^\n]%*c", &obs[search_result].Obstacle_name);
                printf("Update Obstacle texture(32 characters):");
                scanf("%32[^\n]%*c", &obs[search_result].Obstacle_texture);
                printf("Update Obstacle mass(kg):");
                scanf("%f", &obs[search_result].Obstacle_mass);
                printf("\nObstacle updated\n\n");
                update=1;
            }
            else if(yesno = 2)
            {

            }
            else
                printf("Invalid input");
    }
    else
    {
        printf("\nAdd obstacle information \n1.Yes\n2.No\n");
        printf("Select an option: ");
        int yesno ;
        scanf("%d", &yesno);
        if(yesno == 1)
        {
            update=0;
            addObstacle(obs,lines);
        }
        else if(yesno = 2)
        {

        }
        else
            printf("Invalid input");
    }
    return update;
}


void addObstacle(obstacle_t *obs, int availableSlot)
{
    if(availableSlot<50)
    {
        printf("\nEnter Obstacle number:");
        scanf("%d", &obs[availableSlot].Obstacle_number);
        printf("Enter Obstacle name(20 characters):");
        scanf("%20[^\n]%*c", &obs[availableSlot].Obstacle_name);
        printf("Enter Obstacle texture(32 characters):");
        scanf("%32[^\n]%*c", &obs[availableSlot].Obstacle_texture);
        printf("Enter Obstacle mass(kg):");
        scanf("%f", &obs[availableSlot].Obstacle_mass);

        printf("\nObstacle added\n\n");
    }
    else
    {
        printf("\nCannot add more obstacles\n\n");
    }
}


void vehicleTestSimulation(vehicle_t *v, obstacle_t *obs,simulaion_t *s, int test_id)
{

    int t = test_id;
    int v_number,obs_number,searchVehicle=0,searchObstacle=0;

    do{
    printf("Enter a vehicle number: ");
    scanf("%d", &v_number);
    searchVehicle = searchVehicleNumber(v,v_number);
    }while(searchVehicle == -1);


    do{
    printf("Enter an obstacle number: ");
    scanf("%d", &obs_number);
    searchObstacle = searchObstacleNumber(obs,obs_number);
    }while(searchObstacle == -1);


    int vehicle_age;
    SYSTEMTIME today;
    GetLocalTime(&today);
    printf("Enter vehicle age(Year): ");
    scanf("%d", &vehicle_age);
    int manufactured_year = v[searchVehicle].Date_Manufactured;
    while(vehicle_age<0 || vehicle_age > (today.wYear - manufactured_year))
    {
        printf(" Invalid input \n");
        printf("Enter vehicle age(Years): ");
        scanf("%d", &vehicle_age);
    }


    float impact_velocity;
    printf("Enter vehicle impact velocity(km/h): ");
    scanf("%f", &impact_velocity);
    float topSpeed = v[searchVehicle].Top_Speed;
    while(impact_velocity<0 || impact_velocity > topSpeed )
    {
        printf(" Invalid input \n");
        printf("Enter vehicle impact velocity(km/h): ");
        scanf("%f", &impact_velocity);
    }

    float stopping_distance;
    printf("Enter stopping distance(m): ");
    scanf("%f", &stopping_distance);
    while(stopping_distance<0)
    {
        printf(" Invalid input \n");
        printf("Enter stopping distance(m): ");
        scanf("%f", &stopping_distance);
    }


    int number_of_passengers;
    printf("Enter number of passengers: ");
    scanf("%d", &number_of_passengers);
    int seats = v[searchVehicle].Number_Of_Seats;
    while(number_of_passengers<0 || number_of_passengers > seats)
    {
        printf(" Invalid input \n");
        printf("Enter number of passengers: ");
        scanf("%d", &number_of_passengers);
    }

    int number_of_seatbelt_wearing_passengers;
    printf("Enter number of seatbelt wearing passengers: ");
    scanf("%d", &number_of_seatbelt_wearing_passengers);
    while(number_of_seatbelt_wearing_passengers<0 || number_of_seatbelt_wearing_passengers > number_of_passengers)
    {
         printf(" Invalid input \n");
        printf("Enter number of seatbelt wearing passengers: ");
        scanf("%d", &number_of_seatbelt_wearing_passengers);
    }

    float velocity = convertVelocity(impact_velocity);
    float age_factor = calculate_ageFactor(vehicle_age);
    float mass = v[searchVehicle].Mass;

    float impact_force = calculate_impactForce(velocity,stopping_distance,mass);

    float safety_rate, damage_rate,survival_rate,not_wearing_seatbelt_passengers_survival;
    safety_rate = calculate_safetyRate(age_factor,impact_force);
    damage_rate = calculate_damageRate(safety_rate);
    survival_rate = safety_rate;
    if(number_of_passengers > number_of_seatbelt_wearing_passengers)
    {
        survival_rate = calculate_survivalRate(survival_rate,4);
        not_wearing_seatbelt_passengers_survival = calculate_survivalRate(survival_rate,9);
    }

    printf("\n Test Condition \n");
    printf("Vehicle age                                      : %d\n",vehicle_age);
    printf("Impact Velocity                                  : %.2f\n",impact_velocity);
    printf("Number of passengers in vehicle                  : %d\n",number_of_passengers);
    printf("Number of seatbelt wearing passengers in vehicle : %d\n",number_of_seatbelt_wearing_passengers);
    printf("Safety rate                                      : %.2f\n",safety_rate);

    printf("\n1. Accept the information and proceed\n");
    printf("2. Discard all condition\n");

    int selection;
    printf("\n Select an option: ");
    scanf("%d",&selection);

    switch(selection)
    {
      case 1:
        printf("\n\tVehicle Simulation test Result \n");
        printf("TestID                                   :%d \n", test_id+1);
        printf("Vehicle Information:\n");
        printf("Vehicle Number                           :%d \n",v[searchVehicle].Vehicle_Number);
        printf("Vehicle Name                             :%s \n",v[searchVehicle].Vehicle_Name);
        printf("Manufactured Date                        :%d \n",v[searchVehicle].Date_Manufactured);
        printf("Top Speed                                :%.2f \n",v[searchVehicle].Top_Speed);
        printf("Mass(kg)                                 :%.2f \n",v[searchVehicle].Mass);
        printf("Number of seats                          :%d \n",v[searchVehicle].Number_Of_Seats);
        printf("Obstacle Information:\n");
        printf("Obstacle Number                          :%d \n",obs[searchObstacle].Obstacle_number);
        printf("Obstacle Name                            :%s \n",obs[searchObstacle].Obstacle_name);
        printf("Stopping Distance                        :%.2f \n",stopping_distance);
        printf("Vehicle Age                              :%d \n",vehicle_age);
        printf("Impact Velocity                          :%.2f \n",impact_velocity);
        printf("Number of Passengers                     :%d \n",number_of_passengers);
        printf("Number of seatbelt wearing Passengers    :%d \n",number_of_seatbelt_wearing_passengers);
        printf("Simulation Result:\n");
        printf("Vehicle's Safety Rate                    :%.2f \n",safety_rate);
        printf("Vehicle's Damage Rate                    :%.2f \n",damage_rate);
        printf("Simulation Result:\n");
        printf("Survival Rate(wearing seat belts)        :%.2f \n",survival_rate);
        printf("Survival Rate(not wearing seat belts)    :%.2f \n",not_wearing_seatbelt_passengers_survival); break;

        case 2: break;
        default: break;
    }
    //assigning values
    s[t].Test_ID = test_id+1;
    s[t].Vehicle_ID = v[searchVehicle].Vehicle_Number;
    s[t].Obstacle_ID = obs[searchObstacle].Obstacle_number;
    s[t].Impact_velocity = impact_velocity;
    s[t].Damage_rate = damage_rate;
    s[t].Number_of_passengers = number_of_passengers;
    s[t].Number_of_seatbelt_wearing_passengers = number_of_seatbelt_wearing_passengers;
    s[t].Passenger_survival_rate = survival_rate;
    s[t].Safety_rate = safety_rate;
    s[t].Vehicle_age = vehicle_age;

}

int searchVehicleNumber(vehicle_t *v, int vehicle_no)
{
    int i,j=-1,num;
    for(i=0;i<50;i++)
    {
                num = v[i].Vehicle_Number;
                if(num == vehicle_no)
                {
                    j=i;
                }

    }
    return j;
}

int searchObstacleNumber(obstacle_t *obs, int obstacle_no)
{
    int i,j=-1;
    for(i=0;i<50;i++)
    {
                if(obs[i].Obstacle_number == obstacle_no)
                {
                    j=i;
                    break;
                }

    }
    return j;
}

float calculate_survivalRate(float survival_rate, int x)
{
    return survival_rate/(1+x);
}

float calculate_damageRate(float safety_rate)
{
    return ((float)((rand()+1)%25)/100)*(1-safety_rate);
}


float calculate_safetyRate(float factor, float impactForce)
{
    float safety_rate = 1;
    safety_rate = safety_rate - (factor + (impactForce/100000));

    if(safety_rate>1)
        return 1;
    else if(safety_rate<0)
        return 0;
    else
        return safety_rate;
}

float calculate_impactForce(float velocity, float stopping_distance, float mass )
{
   return (0.5*mass*(velocity * velocity))/stopping_distance;
}

float convertVelocity(float velocity)
{
    return  velocity*(5/18);
}

float calculate_ageFactor(int age)
{
    if(age <= 2)
    {
        return 0;
    }
    else if(age > 22)
    {
        return 1;
    }
    else
    {
        return 0.05*(age - 2);
    }
}

void display_simulationResults(simulaion_t *s,int total_results,vehicle_t *v)
{

    printf("\n1. Display simulation by vehicle number\n");
    printf("2. Display simulation by vehicle name\n");

    int menu_option;
    printf("\n Select an option: ");
    scanf("%d",&menu_option);


    if(menu_option == 1)
    {
        int v_number,i=0,j=-1,num;
        printf("\nEnter vehicle number to display simulation results : ");
        scanf("%d", &v_number);
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("|                                                                                              Simulation Results                                                                                              |\n");
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        printf("Test ID\t\tVehicle ID\tObstacle ID\tVehicle Age\tImpact Velocity\t\tNo of Passengers\tNo of seatbelt wearing passengers\tSafety Rate\tDamage Rate\tSurvival Rate\n");
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        while(i<total_results)
        {
            num = s[i].Vehicle_ID;
            if(num == v_number)
            {
                j=i;
                displaySimulation(s,j);
            }
            i++;
        }
        printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        if(j == -1)
        printf("\n Make sure to test the vehicle 1st ");
    }
    else if(menu_option == 2)
    {
        char v_name[10];
        printf("\nEnter a vehicle name to display simulation results : ");
        scanf("%s", &v_name);

        int value = strcmp("all",v_name);
        if(value == 0)
        {
            int i=0;
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            printf("|                                                                                              Simulation Results                                                                                              |\n");
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            printf("Test ID\t\tVehicle ID\tObstacle ID\tVehicle Age\tImpact Velocity\t\tNo of Passengers\tNo of seatbelt wearing passengers\tSafety Rate\tDamage Rate\tSurvival Rate\n");
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            while(i<total_results)
            {
                displaySimulation(s,i);
                i++;
            }
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
        }
        else
        {
            int i=0,value,j=-1;
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            printf("|                                                                                              Simulation Results                                                                                              |\n");
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            printf("Test ID\t\tVehicle ID\tObstacle ID\tVehicle Age\tImpact Velocity\t\tNo of Passengers\tNo of seatbelt wearing passengers\tSafety Rate\tDamage Rate\tSurvival Rate\n");
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            while(i<total_results)
            {
                value = strcmp(v[i].Vehicle_Name,v_name);
                if(value == 0)
                {
                    j = i;
                    displaySimulation(s,j);
                }
                i++;
            }
            printf("+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
            if(j == -1)
                printf("\n Make sure to test the vehicle 1st ");
        }
    }

}

void displaySimulation(simulaion_t *s, int i)
{
    printf("%d\t\t%d\t\t%d\t\t%d\t\t%.2f\t\t\t%d\t\t\t%d\t\t\t\t\t%.2f\t\t%.2f\t\t%.2f\n",s[i].Test_ID, s[i].Vehicle_ID, s[i].Obstacle_ID, s[i].Vehicle_age, s[i].Impact_velocity, s[i].Number_of_passengers, s[i].Number_of_seatbelt_wearing_passengers, s[i].Safety_rate, s[i].Damage_rate, s[i].Passenger_survival_rate);
}

void store_simulationData(simulaion_t *s,int totaltest)
{
    FILE *simulation;
    simulation = fopen("results.txt","w");
    int i=0;
    while( i<totaltest)
    {
     fprintf(simulation,"%d %d %d %d %.2f %d %d %.2f %.2f %.2f\n", s[i].Test_ID, s[i].Vehicle_ID, s[i].Obstacle_ID, s[i].Vehicle_age, s[i].Impact_velocity, s[i].Number_of_passengers, s[i].Number_of_seatbelt_wearing_passengers, s[i].Safety_rate, s[i].Damage_rate, s[i].Passenger_survival_rate);
     i++;
    }
    fclose(simulation);
    printf(" Data Stored successfully \n");
}

void store_vehicleData(vehicle_t *v, int total_vehicles)
{
    FILE *vehicle = fopen("new_Vehicle.txt","w");
    int i=0;
    while(i<total_vehicles)
    {
     fprintf(vehicle,"%d %s %s %d %.2f %d %.2f\n", v[i].Vehicle_Number, v[i].Vehicle_Manufacturer, v[i].Vehicle_Name, v[i].Date_Manufactured, v[i].Top_Speed, v[i].Number_Of_Seats, v[i].Mass);
     i++;
    }
    fclose(vehicle);
    printf(" Data Stored successfully \n");
}

void store_obstacleData(obstacle_t *obs, int total_obstacles)
{
    FILE *obstacle = fopen("new_Obstacle.txt","w");
    int j=0;
    while(j<total_obstacles)
    {
     fprintf(obstacle,"%d %s %s %.2f\n", obs[j].Obstacle_number, obs[j].Obstacle_name, obs[j].Obstacle_texture, obs[j].Obstacle_mass);
     j++;
    }
    fclose(obstacle);
    printf(" Data Stored successfully \n");
}

