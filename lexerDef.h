// Define necessary constants and structures
#define MAX_LEXEME_SIZE 100
#define MAX_TOKEN_SIZE 15
#define MAX_BUFFER_SIZE 5

// Token information structure
typedef struct
{
    char type[MAX_TOKEN_SIZE];
    char lexeme[MAX_LEXEME_SIZE];
    int lineNumber;
    bool end;
} TokenInfo;

// Define structure for twin buffer
typedef struct
{
    char buffer0[MAX_BUFFER_SIZE];
    char buffer1[MAX_BUFFER_SIZE];
    int currentBuffer;   // Indicates the currently active buffer (0 or 1)
    int currentPosition; // Current position in the currently active buffer
    int lineNumber;
    FILE *fp;    // File pointer for input stream
    bool dblret; // double retraction flag
} twinBuffer;
