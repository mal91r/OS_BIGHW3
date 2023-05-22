#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <time.h>

void DieWithError(char *errorMessage);  /* Error handling function */

int main(int argc, char *argv[]) {
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    int variant;
    char variantString[2];
    char answer[100];
    char mark[3];

    if (argc != 3)    /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server IP>\n",
                argv[0]);
        exit(1);
    }

    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    echoServPort = atoi(argv[2]);

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    printf("Студент вошел в аудиторию\n");

    sleep(0.5);
    srand(time(NULL));   // Initialization, should only be called once.
    variant = rand() % 30 + 1;
    printf("Студент вытянул билет %d\n", variant);

    sprintf(variantString, "%d", variant);

    /* Send the string to the server */
    if (send(sock, variantString, strlen(variantString), 0) != strlen(variantString))
        DieWithError("send() sent a different number of bytes than expected");

    printf("Студент приступил к решению билета\n");
    printf("Введите ответ студента: ");
    fgets(answer, 100, stdin);
    if ((strlen(answer) > 0) && (answer[strlen (answer) - 1] == '\n'))
        answer[strlen (answer) - 1] = '\0';
    sleep(0.5);
    printf("Студент передал ответ преподавателю\n");

    /* Send the string to the server */
    if (send(sock, answer, strlen(answer), 0) != strlen(answer))
        DieWithError("send() sent a different number of bytes than expected");
    sleep(0.5);
    printf("Студент ждёт, пока преподаватель поставит оценку\n");

    if (recv(sock, mark, 3, 0) < 0)
        DieWithError("recv() failed");
    sleep(0.5);
    printf("Студент узнает, что преподаватель поставил ему оценку %s\n", mark);
    sleep(0.5);
    printf("Студент покидает аудиторию!\n");
    printf("\n");    /* Print a final linefeed */
    sleep(1);
    close(sock);
    exit(0);
}
