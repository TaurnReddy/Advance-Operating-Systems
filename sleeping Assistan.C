#include <pthread.h>		
#include <time.h>			
#include <unistd.h>			
#include <semaphore.h>		
#include <stdlib.h>			
#include <stdio.h>			

pthread_t *Students;		
pthread_t TA;				
int chair_count = 0;
int Present_Index = 0;

//Declaration of Semaphores and Mutex Lock.
sem_t Teacher_Sleeping;
sem_t Studentsem;
sem_t chairsem[3];
pthread_mutex_t ChairAccess;

//Declared Functions
void *TA_Ready();
void *Student_Ready(void *threadID);

int main(int argc, char* argv[])
{
	int number_of_students;		
	int id;
	srand(time(NULL));

	//Initializing Mutex Lock and Semaphores.
	sem_init(&Teacher_Sleeping, 0, 0);
	sem_init(&Studentsem, 0, 0);
	for(id = 0; id < 3; ++id)			
		sem_init(&chairsem[id], 0, 0);

	pthread_mutex_init(&ChairAccess, NULL);
	
	if(argc<2)
	{
		printf("Number of Students not specified. Using default (5) students.\n");
		number_of_students = 5;
	}
	else
	{
		printf("Number of Students specified. Creating %d threads.\n", number_of_students);
		number_of_students = atoi(argv[1]);
	}
		
	
	Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);


	pthread_create(&TA, NULL, TA_Activity, NULL);	
	for(id = 0; id < number_of_students; id++)
		pthread_create(&Students[id], NULL, Student_Activity,(void*) (long)id);

	
	pthread_join(TA, NULL);
	for(id = 0; id < number_of_students; id++)
		pthread_join(Students[id], NULL);

	free(Students); 
	return 0;
}

void *TA_Ready()
{
	while(1)
	{
		sem_wait(&Teacher_Sleeping);		
		printf("TA has been awakened by a student.\n");

		while(1)
		{
			
			pthread_mutex_lock(&ChairAccess);
			if(chair_count == 0) 
			{
				
				pthread_mutex_unlock(&ChairAccess);
				break;
			}
			
			sem_post(&chairsem[Present_Index]);
			chair_count--;
			printf("Student left his/her chair. Remaining Chairs %d\n", 3 - chair_count);
			Present_Index = (Present_Index + 1) % 3;
			pthread_mutex_unlock(&ChairAccess);
			
			printf("\t TA is currently helping the student.\n");
			sleep(5);
			sem_post(&Studentsem);
			usleep(1000);
		}
	}
}

void *Student_Ready(void *threadID) 
{
	int ProgrammingTime;

	while(1)
	{
		printf("Student %ld is doing programming assignment.\n", (long)threadID);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);		

		printf("Student %ld needs help from the TA\n", (long)threadID);
		
		pthread_mutex_lock(&ChairAccess);
		int count = chair_count;
		pthread_mutex_unlock(&ChairAccess);

		if(count < 3)		
		{
			if(count == 0)		
				sem_post(&Teacher_Sleeping);
			else
				printf("Student %ld sat on a chair waiting for the TA to finish. \n", (long)threadID);

			
			pthread_mutex_lock(&ChairAccess);
			int index = (Present_Index + chair_count) % 3;
			chair_count++;
			printf("Student sat on chair.Chairs Remaining: %d\n", 3 - chair_count);
			pthread_mutex_unlock(&ChairAccess);
			
			sem_wait(&chairsem[index]);		
			printf("\t Student %ld is getting help from the TA. \n", (long)threadID);
			sem_wait(&Studentsem);		
			printf("Student %ld left TA room.\n",(long)threadID);
		}
		else 
			printf("Student %ld will return at another time. \n", (long)threadID);
			
	}
}
