#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int bool;
#define true 1;
#define false 0;
bool debug = 0;
bool encToggle = 0;

/*prototypes*/

void noviNalog();

typedef struct
{
    int datum;
    float kamata;
    int ID;
    int tip;
    int novac;
}racun;

struct korisnik
{
    racun rac[20];
    int brojacR;
    int ID;
    char imeKorisnika[20];
    char prezimeKorisnika[20];
    char JMBG[20];
    char datumRodjenja[30];
    char adresaStanovanja[50];
    char brojTelefona[13];
};

void noviNalog()
{
    if (debug)
    {
        printf("pozvano noviNalog()\n");
    }
    struct korisnik temp;

    //data input
    printf("unesite ime i prezime korisnika:\n");
    fflush(stdin);
    scanf("%s %s", temp.imeKorisnika, temp.prezimeKorisnika);

    printf("unesite JMBG:\n");
    fflush(stdin);
    scanf("%s", temp.JMBG);

    printf("unesite datum rodjenja (dd/mm/gggg):\n");
    fflush(stdin);
    scanf(" %[^\n]s", temp.datumRodjenja);

    printf("unesite adresu stanovanja:\n");
    fflush(stdin);
    scanf(" %[^\n]s", temp.adresaStanovanja);
    printf("unesite broj telefona: \n");
    int i = 0;
    fflush(stdin);
    scanf(" %s", temp.brojTelefona);

    if (provera_jmbg_datumRodjenja(temp.JMBG, temp.datumRodjenja))
    {
        generateID(&temp.ID);
        temp.brojacR = 0; //prvi put!
        writeToFile(temp);
    }
    else
    {
        printf("___GRESKA: JMBG ne odgovara datumu rodjenja\nNije upisano nista u fajl!\n");
    }
}
int getNumAcc()
{
    FILE *fp;
    int temp;
    fp = fopen("IDcounter.txt", "r");
    if (fp == NULL)
    {
        printf("___FATALNA_GREKSA: nije uspesno otvoren fajl IDcounter.txt\n\t   u funkciji getNumAcc()");
        fclose(fp);
        return;
    }
    fscanf(fp, "%d", &temp);
    fclose(fp);
    return temp;
}
void generateID(int *ptr)
{
    FILE *fp;
    fp = fopen("IDcounter.txt", "r");
    if (fp == NULL)
    {
        printf("___FATALNA_GREKSA: nije uspesno otvoren fajl IDcounter.txt\n\t   u funkciji generateID()");
        fclose(fp);
        return;
    }
    int temp;
    fscanf(fp, "%d", &temp);
    fclose(fp);
        printf("uspesno generisan ID za korisnika. ID = %d\n", temp);
    *ptr = temp;
    fp = fopen("IDcounter.txt", "w");
    if (fp == NULL)
    {
        printf("___FATALNA_GREKSA: nije uspesno otvoren fajl IDcounter.txt\n\t   u funkciji generateID()");
        fclose(fp);
        return;
    }
    fprintf(fp, "%d", *ptr + 1);
    fprintf(fp, "\n\nmenjanje countera restartovacete broj akauntova (Global reset baze podataka)\ngubitak svih podataka.\n");
    fclose(fp);
}
int provera_jmbg_datumRodjenja(char jmbg[], char datumR[])
{
    if (strlen(jmbg) != 13)
    {
        return 0;
    }
    if (jmbg[0] == datumR[0] && jmbg[1] == datumR[1])
        if (jmbg[2] == datumR[3] && jmbg[3] == datumR[4])
            if (jmbg[4] == datumR[7] && jmbg[5] == datumR[8] && jmbg[6] == datumR[9])
                return 1;
    return 0;
}
void writeToFile(struct korisnik temp)
{
    FILE *f;
    char tempName[14];
    itoa(temp.ID, tempName, 10);
    strcat(tempName, ".txt");
    char tempNameFinal[20];
    strcpy(tempNameFinal, "baza/");
    strcat(tempNameFinal, tempName);
    f = fopen(tempNameFinal, "w"); //treba promeniti sa w
    if (f == NULL)
    {
        printf("___FATALNA_GRESKA: fajl nije uspesno otvoren\n\t  unutar writeToFile(). informacije izgubljene\n\t\tpokusao otvoriti %s\n", tempNameFinal);
        fclose(f);
        return;
    }
    enc(&temp.imeKorisnika);
    enc(&temp.prezimeKorisnika);
    enc(&temp.JMBG);
    enc(&temp.datumRodjenja);
    enc(&temp.adresaStanovanja);
    enc(&temp.brojTelefona);
    fprintf(f, "%s %s\n", temp.imeKorisnika, temp.prezimeKorisnika);
    fprintf(f, "%s\n", temp.JMBG);
    fprintf(f, "%s\n", temp.datumRodjenja);
    fprintf(f, "%s\n", temp.adresaStanovanja);
    fprintf(f, "%s\n", temp.brojTelefona);
    fprintf(f, "%d\n", temp.brojacR);

    if(temp.brojacR>0)
    {
        int i=0;
        for(i=0;i<temp.brojacR;i++)
        {
                fprintf(f,"%d\n",temp.rac[i].ID);printf("u fajl upisano %d\n",temp.rac[i].ID);
                fprintf(f,"%d\n",temp.rac[i].tip);printf("u fajl upisano %d\n",temp.rac[i].tip);
                fprintf(f,"%d\n",temp.rac[i].novac);printf("u fajl upisano %d\n",temp.rac[i].novac);
                fprintf(f,"%d\n",temp.rac[i].datum);printf("u fajl upisano %d\n",temp.rac[i].datum);
                fprintf(f,"%f\n",temp.rac[i].kamata);printf("u fajl upisano %f\n",temp.rac[i].kamata);
        }
    }
    fclose(f);

    char del;
    fflush(stdin);
    scanf("%c",&del);
}
void dec(char *s)
{
    if(!encToggle){return;}
    int i = 0;
    for (i = 0; i < strlen(s); i++)
    {
        if (*(s + i) > 96 && *(s + i) < 123)
        {
            *(s + i) -= 3;
            if (*(s + i) < 'a')
            {
                *(s + i) = *(s + i) + 'z' - 'a' + 1;
            }
        }
        else if (*(s + i) >= 'A' && *(s + i) <= 'Z')
        {
            *(s + i) -= 3;
            if (*(s + i) < 'A')
            {
                *(s + i) = *(s + i) + 'Z' - 'A' + 1;
            }
        }

        else if (*(s) >= '0' && *(s) <= '9')
        {
            if (*(s) == '0')
            {
                *(s) = '7';
            }
            else if (*(s) == '1')
            {
                *(s) = '8';
            }
            else if (*(s) == '2')
            {
                *(s) = '9';
            }
            else if (*(s) == '3')
            {
                *(s) = '0';
            }
            else if (*(s) == '4')
            {
                *(s) = '1';
            }
            else if (*(s) == '5')
            {
                *(s) = '2';
            }
            else if (*(s) == '6')
            {
                *(s) = '3';
            }
            else if (*(s) == '7')
            {
                *(s) = '4';
            }
            else if (*(s) == '8')
            {
                *(s) = '5';
            }
            else if (*(s) == '9')
            {
                *(s) = '6';
            }
        }
    }
}
void enc(char *s)
{
    if(!encToggle){return;}
    int i = 0;
    int offsetVelika = 65;
    int offsetMala = 97;

    for (i = 0; i < strlen(s); i++)
    {
        // printf("%c -> ",*(s+i));
        if (*(s + i) > 64 && *(s + i) < 91)
        {
            *(s + i) = (((((int)(*(s + i))) - offsetVelika) + 3) % 26) + offsetVelika;
        }

        else if (*(s + i) > 96 && *(s + i) < 123)
        {
            *(s + i) = (((((int)(*(s + i))) - offsetMala) + 3) % 26) + offsetMala;
        }

        else if (*(s) >= '0' && *(s) <= '9')
        {
            if(debug)printf("%c ->", *(s));
            if (*(s) == '0')
            {
                *(s) = '3';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '1')
            {
                *(s) = '4';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '2')
            {
                *(s) = '5';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '3')
            {
                *(s) = '6';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '4')
            {
                *(s) = '7';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '5')
            {
                *(s) = '8';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '6')
            {
                *(s) = '9';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '7')
            {
                *(s) = '0';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '8')
            {
                *(s) = '1';
                if(debug)printf("%c\n", *(s));
            }
            else if (*(s) == '9')
            {
                *(s) = '2';
                if(debug)printf("%c\n", *(s));
            }
        }
    }
}
void driver(int select)
{
    char fileNameFinal[20];
    strcpy(fileNameFinal, "./baza/");
    char tmp[20];
    itoa(select,tmp,10);
    strcat(fileNameFinal, tmp);
    strcat(fileNameFinal, ".txt");
    FILE* fp;
    fp = fopen(fileNameFinal,"r");
    struct korisnik temp;
    fscanf(fp, "%s %s\n%s\n%s\n%[^\n]s\n", temp.imeKorisnika, temp.prezimeKorisnika, temp.JMBG, temp.datumRodjenja, temp.adresaStanovanja);
    fscanf(fp, "%s\n", temp.brojTelefona);
    fscanf(fp, "%d\n", &temp.brojacR);
    temp.ID = select;
    easyPrint(temp);
    fclose(fp);

}
void prikazSvih()
{
    if (debug)
    {
        printf("pozvano prikazSvih()\n");
    }
    int i;
    int brojAkaunta = getNumAcc();
    for (i = 0; i < brojAkaunta; i++)
    {
        driver(i);
    }
}
void pretragaID(int *ptr)
{
    FILE *fp;
    char ID[5];
    printf("unesite ID korisnika za pretragu: ");
    fflush(stdin);
    scanf("%s", &ID);
    while (atoi(ID) >= getNumAcc() || atoi(ID) < 0)
    {
        printf("pogresan unos, unesite ID ponovo: \n");
        scanf("%s");
    }

    char fileNameFinal[20];
    strcpy(fileNameFinal, "./baza/");
    strcat(fileNameFinal, ID);
    strcat(fileNameFinal, ".txt");
    fp = fopen(fileNameFinal, "r");
    if (fp == NULL)
    {
        printf("ne postoji korisnik sa tim IDom ili ___GRESKA:neuspesno otvoren fajl - U pretragaID();\n");
        fclose(fp);
        *ptr = -1;
        return;
    }
    *(ptr) = atoi(ID);
    printf("Korisnik Pronadjen!");
    fclose(fp);
}
void pretragaNaloga()
{
    int select;
    if (debug)
    {
        printf("pozvano pretragaNaloga()\n");
    }
    pretragaID(&select);
    printf("############################\n");
    printf("trenutno selektovan ID = %d\n", select);
    drugi_meni(select);
}
void dodajRacun(int select)
{
    FILE *fp;
    char fileNameFinal[20];
    strcpy(fileNameFinal, "./baza/");
    char idk[20];
    struct korisnik temp;
    itoa(select, idk, 10);
    temp.ID = select;
    printf("idk = %s\n",idk);
    strcat(fileNameFinal, idk);
    strcat(fileNameFinal, ".txt");
    fp = fopen(fileNameFinal, "r");

    fscanf(fp, "%s %s\n%s\n%s\n%[^\n]s\n", temp.imeKorisnika, temp.prezimeKorisnika, temp.JMBG, temp.datumRodjenja, temp.adresaStanovanja);
    fscanf(fp, "%s\n", temp.brojTelefona);
    fscanf(fp, "%d\n", &temp.brojacR);
    int i=0;
    if(temp.brojacR>0)
    {

            for(i=0;i<temp.brojacR;i++)
            {
                fscanf(fp,"%d",&temp.rac[i].ID);
                fscanf(fp,"%d\n",&temp.rac[i].tip);
                fscanf(fp,"%d\n",&temp.rac[i].novac);
                fscanf(fp,"%d\n",&temp.rac[i].datum);
                fscanf(fp,"%f\n",&temp.rac[i].kamata);
            }
    }

    fclose(fp);

    temp.brojacR++;

    printf("unesite tip racuna koji zelite: dinarski, devizni, racun za orocenu stednju\n");
    fflush(stdin);
    scanf("%d",&temp.rac[temp.brojacR-1].tip);

    printf("unesite iznos inicijalne uplate: ");
    fflush(stdin);
    scanf("%d",&temp.rac[temp.brojacR-1].novac);

    temp.rac[temp.brojacR-1].ID = temp.brojacR-1;

    if(temp.rac[temp.brojacR-1].tip != 3){temp.rac[temp.brojacR-1].datum =0;temp.rac[temp.brojacR-1].kamata=0;}
    else
    {
        printf("unesite broj godina na koji se orocava: ");
        fflush(stdin);
        printf("kamata test\n");
        scanf("%d",&temp.rac[temp.brojacR-1].datum);

        temp.rac[temp.brojacR-1].kamata = temp.rac[temp.brojacR-1].datum*2.2;
    }
    printf("id u funkciji wtf() %d\n",temp.ID);
    writeToFile(temp);

}
void ispis_drugi_meni()
{
    printf("dodaj racun.............2\n");
    printf("ispis informacija.......3\n");
    printf("izmena naloga...........4\n");
    printf("brisanje naloga.........5\n");
    printf("transakcija.............6\n");
}
void drugi_meni(int select)
{
    if(select == -1){printf("select = -1 in drugi_meni(), wrong input");return;}
    ispis_drugi_meni();
    char opcode;
    fflush(stdin);
    scanf("%c",&opcode);
    switch(opcode){
    case '2':
        dodajRacun(select);
        break;
    case '3':
        ucitajNalog(select);
        break;
    case '4':
        //izmenaNaloga(int select); // treba videti koji parametri se menjaju, treba ucitati sve i onda dati meni sta ce da se menja.
        break;
    case '5':
        brisanjeNaloga(select); // modifikuj funkciju
        break;
    case '6':
        transakcija(select); // ovo preispitati kako treba da radi.
        break;
    }
}
void ucitajNalog(int select)
{
    FILE *fp;
    char fileNameFinal[20];
    strcpy(fileNameFinal, "./baza/");
    char idk[9];
    itoa(select, idk, 10);
    strcat(fileNameFinal, idk);
    strcat(fileNameFinal, ".txt");
    fp = fopen(fileNameFinal, "r");
    if (debug)
        printf("otvorio %s\n", fileNameFinal);
    //fuk
    struct korisnik temp;
    int i = 0;
    fscanf(fp, "%s %s\n%s\n%s\n%[^\n]s\n", temp.imeKorisnika, temp.prezimeKorisnika, temp.JMBG, temp.datumRodjenja, temp.adresaStanovanja);
    fscanf(fp, "%s\n", temp.brojTelefona);
    fscanf(fp, "%d\n", &temp.brojacR);
    temp.ID = select;
    dec(&temp.imeKorisnika);
    dec(&temp.prezimeKorisnika);
    dec(&temp.JMBG);
    dec(&temp.datumRodjenja);
    dec(&temp.adresaStanovanja);
    dec(&temp.brojTelefona);

    if(temp.brojacR>0)
    {
        int i=0;
        for(i=0;i<temp.brojacR;i++)
        {
            fscanf(fp,"%d",&temp.rac[i].ID);
            fscanf(fp,"%d\n",&temp.rac[i].tip);
            fscanf(fp,"%d\n",&temp.rac[i].novac);
            fscanf(fp,"%d\n",&temp.rac[i].datum);
            fscanf(fp,"%f\n",&temp.rac[i].kamata);

        }
    }

    easyPrint(temp);
    if(temp.brojacR>0){
        printf("zelite li i ispis svih racuna korisnika\n");
        int code;
        scanf("%d",&code);
        if(code==1){easyPrintRacun(temp);}
    }


    fclose(fp);
}
void easyPrintRacun(struct korisnik temp)
{
    int i =0;
    for(i=0;i<temp.brojacR;i++)
    {
        printf("====================\n");
        printf("# racun id: %d/%d\n",temp.ID,temp.rac[i].ID);
        printf("# tip racuna: %d\n",temp.rac[i].ID);
        printf("# novac na racunu: %d\n",temp.rac[i].novac);
        printf("# datum orocavanja: %d godina\n",temp.rac[i].datum);
        printf("# kamata za OR %f\n",temp.rac[i].kamata);
        printf("\n\n");
    }
}
void easyPrint(struct korisnik temp)
{
    printf("##################################\n");
    printf("Id korisnika: %d\n",temp.ID);
    printf("ispis informacija korisnika: \n");
    printf("ime:      %s\n", temp.imeKorisnika);
    printf("prezime:  %s\n", temp.prezimeKorisnika);
    printf("JMBG:     %s\n", temp.JMBG);
    printf("dat.Rodj: %s\n", temp.datumRodjenja);
    printf("adresa:   %s\n", temp.adresaStanovanja);
    printf("broj tel: %s\n", temp.brojTelefona);
    printf("broj Rac: %d\n", temp.brojacR);
    if(temp.brojacR=0){return;}
    int i=0;
    for(i=0;i<temp.brojacR;i++)
    {
        printf("##==RACUN-ID-[%d]==##\n",temp.rac[i].ID);
        printf("tip: ");
        if(temp.rac[i].tip==1){printf("dinarski\n");}
        else if(temp.rac[i].tip==2){printf("devizni\n");}
        else{printf("oroceni racun\n");}
        printf("datum orocavanja: %d godina:",temp.rac[i].datum);
        printf("kamata: %f\n",temp.rac[i].kamata);
        printf("novac: %d\n",temp.rac[i].novac);
    }
}
void izmenaNaloga(int select)
{
    if (debug)
    {
        printf("pozvano izmenaNaloga()\n");
    }
}
void brisanjeNaloga(int select) // ovo modifikovati da radi.
{
    if (debug)
        printf("pozvano brisanjeNaloga()\n");
    int status;
    char ime[40];
    itoa(select,ime,10);
    strcat(ime, ".txt");
    char prefix[30];
    strcpy(prefix, "./baza/");
    strcat(prefix, ime);
    strcpy(ime, prefix);
    status = remove(ime);
    if (status == 0)
        printf("%s fajl je obrisan uspesno.\n", ime);
    else
    {
        printf("Nije moguce obrisati dati fajl: %s\n", ime);
        perror("error: ");
    }
    return;
}
void transakcija(int select)
{
    if (debug)
    {
        printf("pozvano transakcija()\n");
    }
}
void ispismenija()
{
    system("cls");
    printf("dodaj novog korisnika...1\n");
    printf("prikazi sve naloge......2\n");
    printf("pretraga naloga.........3\n");
    char unetaOpcija;
    fflush(stdin);
    scanf("%c", &unetaOpcija);
    switch (unetaOpcija)
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
    default:
        printf("pogresan unos\n");
        break;
    }
}

bool adminlogin()
{
    char ime[20];
    printf("unesi admin ime: ");
    fflush(stdin);
    scanf("%s",ime);
    char lozinka[20];
    fflush(stdin);
    printf("unesi lozinku: ");
    scanf("%s",lozinka);
    FILE* fp;
    char wanted_ime[20];
    char wanted_sifra[20];
    fp = fopen("./login/login_info.txt","r");
    fscanf(fp,"%s",wanted_ime);
    fscanf(fp,"%s",wanted_sifra);
    fclose(fp);
    if(strc(ime,wanted_ime) && strc(lozinka,wanted_sifra)){return 1;}
    return 0;
}
bool strc(char* first, char* second)
{
    if(strlen(first)!=strlen(second)){return 0;}
    int i=0;
    for(i=0;i<strlen(first);i++)
    {
        if(*(first+i) != *(second+i)){return 0;}
    }
    return 1;
}
int main()
{
    if(adminlogin())
    while (1)
    {
        ispismenija();
        printf("zelite li da nastavite dalje? <Y/N> ");
        char temp;
        fflush(stdin);
        scanf("%c", &temp);
        if (temp == 'n' || temp == 'N')
            return 0;
    }
    else{printf("nije tacna sifra ili ime.\n");}
}
