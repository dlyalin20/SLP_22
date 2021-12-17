#include "pipe_networking.h"

int main() {

    int from_client = 0, to_client = 0;

    char *secret_name = server_handshake(&from_client);

    int f = fork();

    if (f) { // parents
        remove(WKP);
        close(from_client);
        main();
    }
    else { // child
        subserver_handshake(&from_client, &to_client, secret_name);

        char *buffer = calloc(BUFFER_SIZE, sizeof(char));

        while(read(from_client, buffer, BUFFER_SIZE)) {

            int i;
            for (i = 0; i < strlen(buffer); i++) {
                if (isalpha(buffer[i])) {
                    buffer[i] = toupper(buffer[i]);
                }
            }   

            write(to_client, buffer, strlen(buffer));

        }

        return 0;

    }

}