#include <stdio.h>
#include <string.h>

#define INPUT_CAPACITY 1024
#define WINDOW_SIZE 255
#define MAX_MATCH 255
#define MIN_MATCH 3

typedef struct {
    int is_match;  // 0 = literal, 1 = back-reference
    unsigned char literal;
    size_t distance;
    size_t length;
} Token;

static void print_text(const char* text, size_t length) {
    putchar('[');
    printf("%.*s", (int)length, text);
    putchar(']');
}

static void print_tokens(const Token tokens[], size_t count) {
    putchar('[');

    for (size_t i = 0; i < count; i++) {
        if (i > 0) printf(", ");

        if (tokens[i].is_match) {
            printf("(distance %zu, length %zu)", tokens[i].distance,
                   tokens[i].length);
        } else {
            printf("'%c'", tokens[i].literal);
        }
    }

    putchar(']');
}

int main(void) {
    char input[INPUT_CAPACITY];

    printf("Enter text to compress (one line, up to %d characters):\n",
           INPUT_CAPACITY - 1);

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Could not read input.\n");
        return 1;
    }

    /* Remove the newline if the user pressed Enter. */
    size_t input_length = strcspn(input, "\n");

    /*
     * If there was no newline, check whether the line exceeded the buffer.
     * A newline may be absent because input ended at EOF, which is okay.
     */
    if (input[input_length] == '\0' && !feof(stdin)) {
        int next_char = getchar();

        if (next_char != '\n' && next_char != EOF) {
            fprintf(stderr,
                    "Input is too long; please keep it under %d characters.\n",
                    INPUT_CAPACITY);
            return 1;
        }
    }

    input[input_length] = '\0';

    if (input_length == 0) {
        printf("You entered an empty line; there is no data to compress.\n");
        return 0;
    }

    /*
     * At most one token is emitted per input character, so this is enough.
     */
    Token tokens[INPUT_CAPACITY];
    size_t token_count = 0;
    size_t pos = 0;
    size_t step = 1;

    printf("\nOriginal data: ");
    print_text(input, input_length);
    putchar('\n');

    while (pos < input_length) {
        size_t best_length = 0;
        size_t best_distance = 0;

        size_t search_start = (pos > WINDOW_SIZE) ? pos - WINDOW_SIZE : 0;

        /* Search earlier input positions for the longest match. */
        for (size_t candidate = search_start; candidate < pos; candidate++) {
            size_t distance = pos - candidate;
            size_t length = 0;

            while (length < MAX_MATCH && pos + length < input_length &&
                   input[pos + length] == input[pos + length - distance]) {
                length++;
            }

            if (length > best_length) {
                best_length = length;
                best_distance = distance;
            }
        }

        size_t consumed;

        printf("\nStep %zu\n", step++);
        printf("History before: ");
        print_text(input, pos);
        printf("\nLookahead:      ");
        print_text(input + pos, input_length - pos);

        if (best_length >= MIN_MATCH) {
            consumed = best_length;

            printf("\nCurrent read:   ");
            print_text(input + pos, consumed);
            printf(" (match)");

            tokens[token_count++] = (Token){.is_match = 1,
                                            .distance = best_distance,
                                            .length = best_length};

            printf("\nEmitted:        (distance %zu, length %zu)",
                   best_distance, best_length);
        } else {
            consumed = 1;

            printf("\nCurrent read:   ");
            print_text(input + pos, consumed);
            printf(" (literal)");

            tokens[token_count++] =
                (Token){.is_match = 0, .literal = (unsigned char)input[pos]};

            printf("\nEmitted:        '%c'", input[pos]);
        }

        pos += consumed;

        printf("\nHistory after:  ");
        print_text(input, pos);
        printf("\nCompressed data: ");
        print_tokens(tokens, token_count);
        putchar('\n');
    }

    printf("\nFinal token count: %zu\n", token_count);
    printf("Final tokens: ");
    print_tokens(tokens, token_count);
    putchar('\n');

    return 0;
}
