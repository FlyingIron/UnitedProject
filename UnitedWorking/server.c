#include "serv_intf.h"



int main(int argc, char *argv[])
{

    char path[255];
    bzero(path, sizeof(path));
    sprintf(path, "./tosend");
    printf("\n");
    printf("TCP transmitter (server)\n");

    struct sock_attr_sv sock_sv, sock_names;
    sock_sv = sock_creator_sv(&sock_sv);
    sock_names = sock_creator_names(&sock_names);

    if (bind(sock_sv.sock_d, (struct sockaddr *)&sock_sv.serv_addr,
             sizeof(sock_sv.serv_addr)) < 0)
    {
        error("An error occurred while binding a socket!");
    }

    if (bind(sock_names.sock_d, (struct sockaddr *)&sock_names.serv_addr,
             sizeof(sock_names.serv_addr)) < 0)
    {
        error("An error occurred while binding NAMES socket!");
    }
    
    listen(sock_names.sock_d, 2);
    sock_sv.clilen = sizeof(sock_sv.cli_addr);

    int names_sock = accept(sock_names.sock_d, (struct sockaddr *)&sock_names.cli_addr,
                               &sock_names.clilen);
    send_names(names_sock, path);
    close(names_sock);

    listen(sock_sv.sock_d, 5);

    while (1)
    {
        int newsockfd = accept(sock_sv.sock_d, (struct sockaddr *)&sock_sv.cli_addr,
                               &sock_sv.clilen);
        if (newsockfd < 0)
        {
            error("An error occurred while accepting connection!");
        }

        struct hostent *hst;
        hst = gethostbyaddr((char *)&sock_sv.cli_addr.sin_addr, 4, AF_INET);

        printf("Added %s [%s] new connection\n", (hst) ? hst->h_name : "Unknown host",
               (char *)inet_ntoa(sock_sv.cli_addr.sin_addr));

        int pid = fork();

        if (pid < 0)
        {
            error("An error occurred while creating a new parallel connection!");
        }
        if (pid == 0)
        {
            close(sock_sv.sock_d);
            send_file(newsockfd);
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    }

    close(sock_sv.sock_d);
    return 0;
}