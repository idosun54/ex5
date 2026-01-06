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
  char ch[2];          
  ch[1] = '\0';  
  char *temp = NULL;
  int currentLen = 0;

  while(TRUE)
  {
    if (scanf("%c", &ch[0])!=1||ch[0]=='\n')
      break;


    if (temp != NULL) 
      currentLen = strlen(temp);
    else 
      currentLen = 0;
    
    char *next = (char*)realloc(temp, currentLen + 2);
    if (next == NULL) {
      free(temp);
      return NULL;
    }
    temp = next;

    if (currentLen == 0) 
     temp[0] = '\0';
    
    strcat(temp, ch);
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

//doesnt work
void expandDB()

{

  if(dbSize==0)

    {


       database=realloc(database ,sizeof(TVShow));
       database[0][0]=calloc(++dbSize, sizeof(TVShow**));
       return;

    }


  database=(TVShow***)realloc(database, ++dbSize*sizeof(TVShow**));

  for(int c=0; c<dbSize; c++)

  {


    database[dbSize][c]=malloc(sizeof(TVShow));


    database[c]=(TVShow**)realloc(database[c], dbSize*sizeof(TVShow*));


    database[c][dbSize-1]=NULL;

  }

  for(int r=0; r<dbSize; r++)

  {


    database[r][dbSize]=malloc(sizeof(TVShow));


    database[dbSize-1][r]=NULL;

  }
  dbSize++;
}

void shrinkDB()
{


}


//doesnt work
int validLength(char *length)
{  
    int nums[3] = {0, 0, 0}; 
    int i = 0;               
    int count = 0;           

    while (count < 3) {
        int digits = 0;
        int val = 0;

        while (length[i] != '\0' && (length[i] >= '0' && length[i] <= '9')) {
            val = val * 10 + (length[i] - '0');
            digits++;
            i++;
        }

        if (digits == 0 || digits > 2) {
            return 0;
        }
        
        nums[count] = val;
        count++;

        if (count < 3) {
            if (length[i] != ':') {
                return 0; 
            }
            i++; 
        }
    }

    if (length[i] != '\0') {
        return 0;
    }

    if (nums[0] > 99 || nums[1] > 59 || nums[2] > 59) {
        return 0;
    }

    return 1;
}

TVShow *findShow(char *temp)
{
  for(int r=0; r<dbSize; r++)
   for(int c=0; c<dbSize; c++)
  {
    if(database[r][c]!=NULL)
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

Episode *findEpisode(Season *sea, char *name)
{
  Episode *temp=sea->episodes;
  while(temp!=NULL)
  {
    if(strcmp(name, temp->name)==0)
     return temp;
    temp=temp->next;
  }
 return NULL;
}

//adds

//works?
void addShow()
{
  if(dbSize==0)
   expandDB();
   else
  if(database[dbSize-1][dbSize-1]!= NULL)
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
  if(dbSize==1)
  {
    TVShow *show=(TVShow*)malloc(sizeof(TVShow));
    show->name=(char*)malloc(strlen(temp)*sizeof(char));
    show->seasons=NULL;
    show->name=temp;
    database[dbSize-1][dbSize-1]=show;
    return;
  }

 for(int r=0; r<dbSize; r++)
  for(int c=0; c<dbSize; c++)
  {
    if(database[r][c]!=NULL)
    {
    int cmp=strcmp(temp, database[r][c]->name);
    if(cmp<0)
     {
       sortShows(r, c);
       TVShow *show=(TVShow*)malloc(sizeof(TVShow));
       show->name=(char*)malloc(strlen(temp)*sizeof(char));
       show->seasons=NULL;
       show->name=temp;
       database[r][c]=show;
       return;
     }
    }
    else
    {
    TVShow *show=(TVShow*)malloc(sizeof(TVShow));
    show->name=(char*)malloc(strlen(temp)*sizeof(char));
    show->seasons=NULL;
    show->name=temp;
    database[r][c]=show;
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
 S->episodes=NULL;
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

void addEpisode()
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
  if(S==NULL)
  {
   printf("Season not found.\n");
   free(temp);
   return;
  }
  printf("Enter the name of the episode:\n");
  temp=getString();
  Episode *ep=findEpisode(S,temp);
  if(ep!=NULL)
  {
   printf("Episode already exists.\n");
   free(temp);
   return;
  }
  ep=realloc(ep, sizeof(Season));
  ep->name=temp;
  printf("Enter the length (xx:xx:xx)\n");
  char *length=NULL;
  scanf("%s", length);
  while(!validLength(length))
  {
    printf("Invalid length, enter again:\n");
    scanf("%s", length);
  }
  ep->length=length;
  int pos;
  ep->next=NULL;
  printf("Enter the position:\n");
  scanf("%d", &pos);
  Episode *tempEP=S->episodes;
 for(int i=0; i<pos; i++)
 {
   if(i==0&&tempEP==NULL)
   {
     S->episodes=ep;
     return;
   }
   if(tempEP->next==NULL)
   {
    tempEP->next=ep;
    return;
   }
   tempEP=tempEP->next;
  }
  if(pos==0)
  {
    ep->next=tempEP;
    tempEP=ep;
    return;
    
  }
 ep->next=tempEP->next;
 tempEP->next=ep;
}

//prints
void printEpisode()
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
  Season *Sea=findSeason(TV, temp);
  if(Sea==NULL)
  {
  printf("Season not found.\n");
  free(temp);
  return;
  }
  printf("Enter the name of the episode:\n");
  temp=getString();
  Episode *Epi=findEpisode(Sea, temp);
  if(Epi==NULL)
  {
   printf("Episode not found.\n");
   free(temp);
   return;
  }
 free(temp);
 printf("Name: %s", Epi->name);
 printf("Length: %s", Epi->length);
}

void printShow()
{
 printf("Enter the name of the show:\n");
 char *temp=getString();
 if(findShow(temp)==NULL)
 {
  return;
 }
 TVShow *show=findShow(temp);
 free(temp);
 int count=0;
 int countEp=0;
 printf("Name: %s\n", show->name);
 printf("Seasons:\n");
 Season *Season=show->seasons;
 Episode *Epi=Season->episodes;
 while(Season!=NULL)
 {
   printf("    Season %d: %s\n", count, Season->name);
   while(Epi!=NULL)
   {
     printf("        Episode %d: %s (%s)\n", count, Epi->name, Epi->length);
     Epi=Epi->next;
     countEp++;
   }
   Season=Season->next;
   count++;
 }
}

void printArray()
{
  for(int r=0; r<dbSize; r++)
  {printf("\n");
   for(int c=0; c<dbSize; c++){
   if(database[r][c]==NULL)
    printf("[NULL]"); 
    else
     printf("%s", database[r][c]->name);
  }
}

}

//need to fix sorting
void deleteShow()
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
 freeShow(TV);
 TV=NULL;
}

void deleteSeason()
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
  Season *Sea=findSeason(TV, temp);
  if(Sea==NULL)
  {
  printf("Season not found.\n");
  free(temp);
  return;
  }
  Season *prevSeason=TV->seasons;
  while(prevSeason->next!=Sea)
  {
    prevSeason=prevSeason->next;
  }
  prevSeason->next=Sea->next;
  freeSeason(Sea);
}

void deleteEpisode()
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
  Season *Sea=findSeason(TV, temp);
  if(Sea==NULL)
  {
  printf("Season not found.\n");
  free(temp);
  return;
  }
  printf("Enter the name of the episode:\n");
  temp=getString();
  Episode *Epi=findEpisode(Sea, temp);
  if(Epi==NULL)
  {
  printf("Season not found.\n");
  free(temp);
  return;
  }
  Episode *prevEpi=Sea->episodes;
  while(prevEpi->next!=Epi)
  {
    prevEpi=prevEpi->next;
  }
  prevEpi->next=Epi->next;
  freeEpisode(Epi);
}

void freeShow(TVShow *TV)
{
 while(TV->seasons!=NULL)
 {
  Season *temp=TV->seasons;
  TV->seasons=TV->seasons->next;
  freeSeason(temp);
 }
 free(TV->name);
 free(TV);
}

void freeSeason(Season *S)
{
  while(S->episodes!=NULL)
  {
    Episode *temp=S->episodes;
    S->episodes=S->episodes->next;
    freeEpisode(temp);
  }
  free(S->name);
  free(S);
}

void freeEpisode(Episode *epi)
{
  free(epi->length);
  free(epi->name);
  free(epi->next);
}

void freeAll()
{
  for(int r=0; r<dbSize; r++)
   for(int c=0; c<dbSize; c++)
   {
    freeShow(database[r][c]);
   }
  free(database);
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
