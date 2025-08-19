// S O C K E T
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

// D E F A U L T
#include <stdio.h>
#include <string.h>
#include <time.h>

// M A C R O S
#define ISVALIDSOCKET(s) ((s) >=0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int // socket() returns negative on failure
#define GETSOCKETERRNO() (errno)

#define LINE "==============================\n"
