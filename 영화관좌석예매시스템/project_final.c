#include <stdio.h>
#include <string.h>

struct user_info
{
	int id_num;
	char id[10];
	char password[10];
};

int load_user_info(struct user_info * first, int b[][5]);
int check_login(char a[], char b[], struct user_info c[], int * d);
int print_seat(int a[][5]);
int reserve(int a[][5], char r[10], int num);
int cancel(int a[][5], char r[10], int num);
int save_reservation_info(int a[][5]);

int main(void)
{
	int a[3][5] = {
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	};
	
	char id_use[10];
	char password_use[10];
	struct user_info user[10];
	int check;
	char seat;
	int seat_num;
	int order;
	
	load_user_info(user, a);
		
	while(1)															//로그인하기
	{
		printf("<로그인을 하시오>\n");
		printf("아이디를 입력하시오 \n");
		scanf("%s", id_use);
		printf("비밀번호를 입력하시오\n");
		scanf("%s", password_use);
		
		check = check_login(id_use, password_use, user, &order);
		
		if(check == -1)
			printf("해당 아이디가 없음\n");
		else if(check == -2)
			printf("**비밀번호가 틀림**\n\n");
		else
		{
			printf("%s님 반갑습니다.\n", id_use);
			break;
		}
	}
	
	while(1)														//메뉴를 입력하기
	{
		int menu;
		int menu_2;
		char seat[10];
		int menu_3;
		char seat_2[10];
		
		printf("1---좌석 확인하기\n");
		printf("2---예약하기\n");
		printf("3---예약취소하기\n");
		printf("4---다른 사용자로 로그인하기\n");
		printf("5---종료하기\n");
		printf("메뉴를 선택하시오\n");
		scanf("%d", &menu);

		if(menu == 1)
			print_seat(a);
		else if(menu == 2)
		{
			printf("예약을 원하는 자리는(예-A2)?");
			scanf("%s",seat);
			menu_2 = reserve(a, seat, (order+1)*10);
			if(menu_2 == 0)
				printf("예약이 완료되었습니다.\n");
			else
				printf("*************이미 예약된 자리입니다.************* \n");
			print_seat(a);
		}
		else if(menu == 3)
		{
			printf("예약취소를 원하는 자리는(예-A2)?\n");
			scanf("%s",seat_2);
			menu_3 = cancel(a, seat_2, (order+1)*10);
			if(menu_3 == 0)
				printf("예약이 취소되었습니다. \n\n");
			else if(menu_3 == -1)
				printf("*************당신이 예약한 자리가 아닙니다.************* \n\n");
			else
				printf("*************예약된 자리가 아닙니다.*************\n\n");
			print_seat(a);
		}
		else if(menu == 4)
		{
			while(1)
			{
				printf("<로그인을 하시오>\n");
				printf("아이디를 입력하시오 \n");
				scanf("%s", id_use);
				printf("비밀번호를 입력하시오\n");
				scanf("%s", password_use);
		
				check = check_login(id_use, password_use, user, &order);
		
				if(check == -1)
					printf("해당 아이디가 없음\n");
				else if(check == -2)
					printf("**비밀번호가 틀림**\n\n");
				else
				{
					printf("%s님 반갑습니다.\n", id_use);
					break;
				}
			}		
		}
		else if(menu == 5)
		{
			printf("종료되었습니다. 이용해주셔서 감사합니다 ^^ \n");
			save_reservation_info(a);
			break;
		}
	}
	return 0;
}

int load_user_info(struct user_info * first, int b[][5])
{
	FILE *fp1;
	fp1 = fopen("reservation_data.txt", "r");				// 예약 데이터 가져오기
	if( fp1 == NULL)
	{
		printf("파일 열기 실패1\n");
		return 0;
	}
	else
		printf("파일 열기 성공1 \n");
	
	int arr;
	int hang;
	for(arr=0; arr<3; arr++)
	{
		for(hang=0; hang<5; hang++)
		{
			fscanf(fp1,"%d",&b[arr][hang]);
		}
	}
	fclose(fp1);
	
	FILE *fp2;
	fp2 = fopen("user_data.txt", "r");			// user 데이터 가져오기
	if( fp2 == NULL)
	{
		printf("파일 열기 실패2\n");
		return 0;
	}
	else
		printf("파일 열기 성공2 \n");
	
	int e;
	for(e=0; e<10; e++)
	{
		fscanf(fp2, "%d %s %s", &first[e].id_num, first[e].id, first[e].password);
	}
	
	fclose(fp2);
	
	int i, j, temp, least;														//user selectionsort
	char box[10];
	for(i=0; i<10; i++)
	{
		least = i;
		for(j=i+1; j<10; j++)
		{	
			if(first[j].id_num<first[least].id_num)
				least=j;
		
		temp = first[i].id_num;
		first[i].id_num = first[least].id_num;
		first[least].id_num = temp;
		
		strcpy(box, first[i].id);
		strcpy(first[i].id, first[least].id);
		strcpy(first[i].id, box);
		
		strcpy(box, first[i].password);
		strcpy(first[i].password, first[least].password);
		strcpy(first[i].password, box);
		}
	
	return 0;
	}
}

int check_login(char a[], char b[], struct user_info c[], int * d)					//아이디, 비번 확인 함수
{
	int low, high, middle;
	low = 0;
	high = 9;
	while(low <= high)
	{
		middle = (low + high)/2;
		
		if(strcmp(a,c[middle].id) == 0)
		{
			if(strcmp(b,c[middle].password)==0)
			{
				*d = middle;
				return 0;
			}
			else
				return -2;
		}
		else if(strcmp(a,c[middle].password)<0)
			high = middle + 1;
		else 
			low = middle + 1;
			
	}
	return -1;
}

int print_seat(int a[][5])																	//자리 출력 함수
{
	printf("   | 0  1  2  3  4\n");
	printf("----------------------\n");
	
	
	int i, e;
	for(i=0; i<3; i++)
	{
		printf("%c |", i+65);
		for(e=0; e<5; e++)
		{
			printf("%3d", a[i][e]);
		}
		printf("\n");
	}
	printf("\n");
	
	return 1;
}
int reserve(int a[][5], char c[10], int num)												//예약함수
{
	char r;
	int e;
	sscanf(c,"%c%d", &r, &e);
	int p;
	p = r-65;
	
	if(a[p][e] == 0)
	{
		a[p][e] = num;
		printf("%d // %d\n", a[p][e], num );
		return 0;
	}	
	
	else
		return -1;
}
int cancel(int a[][5], char c[10], int num)														//예약취소함수
{
	char q;
	int w;
	sscanf(c,"%c%d", &q, &w);
	int k;
	k = q-65;
	if(a[k][w] == num)
	{
		a[k][w] = 0;
		return 0;
	}
	else if(a[k][w] == 0)
		return -2;
	else
		return -1;
}
int save_reservation_info(int a[][5])															//종료하며 예약파일 저장
{
	int arr, hang;
	FILE *fp3;
	fp3 = fopen("reservation_data.txt", "w");
	if( fp3 == NULL)
	{
		printf("파일 열기 실패3\n");
		return 0;
	}
	else
		printf("파일 열기 성공3 \n");
	
	for(arr=0; arr<3; arr++)
	{
		for(hang=0; hang<5; hang++)
		{
			fprintf(fp3, "%d ", a[arr][hang]);
		}
		fputc('\n', fp3);
	}

	
	return 0;
}