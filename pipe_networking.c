#include "pipe_networking.h"

char * server_handshake(int *from_client) {

    int mkf = mkfifo(WKP, 0644);
    if (mkf == -1) {
        printf("[server] mkfifo error\n");
        exit(-1);
    }

    *from_client = open(WKP, O_RDONLY);
    printf("[server] handshake: removed WKP\n");

    char *secret_name = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    read(*from_client, secret_name, HANDSHAKE_BUFFER_SIZE);
    printf("[server] handshake: received secret_name: %s\n", secret_name);

    return secret_name;

}

void subserver_handshake(int *from_client, int *to_client, char *secret_name) {

    *to_client = open(secret_name, O_WRONLY);

    char *acknowledgement = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));

    srand( time(NULL) );
    int r = rand() % BUFFER_SIZE;
    sprintf(acknowledgement, "%d", r);

    write(*to_client, acknowledgement, strlen(acknowledgement));

    char *response = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    read(*from_client, response, sizeof(HANDSHAKE_BUFFER_SIZE));
    printf("[server] handshake: received response: %s\n", response);
    int ra = atoi(response);
    if (ra != r+1) {
        printf("Wrong response\n");
        exit(-1);
    }

}

void client_handshake(int *from_server, int *to_server) {

    char *secret_name = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    sprintf(secret_name, "%d", getpid());

    int mkf = mkfifo(secret_name, 0644);
    if (mkf == -1) {
        printf("[client] mkfifo error\n");
        exit(-1);
    }
    *from_server = open(secret_name, O_RDONLY);

    *to_server = open(WKP, O_WRONLY);
    write(*to_server, secret_name, strlen(secret_name));

    char *acknowledgement = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    read(*from_server, acknowledgement, HANDSHAKE_BUFFER_SIZE);

    printf("[client] handshake: received from server: %s\n", acknowledgement);

    remove(secret_name);

    int r = atoi(acknowledgement) + 1;

    char *response = calloc(HANDSHAKE_BUFFER_SIZE, sizeof(char));
    sprintf(response, "%d", r);

    write(*to_server, response, strlen(response));

}