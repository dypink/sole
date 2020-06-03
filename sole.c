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


void cursor (char show);				//Ŀ������� �Լ�
int gotoxy (int x, int y);				//��ǥ�̵� �Լ�
int searchcalendar (int year, int day);	//�޷���� �Լ�

int schedule (int year, int day);		//�����߰� �Լ�
int printschedule ();					//������� �Լ�(���)
int printschedule2 (int year, int day);	//������� �Լ�(���ϴ� ��)

int inputobjective (int year, int day);	//�̴��� ��ǥ�߰� �Լ�
int printobjective (int year, int day);	//�̴��� ��ǥ��� �Լ�

int inputmemo (int year, int day);		//�޸��߰� �Լ�
int printmemo ();						//�޸���� �Լ�(���)
int printmemo2 (int year, int day);		//�޸���� �Լ�(���ϴ� ��)

int end ();								//�����ϴ� �Լ�

int main (void) // �������̽��� �����ϴ� �Լ�
{
	HANDLE hC = GetStdHandle (STD_OUTPUT_HANDLE);		//�����Լ�
	
	system ("mode con: cols=90 lines=35");				//cmdâ ����
	system ("cls");
	cursor (0);											//cmdâ�� �����Ÿ��� Ŀ�� off
	
	struct tm *t;	//�ð� ����ü
	time_t timer; // �ð�����
	
	timer = time (NULL); // ���� �ð��� �� ������ ���
	t = localtime (&timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
	
	int year, day;
	int i, j, x=0, y=0;
	int dir=0;
	int cont[5][2]=			//��� �迭
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
		SetConsoleTextAttribute (hC,253);		//���� �۾����� �ٲ۴�
		printf ("��\t");
		SetConsoleTextAttribute (hC,240);
		printf ("Ķ �� ��\n");
		
		gotoxy (0,7);
		
		for (i=0; i<5; i++)					//��� ���
		{
			for (j=0; j<2; j++)
			{
				if (cont[i][j]==2)
				{
					printf ("\t�޷� �˻��ϱ�\n");
				}
				
				else if (cont[i][j]==3)
				{
					printf ("\t������ �޷º���\n");
				}
				
				else if (cont[i][j]==4)
				{
					printf ("\t��� ��������\n");
				}
				
				else if (cont[i][j]==5)
				{
					printf ("\t��� �޸𺸱�\n");
				}
				
				else if (cont[i][j]==6)
				{
					printf ("\t�����ϱ�\n");
				}
				
				else if (cont[i][j]==1)
				{
					printf ("  ��  ");
				}
			}
			
			printf ("\n");
		}
		
		dir = getch();									//����Ű �����̰� �ϴ� ����
		
		if (dir==72) // Ű���� ���ʹ���Ű�� �ƽ�Ű�ڵ尪(=72) �Է�
		{
			if (cont[y-1][x]==0)
			{
				cont[y-1][x]=1;
				cont[y][x]=0;
				y--;
			}
		}
		
		if (dir==80) // Ű���� �Ʒ��ʹ���Ű�� �ƽ�Ű�ڵ尪(=80) �Է�
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
				printf ("�˻��ϰ� ���� �޷��� ������ ���� �Է��Ͽ� �ּ���.(XXXX�� XX�� �������� �Է��ϼ���.)\n");

				gotoxy (3,2);
				scanf ("%d%d", &year,&day);
				
				if(day>12)
				{				
					gotoxy (3,4);
					
					printf ("���� 12�������� �ֽ��ϴ�. �ٽ� �Է����ּ���.");
					
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
				
				year= (t->tm_year + 1900); //������ �޷� 
				day= (t->tm_mon + 1); //������ �޷�
				
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

void cursor (char show) 					// Ŀ�� �������� �����ִ� �Լ�
{
	HANDLE hConsole; // �ܼ� �ڵ�
	CONSOLE_CURSOR_INFO ConsoleCursor; // �ܼ�Ŀ�� ���� ����ü

	hConsole = GetStdHandle (STD_OUTPUT_HANDLE); // �ڵ��� ����

	ConsoleCursor.bVisible = show; // SHOW�� ����, HIDE�� �Ⱥ���
	ConsoleCursor.dwSize = 1; // Ŀ�� ������
	SetConsoleCursorInfo (hConsole , &ConsoleCursor); // ����
}

void inputfun (int *x, int*y)    			//���콺 �Է� �Լ�
{
	HANDLE       hIn, hOut; //���콺�� ����ϱ� ���� �ܼ� api ��ɾ��
	DWORD        dwNOER;
	INPUT_RECORD rec;

	hIn=GetStdHandle (STD_INPUT_HANDLE);
	hOut=GetStdHandle (STD_OUTPUT_HANDLE);
	SetConsoleMode (hIn, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

    ReadConsoleInput (hIn,&rec,1,&dwNOER);   //�ܼ� �Է¹���

	if (rec.EventType == MOUSE_EVENT)  //�ܼ� �Է��� ���콺�϶�
	{
        
		if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) //����Ŭ���� ��
		{
			*x = rec.Event.MouseEvent.dwMousePosition.X;
			*y = rec.Event.MouseEvent.dwMousePosition.Y;

		}
	}
        
        
}

int gotoxy (int x, int y) 					// ��ġ�� ���� �� �ִ� �Լ�
{
	COORD pos = {x,y};
	
	SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DisableQuickEdit ()						//cmd �ܼ��� ����������� ���� �Լ�
{
    DWORD enable_quick_edit_mode = ENABLE_QUICK_EDIT_MODE; //GetConsoleMode()�� DWORD�� �����͸� �޴´�.(typedef DWORD* LPDWORD) �׷��� ���� �Լ��� �����ؾ� �Ѵ�.
    HANDLE h =  GetStdHandle (STD_INPUT_HANDLE);
    if (GetConsoleMode(h, &enable_quick_edit_mode))
    {
      SetConsoleMode (h, ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS); //���Ը�常 �����Ѵ�. EXtended_Flags���� ���Ը��� ���� ������尡 ���õǾ� �ִµ� ������� ���Ը����� ���� �ȵǴ� ���Ը��� ������´�.
    }
}

int searchcalendar (int year, int day)		//�޷���� �Լ�
{
	system ("cls");
	system ("mode con: cols=120 lines=35");				//cmdâ ����
	DisableQuickEdit ();						//���콺 ������ ���� win10���� �� �Լ��� ���� ����� �Ѵ�
	
	int x= -1, y= -1;
	int yearday[]= {0,31,28,31,30,31,30,31,31,30,31,30,31}; 	// �� ���� ��������  ..ù��° ���� ����
	int lastyear, startday, i;								// lastyear:�۳� ������ ����� ������ ����ϱ� ���� ����, startday:1���� ���۵Ǵ� ��ġ�� �˱����� ����
	int temp;
	int holiday[4]={0,};									//������ �迭
	int compare[4]={0,};
	
	while (1)
	{
		if ( year%4==0 && year%100!=0 || year%400==0 )			//�����Ǻ�
		{
			yearday[2] = 29;									//�����̸� 2�� ���������� 29�Ϸ� ����
		}
		
		else
		{
			yearday[2] = 28;
		}
		
		lastyear = year-1; 									// �۳� ������ ����� ������ ����ϱ� ����
		startday = (lastyear+(lastyear/4)-(lastyear/100)+(lastyear/400)+1)%7;
		
		/*
		lastyear : 1���� 365��, 1���� ���������� ������ �ѹ� �þ�� (365�� 7�� ������ 1�̱� ����)
		+(lastyear/4)-(lastyear/100)+(lastyear/400) : ������ ���������� ������ �ι� �þ�Ƿ�, lastyear���� �ִ� ��� ������ ���Ѵ�
		+1 : 1�� 1�� 1���� �Ͽ����� �ƴ϶� �������̹Ƿ� 1�� ���Ѵ� (0���� ����)
		%7 : ������ 7���� �̹Ƿ�, ���� ��갪�� 7�� ���� �������� year�� 1���� ù���� ���Ѵ�
		��������: 0:��, 1:��, 2:ȭ, 3:��, 4:��, 5:��, 6:��
		*/
		
		for (i=1;i<day;i++) 								// year�� month�� ���������� �� �� ���� ���ϱ� ���� for��
		{
			startday+=yearday[i];
		}
		
		startday%=7; 											// year�� month�� 1���� ���� ����
		
		printf ("\n			    %d�� %d��\n",year,day); 	// year�� month�� Ÿ��Ʋ ���
		printf ("\n       ��      ��     ȭ      ��      ��      ��      ��");

		for (i=-startday;i<yearday[day];i++)
		{
			if ((i+startday)%7 == 0) 								// ��µ� ���ʰ� �Ͽ����̸�, ����
			{
				printf ("\n\n\n");
			}
			
			if (i<0) 										// month�� 1���� ��µǱ� ������ ��¥�� �������� ä��
			{
				printf ("	");
			}
			
			else 											// ��¥ ���
			{
				printf ("%8d",i+1);
			}
		}
		
		gotoxy (0,22);
		printf ("��������������������������������������������������������������������������������������\n");
		printf ("���̴��� ��ǥ��\n");
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
		
		printobjective (year, day);									//�̴��� ��ǥ ���
		
		gotoxy (59,3);
		printf ("����������������������������");
		gotoxy (59,4);
		printf ("��        �����߰�        ��");
		gotoxy (59,5);
		printf ("����������������������������");
		
		gotoxy (59,7);
		printf ("����������������������������");
		gotoxy (59,8);
		printf ("��        �޸��߰�        ��");
		gotoxy (59,9);
		printf ("����������������������������");
		
		gotoxy (59,11);
		printf ("����������������������������");
		gotoxy (59,12);
		printf ("��        ��ǥ�߰�        ��");
		gotoxy (59,13);
		printf ("����������������������������");
		
		gotoxy (59,15);
		printf ("����������������������������");
		gotoxy (59,16);
		printf ("��        �ڷΰ���        ��");
		gotoxy (59,17);
		printf ("����������������������������");
	;
		gotoxy (91,4);
		printf ("	   ���޸�");
		printmemo2 (year, day);
		
		gotoxy (91,18);
		printf ("	   ��������");
		printschedule2 (year, day);
		
		gotoxy (0,25);

		while (1)
		{
			inputfun (&x,&y); // ���콺�Լ� ȣ��, �������̿��ؼ� x,y ��ǥ �޾ƿ�

			if ( ((x>=60) && (x<=87)) && ((y>=3) && (y<=5)) ) // ��ǥ 60,3   ~   87,5 ���� ��������
			{
				system ("cls");
				
				schedule (year, day);
			
				break;
				
				x=-1, y=-1; 
			}
			
			if ( ((x>=60) && (x<=87)) && ((y>=7) && (y<=9)) ) // ��ǥ 60,7   ~   87,9 ���� ��������
			{
				system ("cls");
				
				inputmemo (year, day);
		
				break;
				
				x=-1, y=-1;
			}
			
			if ( ((x>=60) && (x<=87)) && ((y>=11) && (y<=13)) ) // ��ǥ 60,11   ~   87,13 ���� ��������
			{
				system ("cls");
				
				inputobjective (year, day);
				
				break;
				
				x=-1, y=-1;
			}
			
			if ( ((x>=60) && (x<=87)) && ((y>=15) && (y<=17)) ) // ��ǥ 60,15   ~   87,17 ���� ��������
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

int schedule (int year, int day)			//�����߰� �Լ�
{
	FILE *fp = fopen ("schedule.txt", "at");
	
	char arr[65]= "";
	int temp;
	
	printf ("%d�� %d�� ��ĥ�� ������ �߰��Ͻðڽ��ϱ�?\n", year,day);
	scanf ("%d", &temp);
	fflush (stdin);
	
	printf ("\n���������� �Է��ϼ���. (30�ڰ� �Ѿ�� ©������ �ֽ��ϴ�.)\n");
	
	fgets (arr, sizeof(arr), stdin);
	
	fprintf (fp, "%d%d%d\n%s", year,day,temp,arr);
	
	fclose (fp);
	
	searchcalendar (year,day);
}

int printschedule ()						//������� �Լ�
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

int printschedule2 (int year, int day)	//������� �Լ�(���ϴ� ��)
{
	FILE *fp=fopen("schedule.txt","rt"); //���� �����
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

int inputobjective (int year, int day)		//�̴��� ��ǥ �߰� �Լ� 
{
	FILE *fp = fopen ("objective.txt", "at");
	char obarr[121]="";
	
	printf("%d %d�� �Է��� ��ǥ �Դϴ�.\n������ �Է����ּ���.(60�ڰ� �Ѿ�� ©�� ���� �ֽ��ϴ�.)\n", year,day);
	
	fflush (stdin);
	
	fgets (obarr, sizeof(obarr), stdin); 
	
	fprintf (fp, "%d%d\n%s", year,day,obarr);
	
	fclose (fp);
	
	searchcalendar (year, day);
}

int printobjective (int year, int day)		//�̴��� ��ǥ��� �Լ�
{
	FILE *fp=fopen("objective.txt","rt"); //���� �����
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

int inputmemo (int year, int day)			//�޸��߰� �Լ�
{	
	FILE *memo = fopen("memo.txt", "at");
	
	char mstr[65]= "";
	
	printf ("%d�� %d���� �Է��� �޸� �Դϴ�.\n\n", year,day);
	printf ("�޸𳻿��� �Է��ϼ���.(30�ڰ� �Ѿ�� ©�� ���� �ֽ��ϴ�.)\n\n");
	
	fflush (stdin);
	
	fgets (mstr, sizeof(mstr), stdin); 
	
	fprintf (memo, "%d%d\n%s", year,day,mstr);
	
	fclose (memo);
	
	searchcalendar (year, day);
}

int printmemo ()							//�޸���� �Լ�
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

int printmemo2 (int year, int day)						//�޸���� �Լ�(���ϴ� ��)
{
	FILE *fp=fopen("memo.txt","rt"); //���� �����
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

int end (void)								//������ �Լ�
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
		printf ("�� ");
		SetConsoleTextAttribute(hC,240);
		printf ("Ķ������ �����Ͻðڽ��ϱ�?\n");
		
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
					printf ("\t��\n");
				}
				
				else if (cont[i][j]==3)
				{
					printf ("\t�ƴϿ�\n");
				}
				
				else if (cont[i][j]==1)
				{
					printf ("  ��  ");
				}
			}
			
			printf ("\n");
		}
		
		dir = getch();
		
		if (dir==72) // Ű���� ���ʹ���Ű�� �ƽ�Ű�ڵ尪(=72) �Է�
		{
			if (cont[y-1][x]==0)
			{
				cont[y-1][x]=1;
				cont[y][x]=0;
				y--;
			}
		}
		
		if (dir==80) // Ű���� �Ʒ��ʹ���Ű�� �ƽ�Ű�ڵ尪(=80) �Է�
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

