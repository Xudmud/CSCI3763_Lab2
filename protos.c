socket()
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol)


//domain: specify a communication domain
AF_INET/AF_INET6

//type: specify the communication semantic
SOCK_STREAM
SOCK_DGRAM

//protocol: specify a particular protocol to be used with the socket
//Choosing protocol 0 will have the program chose a suitable protocol on its own

bind()
#include <sys/types.h>
#include <sys/socket.h>
int bind(int sockfd, struct sockaddr *my_addr, int addrlen)

#include <netinet/in.h>

struct sockaddr_in {
    short           sin_family
    unsigned short  sin_port
    struct in_addr  sin_addr
    char            sin_zero[8]
}

listen()
#include <sys/types.h>
#include <sys/socket.h>

int listen(int sockfd, int backlog)

//backlog: how many pending connections you can have before new ones get rejected
//Should be at least 3

connect()
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr * serv_addr, int addrlen)

accept()
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

send() and recv()
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t len, int flags) //Set flags to 0 for normal data
ssize_t recv(int sockfd, void *buf ,size_t len, int flags) //Specify an amount large enough to handle the data

close()
#include <unistd.h>
int close(int s)

gethostbyname()
#include <sys/socket.h>
#include <netdb.h>

struct hostent *gethostbyname(const char *name)

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(void)
{
    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;

}
