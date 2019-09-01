#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <pthread.h>

//�����޸�
#define MAX_SIZE 5000	//�ִ� 5000���� ����
#define SLEEPNUM 1		//Sleep, �׽�Ʈ�� 0, �ùķ��̼� Ȯ�ν� 1

pthread_mutex_t work_mutex;	//���ؽ�

struct Human {		//input.txt���� �Է¹��� ����ü
	int cntPos;
	int goalPos;
};
struct Human human[MAX_SIZE];

int count = 0;	//�Է� ����

enum {			//���μ��� ������ ���� ���������Ͱ� ��Ҵ°�
	ELE_A = 0,
	ELE_B,
	ELE_CHANGE_A,
};


struct shared_use_st {		//���� �޸�
	int eleApos;			//A ���������� ��ġ
	int eleBpos;			//B ���������� ��ġ
	int cntPos;				//���� ��� ��ġ
	int goalPos;			//����� ������� ��ġ
	int current;			//���� ���μ��� ����������
	int cmpNum;				//���밪
	bool cmpCheck;			//A���������Ͱ� �������� B���������Ͱ� ��������
};


/*���������� ���÷���*/
void ElevaterDisplay(int eleApos, int eleBpos)
{
	system("clear");
	printf("\n\n");
	int i =0;
	for(i = 10 ; i >=1; i--) {

		printf("//%d\t\t", i);	//�������� �˷���
		if( i == eleApos )
			printf("��");
		else
			printf("��");

		printf("\t\t");

		if( i == eleBpos )
			printf("��");
		else
			printf("��");

		printf("\n");
	}
}


/*pthread_display ���������� ��ġ ������� ���*/
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

	pthread_mutex_lock(&work_mutex);	//����ȭ ����
	system("clear");


	if( current == ELE_CHANGE_A)
	{
		if(eleApos >= cntpos && cntpos <= goalPos)	//�������̰� �������ε� �ö󰥶�	
		{
			for(; eleApos > cntpos; eleApos--)	//�ϴ� ����������
			{

				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleApos <= goalPos; eleApos++)	//�׸��� �ö�
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos--;
		} 
		else if( eleApos >= cntpos && cntpos >= goalPos)//���� ���̰� �������ε� ��������
		{
			for(; eleApos >= goalPos; eleApos--)	//�ϴ� �����߳�����
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos++;
		}
		else if( eleApos <= cntpos && cntpos >= goalPos)//���� ���̰� �������ε� ��������
		{
			for(; eleApos < cntpos; eleApos++)	//�ϴ� �����ö��
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleApos >= goalPos; eleApos--) //�׸��� ������
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleApos++;
		}
		else if( eleApos <= cntpos && cntpos <= goalPos)//���� ���̰� �������ε� �ö󰥶�
		{
			for(; eleApos <= goalPos; eleApos++)	//�ϴ� �����߿ö��
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
		if(eleBpos >= cntpos && cntpos <= goalPos)	//�������̰� �������ε� �ö󰥶�	
		{
			for(; eleBpos > cntpos; eleBpos--)	//�ϴ� ����������
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleBpos <= goalPos; eleBpos++)	//�׸��� �ö�
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos--;
		} 
		else if( eleBpos >= cntpos && cntpos >= goalPos)//���� ���̰� �������ε� ��������
		{
			for(; eleBpos >= goalPos; eleBpos--)	//�ϴ� �����߳�����
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos++;
		}
		else if( eleBpos <= cntpos && cntpos >= goalPos)//���� ���̰� �������ε� ��������
		{
			for(; eleBpos < cntpos; eleBpos++)	//�ϴ� �����ö��
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			for(; eleBpos >= goalPos; eleBpos--) //�׸��� ������
			{
				ElevaterDisplay(eleApos, eleBpos);
				sleep(SLEEPNUM);
				system("clear");
			}
			eleBpos++;
		}
		else if( eleBpos <= cntpos && cntpos <= goalPos)//���� ���̰� �������ε� �ö󰥶�
		{
			for(; eleBpos <= goalPos; eleBpos++)	//�ϴ� �����߿ö��
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


/*pthread_input txt���� �Է� �޴� ������*/
void *input_pthread(void *arg)
{
	pthread_mutex_lock(&work_mutex);	//����ȭ
	FILE* fp;
	int i =0;

	fp = fopen("input.txt", "r");

	while(!feof(fp)) {
		fscanf(fp, "%d %d", &human[count].cntPos, &human[count].goalPos);	//human����ü�� �ϴ� �޴´�.
		if( count >= MAX_SIZE -1 )
			break;
		count++;

	}
	fclose(fp);
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);	//������ ����
}


/*pthread_output txt���� ��� �ϴ� ������*/
void *output_pthread(void *arg)
{
	struct shared_use_st *shared_stuff = (struct shared_use_st*)arg;

	pthread_mutex_lock(&work_mutex);
	FILE* fp;
	int i =0;

	fp = fopen("output.txt", "a");

	fprintf(fp, "A : %d, B : %d\n", shared_stuff->eleApos, shared_stuff->eleBpos);
	//A����������, B�� ���������� ��ġ�� Log�ۼ�

	fclose(fp);
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);
}


int main (void)
{
	pid_t	pid;		//���μ���
	int		cntPos;		//���� ��ġ
	int		goalPos;	//��ǥ ��ġ
	void	*shared_memory = (void *)0;	//����� �޸�
	struct	shared_use_st *shared_stuff;	//����� �޸� ����ü

	pthread_mutex_init(&work_mutex, NULL);	//���ؽ� �ʱ�ȭ
	int shmid;	//����� �޸� ���̵�
	srand((unsigned int)getpid()); 

	//������ �޸� ���� ���� 
	shmid = shmget((key_t)60075, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (shmid == -1) {
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}
	// �����޸𸮸� ���μ��� �ּҰ����� ����
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory == (void *)-1) {
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %X\n", (int)shared_memory); //�޸� ��ġ Ȯ��
	shared_stuff = (struct shared_use_st *)shared_memory;

	shared_stuff->current = ELE_A; //�ʱ� ����������A�� ����
	shared_stuff->eleApos = 1;		//����������A�� 1��
	shared_stuff->eleBpos = 10;		//���������� B�� 10��


	if ( (pid = fork()) < 0) {	//��ũ����
		printf("fork error\n"); 
		if (shmdt(shared_memory) == -1) {
			fprintf(stderr, "shmdt failed\n");
			exit(EXIT_FAILURE);
		}
		//���� �޸� ����
		if (shmctl(shmid, IPC_RMID, 0) == -1) {
			fprintf(stderr, "shmctl(IPC_RMID) failed\n");
			exit(EXIT_FAILURE);
		}
		exit(1);

	} else if (pid == 0) {	//���������� A
		/* Elevator One*/
		int cntFloor = 1;	//���� ���������� ����
		int i = 0;

		pthread_t inputTread;		//�Է½�����
		void *input_thread_result;

		pthread_create(&inputTread, NULL, input_pthread, NULL);//�Է½�����

		pthread_join(inputTread, &input_thread_result);


		for( ;; )
		{
			if( shared_stuff->current == ELE_A )	//���� ���μ����� ����������A���
			{
				shared_stuff->cntPos = human[i].cntPos;		//�Է¹����� �����޸𸮿� �������
				shared_stuff->goalPos = human[i].goalPos;
				i++;

				/* ���ᰪ -1 ������ �޸� �����ϰ� ����*/
				if( i > count-1 || shared_stuff->cntPos < 0 ||  shared_stuff->goalPos < 0)
				{
					//���� �޸𸮸� ����
					if (shmdt(shared_memory) == -1) {
						//fprintf(stderr, "shmdt failed\n");
						exit(EXIT_FAILURE);
					}
					//���� �޸� ����
					if (shmctl(shmid, IPC_RMID, 0) == -1) {
						//fprintf(stderr, "shmctl(IPC_RMID) failed\n");
						exit(EXIT_FAILURE);
					}
					exit(1);
				}


				/*printf("���� ��ġ�� �ö� ��ġ �Է� : ");
				scanf("%d %d", &shared_stuff->cntPos, &shared_stuff->goalPos);*/

				int cmpNum = 0;		//����������B�� ���Ұ�
				cmpNum = abs(cntFloor - shared_stuff->cntPos);	//���� ���

				shared_stuff->cmpNum = cmpNum;	//���������� A�� ���밪 �ϴ� ����
				shared_stuff->current = ELE_B;	//CPU����� ���������� B�� �ű�
			} else if( shared_stuff->current == ELE_CHANGE_A )	//���� ����������A�� ��ġ�� ������ ����
			{
				pthread_t a_thread;	//���÷��� ������ ����
				void *thread_result;

				pthread_t out_thread;	//��°� ������ ����
				void *out_thread_result;

				cntFloor = shared_stuff->goalPos;	//���� ���������� ������ ������������ ����
				pthread_create(&a_thread, NULL, display, (void *)shared_stuff);	//���÷��� ������
				pthread_join(a_thread, &thread_result);
				shared_stuff->eleApos = cntFloor;	//���������� �����޸� ������ ���������� ����
				//Output ��� ������
				pthread_create(&out_thread, NULL, output_pthread, (void *)shared_stuff);
				pthread_join(out_thread, &out_thread_result);

				//�ٽ� ����A�� CPU�ѱ�
				shared_stuff->current = ELE_A;
			}
		}
	} else {	//���������� B
		/* Elevator Two*/
		int cntFloor = 10;	//���� ���������� ����
		for( ;; )
		{
			if( shared_stuff->cntPos == -1)
			{
				//���� �޸𸮸� ����
				if (shmdt(shared_memory) == -1) {
					//fprintf(stderr, "shmdt failed\n");
					exit(EXIT_FAILURE);
				}
				//���� �޸� ����
				if (shmctl(shmid, IPC_RMID, 0) == -1) {
					//fprintf(stderr, "shmctl(IPC_RMID) failed\n");
					exit(EXIT_FAILURE);
				}
				exit(1);
			}

			if( shared_stuff->current == ELE_B )	//���������� B�� CPU����
			{
				int cmpNum = 0;		//�񱳰�
				bool cmpCheck;		//�񱳰� üũ

				cmpNum = abs(cntFloor - shared_stuff->cntPos);	//���밪 Ȯ��
				/*����A�� ����B ���ؼ� ���� ������ üũ*/
				cmpCheck = (cmpNum > shared_stuff->cmpNum) ? true : false;
				//���� �������� üũ
				shared_stuff->cmpCheck = cmpCheck;
				if( !cmpCheck ) //���������� B�� �� ������.
				{
					pthread_t a_thread; //���÷��� ������ ����
					void *thread_result;

					pthread_t out_thread; //��°� ������ ����
					void *out_thread_result;

					cntFloor = shared_stuff->goalPos;//���� ���������� ������ ������������ ����
					pthread_create(&a_thread, NULL, display, (void *)shared_stuff); //���÷��� ������
					pthread_join(a_thread, &thread_result);
					shared_stuff->eleBpos = cntFloor;//���������� �����޸� ������ ���������� ����
					//Output ��� ������
					pthread_create(&out_thread, NULL, output_pthread, (void *)shared_stuff);
					pthread_join(out_thread, &out_thread_result);
				} else {//���������� A�� �� ������.
					shared_stuff->current = ELE_CHANGE_A;
				}
			}
		}
	}

	//���ؽ� ����
	pthread_mutex_destroy(&work_mutex);
}
