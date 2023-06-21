#define PORT 5555
#define MAXUSR 10
#define NAMELEN 15     // Max name length
#define MAXMSG 1024    // Max message length

// Struct to hold user information
typedef struct user_info {
    int socket;
    int infd;
    int outfd;
    char* name;
} usr_t;
