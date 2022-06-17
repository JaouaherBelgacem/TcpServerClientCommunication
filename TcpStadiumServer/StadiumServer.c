#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

int Commoncounter=0, sum;
int main()
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
        printf("WSAStartup has failed.\n");
    else
    {
        SOCKET s_server;

        s_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (s_server == INVALID_SOCKET)
            printf("Socket function has failed.\n");
        else
        {
            SOCKADDR_IN server;

            server.sin_family       = AF_INET;
            server.sin_addr.s_addr  = htonl(INADDR_ANY);
            server.sin_port         = htons(5050);
            memset(&server.sin_zero, '\0', sizeof(server.sin_zero));

            if (bind(s_server, (SOCKADDR *)&server, sizeof(server)) == SOCKET_ERROR)
                printf("bind has failed.\n");
            else
            {
                if (listen(s_server, 0) == SOCKET_ERROR) /* listen : start listening */
                    printf("listen has failed.\n");
                else
                {
                    SOCKET s_client;
                    SOCKADDR_IN client;
                    int csize = sizeof(client);

                    s_client = accept(s_server, (SOCKADDR *)&client, &csize);

                    if (s_client == INVALID_SOCKET)
                        printf("accept has failed.\n");
                    else
                    {
                        char buffer[100];
                        int n;

                        const char allowFun[20]= "new fan\n";

                        printf("The client is connected !\n", inet_ntoa(client.sin_addr));

                        strcpy(buffer, "Hi from server\n");
                        send(s_client, buffer, (int)strlen(buffer), 0);

                        //for(;;)
                       // {
                           // strcpy(buffer, "Hi from server\n");
                           // send(s_client, buffer, (int)strlen(buffer), 0);

                            n = recv(s_client, buffer, sizeof(buffer) - 1, 0);
                            while(Commoncounter < 3000)
                            {
                                do
                                {
                                    Commoncounter +=1;

                                }while(strcmp(allowFun, buffer) == 0);

                                 printf("%the current counter status is %d\n", Commoncounter);


                            }
                                if (n != SOCKET_ERROR)
                                {
                                    buffer[n] = '\0';
                                    printf("%s", buffer);
                                }
                         //   }

                            //printf("Number is visitors inside the stadium is%d:", Commoncounter);
                            usleep(2000000);

                        //}

                         printf("Stadium is full! no more entry\n");


                        closesocket(s_client);
                    }
                }
            }

            closesocket(s_server);
        }

        WSACleanup();
    }

    return 0;
}
