#define PORT 5555
#define MAXUSR 2

// Struct to hold user information
typedef struct user_info {
    int socket;
    int infd;
    int outfd;
    char* name;
} usr_t;