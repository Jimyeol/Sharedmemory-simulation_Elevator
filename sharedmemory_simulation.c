#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <pthread.h>

//공유메모리
#define MAX_SIZE 5000	//최대 5000까지 받음
#define SLEEPNUM 1		//Sleep, 테스트시 0, 시뮬레이션 확인시 1

pthread_mutex_t work_mutex;	//뮤텍스

struct Human {		//input.txt에서 입력받을 구조체
	int cntPos;
	int goalPos;
};
struct Human human[MAX_SIZE];

int count = 0;	//입력 갯수

enum {			//프로세스 잡을시 무슨 엘레베이터가 잡았는가
	ELE_A = 0,
	ELE_B,
	ELE_CHANGE_A,
};


struct shared_use_st {		//공유 메모리
	int eleApos;			//A 엘레베이터 위치
	int eleBpos;			//B 엘레베이터 위치
	int cntPos;				//현재 사람 위치
	int goalPos;			//사람이 가고싶은 위치
	int current;			//현재 프로세스 엘레베이터
	int cmpNum;				//절대값
	bool cmpCheck;			//A엘레베이터가 먼저올지 B엘레베이터가 먼저올지
};


/*엘레베이터 디스플레이*/
void ElevaterDisplay(int eleApos, int eleBpos)
{
	system("clear");
	printf("\n\n");
	int i =0;
	for(i = 10 ; i >=1; i--) {

		printf("//%d\t\t", i);	//몇층인지 알려줌
		if( i == eleApos )
			printf("■");
		else
			printf("□");

		printf("\t\t");

		if( i == eleBpos )
			printf("■");
		else
			printf("□");

		printf("\n");
	}
}


/*pthread_display 엘레베이터 위치 쓰레드로 출력*/
void *display(void *arg)
{
	struct shared_use_st *shared_stuff = (struct shared_use_st*)arg;
	int i = 0;
	int cntPos = shared_stuff->cntPos;
	int eleApos = shared_stuff->eleApos;
	int eleBpos = shared_stuff->eleBpos;
	int current = shared_stuff->current;
	int goalPos = shared_stuff->goalPos;
	int cntpos =  shared_stuff->cntPos;

	pthread_mutex_lock(&work_mutex);	//동기화 실행
	system("clear");


	if( current == ELE_CHANGE_A)
	{
		if(eleApos >= cntpos && cntpos <= goalPos)	//내가밑이고 엘베위인데 올라갈때	
		{
			for(; eleApos > cntpos; eleApos--)	//일단 엘베내려옴
			{

				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleApos <= goalPos; eleApos++)	//그리고 올라감
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos--;
		} 
		else if( eleApos >= cntpos && cntpos >= goalPos)//내가 밑이고 엘베위인데 내려갈때
		{
			for(; eleApos >= goalPos; eleApos--)	//일단 엘베쭉내려옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos++;
		}
		else if( eleApos <= cntpos && cntpos >= goalPos)//내가 위이고 엘베밑인데 내려갈때
		{
			for(; eleApos < cntpos; eleApos++)	//일단 엘베올라옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleApos >= goalPos; eleApos--) //그리고 내려감
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos++;
		}
		else if( eleApos <= cntpos && cntpos <= goalPos)//내가 위이고 엘베밑인데 올라갈때
		{
			for(; eleApos <= goalPos; eleApos++)	//일단 엘베쭉올라옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos--;
		}
	} 
	else if( current == ELE_B)
	{
		if(eleBpos >= cntpos && cntpos <= goalPos)	//내가밑이고 엘베위인데 올라갈때	
		{
			for(; eleBpos > cntpos; eleBpos--)	//일단 엘베내려옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleBpos <= goalPos; eleBpos++)	//그리고 올라감
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos--;
		} 
		else if( eleBpos >= cntpos && cntpos >= goalPos)//내가 밑이고 엘베위인데 내려갈때
		{
			for(; eleBpos >= goalPos; eleBpos--)	//일단 엘베쭉내려옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos++;
		}
		else if( eleBpos <= cntpos && cntpos >= goalPos)//내가 위이고 엘베밑인데 내려갈때
		{
			for(; eleBpos < cntpos; eleBpos++)	//일단 엘베올라옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleBpos >= goalPos; eleBpos--) //그리고 내려감
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos++;
		}
		else if( eleBpos <= cntpos && cntpos <= goalPos)//내가 위이고 엘베밑인데 올라갈때
		{
			for(; eleBpos <= goalPos; eleBpos++)	//일단 엘베쭉올라옴
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos--;
		}
	}


	shared_stuff->current = ELE_A;
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);
}


/*pthread_input txt파일 입력 받는 쓰레드*/
void *input_pthread(void *arg)
{
	pthread_mutex_lock(&work_mutex);	//동기화
	FILE* fp;
	int i =0;

	fp = fopen("input.txt", "r");

	while(!feof(fp)) {
		fscanf(fp, "%d %d", &human[count].cntPos, &human[count].goalPos);	//human구조체에 일단 받는다.
		if( count >= MAX_SIZE -1 )
			break;
		count++;

	}
	fclose(fp);
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);	//쓰레드 종료
}


/*pthread_output txt파일 출력 하는 쓰레드*/
void *output_pthread(void *arg)
{
	struct shared_use_st *shared_stuff = (struct shared_use_st*)arg;

	pthread_mutex_lock(&work_mutex);
	FILE* fp;
	int i =0;

	fp = fopen("output.txt", "a");

	fprintf(fp, "A : %d, B : %d\n", shared_stuff->eleApos, shared_stuff->eleBpos);
	//A엘레베이터, B의 엘레베이터 위치를 Log작성

	fclose(fp);
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);
}


int main (void)
{
	pid_t	pid;		//프로세스
	int		cntPos;		//현재 위치
	int		goalPos;	//목표 위치
	void	*shared_memory = (void *)0;	//쉐어드 메모리
	struct	shared_use_st *shared_stuff;	//쉐어드 메모리 구조체

	pthread_mutex_init(&work_mutex, NULL);	//뮤텍스 초기화
	int shmid;	//쉐어드 메모리 아이디
	srand((unsigned int)getpid()); 

	//공유할 메모리 영역 생성 
	shmid = shmget((key_t)60075, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	// 공유메모리를 프로세스 주소공간에 연결
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %X\n", (int)shared_memory); //메모리 위치 확인
	shared_stuff = (struct shared_use_st *)shared_memory;

	shared_stuff->current = ELE_A; //초기 엘레베이터A가 잡음
	shared_stuff->eleApos = 1;		//엘레베이터A는 1층
	shared_stuff->eleBpos = 10;		//엘레베이터 B는 10층


	if ( (pid = fork()) < 0) {	//포크에러
		printf("fork error\n"); 
		if (shmdt(shared_memory) == -1) {
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}
		//공유 메모리 제거
		if (shmctl(shmid, IPC_RMID, 0) == -1) {
			fprintf(stderr, "shmctl(IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}
		exit(1);

	} else if (pid == 0) {	//엘레베이터 A
		/* Elevator One*/
		int cntFloor = 1;	//현재 엘레베이터 층수
		int i = 0;

		pthread_t inputTread;		//입력스레드
		void *input_thread_result;

		pthread_create(&inputTread, NULL, input_pthread, NULL);//입력스레드

		pthread_join(inputTread, &input_thread_result);


		for( ;; )
		{
			if( shared_stuff->current == ELE_A )	//현재 프로세스가 엘레베이터A라면
			{
				shared_stuff->cntPos = human[i].cntPos;		//입력받은값 공유메모리에 집어넣음
				shared_stuff->goalPos = human[i].goalPos;
				i++;

				/* 종료값 -1 받으면 메모리 해제하고 종료*/
				if( i > count-1 || shared_stuff->cntPos < 0 ||  shared_stuff->goalPos < 0)
				{
					//공유 메모리를 해제
					if (shmdt(shared_memory) == -1) {
						//fprintf(stderr, "shmdt failed\n");
						exit(EXIT_FAILURE);
					}
					//공유 메모리 제거
					if (shmctl(shmid, IPC_RMID, 0) == -1) {
						//fprintf(stderr, "shmctl(IPC_RMID) failed\n");
						exit(EXIT_FAILURE);
					}
					exit(1);
				}


				/*printf("현재 위치와 올라갈 위치 입력 : ");
				scanf("%d %d", &shared_stuff->cntPos, &shared_stuff->goalPos);*/

				int cmpNum = 0;		//엘레베이터B와 비교할값
				cmpNum = abs(cntFloor - shared_stuff->cntPos);	//절댓값 계산

				shared_stuff->cmpNum = cmpNum;	//엘레베이터 A의 절대값 일단 저장
				shared_stuff->current = ELE_B;	//CPU사용을 엘레베이터 B로 옮김
			} else if( shared_stuff->current == ELE_CHANGE_A )	//만약 엘레베이터A의 위치가 가까우면 실행
			{
				pthread_t a_thread;	//디스플레이 스레드 생성
				void *thread_result;

				pthread_t out_thread;	//출력값 스레드 생성
				void *out_thread_result;

				cntFloor = shared_stuff->goalPos;	//현재 엘레베이터 층수를 최종목적지로 변경
				pthread_create(&a_thread, NULL, display, (void *)shared_stuff);	//디스플레이 스레드
				pthread_join(a_thread, &thread_result);
				shared_stuff->eleApos = cntFloor;	//엘레베이터 공유메모리 층수를 현재층수로 변경
				//Output 출력 스레드
				pthread_create(&out_thread, NULL, output_pthread, (void *)shared_stuff);
				pthread_join(out_thread, &out_thread_result);

				//다시 엘베A로 CPU넘김
				shared_stuff->current = ELE_A;
			}
		}
	} else {	//엘레베이터 B
		/* Elevator Two*/
		int cntFloor = 10;	//현재 엘레베이터 층수
		for( ;; )
		{
			if( shared_stuff->cntPos == -1)
			{
				//공유 메모리를 해제
				if (shmdt(shared_memory) == -1) {
					//fprintf(stderr, "shmdt failed\n");
					exit(EXIT_FAILURE);
				}
				//공유 메모리 제거
				if (shmctl(shmid, IPC_RMID, 0) == -1) {
					//fprintf(stderr, "shmctl(IPC_RMID) failed\n");
					exit(EXIT_FAILURE);
				}
				exit(1);
			}

			if( shared_stuff->current == ELE_B )	//엘레베이터 B가 CPU잡음
			{
				int cmpNum = 0;		//비교값
				bool cmpCheck;		//비교값 체크

				cmpNum = abs(cntFloor - shared_stuff->cntPos);	//절대값 확인
				/*엘베A와 엘베B 비교해서 제일 가까운놈 체크*/
				cmpCheck = (cmpNum > shared_stuff->cmpNum) ? true : false;
				//제일 가까운놈을 체크
				shared_stuff->cmpCheck = cmpCheck;
				if( !cmpCheck ) //엘레베이터 B가 더 가깝다.
				{
					pthread_t a_thread; //디스플레이 스레드 생성
					void *thread_result;

					pthread_t out_thread; //출력값 스레드 생성
					void *out_thread_result;

					cntFloor = shared_stuff->goalPos;//현재 엘레베이터 층수를 최종목적지로 변경
					pthread_create(&a_thread, NULL, display, (void *)shared_stuff); //디스플레이 스레드
					pthread_join(a_thread, &thread_result);
					shared_stuff->eleBpos = cntFloor;//엘레베이터 공유메모리 층수를 현재층수로 변경
					//Output 출력 스레드
					pthread_create(&out_thread, NULL, output_pthread, (void *)shared_stuff);
					pthread_join(out_thread, &out_thread_result);
				} else {//엘레베이터 A가 더 가깝다.
					shared_stuff->current = ELE_CHANGE_A;
				}
			}
		}
	}

	//뮤텍스 종료
	pthread_mutex_destroy(&work_mutex);
}
