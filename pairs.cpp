#include "defkey.h"
#include <string.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int getkey();
char *alltrim(char *s);
void Menu(int color, ...);
void Menu2(int color, ...);
void MenuInTheEnd(int color, ...);
bool DrawMask(char *File, char *Items[]);
char *Line(char *s, int n, int FlagLine);
char *replicate(char *s, unsigned int n);
void outtxt(char *s, int x, int y, int color);
int LineEdit(char *s, int x, int y, int color);
void MaskEdit(char *First, int Length[], int color, int *randomNumber);
void Frame(int x1, int y1, int x2, int y2, int color, int Flag);
void ScrollMenu(int d[], char *c, char *o[], int *x, int *y, int n,int fromTable);
void ScrollMenu2(int d[], char *c, char *o[], int *x, int *y, int n);
void Grid(int m, int n, int w, int h, int x, int y, int color);
void DrawNumbersBoard();//crta tebelu se ispisuju brojevi
void DrawScoreBoard();//crta tabelu sa rezultatima
void DrawLettersBoard();//crta tabelu sa rezultatima pogadjanja
void EnterName(int x, int y);//prvo se poziva ovaj metod radi unosa imena pa tek onda krece igra
void ChooseAction(int action, int fromTable);//nako zavrsene igde se bira akcija New, Ignore, Exit
void NewGame();
void ReadTops();//uzima iz fajla top 10
void CheckScoreBoard();//proverava poziciju na tabeli
void WriteScore();//upise rezultate u file
void DrawTry();//crta broj pokusaja i update
void MaskEdit2();
void hidecursor();
void showcursor();
void ShowUpLetter(int x, int y);
void MoveToNextBoard();
void ShowNumber(int x, int y);
void ShowLetter(int coX, int coY, int x, int y);
bool CheckFilds(char *p, int x, int y);
void ResetMissed();
void GoToFirstFree();
void GoToFirstFree2();
bool isPossible(int direction);
bool isPossible2(int direction);
void GenerateMatrix();
void ResetValues();
void DrawDownMenu();
void PaintInG();

//Global
char names[20][14];
char score[5];
int numberOfTries;
int numberOfTrues;
char player[17];
int nowX,nowY,defX,defY,maskX,maskY;//ovi su vezani za prvu tableu
int now2X,now2Y,mask2X,mask2Y;//ovi su vezani za drugu kolonu
char board1[5][5][3],board2[5][5][2];
int bMask1[5][5],bMask2[5][5];

main()
{
	GenerateMatrix();
	ResetValues();
	hidecursor();
	NewGame();
}
void GenerateMatrix(){
	int i,j,b;
	b = 1;
	char k = 97;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(b < 10){
				board1[i][j][0] = '0';
				board1[i][j][1] = b+48;
			} else {
				int a,l;
				a = b / 10;
				l = b % 10;
				board1[i][j][0] = a+48;
				board1[i][j][1] = l+48;
			}
			board2[i][j][0] = k;
			++b; ++k;
			bMask1[i][j] = 0;
			bMask2[i][j] = 0;
		}
	}
	srand((unsigned)time(NULL)); 
	for (int i=0; i<5; i++){
		char temp[3];
		int r = rand() % 5;
		for (int j = 0; j < 5; j++){
			strcpy(temp,board1[i][j]);
			strcpy(board1[i][j],board1[r][j]);
			strcpy(board1[r][j],temp);
		}
	}
	for (int i=0; i<5; i++){
		char temp[3];
		int r = rand() % 5;
		for (int j = 0; j < 5; j++){
			strcpy(temp,board2[i][j]);
			strcpy(board2[i][j],board2[r][j]);
			strcpy(board2[r][j],temp);
		}
	}
	for (int i=0; i<5; i++){
		char temp[3];
		int r = rand() % 5;
		for (int j = 0; j < 5; j++){
			strcpy(temp,board1[i][j]);
			strcpy(board1[i][j],board1[i][r]);
			strcpy(board1[i][r],temp);
		}
	}
	for (int i=0; i<5; i++){
		char temp[3];
		int r = rand() % 5;
		for (int j = 0; j < 5; j++){
			strcpy(temp,board2[i][j]);
			strcpy(board2[i][j],board2[i][r]);
			strcpy(board2[i][r],temp);
		}
	}
	for (i=0;i<5;i++){
		char temp[5][3];
		int r = rand() % 5;
		for(j=0;j<5;j++){
			strcpy(temp[j],board2[i][j]);
		}
		for(j=0;j<5;j++){
			strcpy(board2[i][j],board2[r][j]); 
		}
		for(j=0;j<5;j++){			
			strcpy(board2[r][j],temp[j]);
		}
	}
}
void ResetValues(){
	numberOfTries = 0;
	numberOfTrues = 0;
	//now znaci mesto na tabeli, tj pikseli na tabeli
	nowX = 0;
	nowY = 0;
	now2X = 0;
	now2Y = 0;
	// mask sluzi za uzimanje vrednosti iz matrice i to su brojevi od 0-5
	maskX = 0;
	maskY = 0;
	mask2X = 0;
	mask2Y = 0;
	defX = 6;
	defY = 10;
}
void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}
void showcursor(){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = TRUE;
   SetConsoleCursorInfo(consoleHandle, &info);
}
//------------------------------------------------------------------
void ReadTops(){
	FILE *top;
	top = fopen("Pairs.txt","r");
	int y,i,k,ind = 0;
	if(top == NULL){
		for(i=0;i<10;i++){
			/*char *p;
			p = replicate(" ",14);
			strcpy(names[i],p);*/
			strcpy(names[i]," \n\0");
		}
		for(i=10;i<20;i++){

			strcpy(names[i],"25\n\0");
		}
		ind = 1;
	}

	y = 9;
	k = 0;
	for(i=0;i<10;i++){
		if(ind == 0)
			fgets(names[k],sizeof(names[k]),top);
		outtxt("        ",40,y,WHITE + CYAN * 16);
		outtxt(names[k],40,y,WHITE + CYAN * 16);
		y++;
		k++;
	}
	y = 9;
	for(i=10;i<20;i++){
		if(ind == 0)
			fgets(names[k],sizeof(names[k]),top);
		outtxt(names[k],72,y,BLUE + LIGHTGRAY * 16);
		y++;
		k++;
	}
	fclose(top);
	WriteScore();
}
//------------------------------------------------------------------
void NewGame(){

	char *Items[] = { "01","02","03","04","05","06","07","08","09","10","" };//redni brojevi
	clrscr();

	if (DrawMask("Magic Square Pairs",Items)) {
		numberOfTries = 0;
		DrawTry();
		ReadTops();
		MaskEdit2();
	}
}
void DrawTry(){
	Grid(2,1,2,1,5,6,WHITE);
	if(numberOfTries >= 10){
		char num[3];
		num[0] = (numberOfTries / 10) + 48;
		num[1] = (numberOfTries % 10) + 48;
		num[2] = '\0';
		outtxt(num,9,7,WHITE + RED * 16);
	} else {
		char num[3];
		num[0] = 48;
		num[1] = numberOfTries + 48;
		num[2] = '\0';
		outtxt(num,9,7,WHITE + RED * 16);
	}
}
void ShowUpLetter(int x, int y){
	x = x - defX;
	y = y - defY;
	x /=3;
	y = y / 2;
	char p[3];
	strcpy(p,board1[maskX][maskY]);
	int br;
	br = atoi(p);
	p[0] = br+ 'a'-1;
	//p[0] = board2[mask2X][mask2Y][0];
	p[1] = ' ';
	p[2] = '\0';
	outtxt(p,6,7,WHITE + MAGENTA * 16);
}
char *alltrim(char *p)
{
	char *q, *r;
	int n;
	while (isspace(*p)) p++;
	if (*p == 0) {
		q = new char[1];
		*q = 0;
		return q;
	}
	q = p + strlen(p);
	while (isspace(*q)) q--;
	n = q - p + 1;
	r = new char [n + 1];
	while (p <= q) *r++ = *p++;
	*r = 0;
	return (r - n);
}
//------------------------------------------------------------------
void Bs (char *s,int Data[])
{
	if (!Data[0]) return; 
	if (Data[0]-- > Data[2]) return;
	Data[4] = 1; Data[1] = Data[2];
	movmem(&s[Data[0] + 1],&s[Data[0]],Data[1] - Data[0]);
	s[Data[1]] = ' ';
	if (s[Data[2]] == ' ') Data[4] = 9;
	return;
}
//------------------------------------------------------------------
void Del (char *s,int Data[])
{
	if (Data[0] == Data[2] && s[Data[2]] != ' ') {
		s[Data[2]] = ' '; Data[4] = 9;
		return;
	}
	if (Data[2] <= Data[0]) return;
	Data[4] = 1; Data[1] = Data[2];
	movmem (&s[Data[0] + 1], &s[Data[0]], Data[1] - Data[0]);
	s[Data[1]] = ' ';
	if (Data[1] == Data[2]) Data[4] = 9;
	return;
}
//------------------------------------------------------------------
void Default (char *s,int Data[])
{
	if (Data[3] < ' ' || Data[3] > 'z') return;
	Data[3] = toupper (Data[3]);
	Data[1] = Data[0];
	if (Data[2] <= Data[0]) {
		s[Data[0]] = Data[3];
		Data[0]++;
		if (s[Data[1]] != ' ') {
			Data[2] = Data[0]; Data[4] = 1;
		}
		return;
	}
	Data[4] = 1; Data[1] = Data[2];
	movmem (&s[Data[0]], &s[Data[0] + 1],Data[1] - Data[0]);
	s[Data[0]] = Data[3];
	Data[0]++; Data[2]++; int len = strlen(s);
	if (Data[2] < len) return;
	Data[2]--;
	if (isspace(s[Data[2]])) Data[4] = 9;
	return;
}
//------------------------------------------------------------------
void ScrollMenu(int d[], char *c, char *o[], int *x, int *y, int n, int fromTable)
{
	int i, j, k, key;
	_setcursortype(_NOCURSOR);
	for (i = 0; i < n; i++) {
		textattr(d[2]); gotoxy(x[i],y[i]); cprintf("%s\n", o[i]);
		Frame(x[i] - 1, y[i] - 1,x[i] + d[5],y[i] + 1,d[0],1);
	}
	textattr (d[1]); gotoxy (x[0],y[0]); cputs (o[0]);
	i = 0; k = 0;
	int ind = 0;
	int kk = 0;
	while (1) {
		if (ind == 1) break;
		key = getkey();
		if (isalpha(key)) key = toupper(key);
		switch (key) {
			case ESC:
				_setcursortype(_NORMALCURSOR);
				gotoxy(1,1);
				return;
			case CR: //ako je pretisnu Enter na dugme
				//printf("%d\n", k);
				ind = 1;
				kk = k;
				break;
			case UP:
				if (d[6]) if (--k < 0) k = n - 1;
				break;
			case LEFT:
				if (!d[6]) if (--k < 0) k = n - 1;
				break;
			case DOWN:
				if (d[6]) if (++k == n) k = 0;
				break;
			case RIGHT:
				if (!d[6]) if (++k == n) k = 0;
				break;
			case HOME:
				k = 0;
				break;
			case END:
				k = n - 1;
				break;
			/*default:
				for (j = 0; j < n; j++) if (key == c[j]) k = j;*/
		}
		if (i == k) continue;
		textattr (d[2]); gotoxy (x[i],y[i]); cputs (o[i]);
		textattr (d[1]); gotoxy (x[k],y[k]); cputs (o[k]);
		i = k;
	}
	ChooseAction(kk,fromTable);
}
//------------------------------------------------------------------
void Frame(int x1, int y1, int x2, int y2, int color, int Flag)
{
	int CoorBox[] = {
	218,191,192,217,196,179,
	201,187,200,188,205,186
	};//ovo su vrednosti karaktera za isrctavanje
	int i, FlagDefault;
	int *p;
	FlagDefault = 0;
	textattr(color);
	if (Flag <= 1) Flag = 0;
	while(1) {
		if (x1 < 1 || y1 < 1 || x2 > 80 || y2 > 25) {
			FlagDefault = 1;
			break;
		}
		if (x2 <= x1 || y2 <= y1) {
			FlagDefault = 1;
			break;
		}
		break;
	}
	if (FlagDefault) {
		x1 = 1; y1 = 1; x2 = 80; y2 = 25;
	}
	if (Flag) Flag = 6;
	p = CoorBox + Flag;
	gotoxy(x1,y1); putch(*p++);
	gotoxy(x2,y1); putch(*p++);
	gotoxy(x1,y2); putch(*p++);
	gotoxy(x2,y2); putch(*p++);
	for (i = x1 + 1; i < x2; i++) {
		gotoxy (i,y1); putch(*p);
		gotoxy (i,y2); putch(*p);
	}
	p++;
	for (i = y1 + 1; i < y2; i++) {
		gotoxy (x1,i); putch(*p);
		gotoxy (x2,i); putch(*p);
	}
	gotoxy(x1,y1);
}
//------------------------------------------------------------------
int LineEdit (char *s, int x, int y, int color)
{
	void Del(char *s, int Data[]); 
	void Bs(char *s, int Data[]);
	void Default(char *s, int Data[]);
	int len, Data[5];
	len = strlen(s); if (!len) return 0;
	Data[2] = len - 1;
	if (isspace (s[Data[2]])) Data[4] = 9; else Data[4] = 1;
	Data[0] = 0;
	while(Data[0] < len) {
		if (Data[4]) {
		outtxt(s,x,y,color);
		if (Data[4] == 9) {
			while(isspace(s[Data[2]]) && Data[2] > 0) Data[2]--;
			if (!isspace (s[Data[2]])) Data[2]++;
		}
		Data[4] = 0;
		}
		gotoxy(x + Data[0],y);
		switch (Data[3] = getkey()) {
			case HOME:
				Data[0] = 0;
				break;
			case END:
				Data[0] = Data[2];
				break;
			case LEFT:
				if (--Data[0] < 0) Data[0] = 0;
				break;
			case RIGHT:
				if (++Data[0] == len) Data[0]--;
				break;
			case CR: case ESC: case UP: case DOWN: case PAGEDOWN:
			case CTRLHOME: case CTRLEND:
				return Data[3];
			case BS: Bs(s,Data); break;
			case DEL: Del(s,Data); break;
			default:
				Default(s,Data);
				break;
		}
	}
	outtxt(s,x,y,color);
	return CR;
}
//------------------------------------------------------------------
void outtxt (char *s, int x, int y, int color)
{
	textattr (color); gotoxy (x,y);
	if (s) cputs (s);
}
//------------------------------------------------------------------
char *replicate(char *s, unsigned int n)
{
	unsigned int i, j, Length;
	char *p;
	Length = strlen(s);
	p = new char [Length * n + 1];
	for (i = 0; i < n; i++) {
	for (j = 0; j < Length; j++) *p++ = s[j];
	}
	*p = 0;
	return (p - (Length * n));
}
//------------------------------------------------------------------
char *Line(char *s, int n, int FlagLine)
{
	int i, len, Start, End;
	char *p, *q, *r;
	q = alltrim(s); len = strlen(q);
	if (len > n) n = len;
	p = new char [n + 1];
	for (i = 0; i < n; i++) p[i] = ' ';
	p[n] = 0;
	switch(FlagLine) {
		case 0:
			Start = 0;
			break;
		case 1:
			Start = (n - len) / 2;
			break;
		default:
			Start = n - len;
			break;
	}
	End = Start + len;
	r = q;
	for (i = Start; i < End; i++) p[i] = *r++;
	delete [] q;
	return p;
}
//------------------------------------------------------------------
bool DrawMask(char *File, char *Items[])
{
	static char *o[] = {
	"New",
	"Ignore",
	"Exit"
	};
	int i, j, n, x1, x2, len, Len;
	char *p;
	p = replicate (" ",22);
	Frame(1,1,80,3,CYAN,1); 
	Frame(1,5,80,20,LIGHTGRAY,1);
	outtxt(p,2,2,RED * 16); 
	outtxt(p,58,2,RED * 16);
	delete [] p;
	p = Line(File,34,1); outtxt(p,24,2,BLUE + LIGHTGRAY * 16);
	delete [] p;
	for (i = 0; i < 3; i++) {
		x1 = 1 + i * 27; x2 = x1 + 25;
		Frame(x1,22, x2,24,CYAN,1);//ovo sa 1 rekulisemo koliko linija
		p = Line(o[i],24,1);
		outtxt(p,x1 + 1,23,BROWN);
		delete [] p;
	}
	len = strlen(Items[0]); Len = len;
	n = 0;
	while (len) {
		if (len != Len) return false;
		len = strlen(Items[++n]);
	}
	if (n > 12) return false;//kraj igre
	DrawScoreBoard();//crta tablu za autora i poteze
	DrawNumbersBoard();
	DrawLettersBoard();
	for (i = 0; i < n; i++) {
		outtxt(Items[i],54, 9 + i,WHITE + CYAN * 16);//ovde se pomeraju redni brojevi
		outtxt(Items[i],58, 9 + i,BLUE + LIGHTGRAY * 16);//lista rednih brojeva
	}
	return true;
}
void DrawDownMenu(){
	static char *o[] = {
	"New",
	"Ignore",
	"Exit"
	};
	int i, j, n, x1, x2, len, Len;
	char *p;
	for (i = 0; i < 3; i++) {
		x1 = 1 + i * 27; x2 = x1 + 25;
		Frame(x1,22, x2,24,CYAN,1);//ovo sa 1 rekulisemo koliko linija
		p = Line(o[i],24,1);
		outtxt(p,x1 + 1,23,BROWN);
		delete [] p;
	}
}
//------------------------------------------------------------------
void DrawScoreBoard(){
	char *bcg;
	outtxt(" Authors:         ",39,7,BLUE + LIGHTGRAY * 16);
	outtxt(" Izborni seminar  ",57,7,BLACK + CYAN * 16);
	bcg = replicate(" ",18);
	int i;
	for(i=0;i<12;i++){
		outtxt(bcg,57,8+i,LIGHTGRAY * 16);
	}
	for(i=0;i<12;i++){
		outtxt(bcg,39,8+i,CYAN * 16);
	}
}
//------------------------------------------------------------------
void DrawNumbersBoard(){
	int i,j,k;
	char p[2];
	strcpy(p," ");
	k = 5;
	for(i=0;i<15;i++){
		for(j=0;j<11;j++){
			outtxt(p,k,j+9,BROWN + BLACK * 16);
		}
		++k;
	}
	Grid(5,5,2,1,5,9,CYAN);
}
//------------------------------------------------------------------
void DrawLettersBoard(){
	int i,j,k;
	char p[2];
	strcpy(p," ");
	k = 24;
	for(i=0;i<15;i++){
		for(j=0;j<11;j++){
			outtxt(p,k,j+9,BROWN + BLACK * 16);
		}
		++k;
	}
	Grid(5,5,1,1,24,9,CYAN);	
}
//------------------------------------------------------------------
void EnterName(int xx, int yy){
	showcursor();
	int i, n, key, x[12], y[12];
	char *name;
	name = replicate(" ",14);
	while(1){
		key = LineEdit(name,xx,yy,WHITE + 16 * CYAN);
		switch (key) {
			case ESC: return;
			case CR: 
				gotoxy(xx,yy); 
				outtxt(name,xx,yy,WHITE + CYAN * 16);
				strcpy (player, name);
				hidecursor();
				return;
		}
	}
}
//------------------------------------------------------------------
void ChooseAction(int action, int fromTable){
	//printf("AKCIJA: %d\n", action);
	if(action==0){
		DrawNumbersBoard();
		DrawLettersBoard();
		GenerateMatrix();
		ResetValues();
		DrawTry();
		ReadTops();
		DrawDownMenu();
		MaskEdit2();
	} else if(action==1){
		if(fromTable==1){
			gotoxy(nowX,nowY);
		} else if (fromTable==2){
			//GoToFirstFree2();
			gotoxy(now2X,now2Y);
		}
		DrawDownMenu();

		//MaskEdit2();
	} else if(action==2){
		exit(1);
	}

}
//------------------------------------------------------------------
void CheckScoreBoard(){
	//proveri tabelu
	int i,j;
	char num[3];
	if(numberOfTries >= 10){
		num[0] = (numberOfTries / 10) + 48;
		num[1] = (numberOfTries % 10) + 48;
		num[2] = '\0';
		//outtxt(num,9,7,WHITE + RED * 16);
	} else {

		num[0] = 48;
		num[1] = numberOfTries + 48;
		num[2] = '\0';
		//outtxt(num,9,7,WHITE + RED * 16);
	}
	for(i=10;i<20;i++){
		if(strcmp(num,names[i])<0){
			for(j=8;j>=(i-10);j--){
				strcpy(names[j+1]," ");
				strcpy(names[j+1],names[j]);
			}
			for(j=18;j>=i;j--){
				strcpy(names[j+1]," ");
				strcpy(names[j+1],names[j]);
			}
			EnterName(40,j);
			if(player[14]=='\0' && player[2]==' ') { //ako je prazan strin od 14 karatkera,tako mora, onda stavi unk
				strcpy(names[i-10],"Unknown"); 
				names[i-10][7]='\n';
			}
			//if(len <= 2) strcpy(names[i-10],"Unknown");
			else {
				int len;
				len = strlen(player);
				player[len-1] = '\0';
				//player[len] = '\0';
				strcpy(names[i-10]," ");
				strcpy(names[i-10],player);
			}
			strcpy(names[i]," ");
			strcpy(names[i],num);
			//names[i-10][strlen(names[i-10])]='\n';
			names[i][strlen(names[i])]='\n';

			//strcpy(names[i-10],player);
			break;
		}
	}
	int y;
	y = 9;
	for(i=0;i<10;i++){
		outtxt("            ",40,y,WHITE + CYAN * 16);
		outtxt(names[i],40,y,WHITE + CYAN * 16);
		y++;
	}
	y = 9;
	for(i=10;i<20;i++){
		outtxt(names[i],72,y,BLUE + LIGHTGRAY * 16);
		y++;
	}
	
	WriteScore();

}
//------------------------------------------------------------------
bool CheckFilds(char *p,int x, int y){
	int a,b;
	sscanf(board1[maskX][maskY],"%d",&a);
	b = board2[x][y][0];//ovde je jedan karatker
	b -= 96;
	//printf("\n%d\n", a);
	//printf("%d\n", b);
	if(a==b){
		return true;
	}
	return false;
}
void WriteScore(){
	FILE *top;
	int i;
	top = fopen("Pairs.txt","w");
	for(i=0;i<20;i++){
		fprintf(top, "%s", names[i]);
	}
	fclose(top);
}
void ShowLetter(int coX, int coY, int x, int y){
	outtxt(board2[x][y],coX,coY,WHITE + BLUE * 16);
}
void ResetMissed(){
	char p[2];
	strcpy(p,"  ");
	gotoxy(6,7);
	outtxt(p,6,7,WHITE + BLACK * 16);
	gotoxy(nowX,nowY);
	outtxt(p,nowX,nowY,WHITE + BLACK * 16);
	strcpy(p," ");
	gotoxy(now2X,now2Y);
	outtxt(p,now2X,now2Y,WHITE + BLACK * 16);
	//bMask1[maskX][maskY] = 0;
	//bMask2[mask2X][mask2Y] = 0;
}

void GoToFirstFree(){
	if(numberOfTrues == 25){
		CheckScoreBoard();
	}
	int i,j;
	int x,y;
	int ind = 0;
	nowX = defX;
	nowY = defY;
	maskX = 0;
	maskY = 0;
	x=0;y=0;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(bMask1[i][j]==0){
				nowX += x;
				nowY += y;
				maskY += i;
				maskX += j;
				char f[2];
				strcpy(f,"  ");
				gotoxy(nowX,nowY);
				outtxt(f,nowX,nowY,WHITE + MAGENTA * 16);
				ind = 1;
				break;
			} else {
				x = x+3;
			}
		}
		y = y+2;
		x = 0;
		if(ind == 1) break;
	}
	if(numberOfTries==25){
		MenuInTheEnd (CYAN, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,"New", "Ignore", "Exit", "");
	}
}
void MoveToNextBoard(){
	int key,i,j;
	int ind = 0;
	//ovde treba da nadje prvo slobodno
	char f[3];
	strcpy(f,"  ");
	/*now2X = defX + 19;//pomerimo ga po x
	now2Y = defY;
	//ove bi trebalo da se uzmu iz prve slobodne
	mask2X = 0;
	mask2Y = 0;
	gotoxy(now2X,now2Y);
	//cprintf("%c",f);
	outtxt(f,now2X,now2Y,WHITE + MAGENTA * 16);*/
	int n;
	n = 5;
	i = 0;
	GoToFirstFree2();
	while(1){
		i = 0;
		key = getkey();
		switch (key){
			case PAGEDOWN:
				i = n;
				break;
			case ESC: gotoxy(1,24); return;
			case CR:
				//treba da prikaze broj, gore slovo i da prebaci na sledecu tabelu
				ShowLetter(now2X,now2Y,mask2X,mask2Y);
				if(CheckFilds(board2[now2X][now2Y],mask2X,mask2Y)){
					// tacan je rezultat i treba da postaviti
					outtxt(f,6,7,WHITE + BLACK * 16);//oboji pokazivac i kad se pogodi
					//ovde podesimo maske za pogodjene
					bMask1[maskY][maskX] = 1;
					bMask2[mask2Y][mask2X] = 1;
					++numberOfTrues;
					GoToFirstFree();
				} else {
					int key2;
					key2 = getkey();
					if(key2 == ESC) exit(1);
					else{
						ResetMissed();
						++numberOfTries;
						DrawTry();
						GoToFirstFree();
					}
				}
				ind = 1;
				break;
			case DOWN:
				if((now2Y+2) <= (defY+8)){
					isPossible2(3);
				}
				break;
			case UP:
				if((now2Y-2) >= defY){
					isPossible2(1);
				}
				break;
			case LEFT:
				if((now2X-2) >= defX+19){
					isPossible2(0);
				}
				break;
			case RIGHT:
				if((now2X+2) <= (defX+28)){
					isPossible2(2);
				}
		}
		if(ind == 1) break;
		if( i == n) Menu2 (CYAN, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,"New", "Ignore", "Exit", "");
		i = 0;
	}
}
void ShowNumber(int x, int y){
	//bMask1[x][y] = 1;
	outtxt(board1[x][y],nowX,nowY,WHITE + BLUE * 16);
}
bool isPossible(int direction){
	int ind = 0;
	char p[2];
	strcpy(p,"  ");
	int i,notFound;
	notFound = 0;
	int pomX,pomY,pomMaskX,pomMaskY;
	pomX = nowX;
	pomY = nowY;
	pomMaskX = maskX;
	pomMaskY = maskY;
	//smer, 0-levo 1-gore 2-desno 3-dole
	if(direction == 0){
		if(bMask1[maskY][maskX-1]==0){
			pomX = pomX - 3;
			pomMaskX = pomMaskX - 1;
		} else if(bMask1[maskY][maskX-1]==1 && bMask1[maskY][maskX-2]==0){
			pomX = pomX - 6;
			pomMaskX = pomMaskX - 2;
		} else if(bMask1[maskY][maskX-1]==1 && bMask1[maskY][maskX-2]==1 && bMask1[maskY][maskX-3]==0){
			pomX = pomX - 9;
			pomMaskX = pomMaskX - 3;
		} else if(bMask1[maskY][maskX-1]==1 && bMask1[maskY][maskX-2]==1 && bMask1[maskY][maskX-3]==1 && bMask1[maskY][maskX-4]==0){
			pomX = pomX - 12;
			pomMaskX = pomMaskX - 4;
		}
		if(pomX < (defX)) ind = 1;
	} else if(direction == 1){
		i = 1;
		notFound = 0;
		while(1){
			ind = 0;
			if(maskY-i<0){ind = 1;}
			if(bMask1[maskY-i][maskX]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				notFound = 1;
			} else if((pomX+3)<=(defX+12) && bMask1[maskY-i][maskX+1]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 3;
				pomMaskX = pomMaskX + 1;
				notFound = 1;
			} else if ((pomX-3)>=defX && bMask1[maskY-i][maskX-1]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 3;
				pomMaskX = pomMaskX - 1;
				notFound = 1;
			} else if ((pomX+6)<=(defX+12) && bMask1[maskY-i][maskX+2]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 6;
				pomMaskX = pomMaskX + 2;
				notFound = 1;
			} else if ((pomX-6)>=defX && bMask1[maskY-i][maskX-2]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 6;
				pomMaskX = pomMaskX - 2;
				notFound = 1;
			} else if ((pomX+9)<=(defX+12) && bMask1[maskY-i][maskX+3]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 9;
				pomMaskX = pomMaskX + 3;
				notFound = 1;
			} else if ((pomX-9)>=(defX) && bMask1[maskY-i][maskX-3]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 9;
				pomMaskX = pomMaskX - 3;
				notFound = 1;
			} else if ((pomX+12)<=(defX+12) && bMask1[maskY-i][maskX+4]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 12;
				pomMaskX = pomMaskX + 4;
				notFound = 1;
			} else if ((pomX-12)>=(defX) && bMask1[maskY-i][maskX-4]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 12;
				pomMaskX = pomMaskX - 4;
				notFound = 1;
			} else {
				ind = 1;
			}
			//if(notFound == 1) { notFound = 0; continue;}
			if(pomY > (defY+8)){ ind = 1; }
			if(notFound == 1){ break; }
			else { ++i; }
		}

	} else if(direction == 2){
		if(bMask1[maskY][maskX+1]==0){
			pomX = pomX + 3;
			pomMaskX = pomMaskX + 1;
		} else if(bMask1[maskY][maskX+1]==1 && bMask1[maskY][maskX+2]==0){
			pomX = pomX + 6;
			pomMaskX = pomMaskX + 2;
		} else if(bMask1[maskY][maskX+1]==1 && bMask1[maskY][maskX+2]==1 && bMask1[maskY][maskX+3]==0){
			pomX = pomX + 9;
			pomMaskX = pomMaskX + 3;
		} else if(bMask1[maskY][maskX+1]==1 && bMask1[maskY][maskX+2]==1 && bMask1[maskY][maskX+3]==1 && bMask1[maskY][maskX+4]==0){
			pomX = pomX + 12;
			pomMaskX = pomMaskX + 4;
		}
		if(pomX > (defX+12)) ind = 1;
	} else if(direction == 3){
		i = 1;
		notFound = 0;
		while(1){
			ind = 0;
			if(maskY+i==5){ind = 1;}
			if(bMask1[maskY+i][maskX]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				notFound = 1;
			} else if((pomX+3)<=(defX+12) && bMask1[maskY+i][maskX+1]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 3;
				pomMaskX = pomMaskX + 1;
				notFound = 1;
			} else if ((pomX-3)>=defX && bMask1[maskY+i][maskX-1]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 3;
				pomMaskX = pomMaskX - 1;
				notFound = 1;
			} else if ((pomX+6)<=(defX+12) && bMask1[maskY+i][maskX+2]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 6;
				pomMaskX = pomMaskX + 2;
				notFound = 1;
			} else if ((pomX-6)>=defX && bMask1[maskY+i][maskX-2]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 6;
				pomMaskX = pomMaskX - 2;
				notFound = 1;
			} else if ((pomX+9)<=(defX+12) && bMask1[maskY+i][maskX+3]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 9;
				pomMaskX = pomMaskX + 3;
				notFound = 1;
			} else if ((pomX-9)>=(defX) && bMask1[maskY+i][maskX-3]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 9;
				pomMaskX = pomMaskX - 3;
				notFound = 1;
			} else if ((pomX+12)<=(defX+12) && bMask1[maskY+i][maskX+4]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 12;
				pomMaskX = pomMaskX + 4;
				notFound = 1;
			} else if ((pomX-12)>=(defX) && bMask1[maskY+i][maskX-4]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 12;
				pomMaskX = pomMaskX - 4;
				notFound = 1;
			} else {
				ind = 1;
			}
			//if(notFound == 1) { notFound = 0; continue;}
			if(pomY > (defY+8)){ ind = 1; }
			if(notFound == 1){ break; }
			else { ++i; }
		}
	}
	if(ind == 0){
		outtxt(p,nowX,nowY,WHITE + BLACK * 16);
		nowX = pomX;
		nowY = pomY;
		maskX = pomMaskX;
		maskY = pomMaskY;
		outtxt(p,nowX,nowY,WHITE + MAGENTA * 16);
	}
	return false;
}
//------------------------------------------------------------------
void MaskEdit2(){
	char f1[2];
	int i, n, k, key;
	strcpy(f1,"  ");
	nowX = 6;
	nowY = 10;
	i = 0;
	n = 5;
	//outtxt(f1,nowX,nowY,WHITE + MAGENTA * 16);
	GoToFirstFree();
	while(1) {
		i = 0;
		key = getkey();
		switch (key){
			case PAGEDOWN:
				i = n;
				break;
			case ESC: gotoxy(1,24); return;
			case CR:
				//treba da prikaze broj, gore slovo i da prebaci na sledecu tabelu
				//++numberOfTries;//uvek se uveca broj pokusaja kad se ovde klikne enter
				ShowUpLetter(nowX,nowY);
				ShowNumber(maskX,maskY);
				MoveToNextBoard();
				break;
			case DOWN:
				//treba poslati smer, 0-levo 1-gore 2-desno 3-dole
				if((nowY+2) <= (defY+8)){
					isPossible(3);
				}
				break;
			case UP:
				if((nowY-2) >= defY){
					isPossible(1);
				}
				break;
			case LEFT:
				if((nowX-3) >= defX){
					isPossible(0);
				}
				break;
			case RIGHT:
				if((nowX+3) <= (defX+12)){	
					isPossible(2);	
				}
		}
		if(numberOfTrues==25) break;
		if( i == n) Menu (CYAN, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,"New", "Ignore", "Exit", "");
		i = 0;
	}
	//samo crtamo jedan sivi
	MenuInTheEnd (CYAN, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,"New", "Ignore", "Exit", "");
	//Menu2 (CYAN, WHITE + 16 * RED, BROWN, 1, 22, 24, 0,"New", "Ignore", "Exit", "");
}
//------------------------------------------------------------------
void MenuInTheEnd(int color, ...)
{
	int i, n, New5X, New5Y, len, Data[7], CoorX[64], CoorY[64];
	char *p, *Items[64], FirstLetter[64];
	va_list ap;
	va_start (ap,color);
	Data[0] = color;
	for (i = 1; i < 7; i++) Data[i] = va_arg(ap, int);
	if (Data[4] + 2 > 25 || Data[3] + Data[5] >= 80) {
		va_end (ap);
		return;
	}
	Items[0] = va_arg(ap, char*); len = strlen(Items[0]);
	n = 1;
	while(len) {
		Items[n] = va_arg(ap, char*);
		len = strlen(Items[n++]);
	}
	va_end (ap); if (!--n) return;
	for (i = 0; i < n; i++) {
		p = alltrim(Items[i]); len = strlen(p);
		FirstLetter[i] = p[0];
		if (len > Data[5] - 2) Data[5] = len + 2;
		delete [] p;
	}
	New5X = Data[3] + 1; New5Y = Data[4] + 1;
	for (i = 0; i < n; i++) {
		if(i==1) strcpy(Items[i],"");
		else 
			Items[i] = Line(Items[i],Data[5],1);
		CoorX[i] = New5X; CoorY[i] = New5Y;
		if (Data[6]) {
			New5Y = New5Y + 3;
			if (New5Y > 24) {
				New5X = New5X + Data[5] + 3; New5Y = Data[4] + 1;
				if (New5X + Data[5] > 80) return;
			}
			continue;
		}
		New5X = New5X + Data[5] + 3;
		if (New5X + Data[5] <= 80 || i == n - 1) continue;
		New5X = Data[3] + 1; New5Y = New5Y + 3;
		if (New5Y > 24) return;
	}
	ScrollMenu2(Data,FirstLetter,Items,CoorX,CoorY,n);
	for (i = 0; i < n; i++) delete [] Items[i];
}
//------------------------------------------------------------------
int getkey() {
	int c, key;
	key = (c = getch()) > 0 ? c : 256 + getch();
	return key;
}
void Grid(int m, int n, int w, int h, int x, int y, int color)
{
	int i, j, k, newx, newy, Char, FlagDefault;
	FlagDefault = 0;
	textattr(color);
	if (m < 1 || n < 1 || w < 1 || h < 1) FlagDefault = 1;
	if (x < 1 || y < 1) FlagDefault = 1;
	if (x + m * (w + 1) > 80) FlagDefault = 1;
	if (y + n * (h + 1) > 25) FlagDefault = 1;
	if (FlagDefault) {m = 26; n = 12; w = 2; h = 1; x = 1; y = 1;}
	for (i = 0; i <= n; i++) {
		newy = y + i * (h + 1);
		for (j = 0; j <= m; j++) {
			newx = x + j * (w + 1);
			while(1) {
				if (i == 0 && j == 0) {Char = 218; break;}
				if (i == 0 && j == m) {Char = 191; break;}
				if (i == n && j == 0) {Char = 192; break;}
				if (i == n && j == m) {Char = 217; break;}
				if (i != 0 && j == 0) {Char = 195; break;}
				if (i != 0 && j == m) {Char = 180; break;}
				if (i == n) {Char = 193; break;}
				if (i == 0) {Char = 194; break;}
				Char = 197;
				break;
			}
			gotoxy(newx,newy); putch(Char);
			if (j == m) break;
			for (k = 1; k <= w; k++) {
				gotoxy(newx + k, newy); putch(196);
			}
		}
		if (i == n) break;
		for (j = 0; j < h; j++) {
			newx = x; newy = newy + 1;
			for (k = 0; k <= m; k++) {
				gotoxy(newx + k * (w + 1),newy); putch(179);
			}
		}
	}
	gotoxy(x,y);
}
//------------------------------------------------------------------
void GoToFirstFree2(){
	int i,j;
	int x,y;
	int ind = 0;
	now2X = defX+19;
	now2Y = defY;
	mask2X = 0;
	mask2Y = 0;
	x=0;y=0;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			if(bMask2[i][j]==0){
				now2X += x;
				now2Y += y;
				mask2Y += i;
				mask2X += j;
				char f[2];
				strcpy(f," ");
				gotoxy(now2X,now2Y);
				outtxt(f,now2X,now2Y,WHITE + MAGENTA * 16);
				ind = 1;
				break;
			} else {
				x = x+2;
			}
		}
		y = y+2;
		x = 0;
		if(ind == 1) break;
	}
}
bool isPossible2(int direction){
	int ind = 0;
	char p[2];
	strcpy(p," ");
	int i,notFound;
	notFound = 0;
	int pomX,pomY,pomMaskX,pomMaskY;
	pomX = now2X;
	pomY = now2Y;
	pomMaskX = mask2X;
	pomMaskY = mask2Y;
	//smer, 0-levo 1-gore 2-desno 3-dole
	if(direction == 0){
		if(bMask2[mask2Y][mask2X-1]==0){
			pomX = pomX - 2;
			pomMaskX = pomMaskX - 1;
		} else if(bMask2[mask2Y][mask2X-1]==1 && bMask2[mask2Y][mask2X-2]==0){
			pomX = pomX - 4;
			pomMaskX = pomMaskX - 2;
		} else if(bMask2[mask2Y][mask2X-1]==1 && bMask2[mask2Y][mask2X-2]==1 && bMask2[mask2Y][mask2X-3]==0){
			pomX = pomX - 6;
			pomMaskX = pomMaskX - 3;
		} else if(bMask2[mask2Y][mask2X-1]==1 && bMask2[mask2Y][mask2X-2]==1 && bMask2[mask2Y][mask2X-3]==1 && bMask2[mask2Y][mask2X-4]==0){
			pomX = pomX - 8;
			pomMaskX = pomMaskX - 4;
		}
		if(pomX < (defX+19)) ind = 1;
	} else if(direction == 1){
		i = 1;
		notFound = 0;
		while(1){
			ind = 0;
			if(mask2Y-i<0){ind = 1;}
			if(bMask2[mask2Y-i][mask2X]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				notFound = 1;
			} else if((pomX+2)<=(defX+28) && bMask2[mask2Y-i][mask2X+1]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 2;
				pomMaskX = pomMaskX + 1;
				notFound = 1;
			} else if ((pomX-2)>=(defX+19) && bMask2[mask2Y-i][mask2X-1]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 2;
				pomMaskX = pomMaskX - 1;
				notFound = 1;
			} else if ((pomX+4)<=(defX+28) && bMask2[mask2Y-i][mask2X+2]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 4;
				pomMaskX = pomMaskX + 2;
				notFound = 1;
			} else if ((pomX-4)>=(defX+19) && bMask2[mask2Y-i][mask2X-2]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 4;
				pomMaskX = pomMaskX - 2;
				notFound = 1;
			} else if ((pomX+6)<=(defX+28) && bMask2[mask2Y-i][mask2X+3]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 6;
				pomMaskX = pomMaskX + 3;
				notFound = 1;
			} else if ((pomX-6)>=(defX+19) && bMask2[mask2Y-i][mask2X-3]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 6;
				pomMaskX = pomMaskX - 3;
				notFound = 1;
			} else if ((pomX+8)<=(defX+28) && bMask2[mask2Y-i][mask2X+4]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX + 8;
				pomMaskX = pomMaskX + 4;
				notFound = 1;
			} else if ((pomX-8)>=(defX+19) && bMask2[mask2Y-i][mask2X-4]==0){
				pomY = pomY - 2*i;
				pomMaskY = pomMaskY - i;
				pomX = pomX - 8;
				pomMaskX = pomMaskX - 4;
				notFound = 1;
			} else {
				ind = 1;
			}
			//if(notFound == 1) { notFound = 0; continue;}
			if(pomY > (defY+8)){ ind = 1; }
			if(notFound == 1){ break; }
			else { ++i; }
		}

	} else if(direction == 2){
		if(bMask2[mask2Y][mask2X+1]==0){
			pomX = pomX + 2;
			pomMaskX = pomMaskX + 1;
		} else if(bMask2[mask2Y][mask2X+1]==1 && bMask2[mask2Y][mask2X+2]==0){
			pomX = pomX + 4;
			pomMaskX = pomMaskX + 2;
		} else if(bMask2[mask2Y][mask2X+1]==1 && bMask2[mask2Y][mask2X+2]==1 && bMask2[mask2Y][mask2X+3]==0){
			pomX = pomX + 6;
			pomMaskX = pomMaskX + 3;
		} else if(bMask2[mask2Y][mask2X+1]==1 && bMask2[mask2Y][mask2X+2]==1 && bMask2[mask2Y][mask2X+3]==1 && bMask2[mask2Y][mask2X+4]==0){
			pomX = pomX + 8;
			pomMaskX = pomMaskX + 4;
		}
		if(pomX > (defX+28)) ind = 1;
	} else if(direction == 3){
		i = 1;
		notFound = 0;
		while(1){
			ind = 0;
			if(mask2Y+i==5){ind = 1;}
			if(bMask2[mask2Y+i][mask2X]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				notFound = 1;
			} else if((pomX+2)<=(defX+28) && bMask2[mask2Y+i][mask2X+1]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 2;
				pomMaskX = pomMaskX + 1;
				notFound = 1;
			} else if ((pomX-2)>=(defX+19) && bMask2[mask2Y+i][mask2X-1]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 2;
				pomMaskX = pomMaskX - 1;
				notFound = 1;
			} else if ((pomX+4)<=(defX+28) && bMask2[mask2Y+i][mask2X+2]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 4;
				pomMaskX = pomMaskX + 2;
				notFound = 1;
			} else if ((pomX-4)>=(defX+19) && bMask2[mask2Y+i][mask2X-2]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 4;
				pomMaskX = pomMaskX - 2;
				notFound = 1;
			} else if ((pomX+6)<=(defX+28) && bMask2[mask2Y+i][mask2X+3]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 6;
				pomMaskX = pomMaskX + 3;
				notFound = 1;
			} else if ((pomX-6)>=(defX+19) && bMask2[mask2Y+i][mask2X-3]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 6;
				pomMaskX = pomMaskX - 3;
				notFound = 1;
			} else if ((pomX+8)<=(defX+28) && bMask2[mask2Y+i][mask2X+4]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX + 8;
				pomMaskX = pomMaskX + 4;
				notFound = 1;
			} else if ((pomX-8)>=(defX+19) && bMask2[mask2Y+i][mask2X-4]==0){
				pomY = pomY + 2*i;
				pomMaskY = pomMaskY + i;
				pomX = pomX - 8;
				pomMaskX = pomMaskX - 4;
				notFound = 1;
			} else {
				ind = 1;
			}
			//if(notFound == 1) { notFound = 0; continue;}
			if(pomY > (defY+8)){ ind = 1; }
			if(notFound == 1){ break; }
			else { ++i; }
		}
	}
	if(ind == 0){
		outtxt(p,now2X,now2Y,WHITE + BLACK * 16);
		now2X = pomX;
		now2Y = pomY;
		mask2X = pomMaskX;
		mask2Y = pomMaskY;
		outtxt(p,now2X,now2Y,WHITE + MAGENTA * 16);
	}
	return false;
}
void Menu(int color, ...)
{
	int i, n, New5X, New5Y, len, Data[7], CoorX[64], CoorY[64];
	char *p, *Items[64], FirstLetter[64];
	va_list ap;
	va_start (ap,color);
	Data[0] = color;
	for (i = 1; i < 7; i++) Data[i] = va_arg(ap, int);
	if (Data[4] + 2 > 25 || Data[3] + Data[5] >= 80) {
		va_end (ap);
		return;
	}
	Items[0] = va_arg(ap, char*); len = strlen(Items[0]);
	n = 1;
	while(len) {
		Items[n] = va_arg(ap, char*);
		len = strlen(Items[n++]);
	}
	va_end (ap); if (!--n) return;
	for (i = 0; i < n; i++) {
		p = alltrim(Items[i]); len = strlen(p);
		FirstLetter[i] = p[0];
		if (len > Data[5] - 2) Data[5] = len + 2;
		delete [] p;
	}
	New5X = Data[3] + 1; New5Y = Data[4] + 1;
	for (i = 0; i < n; i++) {
		Items[i] = Line(Items[i],Data[5],1);
		CoorX[i] = New5X; CoorY[i] = New5Y;
		if (Data[6]) {
			New5Y = New5Y + 3;
			if (New5Y > 24) {
				New5X = New5X + Data[5] + 3; New5Y = Data[4] + 1;
				if (New5X + Data[5] > 80) return;
			}
			continue;
		}
		New5X = New5X + Data[5] + 3;
		if (New5X + Data[5] <= 80 || i == n - 1) continue;
		New5X = Data[3] + 1; New5Y = New5Y + 3;
		if (New5Y > 24) return;
	}
	ScrollMenu(Data,FirstLetter,Items,CoorX,CoorY,n,1);
	for (i = 0; i < n; i++) delete [] Items[i];
}
void Menu2(int color, ...)
{
	int i, n, New5X, New5Y, len, Data[7], CoorX[64], CoorY[64];
	char *p, *Items[64], FirstLetter[64];
	va_list ap;
	va_start (ap,color);
	Data[0] = color;
	for (i = 1; i < 7; i++) Data[i] = va_arg(ap, int);
	if (Data[4] + 2 > 25 || Data[3] + Data[5] >= 80) {
		va_end (ap);
		return;
	}
	Items[0] = va_arg(ap, char*); len = strlen(Items[0]);
	n = 1;
	while(len) {
		Items[n] = va_arg(ap, char*);
		len = strlen(Items[n++]);
	}
	va_end (ap); if (!--n) return;
	for (i = 0; i < n; i++) {
		p = alltrim(Items[i]); len = strlen(p);
		FirstLetter[i] = p[0];
		if (len > Data[5] - 2) Data[5] = len + 2;
		delete [] p;
	}
	New5X = Data[3] + 1; New5Y = Data[4] + 1;
	for (i = 0; i < n; i++) {
		Items[i] = Line(Items[i],Data[5],1);
		CoorX[i] = New5X; CoorY[i] = New5Y;
		if (Data[6]) {
			New5Y = New5Y + 3;
			if (New5Y > 24) {
				New5X = New5X + Data[5] + 3; New5Y = Data[4] + 1;
				if (New5X + Data[5] > 80) return;
			}
			continue;
		}
		New5X = New5X + Data[5] + 3;
		if (New5X + Data[5] <= 80 || i == n - 1) continue;
		New5X = Data[3] + 1; New5Y = New5Y + 3;
		if (New5Y > 24) return;
	}
	ScrollMenu(Data,FirstLetter,Items,CoorX,CoorY,n,2);
	for (i = 0; i < n; i++) delete [] Items[i];
}
void PaintInG(){
	Grid(1,1,24,1,28,22,LIGHTGRAY);//5kolona-1vrsta-2siri-1duzina-5x-5y
	/*char *p;
	strcpy(p,"        Ignore        ");
	outtxt(p,29,23,WHITE + BLACK * 16);*/
}
void ScrollMenu2(int d[], char *c, char *o[], int *x, int *y, int n)
{
	int i, j, k, key;
	_setcursortype(_NOCURSOR);
	for (i = 0; i < n; i++) {
		textattr(d[2]); gotoxy(x[i],y[i]); cprintf("%s\n", o[i]);
		Frame(x[i] - 1, y[i] - 1,x[i] + d[5],y[i] + 1,d[0],1);
	}
	textattr (d[1]); gotoxy (x[0],y[0]); cputs (o[0]);
	i = 0; k = 0;
	int ind = 0;
	int kk = 0;
	PaintInG();
	while (1) {
		if (ind == 1) break;
		//PaintInG();
		key = getkey();
		if (isalpha(key)) key = toupper(key);
		switch (key) {
			case ESC:
				_setcursortype(_NORMALCURSOR);
				gotoxy(1,1);
				return;
			case CR: //ako je pretisnu Enter na dugme
				//printf("%d\n", k);
				if(k!=1){
					ind = 1;
					kk = k;
					break;
				}
			case UP:
				if (d[6]) if (--k < 0) k = n - 1;
				break;
			case LEFT:
				k = 0;
				break;
			case DOWN:
				if (d[6]) if (++k == n) k = 0;
				break;
			case RIGHT:
				k = n - 1;
				break;
			case HOME:
				k = 0;
				break;
			case END:
				k = n - 1;
				break;
			/*default:
				for (j = 0; j < n; j++) if (key == c[j]) k = j;*/
		}
		if (i == k) continue;
		textattr (d[2]); gotoxy (x[i],y[i]); cputs (o[i]);
		textattr (d[1]); gotoxy (x[k],y[k]); cputs (o[k]);
		i = k;
	}
	ChooseAction(kk,10);
}