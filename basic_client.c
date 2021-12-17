#include "pipe_networking.h"

int main() {

    int from_server = 0, to_server = 0;

    client_handshake(&from_server, &to_server);

    char *buffer = calloc(BUFFER_SIZE, sizeof(char));

    printf("Enter input, maxlength %d: ", BUFFER_SIZE);
    while(read(STDIN_FILENO, buffer, BUFFER_SIZE)) {

        write(to_server, buffer, strlen(buffer));
        read(from_server, buffer, strlen(buffer));
        printf("Uppercase: %s\n", buffer);

        printf("Enter input, maxlength %d: ", BUFFER_SIZE);
    }

    return 0;

}