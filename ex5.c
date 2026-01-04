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

#define TRUE 1
#define FALSE 0

TVShow ***database = NULL;
int dbSize = 0;

char *getString();

int validLength(char *s);
int countShows();

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

//Given functions 
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


//My own functions

char *getString()
{
  char ch[1];
  char *temp=NULL;
  while(TRUE)
  {
    scanf("%c", &ch[0]);
    if(ch[0]=='\0')
     break;
    temp=(char*)realloc(temp, strlen(temp)*sizeof(char)+sizeof(char));
    strcat(temp, ch);
    continue;
  }
  return temp;
}

void sortShows(int r, int c) 
{
    TVShow *flatPtr = (TVShow *)database;

    int totalElements = dbSize * dbSize;
    int targetIdx = (r * dbSize) + c;

    for (int i = totalElements - 1; i > targetIdx; i--) {
        flatPtr[i] = flatPtr[i - 1];
    }

    database[r][c] = NULL; 
}

TVShow *findShow(char *temp)
{
  for(int r=0; r<dbSize; r++)
   for(int c=0; c<dbSize; c++)
  {
    if(strcmp(temp, database[r][c]->name)==0)
     {
       return database[r][c];
     }
  }
  return NULL;
}

Season *findSeason(TVShow *show, char *name)
{
  Season *temp=show->seasons;
  while(temp!=NULL)
  {
    if(strcmp(name, temp->name)==0)
    {
      return temp;
    }
   temp=temp->next;
  }
  return NULL;
}

void addShow()
{
  if(database[dbSize][dbSize]!= NULL)
    {
      expandDB();
    }
  printf("Enter the name of the show:\n");
  char *temp=getString();
 if(findShow(temp)!=NULL)
 {
  printf("Show already exists.\n");
  free(temp);
  return;
 }
 
 for(int r=0; r<dbSize; r++)
  for(int c=0; c<dbSize; c++)
  {
    int cmp=strcmp(temp, database[r][c]->name);
    if(cmp<0)
     {
       sortShows(r, c);
       TVShow *show=NULL;
       show->name=(char*)malloc(strlen(temp)*sizeof(char));
       database[r][c]=show;
       return;
     }
  }
}

void addSeason()
{
  printf("Enter the name of the show:\n");
  char *temp=getString();
  TVShow *TV=findShow(temp); 
  if(TV==NULL)
  {
   printf("Show not found.\n");
   free(temp);
   return;
  }
  printf("Enter the name of the season:\n");
  temp=getString();
  Season *S=findSeason(TV, temp);
  if(S!=NULL)
  {
    printf("Season already exists.\n");
    free(temp);
    return;
  }
 printf("Enter the position:\n");
 int pos;
 S=realloc(S, sizeof(Season));
 S->name=temp;
 S->next=NULL;
 free(temp);
 scanf("%d", &pos);
 Season *tempS=TV->seasons;
 for(int i=0; i<pos; i++)
 {
   if(i==0&&tempS==NULL)
   {
    TV->seasons=S;
    return;
   }
   if(tempS->next==NULL)
   {
    tempS->next=S;
    return;
   }
   tempS=tempS->next;
  }
  if(pos==0)
  {
    S->next=tempS;
    tempS=S;
    return;
  }
 S->next=tempS->next;
 tempS->next=S;
}


//main
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
    } while (choice != 4);
    return 0;
}
