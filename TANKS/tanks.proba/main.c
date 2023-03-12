#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include <Windows.h>
#include <WinBase.h>
#include <conio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

int u1=90, u2=90, u;
int x_tanc1,y_tanc1;
int x_tanc2,y_tanc2;
int tura;
int x_proiectil_nou, y_proiectil_nou;
int x_proiectil_vechi, y_proiectil_vechi;
int tura = 1;//contor pentru tura
int X, Y;//variabile in care se vor retine coordonatele tancului cu care vrem sa lucram
int stop = 0;//se va trece la tura urmatoare cand acesta va fi 1
int viata1 = 20, viata2 = 20;
int Crand; //coeficient pentru generarea random a pozitiei de inceput a landscape-ului
int putere;//puterea tancului a carui tura este
int putere1 = 1, putere2 = 1;
int combustibil1=30, combustibil2=30;
int combustibil;//combustibilul tancului a carui tura este
char L;
int tip_proiectil,tip_proiectil1=1,tip_proiectil2=1;
int nr_proiectil1[3] = { 30, 3,3 }, nr_proiectil2[3] = {30, 3,3},nr_proiectil;
char proiectil1[1]="o", proiectil2[1]="o", proiectil[1]="o";
int x_avion_nou, x_avion_vechi;
char player1[20], player2[20];//numele jucatorilor

//Definim constantele programului
//Dimensiunile chenarului

#define DIM_X 115
#define DIM_Y 29

//Laturile si colturile 
#define COLT_STANGA_SUS 201
#define COLT_STANGA_JOS 200
#define COLT_DREAPTA_SUS 187
#define COLT_DREAPTA_JOS 188
#define BARA_VERTICALA 186
#define BARA_ORIZONTALA 205

#define BAZA_TANC 79
#define CORP_TANC 220
#define HASUR 219 //pentru desenarewa landscap-ului

#define for_i for(int i=0;i<=DIM_Y-1;i++)
#define for_j for(int j=0;j<=DIM_X;j++)

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80

#define PUTERE 219
#define LIPSA_PUTERE 176

#define CORP_AVION 205
#define CORP_ARIPI 206
#define ARIPA_SUS  187
#define ARIPA_JOS 188

#define PROIECTIL_AVION 254

char scena[DIM_Y+1][DIM_X+2];

void ascunde_cursor();//Functie care ascunde cursorul pentru a nu deranja vizual in timpul rularii
void meniu_intrare();//Functie care afiseaza pe ecran modul de joc
void afiseaza_scena();//afiseaza matricea initiala
void alege_landscape();//afiseaza pe ecran meniul de alegere a landscape-ului
void nume_jucatori();//Atribuie numele alese de jucatori
void afisare_proiectil();//afiseaza pe ecran tipul de proiectil ales si cate mai are jucatorul la dispozitie
void golire_ecran();
int functie_landscape(int x);//Functie care seteaza functia matematica corespunzatoare fiecarui landscape
void deseneaza_landscape();//afiseaza pe ecran terenul si il atribuie simultan in matricea din spate
void gotoXY(int x, int y);//duce cursorul la o pozitie dorita
void deseneaza_tanc1(int x);//deseneaza tancul 1 de la stanga la dreapta de la coordonata x astfel incat sa fie asezat pe landscape
void deseneaza_tanc2(int x);//deseneaza tancul 2 de la dreapta la stanga de la coordonata x astfel incat sa fie asezat pe landscape
void verifica_citire();//citeste tastele accesate de jucator si executa comenzile corespunzatoare
void detoneaza_proiectil(int u);//calculeaza si afiseaza pozitia proiectelor de tip 1 si 2 dupa detonare
void pregatire_player1();//atribuie variabilelor ce urmeaza a fi folosite in funtii comune prntru ambii playeri valorile jucatorului 1
void pregatire_player2();//atribuie variabilelor ce urmeaza a fi folosite in funtii comune prntru ambii playeri valorile jucatorului 2
int joc_pierdut();//returneaza 1 cand unul din jucatori pierde- ramane fara viata
void crater();//afiseaza si atribuie in matrice spatiile ce vor crea iluzia unui crater
void putere0();//aduce puterea inapoi la 0 dupa detonare
void afisare_combustibil();
void verificare_viata();//verifica daca tancul a fost lovit si in caz afirmativ scade puntele de viata corespunzatoare
void afisare_viata();
void deplasare_dreapta();//muta tancul a carui tura este spre deapta cu un caracter
void deplasare_stanga();//muta tancul a carui tura este spre stanga cu un caracter
void stergere_tanc1();
void stergere_tanc2();
void afisare_tip_proiectil();//afiseaza tipul proiectilului selectat dar nu neaparat si utilizat
void sfarsit_joc();
void avion();//deplasarea avionului care ajuta tancul
void proiectil_avion();//functia de deplasare a proiectilului ce cade din avion

void main()
{ 
	
	ascunde_cursor();
	srand(time(NULL));
	meniu_intrare();
    replay :
	golire_ecran();
	alege_landscape();
	golire_ecran();
	nume_jucatori();
	golire_ecran();
	x_tanc1 =4+ rand()%52 ;//se alege random pozitia initiala a tancului 1 in intervalul [4,55]
	x_tanc2 = x_tanc1 + 35+rand()%21;//se alege random pozitia tancului 2 la o distanta minima de 35 fata de tancul 1
	afiseaza_scena();	
	deseneaza_landscape();
	deseneaza_tanc1(x_tanc1);
	deseneaza_tanc2(x_tanc2);
	
	while (joc_pierdut()!=1)
	{
		stop = 0;//se reseteaza parametrul ce permite schimbarea parametrilor de catre jucatorul a carui tura este
		if (tura % 2 == 1)
			pregatire_player1();
		else pregatire_player2();

		while(stop!=1)
		{
			verifica_citire();
		}
		//afisare a parametrilor ce se pot schimba odata cu detonarea proiectilului
		afisare_combustibil();
		verificare_viata();
		afisare_viata();
		//la finalul turei parametrii generali pt unghi,putere sau combustibil vor fi atribuiti celor specifici tancului care a tras
		if (tura % 2 == 1)
		{
			u1 = u;
			putere1 = putere;
			combustibil1 = combustibil;
		}
		else
		{
			u2 = u;
			putere2 = putere;
			combustibil2 = combustibil;
		}
		tura++;
	}
	golire_ecran();
	sfarsit_joc();
	//se ofera posibilitatea reluarii jocului prin apasarea tastei space sau incheierea prin apasarea altei taste
	char replay='.';
		replay=_getch();
	if (replay ==' ')
			goto replay;
	gotoXY(0, 50);
}

void ascunde_cursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void meniu_intrare()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 10);//seteaza culoarea la verse pe fundal negru
	gotoXY(60, 7);
	printf("TANKS");
	gotoXY(55, 9);
	printf("HOW TO PLAY");
	gotoXY(45, 11);
	printf("To move right press: > (right button)");
	gotoXY(45, 12);
	printf("To move left press: < (left button)");
	gotoXY(45, 13);
	printf("To increase the firing force: /\\ (up button)");
	gotoXY(45, 14);
	printf("To decrease the firing force: \\/ (down button)");
	gotoXY(45, 15);
	printf("To increase the angle: +");
	gotoXY(45, 16);
	printf("To decrease the angle: -");
	gotoXY(45, 17);
	printf("To change the missile: 1,2,3");
	gotoXY(45, 27);
	printf("Press any key to continue");
	char key='.';
	key=_getch();//permite jucatorilor sa pastreze meniul de intrare pe ecran oricat este necesar, o data cu apasarea unei taste functia va fi parasita
}

void afiseaza_scena()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 177);//seteaza culoarea cadranului la albastru pe fundal cyan
	gotoXY(0, 0);
	//se initializeaza matricea cu ' '
	for_i
		for_j
		scena[i][j] = ' ';
	//laturile din stanga si dreapta
	for_i
	{ scena[i][0] = BARA_VERTICALA;
	scena[i][DIM_X] = BARA_VERTICALA;
	}
    //laturile de sus si de jos
		for_j
	{
	scena[0][j] = BARA_ORIZONTALA;
	scena[DIM_Y-1][j] = BARA_ORIZONTALA;
	}
     //colturile
	scena[0][0] = COLT_STANGA_SUS;
	scena[0][DIM_X ] = COLT_DREAPTA_SUS;
	scena[DIM_Y-1 ][0] = COLT_STANGA_JOS;
	scena[DIM_Y-1][DIM_X] = COLT_DREAPTA_JOS;
	//afisaree scena
	for_i
	{ printf("\n");
		for_j
		printf("%c", scena[i][j]); }
    //afiseaza detaliile initiale ale jocului
	SetConsoleTextAttribute(consoleHandle, 176);
	gotoXY(3, 2);
	printf("Viata:%d\t\t\t\t\t\t\t\tViata:%d", viata1, viata2);//afiseaza viata
	gotoXY(3, 3);
	printf("Unghi:%d\t\t\t\t\t\t\t\tUnghi:%d",u1,u2);//unghiurile, initile de 90 grade
	gotoXY(3, 4);
	printf("Combustibil:%d\t\t\t\t\t\t\tCombustibil:%d", combustibil1, combustibil2);//combustibilul
	gotoXY(3, 5);
	printf("Proiectil:1.Proiectil simplu:%d\t\t\t\t\tProiectil:1.Proiectil simplu:%d", nr_proiectil1[0], nr_proiectil2[0]);//tipul de proiectil ce va fi lansat 
	SetConsoleTextAttribute(consoleHandle, 14);
	gotoXY(2, 0);
	printf("Player 1:%s\t\t\t\t\t\t\tPlayer2:%s",player1,player2);gotoXY(1, 5);//numele play-erilor
	SetConsoleTextAttribute(consoleHandle, 189);
	//bara de putere
	printf("%c", PUTERE);
	gotoXY(70, 5);
	printf("%c", PUTERE);
	for (int i = 1; i < 4; i++)
	{
		gotoXY(1, 5- i);
		printf("%c", LIPSA_PUTERE);
		gotoXY(70, 5 - i);
		printf("%c", LIPSA_PUTERE);
	}
}

void alege_landscape()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 14);
	gotoXY(45, 16);
	printf("Alegeti landscape:");
	gotoXY(45, 17);
	printf("1.Forest\t\t");
	gotoXY(45, 18);
	printf("2.Desert\t\t");
	gotoXY(45, 19);
	printf("3.Mountain\t\t");
	gotoXY(45, 20);
	printf("4.Random\t\t");
	L=_getch();//jucatorul va tasta cifra corespunzatoare landscapeului dorit si apoi se va iesi din functie
}

void nume_jucatori()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 12);
	gotoXY(45, 16);
	printf("Name player1:");
	gets(player1);//jucatorul 1 isi introduce numele si acesta va fi afisat in dreptul tancului sau
	gotoXY(45, 18);
	printf("Name player2:");
	gets(player2);//jucatorul 2 isi introduce numele
}

void golire_ecran()//se goleste ecranul pentru a permite afisarea urmatoarelor meniuri fara a aparea franturi din cele precedente
{
	for (int i = 0; i <= DIM_X; i++)
		for (int j = 0; j <= DIM_Y; j++)
		{
			gotoXY(i, j);
			printf(" ");
		}
}

int functie_landscape(int x)
{   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	double y1=1,x1;
	int y;
	x1 = (double)x;
	random://revenirea in switch dupa atribuirea valorii random
	switch (L)
	{
	case '1':
		y1 = (sin(x1 / 12.0) + 1.0) * 2.0 + 3.0;//functia dupa care se va construi terenul "padure"
		SetConsoleTextAttribute(consoleHandle, 178);
		break;
	case '2':
        y1 = (sin(x1 / 9.0) + 1.0) * 3.0+3.0;//functia dupa care se va construi terenul "desert"
		SetConsoleTextAttribute(consoleHandle, 182);
		break;
	case '3':
        y1 = (sin(x1 / 6.0) + 1.0) * 5.0 + 3.0;//functia dupa care va construi terenul "munte"
		SetConsoleTextAttribute(consoleHandle, 191);
		break;
	case '4':
	default://in cazul in care utilizatorul introduce o tasta eronata landscape-ul va fi ales random
		L = '0' + 1 + rand() % 3;//va atribui o valoare random posibila pt landscape
		goto random;
		break;
	}
	y = (int)y1;//coordonata pe y are nevoie sa primeasca valoarea intreaga cea mai apropiata de rezultatul functiei
	return y;
}

void deseneaza_landscape()
{   
	
	Crand = rand() % 50;//se alege random o pozitie de start pt functie,sinus fiind o functie periodica
	for (int j=DIM_Y-1; j>DIM_Y-13;j--)
	{ 
		gotoXY(1, j);
		for(int i=1 ; i<DIM_X ; i++)
		{int y;
			y = functie_landscape(i+Crand);
			if (j < DIM_Y && j >= DIM_Y - y)//afiseaza fiecare linie pe rand plecand de sus in jos
			{
				printf("%c", HASUR);
				scena[j-1][i] = HASUR;//atribuite si in spatele, in matrice
			}
			else printf(" "); 
		}
	}
	}

void gotoXY(int x, int y)
{
	COORD coord = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void deseneaza_tanc1(int x)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 180);
	int y=DIM_Y-1;//pleaca de la baza chenarului
	while (scena[y - 1][x] ==(char)HASUR) y--;//duce coordonata la suprafata pe coordonata x

    //schimba daca este nevoie coordonata pe y astfel incat tancul sa fie pozitionat in functie de cea mai inalta pozitie a terenului de sub el
	// fara acest while tancul poate suprascrie caracterele landscape-ului deformadu-l atat la pozitionarea initiala, cat si la deplasare
	
	while (scena[y - 1][x + 1] == (char)HASUR || scena[y - 1][x + 2] == (char)HASUR || scena[y - 1][x + 3] == (char)HASUR) 
		y--;
	if ((tura>1) && (y_tanc1-y >= 2))//opreste tancul din a urca mai mult de 2 caractere pe verticala la o mutare
	{
		//resetam coordonatele
		x = x_tanc1;
		y = y_tanc1;
		
		/*prin apasarea tastelor combustibilul scade automat, pentru a regla aceasta
		eroare, cand jocul nu permite deplasarea combustibilul "va fi dat inapoi"*/
		combustibil++;
    }
	if (x + 3 == x_tanc2 - 3)//opreste ciocnirea tancurilor si suprascrierea lor
	{
		x = x - 1;//tancul 2 poate fi doar in dreapta deci se reseteaza practil coordonata tancului 1 pe x
		combustibil++;
	}
	if (x <= 0)//nu permite tancului iesirea din scena
	{
		x = 1;
		combustibil++;
	}
	//afiseaza tancul de la stanga la dreapta de sus in jos incepand cu coordonatele fixate anterior
	for (int i = 0; i < 4; i++)
	{
		gotoXY(x + i, y);
		printf("%c", BAZA_TANC);
		scena[y-1][x + i] = BAZA_TANC;
	}
	for (int i = 1; i < 3; i++)
	{
		gotoXY(x + i, y-1);
		printf("%c", CORP_TANC);
		scena[y-2][x + i] = CORP_TANC;
	}
	gotoXY(x + 3, y-1);
	printf("/");
	scena[y-2][x + 3] = '/';
	//dupa mutare retinem pozitia tancului prin variabilele globale alocate
	x_tanc1 = x;
	y_tanc1 = y;
}

void deseneaza_tanc2(int x)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 176);//seteaza culoarea tancului 1 (negru pe cyan)
	int y=DIM_Y-1;//pleaca de la baza planului
	while (scena[y - 1][x] == (char)HASUR) y--;//urca pana la sufrafata terenului din dreptul coordonatei x

	//schimba daca este nevoie coordonata pe y astfel incat tancul sa fie pozitionat in functie de cea mai inalta pozitie a terenului de sub el
	// fara acest while tancul poate suprascrie caracterele landscape-ului deformadu-l atat la pozitionarea initiala, cat si la deplasare
	while (scena[y - 1][x - 1] == (char)HASUR || scena[y - 1][x - 2] == (char)HASUR || scena[y - 1][x - 3] == (char)HASUR)
		y--;
	if ((tura > 1) && (y_tanc2 - y >= 2))//impiedica urcarea tancului cu mai mult de 2 caractere pe verticala 
	{
		//reseteaza parametrii schimbati prin apelarea functiei de deplasare
		x = x_tanc2;
		y = y_tanc2;
		combustibil++;
	}
	if (x - 3 == x_tanc1 + 3)//impiedica ciocnirea si suprascrierea tancurilor
	{
		//reseteaza parametrii
		x = x + 1;
		combustibil++;
	}
	if (x >= DIM_X)//impiedica iesirea din chenar
	{
		//reseteaza parametrii
		x = DIM_X - 1;
		combustibil++;
	}
	//afiseaza de la dreapta la stanga si de jos in sus tancul pornind de la coordonatele setate mai sus
	for (int i = 0; i < 4; i++)
	{
		gotoXY(x - i, y);
		printf("%c", BAZA_TANC);
		scena[y-1][x - i] = BAZA_TANC;
	}
	for (int i = 1; i < 3; i++)
	{
		gotoXY(x - i, y-1);
		printf("%c", CORP_TANC);
		scena[y-2] [x - i]= CORP_TANC;
	}
	gotoXY(x-3 , y-1);
	printf("\\");
	scena[y-2][x - 3] = '\\';
	//retinerea noilor coordonate in variabilele globale corespunzatoare
	x_tanc2 = x;
	y_tanc2 = y;
}

void detoneaza_proiectil(int u)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 176);
	gotoXY(x_proiectil_nou, y_proiectil_nou);
	printf("%c",proiectil[0]);
	int hmax, urcare,drum,dmax; 
	double u2,h,d,dx,k; 
	
	//daca unghiul este <90 functia va deplasa proiectilul la dreapta
	if (u <= 90)
	{//dublura folosita in cadrul functiilor trigonometrice
		u2 = (double)(u);
	 //fixarea  unei inaltimi maxime pana la care poate urca proiectilul in functie de unghiul si puterea selectate de jucator
	hmax = u / (7*(5-putere));
	h = (double)(hmax);
    //contor prntru urcare
	urcare = hmax;
	//fixarea unei distante maxime de deplasare pe Ox in functie de unghi si putere
	dmax = abs((90 - u) /( 7*(5-putere))*2);
	//contor de deplasare
	drum = dmax;
	d = (double)(dmax);
	//contor de crestere a coordonatei pe Ox
	k = d / (h+1.0);
	//o copie a coordonatei pe x, va ajuta la micsorarea erorilor de conversie (de ex daca k=0,5 fara aceasta conversie x nu va creste niciodata)
	dx = (double)(x_proiectil_nou);
	if (u != 90 && u != 0)
	{  //afisarea proiectilului cu efect de urcare cat timp nu s-a atins inaltimea maxima, proiectilul nu s-a ciocnit inca de teren si nu a iesit fin chenar 
		while ((drum != 0 || urcare != 0) && scena[y_proiectil_nou-1][x_proiectil_nou]==' ' && x_proiectil_nou<DIM_X-2 && x_proiectil_nou>0)
		{
			
			gotoXY(x_proiectil_nou, y_proiectil_nou);
			printf("%c",proiectil[0]);
			scena[y_proiectil_nou-1][x_proiectil_nou] = proiectil[0];
			gotoXY(x_proiectil_vechi, y_proiectil_vechi);
			printf(" ");
			scena[y_proiectil_vechi-1][x_proiectil_vechi] = ' ';
			x_proiectil_vechi = x_proiectil_nou;
			y_proiectil_vechi = y_proiectil_nou;
			dx += k;
			if (drum != 0 && urcare!=0)//creste proportional cu k pana la dmax sau hmax
			{
				x_proiectil_nou = (int)(dx);
				drum--;
			}
			else if (drum != 0)//daca s-a trecut de hmax dar nu a fosr atinsa distanta maxima cresterea va fi 1
			{
				x_proiectil_nou++;
				drum--;
			}
			if (urcare != 0)// nu a fost atinsa inaltimea maxima proiectilul va urca cu 1 la fiecare mutare
			{
				y_proiectil_nou--;
				urcare--;
			}
			Sleep(50);
		}
		//afisarea proiectilului cu efect de cadere
		while (scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou < DIM_X && y_proiectil_nou<DIM_Y )
		{
			
			gotoXY(x_proiectil_nou, y_proiectil_nou);
			printf("%c",proiectil[0]);
			scena[y_proiectil_nou-1][x_proiectil_nou] = proiectil[0];
			gotoXY(x_proiectil_vechi, y_proiectil_vechi);
			printf(" ");
			scena[y_proiectil_vechi-1][x_proiectil_vechi] = ' ';
			x_proiectil_vechi = x_proiectil_nou;
			y_proiectil_vechi = y_proiectil_nou;
			if (u > 30)
			{
				dx += k;
				x_proiectil_nou = (int)(dx);
			}
			else x_proiectil_nou += 2;//implementeaza o traiectorie particulara pentru unghiurile mici, mai greu de reprezentat
			y_proiectil_nou++;//la fiecare mutare y creste cu 1
			Sleep(100);
		}
	}
	//cazul u=90 este un caz tratat particular
	if (u == 90)
	{
		//urcarea
		while (urcare != 0 && scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ')
		{
			gotoXY(x_proiectil_nou, y_proiectil_nou);
			printf("%c",proiectil[0]);
			scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
			gotoXY(x_proiectil_vechi, y_proiectil_vechi);
			printf(" ");
			scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
			x_proiectil_vechi = x_proiectil_nou;
			y_proiectil_vechi = y_proiectil_nou;
			y_proiectil_nou--;
			urcare--;
			Sleep(70);
		}
		//coborarea pana la lovirea propriului tanc
		while (scena[y_proiectil_nou-1][x_proiectil_nou] == ' ')
		{gotoXY(x_proiectil_nou, y_proiectil_nou);
		printf("%c",proiectil[0]);
		scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
		gotoXY(x_proiectil_vechi, y_proiectil_vechi);
		printf(" ");
		scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
		x_proiectil_vechi = x_proiectil_nou;
		y_proiectil_vechi = y_proiectil_nou;
		y_proiectil_nou++;
		Sleep(70);
		}

	}
	//cazul u=0 este un caz tratat particular
	if (u == 0)
	{    
		//deplasarea dreapta
		while (drum != 0 && scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou < DIM_X)
		{
			gotoXY(x_proiectil_nou, y_proiectil_nou);
			printf("%c",proiectil[0]);
			scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
			gotoXY(x_proiectil_vechi, y_proiectil_vechi);
			printf(" ");
			scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
			x_proiectil_vechi = x_proiectil_nou;
			y_proiectil_vechi = y_proiectil_nou;
			x_proiectil_nou++;
			drum--;
			Sleep(50);
		}
		//coborarea dupa atingerea distantei prestabilite de deplasare orizontala
		while (scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou < DIM_X && y_proiectil_nou<DIM_Y)
		{
			gotoXY(x_proiectil_nou, y_proiectil_nou);
			printf("%c",proiectil[0]);
			scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
			gotoXY(x_proiectil_vechi, y_proiectil_vechi);
			printf(" ");
			scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
			x_proiectil_vechi = x_proiectil_nou;
			y_proiectil_vechi = y_proiectil_nou;
			y_proiectil_nou++;
			x_proiectil_nou+=2;
			Sleep(70);
		}
	}
	}
	//pentru unghiuri mai mari se realizeaza o simetrie la stanga a algoritmului anterior
	if (u > 90)
	{
		int aux;
		aux = 180 - u;//aducem unghiul la valoarea corespunzatoare din chenarul I
		//valorile urmatoare sunt implementate prin logica u<90 corespondentul lui u fiind aux
		u2 = (double)(aux);
		hmax = aux / (7*(5-putere)); h = (double)(hmax);
		urcare = hmax;
		dmax = abs((90 - aux) / (7*(5-putere))) * 2;
		drum = dmax;
		d = (double)(dmax);
		k = d / (h + 1.0);
		dx = (double)(x_proiectil_nou);
		if ( aux != 0)
		{   
			//urcarea si deplasarea la stanga pana la lovirea terenului, tancului sau iesirii din chenar
			while ((drum != 0 || urcare != 0) && scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou >0 && y_proiectil_nou<DIM_Y)
			{

				gotoXY(x_proiectil_nou, y_proiectil_nou);
				printf("%c",proiectil[0]);
				scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
				gotoXY(x_proiectil_vechi, y_proiectil_vechi);
				printf(" ");
				scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
				x_proiectil_vechi = x_proiectil_nou;
				y_proiectil_vechi = y_proiectil_nou;
				dx -= k;
				if (drum != 0 && urcare!=0)
				{
					x_proiectil_nou = (int)(dx);
					drum--;
				}
				else if (drum != 0)
				{
					x_proiectil_nou--;
					drum--;
				}
				if (urcare != 0)
				{
					y_proiectil_nou--;
					urcare--;
				}
				Sleep(50);
			}
			//coborarea pana la lovirea terenului, a tancului sau iesirea din chenar
			while (scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou >0 && y_proiectil_nou<DIM_Y)
			{

				gotoXY(x_proiectil_nou, y_proiectil_nou);
				printf("%c",proiectil[0]);
				scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
				gotoXY(x_proiectil_vechi, y_proiectil_vechi);
				printf(" ");
				scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
				x_proiectil_vechi = x_proiectil_nou;
				y_proiectil_vechi = y_proiectil_nou;
				if (u > 30)
				{
					dx -= k;
					x_proiectil_nou = (int)(dx);
				}
				else x_proiectil_nou -= 2;//particularizarea cazului pentru unghiuri mici
				y_proiectil_nou++;
				Sleep(100);
			}
		}
		//cazul u=180, aux=0 este tratat particular analog ca cel prentru u=0, dar deplasarea se face la stanga
		if (aux == 0)
		{
			//deplasarea orizontala pe distanta setata anterior
			while (drum != 0 && scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou > 0 )
			{
				gotoXY(x_proiectil_nou, y_proiectil_nou);
				printf("%c",proiectil[0]);
				scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
				gotoXY(x_proiectil_vechi, y_proiectil_vechi);
				printf(" ");
				scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
				x_proiectil_vechi = x_proiectil_nou;
				y_proiectil_vechi = y_proiectil_nou;
				x_proiectil_nou--;
				drum--;
				Sleep(50);
			}
			//coborarea
			while (scena[y_proiectil_nou - 1][x_proiectil_nou] ==' ' && x_proiectil_nou >0 && y_proiectil_nou<DIM_Y)
			{
				gotoXY(x_proiectil_nou, y_proiectil_nou);
				printf("%c",proiectil[0]);
				scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
				gotoXY(x_proiectil_vechi, y_proiectil_vechi);
				printf(" ");
				scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
				x_proiectil_vechi = x_proiectil_nou;
				y_proiectil_vechi = y_proiectil_nou;
				y_proiectil_nou++;
				x_proiectil_nou -= 2;
				Sleep(70);
			}
		}
	}
	//in final trebuie stearsa de pe ecran si din matrice ultima pozitie a proiectilului
			gotoXY(x_proiectil_vechi, y_proiectil_vechi);
		    printf(" ");
			scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
}
void pregatire_player1()
{   
	//se actualizeaza parametrii generali cu valorile retinute particular pentru tancul 1
	//aceste variabile generale se folosesc in functiile comune precum cresterea/scadere unghiului, puterii, combustivilului,deplasarile la dreapta si stanga, etc.
	u = u1;
	X = x_tanc1;
	Y = y_tanc1;
	x_proiectil_nou=x_proiectil_vechi= x_tanc1 + 3;
	y_proiectil_nou = y_proiectil_vechi = y_tanc1 -2;
	putere = putere1;
	combustibil = combustibil1;
	nr_proiectil = nr_proiectil1[tip_proiectil1 - 1];
	tip_proiectil = tip_proiectil1;
	proiectil[0] = proiectil1[0];
}
void pregatire_player2()
{
	//se actualizeaza parametrii generali cu valorile retinute particular pentru tancul 2
	x_proiectil_nou=x_proiectil_vechi = x_tanc2-3;
	y_proiectil_nou=y_proiectil_vechi =y_tanc2-2;
	u = u2;
	X = x_tanc2;
	Y = y_tanc2;
	putere = putere2;
	combustibil = combustibil2;
	nr_proiectil = nr_proiectil2[tip_proiectil2 - 1];
	tip_proiectil = tip_proiectil2;
	proiectil[0] = proiectil2[0];
}

int joc_pierdut()
{   
	if(viata1 ==0 || viata2 ==0)//verifica viata celor doi jucatori
	return 1;
else
	return 0;
}

void verifica_citire()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	char ch;
	if (_kbhit())
	{
		ch = _getch();//citeste fara ecou un caracter
		switch (ch)
		{
		case '1'://afiseaza proiectilul 1 si schimba in spate parametrii pentru pregatirea lansarii, dar nu lanseaza
			if (tura % 2 == 1)
			{
				tip_proiectil=tip_proiectil1 = 1;
				nr_proiectil = nr_proiectil1[tip_proiectil1 - 1];
				proiectil[0]=proiectil1[0] ='o';
			}
			else
			{
				tip_proiectil=tip_proiectil2 = 1;
				nr_proiectil = nr_proiectil2[tip_proiectil2 - 1];
				proiectil[0]=proiectil2[0] ='o';
			}
			afisare_tip_proiectil();
			break;
		case '2'://afiseaza proiectilul 2 si schimba in spate parametrii pentru pregatirea lansarii, dar nu lanseaza
			
			if (tura % 2 == 1)
			{
				tip_proiectil=tip_proiectil1 = 2;
				nr_proiectil = nr_proiectil1[tip_proiectil1 - 1];
				proiectil[0]=proiectil1[0] ='*';
			}
			else
			{
				tip_proiectil=tip_proiectil2 = 2;
				nr_proiectil = nr_proiectil2[tip_proiectil2 - 1];
				proiectil[0]=proiectil2[0] ='*';
			}
			afisare_tip_proiectil();
			break;
		case '3'://afiseaza proiectilul 3 si schimba in spate parametrii pentru pregatirea lansarii, dar nu lanseaza
			if (tura % 2 == 1)
			{
				tip_proiectil = tip_proiectil1 = 3;
				nr_proiectil = nr_proiectil1[tip_proiectil1 - 1];
				proiectil[0] = proiectil1[0] = PROIECTIL_AVION;
			}
			if(tura%2==0)
			{
				tip_proiectil = tip_proiectil2 = 3;
				nr_proiectil = nr_proiectil2[tip_proiectil2 - 1];
				proiectil[0] = proiectil2[0] = PROIECTIL_AVION;
			}
			afisare_tip_proiectil();
			break;
		case LEFT://deplaseaza la stanga tancul a carui tura este
			if (combustibil != 0)//opreste intrarea in functii daca jucatorul nu mai are combustibil
				deplasare_stanga();
			afisare_combustibil();
			break;
		case RIGHT://deplaseaza la dreapta 
			if (combustibil != 0)//impiedica deplasarea daca jucatorul nu mai are combustibil
			deplasare_dreapta();
			afisare_combustibil();
			break;
		case UP://creste puterea si o pregateste cu valoarea corespunzatoate pentru lansare
			SetConsoleTextAttribute(consoleHandle, 189);
			if (putere < 4)//impiedica cresterea acesteia peste pragul maxim
			putere++;
			//afisarea modificarilor
			if (tura % 2 == 1)
			{
				gotoXY(1, 6 - putere);
				printf("%c", PUTERE);
			  }
			else
			{
				gotoXY(70, 6- putere);
				printf("%c", PUTERE);
			}
			break;
		case DOWN://scade puterea si o pregateste pt o eventuala lansare
			SetConsoleTextAttribute(consoleHandle, 189);
			if (putere > 1) // impiedica scaderea ei sub 1, pragul minim
				putere--;
			//afisarea schimbarilor
			if (tura % 2 == 1)
			{
				gotoXY(1, 5- putere);
				printf("%c", LIPSA_PUTERE);
			}
			else
			{
				gotoXY(70, 5 - putere);
				printf("%c", LIPSA_PUTERE);
			}
			break;
		case'+'://cresterea unghiului
			if (u < 180)//oprirea la pragul de 180
				u++;
			//afisarea schimbarii
			SetConsoleTextAttribute(consoleHandle, 176);
			if (tura % 2 == 1) gotoXY(9, 3);
			else gotoXY(78, 3);
			printf("   ");
			if (tura % 2 == 1) gotoXY(9, 3);
			else gotoXY(78, 3);
			printf("%d", u);
			break;
		case'-'://scaderea unghiului
			if (u > 0)//oprirea la pragul minim de 0
				u--;
			//afisarea schimbarilor
			SetConsoleTextAttribute(consoleHandle, 176);
			if (tura % 2 == 1) gotoXY(9,3);
			else gotoXY(78, 3);
			printf("   ");
			if (tura % 2 == 1) gotoXY(9, 3);
			else gotoXY(78, 3);
			printf("%d", u);
			break;
		case ' '://detonarea proiectilului ales
			if (nr_proiectil)//verifica disponibilitatea proiectilului, daca acesta nu poate fi actionat jucatorul are posibilitatea sa schimbe 
			{
				if (tip_proiectil == 3) avion();
				else
				{
					detoneaza_proiectil(u);
					crater();
				}
				//dupa fiecare detonare de vor reafisa tancurile in prevederea unui eventual crater format sub acestea
					stergere_tanc1();
					deseneaza_tanc1(x_tanc1);
					stergere_tanc2();
					deseneaza_tanc2(x_tanc2);
				//scaderea nr de proiectile disponibile de tipul celui detonat
				if (tura % 2 == 1)
					nr_proiectil1[tip_proiectil1 - 1]--;
				else nr_proiectil2[tip_proiectil2 - 1]--;
				afisare_tip_proiectil();
				putere0();//resetarea puterii la 0
				stop = 1;//marcheaza sfarsitul unei ture, jucatorul nu imi mai poate regla setarile timp de o tura
			}
			break;
		default: break; // la afisarea unei taste neprevazute nu se va intampla nimic
		}
	}
}

void crater()
{
		int i = 0;
		if (scena[y_proiectil_nou - 1][x_proiectil_nou] == (char)HASUR && scena[y_proiectil_nou - 2][x_proiectil_nou] == (char)HASUR && (tip_proiectil==1 || tip_proiectil==2))
		{
			if (u < 90)
			{
				while (scena[y_proiectil_nou - 1 - i][x_proiectil_nou - i] == (char)HASUR)
					i++;
				i--;
				if (tip_proiectil == 1)
				{   gotoXY(x_proiectil_nou - i, y_proiectil_nou - i);
					printf(" ");
					scena[y_proiectil_nou - 1 - i][x_proiectil_nou - i] = ' ';
				}
				else if (tip_proiectil == 2)
				{
					for(int j=0;j<3 && y_proiectil_nou - i + j<DIM_Y;j++)
						for (int k = 0; k <= 2 - j && x_proiectil_nou - i - k>0 && x_proiectil_nou - i + k<DIM_X; k++)
						{
							gotoXY(x_proiectil_nou - i - k, y_proiectil_nou - i + j);
							printf(" ");
							scena[y_proiectil_nou - 1 - i+j][x_proiectil_nou - i-k] = ' ';
							gotoXY(x_proiectil_nou - i + k, y_proiectil_nou - i + j);
							printf(" ");
							scena[y_proiectil_nou - 1 - i + j][x_proiectil_nou - i + k] = ' ';
						}
				}
			}
			if (u > 90)
			{
				while (scena[y_proiectil_nou - 1 - i][x_proiectil_nou + i] == (char)HASUR)
					i++;
				i--;
				if (tip_proiectil == 1)
				{
					gotoXY(x_proiectil_nou + i, y_proiectil_nou - i);
					printf(" ");
					scena[y_proiectil_nou - 1 - i][x_proiectil_nou + i] = ' ';
				}
				else if (tip_proiectil == 2)
				{
					for (int j = 0; j < 3 && y_proiectil_nou - i + j<DIM_Y; j++)
						for (int k = 0; k <= 2 - j && x_proiectil_nou + i - k > 0 && x_proiectil_nou + i + k < DIM_X; k++)
						{
							gotoXY(x_proiectil_nou + i - k, y_proiectil_nou - i + j);
							printf(" ");
							scena[y_proiectil_nou - 1 - i + j][x_proiectil_nou + i - k] = ' ';
							gotoXY(x_proiectil_nou + i + k, y_proiectil_nou - i + j);
							printf(" ");
							scena[y_proiectil_nou - 1 - i + j][x_proiectil_nou + i + k] = ' ';
						}
				}
			}
		}
		else if (scena[y_proiectil_nou - 1][x_proiectil_nou] == (char)HASUR)
		{
			if (tip_proiectil == 1)
			{
				gotoXY(x_proiectil_nou, y_proiectil_nou);
				printf(" ");
				scena[y_proiectil_nou - 1][x_proiectil_nou] = ' ';
			}
			else if (tip_proiectil == 2)
			{
				for (int j = 0; j < 3; j++)
					for (int k = 0; k <= 2 - j; k++)
					{
						gotoXY(x_proiectil_nou - k, y_proiectil_nou + j);
						printf(" ");
						scena[y_proiectil_nou - 1 + j][x_proiectil_nou - k] = ' ';
						gotoXY(x_proiectil_nou + k, y_proiectil_nou + j);
						printf(" ");
						scena[y_proiectil_nou - 1 + j][x_proiectil_nou + k] = ' ';
					}
			}
		}
		 if (tip_proiectil == 3)
			{
				int y = DIM_Y - 1,x;
				x = x_proiectil_nou;
				while (scena[y - 1][x] == (char)HASUR) y--;
				while (scena[y - 1][x + 1] == (char)HASUR || scena[y - 1][x + 2] == (char)HASUR || scena[y - 1][x-1] == (char)HASUR || scena[y - 1][x - 2] == (char)HASUR)
					y--;
				y++;
				for (int j = 0; j < 3 && y + j < DIM_Y; j++)
					for (int k = 0; k <= 2 - j && x - i - k > 0 && x - i + k < DIM_X; k++)
					{
						gotoXY(x - k, y + j);
						printf(" ");
						scena[y - 1+ j][x - k] = ' ';
						gotoXY(x + k, y + j);
						printf(" ");
						scena[y - 1 + j][x + k] = ' ';
					}
				y = DIM_Y - 1;
					x = x - 15;
					while (scena[y - 1][x] == (char)HASUR) y--;
					y++;
						for (int k = 0; k < 2 && x - k > 0 && x - k < DIM_X; k++)
						{
							gotoXY(x - k, y);
							printf(" ");
							scena[y - 1][x - k] = ' ';
						}
						y = DIM_Y - 1;
					x = x + 30;
					while (scena[y - 1][x] == (char)HASUR) y--;
					y++;
					for (int k = 0; k < 2 && x + k > 0 && x + k < DIM_X; k++)
					{
						gotoXY(x + k, y);
						printf(" ");
						scena[y - 1][x + k] =' ';
					}
			}
}

void afisare_combustibil()
{ 
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 176);
	if(tura%2==1)
	{gotoXY(15, 4);
	printf("  ");
	gotoXY(15, 4);
	printf("%d", combustibil); }
else if (tura % 2 == 0)
{
	gotoXY(84, 4);
	printf("  ");
	gotoXY(84, 4);
	printf("%d", combustibil);

}
}

void putere0()
 {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 189);
		if(tura%2==1)
		{
			putere = 1;
			gotoXY(1, 5);
			printf("%c", PUTERE);
	for (int i = 1; i < 4; i++)
	{
		gotoXY(1, 5 - i);
		printf("%c", LIPSA_PUTERE);
	}
	    }
		else if(tura%2==0)
			{
				putere = 1;
	            gotoXY(70, 5);
	            printf("%c", PUTERE);
				for (int i = 1; i < 4; i++)
				{
					gotoXY(70, 5 - i);
					printf("%c", LIPSA_PUTERE);
				}
			}

}

void verificare_viata()
{
	if (tura % 2 == 1 && tip_proiectil==1)
	{
		if (u == 90)
		{
			viata1--;
			y_proiectil_nou = y_tanc1 +1;
			crater();
		}
		else if (x_proiectil_nou <= x_tanc2 + 3 && x_proiectil_nou >= x_tanc2 - 3)
		{
			viata2--;
			y_proiectil_nou = y_tanc2 + 1;
			crater();
		}
	}
	if (tura % 2 == 1 && tip_proiectil == 2)
	{
		if (u == 90)
		{
			viata1 -= 4;
			y_proiectil_nou = y_tanc1 + 1;
			crater();
		}
		else if (x_proiectil_nou <= x_tanc2 + 3 && x_proiectil_nou >= x_tanc2 - 3)
			viata2 -= 4;
		else if (x_proiectil_nou <= x_tanc2 + 5 && x_proiectil_nou >= x_tanc2 - 5)
		{
			viata2 -= 2;
			y_proiectil_nou = y_tanc2 + 1;
			crater();
		}
	}
	if (tura % 2 == 0 && tip_proiectil==1)
	{
		if (u == 90)
		{
			viata2--;
			y_proiectil_nou = y_tanc2 + 1;
			crater();
		}
		else if (x_proiectil_nou <= x_tanc1 + 3 && x_proiectil_nou >= x_tanc1 - 3)
		{
			viata1--;
			y_proiectil_nou = y_tanc1 + 1;
			crater();
		}
	}
	if (tura % 2 == 0 && tip_proiectil == 2)
	{
		if (u == 90)
		{
			viata2 -= 4;
			y_proiectil_nou = y_tanc2 + 1;
			crater();
		}
		else if (x_proiectil_nou <= x_tanc1 + 3 && x_proiectil_nou >= x_tanc1 - 3)
			viata1 -= 4;
		else if (x_proiectil_nou <= x_tanc1 + 5 && x_proiectil_nou >= x_tanc1 - 5)
		{viata1 -= 2;
		y_proiectil_nou = y_tanc1 + 1;
		crater();
		}
	}
	if (tip_proiectil == 3)
	{
		if (tura % 2 == 1) viata2 -= 2;
		if (tura % 2 == 0) viata1 -= 2;
	}
}

void afisare_viata()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 176);
	gotoXY(9, 2);
	printf("  ");
	gotoXY(9, 2);
	printf("%d", viata1);
	gotoXY(78, 2);
	printf("  ");
	gotoXY(78, 2);
	printf("%d", viata2);
}

void deplasare_dreapta()
{
	
		
		if (tura % 2 == 1)
		{
			stergere_tanc1();
			deseneaza_tanc1(x_tanc1 + 1);
			X = x_tanc1;
			Y = y_tanc1;
			x_proiectil_nou = x_proiectil_vechi = x_tanc1 + 3;
			y_proiectil_nou = y_proiectil_vechi = y_tanc1 - 2;
		}
		else if (tura % 2 == 0)
		{
			stergere_tanc2();
			deseneaza_tanc2(x_tanc2 + 1);
			X = x_tanc2;
			Y = y_tanc2;
			x_proiectil_nou = x_proiectil_vechi = x_tanc2 - 3;
			y_proiectil_nou = y_proiectil_vechi = y_tanc2 - 2;
		}
		combustibil--;

}

void deplasare_stanga()
{
	if (tura % 2 == 1)
	{
		stergere_tanc1();
		deseneaza_tanc1(x_tanc1 - 1);
		X = x_tanc1;
		Y = y_tanc1;
		x_proiectil_nou = x_proiectil_vechi = x_tanc1 + 3;
		y_proiectil_nou = y_proiectil_vechi = y_tanc1 - 2;
	}
	else if (tura % 2 == 0)
	{
		stergere_tanc2();
		deseneaza_tanc2(x_tanc2 - 1);
		X = x_tanc2;
		Y = y_tanc2;
		x_proiectil_nou = x_proiectil_vechi = x_tanc2 - 3;
		y_proiectil_nou = y_proiectil_vechi = y_tanc2 - 2;
	}
	combustibil--;
}

void stergere_tanc1()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 187);
	for (int i = 0; i < 4; i++)
	{
		gotoXY(x_tanc1 + i, y_tanc1);
		printf(" ");
		scena[y_tanc1- 1][x_tanc1+ i] =' ';
	}
	for (int i = 1; i < 3; i++)
	{
		gotoXY(x_tanc1 + i, y_tanc1-1);
		printf(" ");
		scena[y_tanc1 - 2][x_tanc1 + i] = ' ';
	}
	gotoXY(x_tanc1 + 3, y_tanc1-1);
	printf(" ");
	scena[y_tanc1 - 2][x_tanc1 + 3] = ' ';
}

void stergere_tanc2()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 187);
	for (int i = 0; i < 4; i++)
	{
		gotoXY(x_tanc2- i, y_tanc2);
		printf("%c", ' ');
		scena[y_tanc2 - 1][x_tanc2 - i] = ' ';
	}
	for (int i = 1; i < 3; i++)
	{
		gotoXY(x_tanc2- i, y_tanc2 - 1);
		printf("%c", ' ');
		scena[y_tanc2 - 2][x_tanc2 - i] =' ';
	}
	gotoXY(x_tanc2 - 3, y_tanc2 - 1);
	printf(" ");
	scena[y_tanc2 - 2][x_tanc2 - 3] = ' ';
	//x_tanc2 = X;
	//y_tanc2 = Y;
}
 
void afisare_tip_proiectil()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleHandle, 176);
	gotoXY(13, 5);
	printf("                      ");
	gotoXY(13, 5);
	switch (tip_proiectil1)
	{  case 1:
		printf("1.Proiectil simplu:%d", nr_proiectil1[tip_proiectil1-1]);
		break;
	case 2:
		printf("2.Bomba:%d", nr_proiectil1[tip_proiectil1 - 1]);
		break;
	case 3:printf("3.Avion:%d", nr_proiectil1[tip_proiectil1 - 1]);
		break;
	}
	gotoXY(82, 5);
	printf("                      ");
	gotoXY(82, 5);
	switch (tip_proiectil2)
	{
	case 1:
		printf("1.Proiectil simplu:%d", nr_proiectil2[tip_proiectil2 - 1]);
		break;
	case 2:
		printf("2.Bomba:%d", nr_proiectil2[tip_proiectil2 - 1]);
		break;
	case 3:
		printf("3.Avion:%d", nr_proiectil2[tip_proiectil2 - 1]);
		break;
	}
}

void sfarsit_joc()
{
	gotoXY(50, 15);
	printf("GAME OVER");
	if (viata1 == 0)
	{
		gotoXY(45, 16);
		printf("WINNER : PLAYER 2");
	}
	else if (viata2 == 0)
	{
		gotoXY(45, 16);
		printf("WINNER : PLAYER 1");
	}
	gotoXY(45, 18);
		printf("To play again press SPACE");
				
}

void avion()
{    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if(tura%2==1)
	SetConsoleTextAttribute(consoleHandle, 180);
    if(tura%2==0)
	SetConsoleTextAttribute(consoleHandle, 176);
	x_avion_nou = 1;
	gotoXY(x_avion_nou, 9);   printf(">");   printf(" ");
	Sleep(100);
	gotoXY(x_avion_nou, 8);   printf("%c",ARIPA_SUS);
	gotoXY(x_avion_nou, 9);   printf("%c>", CORP_ARIPI);
	gotoXY(x_avion_nou, 10);  printf("%c",ARIPA_JOS);
	Sleep(100);
	gotoXY(x_avion_nou, 8);   printf(" ");
	gotoXY(x_avion_nou, 9);   printf("  ");
	gotoXY(x_avion_nou, 10);  printf(" ");
	gotoXY(x_avion_nou+1, 8);   printf("%c", ARIPA_SUS);
	gotoXY(x_avion_nou, 9);   printf("%c%c>", CORP_AVION,CORP_ARIPI);
	gotoXY(x_avion_nou+1, 10);  printf("%c", ARIPA_JOS);
	Sleep(100);
	gotoXY(x_avion_nou+1, 8);   printf(" ");
	gotoXY(x_avion_nou, 9);   printf("   ");
	gotoXY(x_avion_nou+1, 10);  printf(" ");
	while (x_avion_nou < DIM_X-4)
    {    
		if (tura % 2 == 1)
			if (x_avion_nou + 1 == x_tanc2 - 1)
			{
				proiectil_avion();
				crater();
				stergere_tanc1();
				deseneaza_tanc1(x_tanc1);
				stergere_tanc2();
				deseneaza_tanc2(x_tanc2);
				SetConsoleTextAttribute(consoleHandle, 180);
			}
		if(tura%2==0)
			if (x_avion_nou + 1 == x_tanc1 + 1)
			{
				proiectil_avion();
				crater();
				stergere_tanc1();
				deseneaza_tanc1(x_tanc1);
				stergere_tanc2();
				deseneaza_tanc2(x_tanc2);
			}
		gotoXY(x_avion_nou + 2, 8);   printf("%c", ARIPA_SUS);
		gotoXY(x_avion_nou, 9);   printf(">%c%c>", CORP_AVION, CORP_ARIPI);
		gotoXY(x_avion_nou + 2, 10);  printf("%c", ARIPA_JOS);
		Sleep(100);
		gotoXY(x_avion_nou + 2, 8);   printf(" ");
		gotoXY(x_avion_nou, 9);   printf("    ");
		gotoXY(x_avion_nou + 2, 10);  printf(" ");
		x_avion_nou++;
		
    }
	gotoXY(x_avion_nou + 1, 8);   printf("%c", ARIPA_SUS);
	gotoXY(x_avion_nou, 9);   printf(">%c%c", CORP_AVION, CORP_ARIPI);
	gotoXY(x_avion_nou + 2, 10);  printf("%c", ARIPA_JOS);
	Sleep(100);
	gotoXY(x_avion_nou + 1, 8);   printf(" ");
	gotoXY(x_avion_nou, 9);   printf("   ");
	gotoXY(x_avion_nou + 2, 10);  printf(" ");
    x_avion_nou++;
	gotoXY(x_avion_nou, 9);   printf(">%c", CORP_AVION);
    Sleep(100);
	gotoXY(x_avion_nou, 9);   printf("   ");
	x_avion_nou++;
	gotoXY(x_avion_nou, 9);   printf(">");
	Sleep(100);
	gotoXY(x_avion_nou, 9);   printf(" ");
}

void proiectil_avion()
{   
	x_proiectil_nou = x_proiectil_vechi = x_avion_nou + 1;
	y_proiectil_nou = y_proiectil_vechi = 10;
	while (scena[y_proiectil_nou - 1][x_proiectil_nou] == ' ')
	{
		gotoXY(x_proiectil_nou, y_proiectil_nou);
		printf("%c", proiectil[0]);
		scena[y_proiectil_nou - 1][x_proiectil_nou] = proiectil[0];
		gotoXY(x_proiectil_vechi, y_proiectil_vechi);
		printf(" ");
		scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
		x_proiectil_vechi = x_proiectil_nou;
		y_proiectil_vechi = y_proiectil_nou;
		y_proiectil_nou++;
		Sleep(50);
	}
	gotoXY(x_proiectil_vechi, y_proiectil_vechi);
	printf(" ");
	scena[y_proiectil_vechi - 1][x_proiectil_vechi] = ' ';
}