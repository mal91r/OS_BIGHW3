#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>
#include <time.h>
#include <string.h>

void DieWithError(char *errorMessage);  /* Error handling function */

void HandleTCPClient(int clntSocket)
{
    char variant[2];        /* Buffer for echo string */
    char answer[100];
    int mark;
    char markString[3];
    printf("Преподаватель предлагает студенту вытянуть билет\n");
    /* Receive message from client */
    if (recv(clntSocket, variant, 2, 0) < 0)
        DieWithError("recv() failed");

    sleep(0.5);
    printf("Студент сообщил преподавателю, что вытянул билет %s\n", variant);

    if (recv(clntSocket, answer, 100, 0) < 0)
        DieWithError("recv() failed");

    sleep(0.5);
    printf("Преподаватель получил от студента ответ на задание: \n%s\n", answer);

    srand(time(NULL));   // Initialization, should only be called once.
    mark = rand() % 10 + 1;

    sleep(0.5);
    printf("Преподаватель проверяет работу и ставит оценку: %d\n", mark);

    sprintf(markString, "%d", mark);

    if (send(clntSocket, markString, strlen(markString), 0) != strlen(markString))
        DieWithError("send() sent a different number of bytes than expected");

    close(clntSocket);    /* Close client socket */
}
