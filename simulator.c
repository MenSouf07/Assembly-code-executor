/***************************************************************************************************************


				PROJET ARCHITECTURE L2 : Camille JOUANET & Mendel SOUFFIR

		--- Partie 0 : Modules, variables globales
		--- Partie 1 : Traduction
		--- Partie 2 : Execution du language machine
		--- Partie 7 : Programme principal (main)

***************************************************************************************************************/


/***************************************************************************************************************

		PARTIE 0 : Modules, variables globales

	--- module
	--- variable : list, hexa

***************************************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define N 300

char* list[]={"push","push#","ipush","pop","ipop","dup","op","jmp","jpz","rnd","read","write","call","ret","halt"};
char* hexa[]={"00","01","02","03","04","05","06","07","08","09","0a","0b","0c","0d","0e"};


/***************************************************************************************************************

		PARTIE 1 : Traduction

	--- estAlphaNumeriqueOuUnderscore
	--- egalite
	--- copie
	--- in
	--- est_entier
	--- puissance
	--- copieFichier
	--- zero
	--- numligne
	--- trad_entier
	--- trad
	--- etiquette
	--- compte_etiquette

***************************************************************************************************************/

//Prend une etiquette en argument et renvoie 1 si elle est composé de caractère alphanumerique ou du caractere souligne. Renvoie 0 sinon.
int estAlphanumeriqueOuUnderscore(char* ch) {
  for (int i=0;i<strlen(ch)-1;i++){
    char c=ch[i];
    if(!((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (('0' <= c) && (c <= '9')) || (c == '_'))) {
      printf("-------------     %c    ----------\n",c);
		  return 0;
	  }
  }
	return 1;
}

//renvoie 1 si les 2 chaines passees en argument sont egales et 0 sinon
int egalite(char a[],char b[]){
  if (strlen(a)!=strlen(b)){
    return 0;
  }else {
    for (int i=0; i<strlen(a); i++){
      if (a[i]!=b[i]){
        return 0;
      }
    }
  }
  return 1;
}

//prend en argument une etiquette et renvoie une chaine de caractere identique mais sans les 2 points :
char* copie(char *str) {
  char *p = malloc(strlen(str));
  char *tmp = p;
  while (*str != ':'){
    *p++=*str++;
  }
  *p='\0';
  return tmp;
}

//renvoie 1 si le caractere c est dans la chaine de caractere ch et 0 sinon (l est la taille de ch)
int in(char c, char ch[], int l){
  for (int i=0; i<l ; i++){
    if (c==ch[i]){
      return 1;
    }
  }
  return 0;
}

//renvoie 1 si la chaine de caractere ch est composé de chiffre et 0 sinon
int est_entier(char ch[]){
    char chiffre[]={'0','1','2','3','4','5','6','7','8','9'};
    int cpt=0;
    for (int i=0; i<strlen(ch); i++){
        if (in(ch[i],chiffre,10)==1){
            cpt++;
        }
        /*for (int k=0 ; k<10 ; k++){
            if (ch[i]==chiffre[k]){
                cpt++;
            }
        }*/
    }
    if (ch[0]=='-'){ cpt++;}
    if (cpt==strlen(ch)){
        return 1;
    }
    return 0;
}

//renvoie a puissance b (equivalent de la fonction pow)
long puissance(int a, int b){
  if (b==0){
    return 1;
  }else{
    return a*puissance(a,b-1);
  }
}

//Cree un fichier appelé "copie.txt" qui est le même que celui passé en argument mais sans les commentaires ;
int copieFichier(char* nom){
	FILE* f=fopen(nom,"r");
    if (f==NULL){      printf("ERROR --- Aucun fichier ne porte le nom %s \n", nom); return 0;    }
 	FILE* fp=fopen("copie.txt","w");
	char c=fgetc(f);
	int cpt =0;
	do {
        if (c==';'){    cpt=1;    }
        if ((c=='\n')||(c=='\r')){    cpt=0;    }
        if (cpt==0){    fprintf(fp,"%c",c);    }
        c=fgetc(f);
    }while (c!=EOF);
  fclose(f);
  fclose(fp);
  return 1 ;
}

//modifie nb et p tel que p contiennt les zeros necessaire pour que nb soit ecrit sur 8 caractères
int zero(long long *nb, char p[]){
  *nb=*nb%puissance(16,7);
  if (*nb<0){return 1;}
  if (*nb==0){
    for (int k=0; k<7;k++){
      p[k]='0';
      p[k+1]='\0';
    }
    return 0;
  }
  for (int i=0; i<7;i++){
    if ( (*nb>=puissance(16,i)) && (*nb<puissance(16,i+1)) ){
      for (int k=0; k<7-i;k++){
        p[k]='0';
        p[k+1]='\0';
      }
      return 0;
    }
  }
  return 0;
}

//gere le compteur de ligne et renvoie 1 si le compteur a été incrémenté(ou si on arrive a la fin du fichier) et 0 sinon
int numligne(int* ligne, FILE* f){
    int cpt = 0, r =0 ;
    char c;
    do {
        do{
            c=fgetc(f);
        }while ((c==' ')||(c=='\t'));
        if ((c=='\n')||(c=='\r')){
            (*ligne)++;
            cpt++;
            r=1;
        }if (c==EOF){
            (*ligne)-=cpt-1;
            return 1;

        }
    }while ((c==' ')||(c=='\t')||(c=='\n')||(c=='\r'));
	fseek(f,-(cpt+1),SEEK_CUR);
	return r;
}

//s'occupe de traduire les valeurs numériques et de les ecrire dans le fichier hexa, renvoie est_entier(instr)
int trad_entier(FILE* f_dest, FILE* f_orig, char instr[]){
  int p=strlen(instr);
  long long nb;
  if (!(est_entier(instr))){
    return 0;
  }else{
    fseek(f_orig,-p,SEEK_CUR);
    fscanf(f_orig,"%lld",&nb);
    char p[8];
    int a=zero(&nb,p);
    if (a==0){
      fprintf(f_dest,"%s%llx\n",p,nb);
    }
    else if (a==1){
      int l=nb;
      fprintf(f_dest,"%x\n",l);
    }
    return 1;
  }
}

//s'occupe de traduire le fichier et d'en ecrire la traduction dans le fichier hexa
int trad(FILE* f_dest,FILE* f_orig, char instr[], char** eti_nom, int* eti_ligne, int* ligne,int cardinal){
  int p=strlen(instr)-1;
  int ncec = numligne(ligne,f_orig);
  if (instr[p]==':'){                                               //si instr est une etiquette on ne la traduite pas et on passe au mot suivant
    if (ncec==1) {                                                  //verification de format
          printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne)-1);
          return -1;
      }
    return 0;
  }else{
    long long nb;
    for (int k=0; k<15;k++){
      if (egalite(instr,list[k])==1){
        fprintf(f_dest,"%s ", hexa[k]);
        if ((k==7)||(k==8)||(k==12)){                               //intr = jmp ou jpz ou call
            if (ncec==1) {                                          //verification de format
                printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne)-1);
                return -1;
            }
            fscanf(f_orig,"%s",instr);
            for (int i=0; i<(cardinal) ; i++){
                if (egalite(instr,eti_nom[i])==1){
                    nb=eti_ligne[i]-(*ligne)-1;
                    char p[8];
                    int a=zero(&nb,p);
                    if (a==0){fprintf(f_dest,"%s%llx\n",p,nb);}
                    else if (a==1){
                        int l=nb;
                        fprintf(f_dest,"%x\n",l);}
                    else if (a==-1){return -1;}
                    ncec = numligne(ligne,f_orig);
                    if (!ncec) {                                    //verification de format
                        printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne));
                        return -1;
                    }
                    return 0;
                }
          }
          int e = trad_entier(f_dest, f_orig, instr);
          if (!e) {                                                 //si la donnee apres un jump n'est ni une valeur numerique ni une etiquette alors c'est une etiquette non initialise ou une erreur
            printf("ERROR---Vous essayez d'aller a une etiquette non initialisee\n\tLe mot qui pose probleme est \"%s\" present a la ligne %d\n\n", instr, *ligne);
          return -1;
          }
          ncec = numligne(ligne,f_orig);
          if (!ncec) {                                              //verification de format
            printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne));
            return -1;
                }
          return 0;


        }else if ((k==2)||(k==4)||(k==5)||(k==13)||(k==14)){        //instr = iPush ou iPop ou dup ou ret ou halt
            if (ncec==0) {                                          //verification de format
                printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne));
                return -1;
            }fprintf(f_dest,"00000000\n");                          //ces instructions n'attendent pas de données donc on remplit la ligne de 0
            return 0;
        }else{                                                      //instr= push ou push# ou pop ou op ou rnd ou read ou write
            if (ncec==1) {                                          //verification de format
                printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne)-1);
                return -1;
            }
            fscanf(f_orig,"%s",instr);
            int e = trad_entier(f_dest, f_orig, instr);
            if (!e) {                                               //ces instructions attendent specifiquement une valeur numérique
                printf("ERROR---Vous avez mis une chaine de caractere au lieu d'un entier\n\tle mot qui pose probleme est \"%s\" present a la ligne %d\n\n", instr, *ligne);
                return -1;
            }
            ncec=numligne(ligne,f_orig);
            if (!ncec) {                                            //verification de format
                printf("ERROR---le format de la ligne numero %d est incorrecte \n\t(format attendu : 1 etiquette optionnel, 1 instruction et 1 donnee) \n",(*ligne));
                return -1;
            }
            return 0;
        }
      }
    }
    printf("ERROR---Il y a un mot qui n'est ni une instruction, ni une valeur, ni une etiquette\n\tle mot qui pose probleme est \"%s\" present a la ligne %d\n\n",instr,*ligne);
    return -1;
  }
  return 0;
}    //renvoie code d'erreur

//permet de remplir le tableau regroupant les etiquettes du fichier
int etiquette(FILE* f, char instr[],char* eti_nom[], int eti_ligne[], int* ligne, int* cardinal, int* res){
    char alphabet[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };
    int p=strlen(instr)-1;
    if (instr[p]==':'){                     //permet de savoir si la chaine de caractere contenue dans instr est une etiquette
        if (strlen(instr)==1){
          printf("ERROR --- Vous avez certainement mis un espace entre le nom de votre etiquette et les \':\' a la ligne %d\n",*ligne);
          return -1;
        }
        if ((in(instr[0],alphabet,52)==1)&&(estAlphanumeriqueOuUnderscore(instr))){      //permet d'etre sur que l'etiquette ne commence pas par un chiffre
            if ((*res)==0){                     //permet d'etre sur que l'etiquette est au debut d'une ligne
                char* ch=copie(instr);
                for(int i=0; i<(*cardinal); i++){
                    if(egalite(ch,eti_nom[i])==1){
                        printf("ERROR --- Il y a 2 etiquettes de meme nom aux lignes %d et %d\n\n",eti_ligne[i],*ligne);
                        return -1;
                    }/*
                    if((*ligne)==eti_ligne[i]){
                        printf("ERROR --- Il y a 2 etiquettes sur la ligne %d\n\n", *ligne);
                        return -1;
                    }*/
                }
                eti_nom[*cardinal]=ch;
                eti_ligne[(*cardinal)++]=(*ligne);
            }else {
                printf("ERROR --- Une etiquette ne peut etre initialisee qu'au debut d'une ligne\n\tCe n'est pas le cas a la ligne %d pour le mot \"%s\"\n\n",*ligne, instr);
                return -1;
            }
        }else {
            printf("ERROR --- Une etiquette doit commencer par une lettre et etre compose de caractere uniquement alphanumerique \n\tCe n'est pas le cas a la ligne %d pour le mot \"%s\"\n\n",*ligne, instr );
            return -1;
        }
    }
    int ncec = numligne(ligne,f);           //incremente le compteur de ligne si il y a un retour a la ligne avant le prochain mot
    if (ncec){
        *res = 0;                           //si on l'a incremente on remet le compteur res a 0
    }else{
        (*res)++;                           //sinon on incremente res car on est plus au debut d'une ligne
    }
    return 0;
}

//compte le nombre d'etiquette dans le fichier
int compte_etiquette(char ch[]){
  FILE* f=fopen(ch,"r");
  char instr[N];
  int a=fscanf(f,"%s",instr);
  int p=strlen(instr)-1, cpt=0;     //cpt compte le nombre d'etiquette
  while (a!=EOF){
    p=strlen(instr)-1;
    if (instr[p]==':'){cpt++;}      //incremente le compteur d'etiquette si le dernier caractere est la ponctuation 2 points :
    a=fscanf(f,"%s",instr);
  }
  fclose(f);
  return cpt;
}


/***************************************************************************************************************

		PARTIE 2 : Execution du language machine

	--- printdeb
	--- push
	--- pushd
	--- ipush
	--- pop
	--- ipop
	--- dup
	--- jmp
	--- jpz
	--- rnd
	--- read
	--- write
	--- call
	--- ret
	--- op

***************************************************************************************************************/

int push(int x,int* SP,int* mem,int** adr,int* PC){
  if ((SP)>&(mem[4999])){
    printf("ERROR : pile pleine (ligne %d)\n", *PC);
    return -1;
  }
  if ((x>4999)||(x<0)){
    printf("ERROR : push() accede a une case inexistante : %d (ligne %d)\n",x,*PC);
    return -1;
  }
  *SP=mem[x];
  (*adr)++;
  return 0;
}

int pushd(int i,int* SP,int* mem,int** adr,int* PC){
  if (SP>&mem[4999]){
    printf("ERROR : pile pleine (ligne %d)\n",*PC);
    return -1;
  }
  *SP=i;
  (*adr)++;
  return 0;
}

int ipush(int* SP,int* mem,int* PC){
  if (SP<&mem[1]){
    printf("ERROR : pile vide (ligne %d)\n",*PC);
    return -1;
  }
  int n=*(SP-1);
  if ((n<0)||(n>4999)){
    printf("ERROR : ipush() accede a un case inexistante : %d (ligne %d)\n",n,*PC);
    return -1;
  }
  *(SP-1)=mem[n];
  return 0;
}

int pop(int x,int* SP,int* mem,int** adr,int* PC){
  if((x<0)||(x>4999)){
    printf("ERROR : pop() accede a une case inexistante : %d (ligne %d)\n",x,*PC);
    return -1;
  }
  if(SP<&mem[1]){
    printf("ERROR : pile vide (ligne %d)\n",*PC);
    return -1;
  }
  (*adr)--;
  mem[x]=**adr;
  return 0;
}

int ipop(int* SP, int* mem,int** adr,int* PC){
  if (SP<&mem[2]){
    if (SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
    else printf("ERROR : pile vide\n");
    return -1;
  }
  if ((*(SP-1)<0)||(*(SP-1)>4999)){
    printf("ERROR : ipop() accede a une case inexistante : %d\n (ligne %d)",*(SP-1),*PC);
    return -1;
  }
  mem[*(SP-1)]=*SP-2;
  (*adr)-=2;
  return 0;
}

int dup(int* SP, int* mem,int** adr,int* PC){
  if((SP>&mem[4999])||(SP<&mem[1])){
    if (SP>&mem[4999]) printf("ERROR : pile pleine (ligne %d)\n",*PC);
    else printf("ERROR : pile vide (ligne %d)\n",*PC);
    return -1;
  }
  *SP=*(SP-1);
  (*adr)++;
  return 0;
}

int jmp(int x,int* PC){
  if (x==-1){
    printf("ERROR : boucle infinie (ligne %d)\n",*PC);
    return -1;
  }
  *(PC)+=x;
  return 0;
}

int jpz(int x,int* SP,int* mem, int* PC,int** adr){
  if (SP<&mem[1]){
    printf("ERROR : Pile vide (ligne %d)\n",*PC);
    return -1;
  }
  (*adr)--;
  if(**adr==0){
    *(PC)+=x;
    return 0;
  }
  return 0;
}

int rnd(int x,int* SP, int* mem,int** adr,int* PC){
  if (SP>&mem[4999]){
    printf("ERROR : pile pleine (ligne %d)\n",*PC);
    return -1;
  }
  srand(time(NULL));
  *SP=rand()%x;
  (*adr)++;
  return 0;
}

int read(int x,int* mem,int* PC){
  if ((x<0)||(x>4999)){
    printf("ERROR : read() accede a une case inexistante : %d (ligne %d)\n",x,*PC);
    return -1;
  }
  printf("Rentrer valeur : ");
  scanf("%d",&mem[x]);
  return 0;
}

int write(int x,int* mem,int* PC){
  if ((x<0)||(x>4999)){
    printf("ERROR : write() accede a une case inexistante : %d (ligne %d)\n",x,*PC);
    return -1;
  }
  printf("%d\n",mem[x]);
  return 0;
}

int call(int i,int* PC, int ligne,int* SP,int** adr,int* mem){
  if ((*PC)+i>ligne){
    printf("ERROR : PC out of index (ligne %d)\n",*PC);
    return -1;
  }
  if ((SP)>&(mem[4999])){
      printf("ERROR : pile pleine (ligne %d)\n",*PC);
      return -1;

  }
  *SP=*PC;
  (*adr)++;
  (*PC)+=i;
  return 0;
}

int ret(int suite,int* PC,int* SP, int** adr,int* mem){
  if (suite==-1){
    (*PC)++;        //si ret sans call, on passe juste à la suite
    return 0;
  }
  if (SP<&mem[0]){
    printf("ERROR : pile vide (ligne %d)\n",*PC);
    return -1;
  }
  (*adr)--;      //on dépile PC
  *PC=(**adr);
  return 0;
}

int op(int i,int* SP, int* mem,int** adr,int* PC){
  if (i==0){         //add
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    *(*adr-1)=*(*adr-1)+**adr;
    return 0;
  }

  if (i==1){      //sub
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    *(*adr-1)=*(*adr-1)-**adr;
    return 0;
  }

  if (i==2){      //mult
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;

    *(*adr-1)=*(*adr-1)*(**adr);
    return 0;
  }

  if (i==3){      //div entiere
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    if (**adr==0){
      printf("ERROR : division par 0 (ligne %d)\n",*PC);
      return -1;
    }
    *(*adr-1)=*(*adr-1)/(**adr);
    return 0;
  }

  if (i==4){      //modulo
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    if (**adr==0){
      printf("ERROR : division par 0 (ligne %d)\n",*PC);
      return -1;
    }

    *(*adr-1)=*(*adr-1)%(**adr);
    return 0;
  }

  if (i==5){        //inverse sommet
    *(SP-1)=-*(SP-1);
    return 0;
  }

  if (i==6){      //egalite
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile\n");
      else printf("ERROR : pile vide\n");
      return -1;
    }
    (*adr)--;
    if (**adr==*(*adr-1)){
      *(*adr-1)=0;
    }
    else {
      *(*adr-1)=1;
    }
    return 0;
  }

  if (i==7){      //inegalite
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR$ : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    if (**adr!=*(*adr-1)){
       *(*adr-1)=0;
     }
    else {
      *(*adr-1)=1;
    }
    return 0;
  }

  if (i==8){      //test >
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    if (*(*adr-1)>**adr){
       *(*adr-1)=0;
     }
    else {
      *(*adr-1)=1;
    }
    return 0;
  }

  if (i==9){      //test >=
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;

    if (*(*adr-1)>=**adr){
       *(*adr-1)=0;
     }
    else{
       *(*adr-1)=1;
    }
    return 0;
  }

  if (i==10){      //test <
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;    if (*(*adr-1)<**adr){
       *(*adr-1)=0;     }
    else{      *(*adr-1)=1;
    }
    return 0;
  }

  if (i==11){     //test <=
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;

    if (*(*adr-1)<=**adr){
       *(*adr-1)=0;
     }
    else{
      *(*adr-1)=1;
    }
    return 0;
  }

  if (i==12){      //& bit a bit
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    *(*adr-1)=*(*adr-1)&(**adr);
    return 0;
  }

  if (i==13){     //| bit a bit
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    (*adr)--;
    *(*adr-1)=*(*adr-1)|(**adr);
    return 0;
  }

  if (i==14){     //XOR bit a bit
    if(SP<&mem[2]){
      if(SP==&mem[1])printf("ERROR : pas assez d'elements dans la pile (ligne %d)\n",*PC);
      else printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }

    (*adr)--;
    *(*adr-1)=*(*adr-1)^(**adr);
    return 0;
  }

  if (i==15){     //NO bit a bit
    if (SP<&mem[1]){
      printf("ERROR : pile vide (ligne %d)\n",*PC);
      return -1;
    }
    *(*adr-1)=~*(*adr-1);
    return 0;
  }

  printf("ERROR : operation inexistante (op prend un argument de 0 a 15) (ligne %d)\n",*PC);
  return -1;
}


/***************************************************************************************************************

		PARTIE 3 : Programme principal (main)

***************************************************************************************************************/


int main(int argc, char* argv[]){
    printf("\n******************************************* SIMULATION DU FICHIER  *******************************************\n\n");
    int erreur = copieFichier(argv[1]);
    if (!erreur){   return -1;printf("\n**************************************************************************************************************\n");    }
    FILE* f=fopen("copie.txt","r");
    FILE* fp=fopen("hexa.txt","w");

    char instr[N];                          //contiendra chaque mot du fichier
    int cpt=compte_etiquette(argv[1]);      //designe le nombre d'etiquette dans le fichier
    char* eti_nom[cpt];                     //tableau qui contiendra les etiquettes
    int eti_ligne[cpt];                     //tableau qui contiendra les lignes de chaques etiquettes
    int e=fscanf(f,"%s",instr);
    int ligne=1;                            //compteur de lignes
    int cardinal=0;                         //compte le nombre d'element dans eti_nom
    int res=0;                              // si on est au debut d'une ligne res est égale a 0 et a 1 sinon
    int r=0;                                //Si une erreur est faite dans le programme est égale a -1

    while (e!=EOF){                         //boucle permettant de remplir eti_nom et eti_ligne
        erreur = etiquette(f, instr, eti_nom, eti_ligne, &ligne,&cardinal, &res);
        if (erreur==-1){    r=-1; break;    }
        e=fscanf(f,"%s",instr);
    }

    rewind(f);
    e=fscanf(f,"%s",instr);
    ligne=1;
    while ( (e!=EOF)&&(r!=-1) ){            //boucle permettant de traduire chaque mot du fichier
        erreur = trad(fp, f, instr, eti_nom, eti_ligne, &ligne, cardinal);
        if (erreur==-1){r= -1;break;}
        e=fscanf(f,"%s",instr);
    }

    fclose(f);
    fclose(fp);
    if (r==-1){
        remove("hexa.txt");                        //supprime le fichier hexa si on a des erreurs lors de la traduction
        printf("\n**************************************************************************************************************\n");
        return -1;
    }else{
        printf("\t\t\t\t Aucun probleme a signaler lors de la traduction \n\n");
    }

    for (int k=0;k<cardinal;k++){           //libere l'espace alloué pour les elements de eti_nom
        free(eti_nom[k]);
    }
    remove("copie.txt");

    f=fopen("hexa.txt","r");
    if (f==NULL){
        printf("ERROR : le fichier hexa n'existe pas.\n\n**************************************************************************************************************\n");
        return -1;
    }
    int mem[5000]={0};  //espace memoire
    int* SP;            //pointeur de pile
    SP=&mem[0];
    int** adr;   //pointeur sur SP pour modifier SP dans les fonctions
    adr=&SP;

    int* PC=malloc(sizeof(int));
    if (PC==NULL){
        printf("ERROR : problème d'allocation mémoire\n\n**************************************************************************************************************\n");
        return -1;
    }

    (*PC)=0;       //prochaine ligne

    ligne--;
    int code=-1;     //recup l'instruction (les 2 premiers chiffres en hexa)
    int arg;        //recup l'argument (hexa)
    int a;          //recup fscanf
    int b;          //recup fclose
    int suite=-1;    //pour eviter erreur ret()

    while(code!=0x0e){                  //tant que pas de halt, on lit *PC
        if ((*PC<0)||(*PC>ligne)){            //verifie que PC ok (possible a cause d'un jmp)
            printf("ERROR : PC out of index (PC = %d, should be between 0 and %d)",*PC,ligne);
            free(PC);
            b=fclose(f);
            if (b!=0){
                printf("ERROR : le fichier ne se ferme pas correctement\n\n**************************************************************************************************************\n");
                return -1;
            }
            printf("\n**************************************************************************************************************\n");
            return -1;
        }
        rewind(f);
        if (*PC!=0){
            for (int i=0; i < *PC; i++){
                fscanf(f, "%*x %*x");       //parcourt les i lignes avant PC sans les lire
            }
        }
        a=fscanf(f,"%x %x",&code,&arg);     //lit PC
        if (a==EOF){
            printf("Fin du programme (pas de halt)\n");
            free(PC);
            b=fclose(f);
            if (b!=0){
                printf("ERROR : le fichier ne se ferme pas correctement\n\n**************************************************************************************************************\n");
                return -1;
            }
            printf("\n**************************************************************************************************************\n");
            return 0;
        }
        int r=-2;                //initialisation en cas de problème d'appel de fonction
        if (code==0x00){          //push
            r=push(arg,SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x01){     //push#
            r=pushd(arg,SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x02){     //ipush
            r=ipush(SP,mem,PC);
            (*PC)++;
        }
        else if (code==0x03){       //pop
            r=pop(arg,SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x04){       //ipop
            r=ipop(SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x05){       //dup
            r=dup(SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x06){       //op
            r=op(arg,SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x07){       //jmp
            r=jmp(arg,PC);
            (*PC)++;
        }
        else if (code==0x08){       //jpz
            r=jpz(arg,SP,mem,PC,adr);
            (*PC)++;
        }
        else if (code==0x09){       //rnd
            r=rnd(arg,SP,mem,adr,PC);
            (*PC)++;
        }
        else if (code==0x0a){       //read
            r=read(arg,mem,PC);
            (*PC)++;
        }
        else if (code==0x0b){       //write
            r=write(arg,mem,PC);
            (*PC)++;
        }
        else if (code==0x0c){        //call
            int suite=1,temp;         //verifie que call appele avant un ret()
            temp=suite;               //pour enlever warning unused value
            temp++;
            r=call(arg,PC,ligne,SP,adr,mem);
            (*PC)++;
        }
        else if (code==0x0d){         //ret
            r=ret(suite,PC,SP,adr,mem);
            (*PC)++;
            suite=-1;                 //on remet suite à -1 en cas d'appel de ret sans call
        }
        else if (code==0x0e){         //halt
            break;
        }
        if(r==-1){            //problème dans une fonction
            free(PC);
            b=fclose(f);
            if (b!=0){
                printf("ERROR : le fichier ne se ferme pas correctement\n\n**************************************************************************************************************\n");
                return -1;
            }
            printf("\n**************************************************************************************************************\n");
            return -1;
        }
        else if(r==-2){       //si r=-2, problème de trad assembleur
            printf("ERROR : instruction invalide (ligne %d)\n",*PC);
            free(PC);
            b=fclose(f);
            if (b!=0){
                printf("ERROR : le fichier ne se ferme pas correctement\n\n**************************************************************************************************************\n");
                return -1;
            }
            printf("\n**************************************************************************************************************\n");
            return -1;
        }
    }
    free(PC);
    b=fclose(f);
    if (b!=0){
        printf("ERROR : le fichier ne se ferme pas correctement\n\n**************************************************************************************************************\n");
        return -1;
    }
    printf("Fin du programme\n\n**************************************************************************************************************\n");
    return 0;
}
