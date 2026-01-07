/***********
 ID:idoshem
 NAME:Ido Shemesh
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Episode {
    char *name;
    char *length;
    struct Episode *next;
} Episode;

typedef struct Season {
    char *name;
    Episode *episodes;
    struct Season *next;
} Season;

typedef struct TVShow {
    char *name;
    Season *seasons;
} TVShow;

//My defines
#define TRUE 1
#define FALSE 0
#define MAX_SEC_MIN 59
#define MAX_HOURS 99
#define TIME_FORMAT_NUM 3
#define DIGITS 2


TVShow ***database = NULL;
int dbSize = 0;

char *getString();
//int *getInt();

int validLength(char *s);
//int countShows();

void shrinkDB();
void expandDB();

void freeEpisode(Episode *e);
void freeSeason(Season *s);
void freeShow(TVShow *show);
void freeAll();

TVShow *findShow(char *name);
Season *findSeason(TVShow *show, char *name);
Episode *findEpisode(Season *season, char *name);

void addShow();
void addSeason();
void addEpisode();

void deleteShow();
void deleteSeason();
void deleteEpisode();

void printEpisode();
void printShow();
void printArray();

void sortShows(int, int);

// --- Given Menu Functions ---
void addMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Add a TV show\n");
    printf("2. Add a season\n");
    printf("3. Add an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: addShow(); break;
        case 2: addSeason(); break;
        case 3: addEpisode(); break;
    }
}

void deleteMenu() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Delete a TV show\n");
    printf("2. Delete a season\n");
    printf("3. Delete an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: deleteShow(); break;
        case 2: deleteSeason(); break;
        case 3: deleteEpisode(); break;
    }
}

void printMenuSub() {
    int choice;
    printf("Choose an option:\n");
    printf("1. Print a TV show\n");
    printf("2. Print an episode\n");
    printf("3. Print the array\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: printShow(); break;
        case 2: printEpisode(); break;
        case 3: printArray(); break;
    }
}

void mainMenu() {
    printf("Choose an option:\n");
    printf("1. Add\n");
    printf("2. Delete\n");
    printf("3. Print\n");
    printf("4. Exit\n");
}

// --- All My Functions ---
char *getString() {
    char ch;
    char *temp = NULL;
    int currentLen = 0;
    while (scanf("%c", &ch) == 1 && ch != '\n') {
        char *next = realloc(temp, currentLen + 2);
        if (!next) { 
        free(temp); 
        return NULL; 
        }
        temp = next;
        temp[currentLen++] = ch;
        temp[currentLen] = '\0';
    }
    return temp;
}

int validLength(char *length) {
    if (length == NULL) 
        return 0;

    int nums[TIME_FORMAT_NUM] = {0, 0, 0}; 
    int i = 0;               
    int count = 0;           

    while (count < TIME_FORMAT_NUM) {
        int digits = 0;
        int val = 0;

        while (length[i] >= '0' && length[i] <= '9') {
            val = val * 10 + (length[i] - '0');
            digits++;
            i++;
        }

        if (digits != DIGITS)
            return 0;

        nums[count] = val;
        count++;


        if (count < TIME_FORMAT_NUM) {
            if (length[i] != ':') 
                return 0;
            i++; 
        }
    }

    if (length[i] != '\0') 
        return 0;

    if (nums[0] > MAX_HOURS || nums[1] > MAX_SEC_MIN || nums[2] > MAX_SEC_MIN) 
        return 0;

    return 1;
}

void sortShows(int r, int c) {
    for (int i = dbSize - 1; i >= r; i--) {
        // Shift within the row
        for (int j = dbSize - 1; j > 0; j--) {
  
            if (i == r && j <= c) break;
            database[i][j] = database[i][j - 1];
        }
        
        if (i > r) {
            database[i][0] = database[i - 1][dbSize - 1];
        }
    }
    database[r][c] = NULL;
}
// --- Expand and shrink ---
void expandDB() {
    int oldSize = dbSize;
    int newSize = dbSize + 1;

    TVShow **tempList = malloc(oldSize * oldSize * sizeof(TVShow *));
    int count = 0;
    for (int r = 0; r < oldSize; r++) {
        for (int c = 0; c < oldSize; c++) {
            tempList[count++] = database[r][c];
        }
    }
    
    database = realloc(database, newSize * sizeof(TVShow **));
    for (int i = 0; i < oldSize; i++) {
        free(database[i]); 
    }
    for (int i = 0; i < newSize; i++) {
        database[i] = calloc(newSize, sizeof(TVShow *));
    }

    dbSize = newSize;
    
    for(int i = 0; i < count; i++) {
        database[i / dbSize][i % dbSize] = tempList[i];
    }
    free(tempList);
}

void shrinkDB() {
    if (dbSize <= 1) 
     return;

    int count = 0;
    for (int r = 0; r < dbSize; r++) {
        for (int c = 0; c < dbSize; c++) {
            if (database[r][c] != NULL) count++;
        }
    }

    int newSize = dbSize - 1;
    if (count <= newSize * newSize) {
     TVShow **tempList = malloc(count * sizeof(TVShow *));
     int k = 0;
        for (int r = 0; r < dbSize; r++) {
         for (int c = 0; c < dbSize; c++) {
          if (database[r][c] != NULL) {
           tempList[k++] = database[r][c];
            }
          }
        }

        for (int i = 0; i < dbSize; i++) {
            free(database[i]);
        }

        TVShow ***tempDB = realloc(database, newSize * sizeof(TVShow **));
        if (tempDB != NULL) database = tempDB;
        
        dbSize = newSize;

        for (int i = 0; i < dbSize; i++) {
            database[i] = calloc(dbSize, sizeof(TVShow *));
        }

        for (int i = 0; i < count; i++) {
            database[i / dbSize][i % dbSize] = tempList[i];
        }
        free(tempList);
    }
}

// --- Search Functions ---

TVShow *findShow(char *name) {
    if (!name||name[0]=='\0') 
     return NULL;
    for (int r = 0; r < dbSize; r++) {
        for (int c = 0; c < dbSize; c++) {
            if (database[r][c] && strcmp(database[r][c]->name, name) == 0)
                return database[r][c];
        }
    }
    return NULL;
}

Season *findSeason(TVShow *show, char *name) {
    Season *curr = show->seasons;
    while (curr) {
        if (strcmp(curr->name, name) == 0) 
        return curr;

    curr = curr->next;
    }
    return NULL;
}

Episode *findEpisode(Season *sea, char *name) {
    Episode *curr = sea->episodes;
    while (curr) {
        if (strcmp(curr->name, name) == 0) 
         return curr;

    curr = curr->next;
    }
    return NULL;
}

// --- Add Functions ---
void addShow() {

    if (dbSize == 0 || database[dbSize - 1][dbSize - 1] != NULL) {
        expandDB();
    }

    printf("Enter the name of the show:\n");
    char *temp = getString();
    if (findShow(temp) != NULL) {
        printf("Show already exists.\n");
        free(temp);
        return;
    }

    TVShow *newShow = (TVShow*)malloc(sizeof(TVShow));
    newShow->name = temp;
    newShow->seasons = NULL;


    for (int r = 0; r < dbSize; r++) {
        for (int c = 0; c < dbSize; c++) {
            if (database[r][c] != NULL) {
                if (strcmp(newShow->name, database[r][c]->name) < 0) {
                    sortShows(r, c); 
                    database[r][c] = newShow;
                    return;
                }
            } 

            else {
                database[r][c] = newShow;
                return;
            }
        }
    }
}

void addSeason() {
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *tv = findShow(showName);
    free(showName);
    if (!tv) { 
     printf("Show not found.\n"); 
     return; 
    }

    printf("Enter the name of the season:\n");
    char *seaName = getString();
    if (findSeason(tv, seaName)) { 
     printf("Season already exists.\n"); 
     free(seaName); 
     return; 
    }

    Season *newS = malloc(sizeof(Season));
    newS->name = seaName;
    newS->episodes = NULL;
    
    printf("Enter the position:\n");
    int pos; 
    scanf("%d", &pos); 
    getchar();

    if (!tv->seasons) {
        newS->next = tv->seasons;
        tv->seasons = newS;
    } else {
        Season *curr = tv->seasons;
        for (int i = 0; i < pos - 1 && curr->next; i++) 
         curr = curr->next;
    
        newS->next = curr->next;
        curr->next = newS;
    }
}

void addEpisode() {
    printf("Enter the name of the show:\n");
    char *sn = getString(); 
    TVShow *tv = findShow(sn); 
    free(sn);
    if (!tv) { 
    printf("Show not found.\n"); 
    return;  
    }

    printf("Enter the name of the season:\n");
    char *sen = getString();
    Season *sea = findSeason(tv, sen); 
    free(sen);
    if (!sea) { 
    printf("Season not found.\n"); 
    return; 
    }

    printf("Enter the name of the episode:\n");
    char *en = getString();
    if (findEpisode(sea, en)) { 
    printf("Episode already exists.\n"); 
    free(en); 
    return; 
    }

    printf("Enter the length (xx:xx:xx):\n");
    char *len = getString();
    while (!validLength(len)) {
        printf("Invalid length, enter again:\n");
        free(len); len = getString();
    }

    Episode *newE = malloc(sizeof(Episode));
    newE->name = en; 
    newE->length = len;
    
    printf("Enter the position:\n");
    int pos; scanf("%d", &pos); getchar();

    if (!sea->episodes) {
        newE->next = sea->episodes;
        sea->episodes = newE;
    } else {
        Episode *curr = sea->episodes;
        for (int i = 0; i < pos - 1 && curr->next; i++) 
         curr = curr->next;

        newE->next = curr->next;
        curr->next = newE;
    }
}

// --- Print Functions ---
void printShow() {
    printf("Enter the name of the show:\n");
    char *name = getString();
    TVShow *show = findShow(name);
    free(name);
    if (!show) { 
    printf("Show not found.\n"); 
    return; 
    }

    printf("Name: %s\n", show->name);
    Season *s = show->seasons;
    int countS = 0;
    printf("Seasons:\n");
    while (s) {
        printf("    Season %d: %s\n", countS++, s->name);
        Episode *e = s->episodes;
        int countE = 0;
        while (e) {
            printf("        Episode %d: %s (%s)\n", countE++, e->name, e->length);
            e = e->next;
        }
        s = s->next;
    }
}

void printArray() {
  for(int r=0; r<dbSize; r++)
  {printf("\n");
   for(int c=0; c<dbSize; c++){
   if(database[r][c]==NULL)
    printf("[NULL] "); 
    else
     printf("[%s] ", database[r][c]->name);
  }
}
 printf("\n");
}

void printEpisode() {
    printf("Enter the name of the show:\n");
    char *sn = getString(); 
    TVShow *tv = findShow(sn); 
    free(sn);
    if (!tv) 
     return;
     
    printf("Enter the name of the season:\n");
    char *sen = getString(); 
    Season *sea = findSeason(tv, sen); 
    free(sen);
    if (!sea) 
     return;

    printf("Enter the name of the episode:\n");
    char *en = getString(); 
    Episode *ep = findEpisode(sea, en); 
    free(en);
    if (ep) 
     printf("Name: %s Length: %s\n", ep->name, ep->length);
}

// --- Delete & Free Functions ---
void freeEpisode(Episode *e) {
    if (!e) 
     return;
    free(e->name); 
    free(e->length); 
    free(e);
}

void freeSeason(Season *s) {
    if (!s) 
     return;
    Episode *curr = s->episodes;
    while (curr) { 
     Episode *next = curr->next; freeEpisode(curr); curr = next; 
    }
    free(s->name); 
    free(s);
}

void freeShow(TVShow *s) {
    if (!s) 
     return;
    Season *curr = s->seasons;
    while (curr) {
     Season *next = curr->next; freeSeason(curr); curr = next; 
    }
    free(s->name); 
    free(s);
}

void deleteShow() {
    printf("Enter the name of the show:\n");
    char *name = getString();
    int foundR = -1, foundC = -1;
    for (int r = 0; r < dbSize; r++) {
        for (int c = 0; c < dbSize; c++) {
            if (database[r][c] && strcmp(database[r][c]->name, name) == 0) {
                foundR = r; 
                foundC = c; 
                break;
            }
        }
    }
    free(name);
    if (foundR == -1) 
    {
        printf("Show not found.\n");
        return;
    }
    freeShow(database[foundR][foundC]);

    for (int i = foundR * dbSize + foundC; i < dbSize * dbSize - 1; i++) {
        database[i / dbSize][i % dbSize] = database[(i + 1) / dbSize][(i + 1) % dbSize];
    }
    database[dbSize - 1][dbSize - 1] = NULL;
    shrinkDB();
}

void deleteSeason() {
    printf("Enter the name of the show:\n");
    char *sn = getString(); 
    TVShow *tv = findShow(sn); 
    free(sn);
    if (!tv)
    {
      printf("Show not found.\n");
      return;
    }
    printf("Enter the name of the season:\n");
    char *sen = getString();
    Season *curr = tv->seasons, *prev = NULL;
    while (curr && strcmp(curr->name, sen) != 0) { 
     prev = curr; curr = curr->next; 
    }
    free(sen);
    if (!curr) 
    {
      printf("Season not found.\n");
      return;
    }
    if (!prev) 
     tv->seasons = curr->next;
    else 
     prev->next = curr->next;
    freeSeason(curr);
}

void deleteEpisode() {
    printf("Enter the name of the show:\n");
    char *sn = getString(); 
    TVShow *tv = findShow(sn); free(sn);
    if (!tv)
    {
      printf("Show not found.\n");
      return;
    }
    printf("Enter the name of the season:\n");
    char *sen = getString(); 
    Season *sea = findSeason(tv, sen); 
    free(sen);
    if (!sea)
    {
      printf("Season not found.\n");
      return;
    }
    printf("Enter the name of the episode:\n");
    char *en = getString();
    Episode *curr = sea->episodes, *prev = NULL;
    while (curr && strcmp(curr->name, en) != 0) { 
     prev = curr; curr = curr->next; 
     }
    free(en);
    if (!curr) 
    {
      printf("Episode not found.\n");
      return;
    }
    if (!prev) 
     sea->episodes = curr->next;
    else 
     prev->next = curr->next;
    freeEpisode(curr);
}

void freeAll() {
    for (int r = 0; r < dbSize; r++) {
        for (int c = 0; c < dbSize; c++) 
        freeShow(database[r][c]);
        free(database[r]);
    }
    free(database);
}

// --- Main ---
int main() {
    int choice;
    do {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: addMenu(); break;
            case 2: deleteMenu(); break;
            case 3: printMenuSub(); break;
            case 4: freeAll(); break;
        }
    } while(choice != 4);
    return 0;
}