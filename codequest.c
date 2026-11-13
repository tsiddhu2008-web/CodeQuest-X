#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_SIZE 60
#define ANSWER_SIZE 160
#define SCORE_FILE "codequest_x_progress.txt"
#define XP_PER_MISSION 10

typedef struct {
    const char *level, *topic, *question, *answer, *hint, *explanation;
} Mission;

const Mission missions[] = {
    {"LEVEL 1 - ROOKIE","Operators","Predict output: printf(\"%d\", 4 + 3 * 2);","10","Remember operator precedence.","Multiplication first: 4 + 6 = 10."},
    {"LEVEL 1 - ROOKIE","Control Flow","If x=8, what is printed by if (x > 5) printf(\"OPEN\");","OPEN","Check whether 8 is greater than 5.","The condition is true, so OPEN is printed."},
    {"LEVEL 2 - EXPLORER","Loops","How many times does for (int i=0; i<5; i++) execute?","5","Count from zero.","The loop runs for 0, 1, 2, 3 and 4: five times."},
    {"LEVEL 2 - EXPLORER","Strings","Which function returns the length of a C string?","strlen","It is declared in string.h.","strlen() counts characters before the null character."},
    {"LEVEL 3 - HACKER","Functions","Which return type is used when a function returns no value?","void","It is a C keyword.","A void function does not return a value."},
    {"LEVEL 3 - HACKER","Arrays","What is the index of the first element in a C array?","0","C uses zero-based indexing.","The first array element is at index 0."},
    {"LEVEL 4 - MASTER","File Handling","Which C function is used to open a file?","fopen","It returns a FILE pointer.","fopen() opens a file in a specified mode."},
    {"LEVEL 4 - MASTER","Pointers","Which operator gives the address of a variable?","&","It is called address-of.","The & operator obtains a variable's memory address."},
    {"LEVEL 4 - MASTER","Operators","What is the result of 5 + 3 * 2 - 8 / 4?","9","Multiplication and division have higher precedence.","5 + 6 - 2 = 9."},
    {"LEVEL 5 - LEGEND","Strings","Which character marks the end of a C string?","\\0","It is the null character.","C strings end with the null terminator, written as \\0."}
};
#define MISSION_COUNT ((int)(sizeof(missions)/sizeof(missions[0])))

void trim(char *s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}
void normalize(char *s) {
    char *p=s;
    while (isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s,p,strlen(p)+1);
    size_t n=strlen(s);
    while (n && isspace((unsigned char)s[n-1])) s[--n]='\0';
    for (size_t i=0;s[i];i++) s[i]=(char)tolower((unsigned char)s[i]);
}
void divider(void) { puts("============================================================"); }
void header(void) {
    puts("\n");
    divider();
    puts("             C O D E Q U E S T   X");
    puts("             NEXT-GEN C PROGRAMMING ARENA");
    divider();
}
void save_progress(const char *name,int xp,int cleared) {
    FILE *fp=fopen(SCORE_FILE,"a");
    if (!fp) { puts("WARNING: Could not save progress."); return; }
    fprintf(fp,"%s|%d|%d\n",name,xp,cleared);
    fclose(fp);
    puts("SYSTEM: Progress saved successfully.");
}
void leaderboard(void) {
    FILE *fp=fopen(SCORE_FILE,"r");
    char row[240],name[NAME_SIZE];
    int xp,cleared;
    puts("\n                 HALL OF OPERATORS");
    divider();
    printf("%-22s %-10s %-12s\n","OPERATOR","XP","MISSIONS");
    divider();
    if (!fp) { puts("No records yet. Complete your first mission!"); return; }
    while (fgets(row,sizeof(row),fp))
        if (sscanf(row,"%59[^|]|%d|%d",name,&xp,&cleared)==3)
            printf("%-22s %-10d %d/%d\n",name,xp,cleared,MISSION_COUNT);
    fclose(fp);
}
void show_rank(int xp) {
    if (xp>=80) puts("RANK: LEGEND");
    else if (xp>=50) puts("RANK: MASTER");
    else if (xp>=30) puts("RANK: HACKER");
    else if (xp>=10) puts("RANK: EXPLORER");
    else puts("RANK: ROOKIE");
}
void arena(const char *name) {
    char answer[ANSWER_SIZE],expected[ANSWER_SIZE];
    int xp=0,cleared=0;
    puts("\nACCESS GRANTED. ENTER THE ARENA.");
    puts("Correct answers earn 10 XP. Type your answer and press Enter.");
    for (int i=0;i<MISSION_COUNT;i++) {
        divider();
        printf("%s\nMISSION %02d | TOPIC: %s\n",missions[i].level,i+1,missions[i].topic);
        printf("CHALLENGE: %s\nHINT: %s\nANSWER > ",missions[i].question,missions[i].hint);
        if (!fgets(answer,sizeof(answer),stdin)) answer[0]='\0';
        trim(answer);
        strncpy(expected,missions[i].answer,sizeof(expected)-1);
        expected[sizeof(expected)-1]='\0';
        normalize(answer); normalize(expected);
        if (strcmp(answer,expected)==0) {
            puts("[+] CORRECT | +10 XP");
            xp+=XP_PER_MISSION; cleared++;
        } else printf("[-] NOT QUITE | Expected answer: %s\n",missions[i].answer);
        printf("DEBRIEF: %s\n",missions[i].explanation);
    }
    divider();
    puts("                  ARENA RESULTS");
    divider();
    printf("OPERATOR: %s\nMISSIONS CLEARED: %d/%d\nTOTAL XP: %d\nCOMPLETION: %d%%\n",
           name,cleared,MISSION_COUNT,xp,cleared*100/MISSION_COUNT);
    show_rank(xp);
    if (cleared==MISSION_COUNT) puts("PERFECT RUN! You are a CodeQuest X LEGEND.");
    else if (cleared>=7) puts("Excellent work, operator!");
    else if (cleared>=4) puts("Good run. Keep training!");
    else puts("Training required. Try the arena again!");
    save_progress(name,xp,cleared);
}
void guide(void) {
    puts("\nCODEQUEST X SYSTEM GUIDE");
    divider();
    puts("1. Enter the arena and solve ten C programming missions.");
    puts("2. Every correct answer earns 10 XP.");
    puts("3. Your rank is based on XP earned in that run.");
    puts("4. Progress is appended to a local text file.");
    puts("5. Use Hall of Operators to view saved run records.");
    puts("\nAUTO-EVALUATION: This prototype compares short answers/output.");
    puts("It does not compile or execute arbitrary student-written code.");
}
int main(void) {
    char name[NAME_SIZE],input[24];
    int choice=0;
    header();
    printf("INITIALIZE OPERATOR NAME: ");
    if (!fgets(name,sizeof(name),stdin)) return 0;
    trim(name);
    if (!strlen(name)) strcpy(name,"Operator");
    do {
        header();
        printf("OPERATOR: %s\n",name);
        puts("[1] ENTER THE ARENA");
        puts("[2] HALL OF OPERATORS");
        puts("[3] SYSTEM GUIDE");
        puts("[4] EXIT SYSTEM");
        divider();
        printf("SELECT MODULE > ");
        if (!fgets(input,sizeof(input),stdin)) break;
        choice=atoi(input);
        switch(choice) {
            case 1: arena(name); break;
            case 2: leaderboard(); break;
            case 3: guide(); break;
            case 4: puts("\nSESSION TERMINATED. KEEP BUILDING, OPERATOR."); break;
            default: puts("\nERROR: Choose a number from 1 to 4.");
        }
        if (choice!=4) {
            printf("\nPress Enter to return to dashboard...");
            if (!fgets(input,sizeof(input),stdin)) break;
        }
    } while(choice!=4);
    return 0;
}