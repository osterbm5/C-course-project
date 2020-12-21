#include "projekti.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// the main loop that takes the commands
void doCommand(Slot *slots){
    char buffer[80];
    char buf;
    char name[80];
    int month;
    int date;
    int hour;
    int go = 1;
    


    while(go){
        fgets(buffer, 80, stdin);
        sscanf(buffer, "%c", &buf);
        switch(tolower((int) buf)){
        // a is for adding an entry to the calendar, specify name(string), month(int), day(int) and hour(int)
        case 'a':
            sscanf(buffer, "%c %s %d %d %d", &buf, name, &month, &date, &hour);
            slots = addEntry(slots, name, month, date, hour);
            break;
        // d is for deleting an entry, specify, month(int), day(int) and hour(int)
        case 'd':
            sscanf(buffer, "%c %d %d %d", &buf, &month, &date, &hour);
            deleteEntry(slots, month, date, hour);
            break;
        // l lists all the entries in the calendar sorted by the date and time
        case 'l':
            printCalendar(slots);
            break;
        // w writes the calendar to a binary file, specify name(string) of the file
        case 'w':
            sscanf(buffer, "%c %s", &buf, name);
            if (saveCalendar(slots, name)){
                printf("Error: error in writing to file.");
            }
            break;
        // o opens a written file, specify name(string)
        case 'o':
            sscanf(buffer, "%c %s", &buf, name);
            free(slots);
            slots = loadCalendar(name);
            break;
        //q quits the program, frees all allocated memory first
        case 'q':
            free(slots);
            go = 0;
            break;
        default:
            printf("Invalid command\n");
            break;
        }
    }
}

// adds one entry to the array of slots and returns it

Slot* addEntry(Slot *slots, char * name, int month, int day, int hour){
    int i = 0;
    //checks if date is already booked
    while(slots->name[0] != 0) {
        if (slots->month == month && slots->hour == hour && slots->day == day){
            printf("Error: time already booked.\n");
            return slots;
        }
        i++;
        slots++;
    }
    slots -= i;
    //checks if the date is a valid date
    if (month > 0 && day > 0 && hour > 0 && month <= 12 && day <= 31 && hour <= 24){   
        slots = realloc(slots, ((i + 2) * sizeof(Slot)));
        memset(&slots[i], 0, sizeof(Slot));
        strcpy(slots[i].name, name);
        slots[i].month = month;
        slots[i].day = day;
        slots[i].hour = hour;
        slots[i+1].name[0] = 0;
        slots[i+1].month = 0;
        slots[i+1].day = 0;
        slots[i+1].hour = 0;
    } else {
        printf("Error: date not valid.\n");
    }
    return slots;
}

//removes one entry from the list that matches the given parameters

int deleteEntry(Slot *slots, int month, int day, int hour){
    int i = 0;
    while(slots[i].name[0] != 0){
        if (slots[i].month == month && slots[i].hour == hour && slots[i].day == day){
            while(slots[i+1].name[0] != 0){
                slots[i].month = slots[i+1].month;
                slots[i].day = slots[i+1].day;
                slots[i].hour = slots[i+1].hour;
                strcpy(slots[i].name, slots[i+1].name);
                i++;
            }
            slots[i].name[0] = 0;
            slots[i].month = 0;
            slots[i].day = 0;
            slots[i].hour = 0;
            return 1;
        }
        i++;
    }
    printf("Error: entry does not exist.\n");
    return 0;
}

//the compare function used for qsort
//sorts from smallest to largest

int compareNum(const void* a, const void* b)
{
	const Slot *slota = a;
	const Slot *slotb = b;
    if (slota->month == slotb->month && slota->day == slotb->day){
        return slota->hour < slotb->hour? -1 : 1;
    } else if (slota->month == slotb->month && slota->day != slotb->day) {
        return slota->day < slotb->day? -1 : 1;
    } else {
        return slota->month < slotb->month? -1 : 1;
    }
}

// sorts the array with qsort and then prints it

void printCalendar(Slot *slots){
    int i = 0;
    while (slots[i].name[0] != 0) {
        i++;
    }
    qsort(slots, i, sizeof(Slot), compareNum);
    while (slots->name[0] != 0) {
        printf("%s %02d.%02d. at %02d\n", slots->name, slots->day, slots->month, slots->hour);
        slots++;
    }
}


//writes the array to a binary file

int saveCalendar(const Slot* slots, const char* filename)
{
	FILE *f = fopen(filename, "w");
    if (!f){
        return 1;
    }
    while(slots->name[0] != 0){
        fwrite(slots, sizeof(Slot), 1, f);
        if (ferror(f)){
            fclose(f);
            return 1;
        }
        slots++;
    }    
    fclose(f);
    return 0;
    
}

//reads a binary file and imports the contents to an array of slots that is then returned

Slot* loadCalendar(const char* filename)
{
	FILE *f = fopen(filename, "r");
    if (!f){
        Slot *array = malloc(sizeof(Slot));
        array[0].name[0]=0;
        array[0].month = 0;
        array[0].day = 0;
        array[0].hour = 0;
        printf("Error in reading file\n");
        return array;
    }
    Slot *array = malloc(sizeof(Slot));
    int i = -1;
    do {
        i++;
        array = realloc(array, sizeof(Slot) * (i+1));
        if(ferror(f)){
            array = realloc(array, sizeof(Slot));
            array[0].name[0] = 0;
            array[0].month = 0;
            array[0].day = 0;
            array[0].hour = 0;
            printf("Error in reading file\n");
            return array;
        }
    } while (fread(&array[i], sizeof(array[i]), 1, f) == 1);
    array[i].name[0] = 0;
    array[i].month = 0;
    array[i].day = 0;
    array[i].hour = 0;
    return array;
}

//the main function

int main(void){
    Slot *slots;
    slots = malloc(sizeof(Slot));
    slots[0].name[0] = 0;
    slots[0].month = 0;
    slots[0].day = 0;
    slots[0].hour = 0;
    doCommand(slots);
    return 0;
}