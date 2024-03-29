#define PORT 5555
#define MAXUSR 10
#define NAMELEN 5     // Max name length
#define MAXMSG 1024    // Max message length
#define MSGGAP 0   // Gap between messages

// UI stuff
#define TYPEBOX_HEIGHT 2

// Extern user name
extern char* name;
extern int namelen;
extern pthread_mutex_t mutex;

// Shared funcs
void* start_interface(void* argv);
void write_messages();
void add_remote(char* buf, int len);
void send_message(char* buf, int size);

// Struct to hold user information
typedef struct user_info {
    int socket;
    int infd;
    int outfd;
    char* name;
} usr_t;
