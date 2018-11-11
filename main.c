#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int bool;
#define true 1;
#define false 0;
bool debug = 1;
bool encToggle = 1;
struct racun
{
    int tip;
    int brojRacuna;
    int novac;
};

struct korisnik
{
    struct racun rac[20];
    int brojacR;
    int ID;
    char imeKorisnika[20];
    char prezimeKorisnika[20];
    char JMBG[20];
    char datumRodjenja[30];
    char adresaStanovanja[50];
    char brojTelefona[13];
};

/*! \fn int read(int fd,char *buf,size_t count)
    \brief Read bytes from a file descriptor.
    \param fd The descriptor to read from.
    \param buf The buffer to read into.
    \param count The number of bytes to read.
*/

void noviNalog()
{
    if(debug){printf("pozvano noviNalog()\n");}
    struct korisnik temp;


    //data input
    printf("unesite ime i prezime korisnika:\n");
    fflush(stdin);
    scanf("%s %s",temp.imeKorisnika,temp.prezimeKorisnika);

    printf("unesite JMBG:\n");
    fflush(stdin);
    scanf("%s",temp.JMBG);

    printf("unesite datum rodjenja (dd/mm/gggg):\n");
    fflush(stdin);
    scanf(" %[^\n]s",temp.datumRodjenja);

    printf("unesite adresu stanovanja:\n");
    fflush(stdin);
    scanf(" %[^\n]s",temp.adresaStanovanja);
    printf("unesite broj telefona: \n");
    int i=0;
    fflush(stdin);
    scanf(" %s",temp.brojTelefona);

    if(provera_jmbg_datumRodjenja(temp.JMBG,temp.datumRodjenja)){
        generateID(&temp.ID);
        temp.brojacR = 0; //prvi put!
        writeToFile(temp);
    }
    else{printf("___GRESKA: JMBG ne odgovara datumu rodjenja\nNije upisano nista u fajl!\n");}
}
int getNumAcc()
{
    FILE* fp;
    int temp;
    fp = fopen("IDcounter.txt","r");
    if(fp == NULL){printf("___FATALNA_GREKSA: nije uspesno otvoren fajl IDcounter.txt\n\t   u funkciji getNumAcc()");fclose(fp);return;}
    fscanf(fp,"%d",&temp);fclose(fp);
    return temp;
}
void generateID(int* ptr)
{
    FILE* fp;
    fp=fopen("IDcounter.txt","r");
    if(fp == NULL){printf("___FATALNA_GREKSA: nije uspesno otvoren fajl IDcounter.txt\n\t   u funkciji generateID()");fclose(fp);return;}
    int temp;
    fscanf(fp,"%d",&temp);fclose(fp);
    if(debug){printf("uspesno generisan ID za korisnika. ID = %d\n",temp);}
    *ptr=temp;
    fp=fopen("IDcounter.txt","w");
    if(fp == NULL){printf("___FATALNA_GREKSA: nije uspesno otvoren fajl IDcounter.txt\n\t   u funkciji generateID()");fclose(fp);return;}
    fprintf(fp,"%d",*ptr+1);
    fprintf(fp,"\n\nmenjanje countera restartovacete broj akauntova (Global reset baze podataka)\ngubitak svih podataka.\n");
    fclose(fp);
}
int provera_jmbg_datumRodjenja(char jmbg[],char datumR[])
{
    if(strlen(jmbg)!=13){return 0;}
    if(jmbg[0]==datumR[0] && jmbg[1]==datumR[1])
        if(jmbg[2]==datumR[3] && jmbg[3]==datumR[4])
            if(jmbg[4]==datumR[7] && jmbg[5]==datumR[8] && jmbg[6]==datumR[9])
                return 1;
    return 0;
}
void writeToFile(struct korisnik temp)
{
    FILE *f;
    char tempName[14];
    itoa(temp.ID,tempName,10);
    strcat(tempName,".txt");
    char tempNameFinal[20];
    strcpy(tempNameFinal,"baza/");
    strcat(tempNameFinal,tempName);
    f = fopen(tempNameFinal,"w+"); //treba promeniti sa w
    if(f==NULL){printf("___FATALNA_GRESKA: fajl nije uspesno otvoren\n\t  unutar writeToFile(). informacije izgubljene\n\t\tpokusao otvoriti %s\n",tempNameFinal);fclose(f);return;}
    enc(&temp.imeKorisnika);enc(&temp.prezimeKorisnika);enc(&temp.JMBG);enc(&temp.datumRodjenja);enc(&temp.adresaStanovanja);enc(&temp.brojTelefona);
    fprintf(f,"%s %s\n",temp.imeKorisnika,temp.prezimeKorisnika);
    fprintf(f,"%s\n",temp.JMBG);
    fprintf(f,"%s\n",temp.datumRodjenja);
    fprintf(f,"%s\n",temp.adresaStanovanja);
    fprintf(f,"%s\n",temp.brojTelefona);
    fprintf(f,"%d\n",temp.brojacR);
    fclose(f);

}
void dec(char *s)
{
    int i=0;
    for(i= 0; i <strlen(s);i++){
         if(*(s+i)>96 && *(s+i)<123)
        {
            *(s+i) -= 3;
            if(*(s+i)<'a')
            {
                *(s+i) = *(s+i) + 'z' - 'a' +1;
            }
        }
        else if(*(s+i)>='A' && *(s+i)<='Z')
        {
            *(s+i) -= 3;
            if(*(s+i)<'A')
            {
                *(s+i) = *(s+i) + 'Z' - 'A' +1;
            }
        }
    }
}
void enc(char *s)
{
    int i;
    int offsetVelika=65;
    int offsetMala=97;

    for(i=0;i<strlen(s);i++)
    {
       // printf("%c -> ",*(s+i));
        if(*(s+i)>64 && *(s+i)<91){
            *(s+i) = (((((int)(*(s+i)))-offsetVelika)+3)%26)+offsetVelika;
        }
        else if(*(s+i)>96 && *(s+i)<123)
        {
            *(s+i) = (((((int)(*(s+i)))-offsetMala)+3)%26)+offsetMala;
        }
        //printf("%c\n",*(s+i));
    }
}

void prikazSvih()
{
    if(debug){printf("pozvano prikazSvih()\n");}
    int i;
    int brojAkaunta = getNumAcc();
    for(i=0;i<brojAkaunta;i++)
    {






    }

}
void pretragaID(int *ptr)
{
    FILE* fp;
    char ID[5];
    printf("unesite ID korisnika za pretragu: ");
    fflush(stdin);
    scanf("%s",&ID);
    while(atoi(ID)>=getNumAcc() || atoi(ID)<0){
        printf("pogresan unos, unesite ID ponovo: \n");
        scanf("%s");
    }

    char fileNameFinal[20];
    strcpy(fileNameFinal,"./baza/");
    strcat(fileNameFinal,ID);
    strcat(fileNameFinal,".txt");
    fp = fopen(fileNameFinal,"r");
    if(fp == NULL){printf("ne postoji korisnik sa tim IDom ili ___GRESKA:neuspesno otvoren fajl - U pretragaID();\n");fclose(fp);*ptr=-1;return;}
    *(ptr) = atoi(ID);
    printf("Korisnik Pronadjen!");
    fclose(fp);
}
void pretragaNaloga()
{
    int select;
    if(debug){printf("pozvano pretragaNaloga()\n");}
    fflush(stdin);
    //printf("izaberi nacin pretrage:\n 1 - ID pretraga \n 2 - pretraga po imenu \n");
    char opcode;
    //scanf("%c",&opcode);
    opcode = '1';
    switch(opcode)
    {
    case '1':
        pretragaID(&select);
        break;

    case '2':

        break;

    case '3':

        break;

    }
    printf("trenutno selektovan ID = %d\n",select);
    ucitajNalog(select);

}

void ucitajNalog(int select)
{
    FILE* fp;
    char fileNameFinal[20];
    strcpy(fileNameFinal,"./baza/");
    char idk[9];
    itoa(select,idk,10);
    strcat(fileNameFinal,idk);
    strcat(fileNameFinal,".txt");
    fp=fopen(fileNameFinal,"r");
    if(debug)printf("otvorio %s\n",fileNameFinal);
        //fuk
        struct korisnik temp;
        int i=0;
        int acc;
        fscanf(fp,"%s %s\n%s\n%s\n%[^\n]s\n",temp.imeKorisnika,temp.prezimeKorisnika,temp.JMBG,temp.datumRodjenja,temp.adresaStanovanja);
        fscanf(fp,"%s\n",temp.brojTelefona);
        fscanf(fp,"%d\n",&temp.brojacR);
        dec(&temp.imeKorisnika);dec(&temp.prezimeKorisnika);dec(&temp.JMBG);dec(&temp.datumRodjenja);dec(&temp.adresaStanovanja);dec(&temp.brojTelefona);

        easyPrint(temp);


        //ako nijeo
        fclose(fp);
}
void easyPrint(struct korisnik temp)
{
    printf("##################################\n");
    printf("ispis informacija korisnika: \n");
    printf("ime:      %s\n",temp.imeKorisnika);
    printf("prezime:  %s\n",temp.prezimeKorisnika);
    printf("JMBG:     %s\n",temp.JMBG);
    printf("dat.Rodj: %s\n",temp.datumRodjenja);
    printf("adresa:   %s\n",temp.adresaStanovanja);
    printf("broj tel: %s\n",temp.brojTelefona);
    printf("broj Rac: %d\n",temp.brojacR);
}
void izmenaNaloga()
{
    if(debug){printf("pozvano izmenaNaloga()\n");}


}

void brisanjeNaloga()
{
    if(debug){printf("pozvano brisanjeNaloga()\n");}
    int status;
  char ime[10];

  printf("unesite ID korisnika kojeg zelite da obrisete \n");
    scanf("%s",ime);
  strcat(ime,".txt");
  char prefix[30];
  strcpy(prefix,"./baza/");
  strcat(prefix,ime);
  strcpy(ime,prefix);
   status = remove(ime);

  if (status == 0)
    printf("%s fajl je obrisan uspesno.\n", ime);
  else
  {
    printf("Nije moguce obrisati dati fajl: %s\n",ime);
    perror("error: ");
  }

  return 0;
}
void dodajRacun()
{


}
void transakcija()
{
    if(debug){printf("pozvano transakcija()\n");}

}


void ispismenija(){
    system("cls");
    printf("dodaj novog korisnika...1\n");
    printf("prikazi sve naloge......2\n");
    printf("pretraga naloga.........3\n");
    printf("izmena naloga...........4\n");
    printf("brisanje naloga.........5\n");
    printf("transakcija.............6\n");
    char unetaOpcija;
    fflush(stdin);
    scanf("%c",&unetaOpcija);
    switch(unetaOpcija)
    {
    case '1':
        noviNalog();
        break;
    case '2':
        prikazSvih();
        break;
    case '3':
        pretragaNaloga();
        break;
    case '4':
        izmenaNaloga();//treba gore
        break;
    case '5':
        brisanjeNaloga();//treba gore
        break;
    case '6':
        transakcija();
        break;
    default:
        printf("pogresan unos\n");
        break;
    }
}

int main()
{

    while(1){
        ispismenija();
        printf("zelite li da nastavite dalje? <Y/N> ");
        char temp;
        fflush(stdin);
        scanf("%c",&temp);
        if(temp == 'n' || temp == 'N'){break;}
    }

    return 0;
}
