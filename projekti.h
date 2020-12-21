#ifndef PROJECT_H
#define PROJECT_H

//struct for one slot:

typedef struct {
    char name[80];
    int month;
    int day;
    int hour;
} Slot;

//the functions used, see what they do in the .c file.

void doCommand(Slot *slots);
Slot* addEntry(Slot *slots, char * name, int month, int day, int hour);
int deleteEntry(Slot *slots, int month, int day, int hour);
int compareNum(const void* a, const void* b);
void printCalendar(Slot *slots);
int saveCalendar(const Slot* slots, const char* filename);
Slot* loadCalendar(const char* filename);



#endif