#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <pthread.h>
#include <string.h>

int answer, eastCounter=0, westCounter=0, northCounter=0, southCounter=0;
int nbrEnterFans, nbrLeaveFans;

pthread_mutex_t mutexCounter= PTHREAD_MUTEX_INITIALIZER;

void EnterFan()
{

    printf("please enter the number of entering fans\n");
    scanf("%d", &nbrEnterFans);

}

void LeaveFun()
{
    printf("please enter the number of leaving fans\n" );
    scanf("%d", &nbrLeaveFans);

}

void *EastTurnstile (void *arg)
{
    pthread_mutex_lock(&mutexCounter);
    {
         printf(" This is the East Turnstile\n");
        printf("please identify whether entering fans:  type 1 or leaving fans: type 0 \n");
        scanf("%d", &answer);

        if (answer == 1)
        {
            EnterFan();
            eastCounter += nbrEnterFans;

        }
        else
        {
            LeaveFun();
            if(eastCounter - nbrLeaveFans >0 )
            {
                eastCounter= eastCounter- nbrLeaveFans;
            }

        }
         printf("The current East Counter state is %d: \n\n", eastCounter);

        }
        pthread_mutex_unlock(&mutexCounter);
}
void *WestTurnstile (void *arg)
{
    pthread_mutex_lock(&mutexCounter);
    {
        printf(" This is the West Turnstile\n");
        printf("please identify whether entering fans:  type 1 or leaving fans: type 0 \n");
        scanf("%d", &answer);

        if (answer == 1)
        {
            EnterFan();
            westCounter += nbrEnterFans;

        }
        else
        {
            LeaveFun();
            if(westCounter - nbrLeaveFans >0 )
            {
                westCounter= westCounter- nbrLeaveFans;
            }

        }
         printf("The current West Counter state is %d: \n\n", westCounter);


        }
        pthread_mutex_unlock(&mutexCounter);

}
void *NorthTurnstile (void *arg)
{
    pthread_mutex_lock(&mutexCounter);
    {
      printf(" This is the North Turnstile\n");
        printf("please identify whether entering fans:  type 1 or leaving fans: type 0 \n");
        scanf("%d", &answer);

        if (answer == 1)
        {
            EnterFan();
            northCounter += nbrEnterFans;

        }
        else
        {
            LeaveFun();
            if(northCounter - nbrLeaveFans >0 )
            {
                northCounter= northCounter- nbrLeaveFans;
            }

        }
         printf("The current North Counter state is %d: \n\n", northCounter);


    }
    pthread_mutex_unlock(&mutexCounter);
}

void *SounthTurnstile (void *arg)
{
    pthread_mutex_lock(&mutexCounter);
    {
      printf(" This is the South Turnstile\n");
        printf("please identify whether entering fans:  type 1 or leaving fans: type 0 \n");
        scanf("%d", &answer);

        if (answer == 1)
        {
            EnterFan();
            southCounter += nbrEnterFans;

        }
        else
        {
            LeaveFun();
            if(southCounter - nbrLeaveFans >0 )
            {
                southCounter= southCounter- nbrLeaveFans;
            }

        }
         printf("The current South Counter state is %d: \n\n", southCounter);


    }
    pthread_mutex_unlock(&mutexCounter);

}
int main()
{
    pthread_t th1, th2, th3, th4;
    char sSum[50];

    WSADATA wsaData;

    if ( WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
        fprintf(stderr, "WSAStartup has failed.\n");
    else
    {
        SOCKET s;

        s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s == INVALID_SOCKET)
            printf("socket has failed.\n");
        else
        {
            SOCKADDR_IN server;

            server.sin_family       = AF_INET;
            server.sin_addr.s_addr  = inet_addr("127.0.0.1");
            server.sin_port         = htons(5050);
            memset(&server.sin_zero, '\0', sizeof(server.sin_zero));

            if (connect(s, (SOCKADDR *)&server, sizeof(server)) == SOCKET_ERROR)
                printf("connect has failed.\n");
            else
            {
                for (;;)
                {

                  pthread_create(&th1, NULL, EastTurnstile, NULL );
                  pthread_create(&th2, NULL, WestTurnstile, NULL );
                  pthread_create(&th3, NULL, NorthTurnstile,  NULL );
                  pthread_create(&th4, NULL, SounthTurnstile, NULL );



                  pthread_join(th1, NULL);
                  pthread_join(th2, NULL);
                  pthread_join(th3, NULL);
                  pthread_join(th4, NULL);

                  int sum = eastCounter + westCounter+ northCounter + southCounter;

                  printf("the actual sum of visitors is %d\n", sum);

                  //sprintf(sSum, "%f", sum);

                char buffer[100];
                int n;

                n = recv(s, buffer, sizeof(buffer) - 1, 0); /* read until max sizeof(buffer) - 1 octets */
                /* the last octect is booked for end string char, '\0'.             */

                if (n != SOCKET_ERROR)
                {


                    for ( int i= 0; i <=sum; i++)
                    {
                        buffer[n] = '\0';
                        printf("%s", buffer);
                        strcpy(buffer, "new fan\n");

                        send(s, buffer, (int)strlen(buffer), 0);
                    }


                    //strcpy(buffer, sSum);

                    //send(s, buffer, (int)strlen(buffer), 0);


                }

                }
              
            }

            closesocket(s);
        }

        WSACleanup();
    }

    return 0;
}
