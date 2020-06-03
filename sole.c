#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct calendar 
{
  char yearday[60];
  char content[60];
}stschedule[100],stmemo[100],stobjective[100];


void cursor (char show);				//커서숨기는 함수
int gotoxy (int x, int y);				//좌표이동 함수
int searchcalendar (int year, int day);	//달력출력 함수

int schedule (int year, int day);		//일정추가 함수
int printschedule ();					//일정출력 함수(모든)
int printschedule2 (int year, int day);	//일정출력 함수(원하는 달)

int inputobjective (int year, int day);	//이달의 목표추가 함수
int printobjective (int year, int day);	//이달의 목표출력 함수

int inputmemo (int year, int day);		//메모추가 함수
int printmemo ();						//메모출력 함수(모든)
int printmemo2 (int year, int day);		//메모출력 함수(원하는 달)

int end ();								//종료하는 함수

int main (void) // 인터페이스를 구현하는 함수
{
	HANDLE hC = GetStdHandle (STD_OUTPUT_HANDLE);		//색깔함수
	
	system ("mode con: cols=90 lines=35");				//cmd창 조절
	system ("cls");
	cursor (0);											//cmd창에 깜빡거리는 커서 off
	
	struct tm *t;	//시간 구조체
	time_t timer; // 시간측정
	
	timer = time (NULL); // 현재 시각을 초 단위로 얻기
	t = localtime (&timer); // 초 단위의 시간을 분리하여 구조체에 넣기
	
	int year, day;
	int i, j, x=0, y=0;
	int dir=0;
	int cont[5][2]=			//목록 배열
	{
		{1,2},
		{0,3},
		{0,4},
		{0,5},
		{0,6}
	};
	
	while (1)
	{
		system ("cls");
		
		gotoxy (35,2);
		SetConsoleTextAttribute (hC,253);		//배경과 글씨색깔 바꾼다
		printf ("쏠\t");
		SetConsoleTextAttribute (hC,240);
		printf ("캘 린 더\n");
		
		gotoxy (0,7);
		
		for (i=0; i<5; i++)					//목록 출력
		{
			for (j=0; j<2; j++)
			{
				if (cont[i][j]==2)
				{
					printf ("\t달력 검색하기\n");
				}
				
				else if (cont[i][j]==3)
				{
					printf ("\t오늘의 달력보기\n");
				}
				
				else if (cont[i][j]==4)
				{
					printf ("\t모든 일정보기\n");
				}
				
				else if (cont[i][j]==5)
				{
					printf ("\t모든 메모보기\n");
				}
				
				else if (cont[i][j]==6)
				{
					printf ("\t종료하기\n");
				}
				
				else if (cont[i][j]==1)
				{
					printf ("  ▶  ");
				}
			}
			
			printf ("\n");
		}
		
		dir = getch();									//방향키 움직이게 하는 변수
		
		if (dir==72) // 키보드 위쪽방향키의 아스키코드값(=72) 입력
		{
			if (cont[y-1][x]==0)
			{
				cont[y-1][x]=1;
				cont[y][x]=0;
				y--;
			}
		}
		
		if (dir==80) // 키보드 아랫쪽방향키의 아스키코드값(=80) 입력
		{
			if (cont[y+1][x]==0)
			{
				cont[y+1][x]=1;
				cont[y][x]=0;
				y++;
			}
		}
		
		if (dir==13)
		{
			if (y==0)
			{
				system ("cls");

				gotoxy (3,1);
				printf ("검색하고 싶은 달력의 연도와 월을 입력하여 주세요.(XXXX년 XX월 형식으로 입력하세요.)\n");

				gotoxy (3,2);
				scanf ("%d%d", &year,&day);
				
				if(day>12)
				{				
					gotoxy (3,4);
					
					printf ("달을 12월까지만 있습니다. 다시 입력해주세요.");
					
					Sleep (1000);
					
					main ();
					
					return 0;
				}
				
				searchcalendar (year, day);
			}
			
			else if (y==1)
			{
				system ("cls");
				
				gotoxy (0,0);
				
				year= (t->tm_year + 1900); //오늘의 달력 
				day= (t->tm_mon + 1); //오늘의 달력
				
				searchcalendar (year, day);
			}
			
			else if (y==2)
			{
				system ("cls");
				
				gotoxy (0,0);
				
				printschedule ();
			}
			
			else if (y==3)
			{
				system ("cls");
				
				gotoxy (0,0);
				
				printmemo ();
			}
			
			else if (y==4)
			{
				system ("cls");
				
				gotoxy (0,0);
				
				end ();
				
				return 0;
			}
		}
	}
	
	return 0;
}

void cursor (char show) 					// 커서 깜빡임을 없애주는 함수
{
	HANDLE hConsole; // 콘솔 핸들
	CONSOLE_CURSOR_INFO ConsoleCursor; // 콘솔커서 정보 구조체

	hConsole = GetStdHandle (STD_OUTPUT_HANDLE); // 핸들을 구함

	ConsoleCursor.bVisible = show; // SHOW는 보임, HIDE는 안보임
	ConsoleCursor.dwSize = 1; // 커서 사이즈
	SetConsoleCursorInfo (hConsole , &ConsoleCursor); // 설정
}

void inputfun (int *x, int*y)    			//마우스 입력 함수
{
	HANDLE       hIn, hOut; //마우스를 사용하기 위한 콘솔 api 명령어들
	DWORD        dwNOER;
	INPUT_RECORD rec;

	hIn=GetStdHandle (STD_INPUT_HANDLE);
	hOut=GetStdHandle (STD_OUTPUT_HANDLE);
	SetConsoleMode (hIn, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    ReadConsoleInput (hIn,&rec,1,&dwNOER);   //콘솔 입력받음

	if (rec.EventType == MOUSE_EVENT)  //콘솔 입력이 마우스일때
	{
        
		if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) //왼쪽클릭일 때
		{
			*x = rec.Event.MouseEvent.dwMousePosition.X;
			*y = rec.Event.MouseEvent.dwMousePosition.Y;

		}
	}
        
        
}

int gotoxy (int x, int y) 					// 위치를 정할 수 있는 함수
{
	COORD pos = {x,y};
	
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DisableQuickEdit ()						//cmd 콘솔의 빠른편집기능 끄는 함수
{
    DWORD enable_quick_edit_mode = ENABLE_QUICK_EDIT_MODE; //GetConsoleMode()는 DWORD의 포인터를 받는다.(typedef DWORD* LPDWORD) 그래서 따로 함수로 구현해야 한다.
    HANDLE h =  GetStdHandle (STD_INPUT_HANDLE);
    if (GetConsoleMode(h, &enable_quick_edit_mode))
    {
      SetConsoleMode (h, ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS); //삽입모드만 설정한다. EXtended_Flags에는 삽입모드와 빠른 편집모드가 관련되어 있는데 사용자의 삽입모드까지 끄면 안되니 삽입모드는 적어놓는다.
    }
}

int searchcalendar (int year, int day)		//달력출력 함수
{
	system ("cls");
	system ("mode con: cols=120 lines=35");				//cmd창 조절
	DisableQuickEdit ();						//마우스 구현을 위해 win10에선 이 함수를 실행 해줘야 한다
	
	int x= -1, y= -1;
	int yearday[]= {0,31,28,31,30,31,30,31,31,30,31,30,31}; 	// 각 달의 마지막일  ..첫번째 수는 제외
	int lastyear, startday, i;								// lastyear:작년 말까지 진행된 요일을 계산하기 위한 변수, startday:1일이 시작되는 위치를 알기위한 변수
	int temp;
	int holiday[4]={0,};									//공휴일 배열
	int compare[4]={0,};
	
	while (1)
	{
		if ( year%4==0 && year%100!=0 || year%400==0 )			//윤년판별
		{
			yearday[2] = 29;									//윤년이면 2월 마지막일을 29일로 지정
		}
		
		else
		{
			yearday[2] = 28;
		}
		
		lastyear = year-1; 									// 작년 말까지 진행된 요일을 계산하기 위해
		startday = (lastyear+(lastyear/4)-(lastyear/100)+(lastyear/400)+1)%7;
		
		/*
		lastyear : 1년은 365일, 1년이 지날때마다 요일이 한번 늘어난다 (365를 7로 나누면 1이기 때문)
		+(lastyear/4)-(lastyear/100)+(lastyear/400) : 윤년의 다음연도는 요일이 두번 늘어므로, lastyear까지 있던 모든 윤년을 더한다
		+1 : 1년 1월 1일은 일요일이 아니라 월요일이므로 1을 더한다 (0년은 없다)
		%7 : 요일은 7가지 이므로, 앞의 계산값을 7로 나눈 나머지가 year년 1월의 첫요일 뜻한다
		나머지값: 0:일, 1:월, 2:화, 3:수, 4:목, 5:금, 6:토
		*/
		
		for (i=1;i<day;i++) 								// year년 month월 직전까지의 총 일 수를 구하기 위한 for문
		{
			startday+=yearday[i];
		}
		
		startday%=7; 											// year년 month월 1일의 시작 요일
		
		printf ("\n			    %d년 %d월\n",year,day); 	// year년 month월 타이틀 출력
		printf ("\n       일      월     화      수      목      금      토");

		for (i=-startday;i<yearday[day];i++)
		{
			if ((i+startday)%7 == 0) 								// 출력될 차례가 일요일이면, 개행
			{
				printf ("\n\n\n");
			}
			
			if (i<0) 										// month월 1일이 출력되기 이전의 날짜는 공백으로 채움
			{
				printf ("	");
			}
			
			else 											// 날짜 출력
			{
				printf ("%8d",i+1);
			}
		}
		
		gotoxy (0,22);
		printf ("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf ("「이달의 목표」\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		printf (" -\n");
		
		printobjective (year, day);									//이달의 목표 출력
		
		gotoxy (59,3);
		printf ("┏━━━━━━━━━━━━┓");
		gotoxy (59,4);
		printf ("┃        일정추가        ┃");
		gotoxy (59,5);
		printf ("┗━━━━━━━━━━━━┛");
		
		gotoxy (59,7);
		printf ("┏━━━━━━━━━━━━┓");
		gotoxy (59,8);
		printf ("┃        메모추가        ┃");
		gotoxy (59,9);
		printf ("┗━━━━━━━━━━━━┛");
		
		gotoxy (59,11);
		printf ("┏━━━━━━━━━━━━┓");
		gotoxy (59,12);
		printf ("┃        목표추가        ┃");
		gotoxy (59,13);
		printf ("┗━━━━━━━━━━━━┛");
		
		gotoxy (59,15);
		printf ("┏━━━━━━━━━━━━┓");
		gotoxy (59,16);
		printf ("┃        뒤로가기        ┃");
		gotoxy (59,17);
		printf ("┗━━━━━━━━━━━━┛");
	;
		gotoxy (91,4);
		printf ("	   「메모」");
		printmemo2 (year, day);
		
		gotoxy (91,18);
		printf ("	   「일정」");
		printschedule2 (year, day);
		
		gotoxy (0,25);

		while (1)
		{
			inputfun (&x,&y); // 마우스함수 호출, 포인터이용해서 x,y 좌표 받아옴

			if ( ((x>=60) && (x<=87)) && ((y>=3) && (y<=5)) ) // 좌표 60,3   ~   87,5 사이 눌렀을때
			{
				system ("cls");
				
				schedule (year, day);
			
				break;
				
				x=-1, y=-1; 
			}
			
			if ( ((x>=60) && (x<=87)) && ((y>=7) && (y<=9)) ) // 좌표 60,7   ~   87,9 사이 눌렀을때
			{
				system ("cls");
				
				inputmemo (year, day);
		
				break;
				
				x=-1, y=-1;
			}
			
			if ( ((x>=60) && (x<=87)) && ((y>=11) && (y<=13)) ) // 좌표 60,11   ~   87,13 사이 눌렀을때
			{
				system ("cls");
				
				inputobjective (year, day);
				
				break;
				
				x=-1, y=-1;
			}
			
			if ( ((x>=60) && (x<=87)) && ((y>=15) && (y<=17)) ) // 좌표 60,15   ~   87,17 사이 눌렀을때
			{
				system ("cls");
				
				main ();
				
				exit(0);
				
				x=-1, y=-1;
			}
		}
		
		return 0;
	}
}

int schedule (int year, int day)			//일정추가 함수
{
	FILE *fp = fopen ("schedule.txt", "at");
	
	char arr[65]= "";
	int temp;
	
	printf ("%d년 %d월 며칠에 일정을 추가하시겠습니까?\n", year,day);
	scanf ("%d", &temp);
	fflush (stdin);
	
	printf ("\n일정내용을 입력하세요. (30자가 넘어가면 짤릴수도 있습니다.)\n");
	
	fgets (arr, sizeof(arr), stdin);
	
	fprintf (fp, "%d%d%d\n%s", year,day,temp,arr);
	
	fclose (fp);
	
	searchcalendar (year,day);
}

int printschedule ()						//일정출력 함수
{
	FILE *fp = fopen ("schedule.txt", "rt");
	
	char arr[65]= "";
	int temp=0;
	
	while ( (fgets(arr,sizeof(arr),fp))!=NULL )
	{
		printf ("%s", arr);
		temp++;
		
		if (temp%2==0)
			printf ("\n");
	}
	
	temp = getch ();
	
	fclose (fp);
}

int printschedule2 (int year, int day)	//일정출력 함수(원하는 달)
{
	FILE *fp=fopen("schedule.txt","rt"); //파일 입출력
	char n[60];
	int i=0;
	int y=20;
	
	sprintf(n,"%d%d",year,day);

    for (i=0; i<100; i++)
    {
		fscanf(fp,"%s %[^\n]", stschedule[i].yearday, stschedule[i].content);

		if (strncmp(stschedule[i].yearday, n,6)==0)
		{
			gotoxy(91,y);
			printf(" - %s %s",stschedule[i].yearday, stschedule[i].content);
			y++;
			if(y==34)
			{
				break;
			}
		}
    }
   fclose(fp);
}

int inputobjective (int year, int day)		//이달의 목표 추가 함수 
{
	FILE *fp = fopen ("objective.txt", "at");
	char obarr[121]="";
	
	printf("%d %d에 입력할 목표 입니다.\n내용을 입력해주세요.(60자가 넘어가면 짤릴 수도 있습니다.)\n", year,day);
	
	fflush (stdin);
	
	fgets (obarr, sizeof(obarr), stdin); 
	
	fprintf (fp, "%d%d\n%s", year,day,obarr);
	
	fclose (fp);
	
	searchcalendar (year, day);
}

int printobjective (int year, int day)		//이달의 목표출력 함수
{
	FILE *fp=fopen("objective.txt","rt"); //파일 입출력
	char n[60];
	int i=0;
	int x=0;
	int y=24;
	
	sprintf(n,"%d%d",year,day);

    for (i=0; i<100; i++)
    {
		fscanf(fp,"%s %[^\n]", stobjective[i].yearday, stobjective[i].content);

		if (strcmp(stobjective[i].yearday, n)==0)
		{
			gotoxy(x,y);
			printf(" - %s",stobjective[i].content);
			y++;
			if(y==34)
			{
				x=45;
				y=24;
			}
		}
    }
   fclose(fp);
}

int inputmemo (int year, int day)			//메모추가 함수
{	
	FILE *memo = fopen("memo.txt", "at");
	
	char mstr[65]= "";
	
	printf ("%d년 %d월에 입력할 메모 입니다.\n\n", year,day);
	printf ("메모내용을 입력하세요.(30자가 넘어가면 짤릴 수도 있습니다.)\n\n");
	
	fflush (stdin);
	
	fgets (mstr, sizeof(mstr), stdin); 
	
	fprintf (memo, "%d%d\n%s", year,day,mstr);
	
	fclose (memo);
	
	searchcalendar (year, day);
}

int printmemo ()							//메모출력 함수
{
	FILE *fp = fopen ("memo.txt", "rt");
	
	char arr[65]= "";
	int temp=0;
	
	while ( (fgets(arr,sizeof(arr),fp))!=NULL )
	{
		printf ("%s", arr);
		temp++;
		
		if (temp%2==0)
			printf ("\n");
	}
	
	temp = getch ();
	
	fclose (fp);
}

int printmemo2 (int year, int day)						//메모출력 함수(원하는 달)
{
	FILE *fp=fopen("memo.txt","rt"); //파일 입출력
	char n[60];
	int i=0;
	int y=6;
	
	sprintf(n,"%d%d",year,day);

    for (i=0; i<100; i++)
    {
		fscanf(fp,"%s %[^\n]", stmemo[i].yearday, stmemo[i].content);

		if (strcmp(stmemo[i].yearday, n)==0)
		{
			gotoxy(91,y);
			printf(" - %s",stmemo[i].content);
			y++;
			if(y==16)
			{
				break;
			}
		}
    }
   fclose(fp);
}

int end (void)								//끝내는 함수
{
	HANDLE hC = GetStdHandle(STD_OUTPUT_HANDLE);
	
	system ("cls");
	cursor (0);
	
	int i, j, x=0, y=0;
	int dir=0;
	int cont[2][2]=
	{
		{1,2},
		{0,3}
	};	
	
	while (1)
	{
		system("cls");
		
		gotoxy (30,2);
		SetConsoleTextAttribute(hC,253);
		printf ("쏠 ");
		SetConsoleTextAttribute(hC,240);
		printf ("캘린더를 종료하시겠습니까?\n");
		
		gotoxy (0,5);
	
		for (i=0; i<2; i++)
		{
			for (j=0; j<2; j++)
			{
				if (cont[i][j]==0)
				{
					printf ("  ");
				}
				else if (cont[i][j]==2)
				{
					printf ("\t예\n");
				}
				
				else if (cont[i][j]==3)
				{
					printf ("\t아니오\n");
				}
				
				else if (cont[i][j]==1)
				{
					printf ("  ▶  ");
				}
			}
			
			printf ("\n");
		}
		
		dir = getch();
		
		if (dir==72) // 키보드 위쪽방향키의 아스키코드값(=72) 입력
		{
			if (cont[y-1][x]==0)
			{
				cont[y-1][x]=1;
				cont[y][x]=0;
				y--;
			}
		}
		
		if (dir==80) // 키보드 아랫쪽방향키의 아스키코드값(=80) 입력
		{
			if (cont[y+1][x]==0)
			{
				cont[y+1][x]=1;
				cont[y][x]=0;
				y++;
			}
		}
		
		if (dir==13)
		{
			if (y==0)
			{
				system ("cls");
				
				return 0;
			}
			
			else if (y==1)
			{
				system ("cls");
				
				main ();
			}
			
		}
	}
}

