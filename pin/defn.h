#define PORT 5555
#define MAXUSR 10
#define NAMELEN 15     // Max name length
#define MAXMSG 1024    // Max message length

// UI stuff
#define TYPEBOX_HEIGHT 3

// Struct to hold user information
typedef struct user_info {
    int socket;
    int infd;
    int outfd;
    char* name;
} usr_t;
