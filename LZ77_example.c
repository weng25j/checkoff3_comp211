#include <stdio.h>
#include <string.h>

#define WINDOW_SIZE 255
#define MAX_MATCH   255
#define MIN_MATCH   3

typedef struct {
    int is_match;           // 0 = literal, 1 = back-reference
    unsigned char literal;
    size_t distance;
    size_t length;
} Token;

static void print_text(const char *text, size_t length)
{
    putchar('[');
    printf("%.*s", (int)length, text);
    putchar(']');
}

static void print_tokens(const Token tokens[], size_t count)
{
    putchar('[');

    for (size_t i = 0; i < count; i++) {
        if (i > 0)
            printf(", ");

        if (tokens[i].is_match) {
            printf("(distance %zu, length %zu)",
                   tokens[i].distance, tokens[i].length);
        } else {
            printf("'%c'", tokens[i].literal);
        }
    }

    putchar(']');
}

int main(void)
{
    const char input[] = "ABCABCABCX";
    const size_t input_length = strlen(input);
    Token tokens[sizeof(input)];
    size_t token_count = 0;
    size_t pos = 0;
    size_t step = 1;

    while (pos < input_length) {
        size_t best_length = 0;
        size_t best_distance = 0;

        /* Search backward, but no farther than WINDOW_SIZE bytes. */
        size_t search_start =
            (pos > WINDOW_SIZE) ? pos - WINDOW_SIZE : 0;

        for (size_t candidate = search_start; candidate < pos; candidate++) {
            size_t distance = pos - candidate;
            size_t length = 0;

            /* Extend this match for as long as the bytes keep matching. */
            while (length < MAX_MATCH &&
                   pos + length < input_length &&
                   input[pos + length] ==
                       input[pos + length - distance]) {
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

            tokens[token_count++] = (Token) {
                .is_match = 1,
                .distance = best_distance,
                .length = best_length
            };

            printf("\nEmitted:        (distance %zu, length %zu)",
                   best_distance, best_length);
        } else {
            consumed = 1;

            printf("\nCurrent read:   ");
            print_text(input + pos, consumed);
            printf(" (literal)");

            tokens[token_count++] = (Token) {
                .is_match = 0,
                .literal = (unsigned char)input[pos]
            };

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
    return 0;
}