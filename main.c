#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Searches a file for any lines of text that contain the given query.
 * Prints out all lines of text to the stdout (eventually customizable)
 *
 * 1) Parse the command line arguments passed
 * 2) Attempt to open the file (check for errors)
 * 3) Search the file for the given query
 *
 */

int f_search(FILE *fp, char *query);
int read_line(FILE *fp, char *buf, size_t *buf_size);
bool search_line(char *buf, size_t buf_size);

int main(int argc, char** argv) {
    // Verify correct number of arguments were passed
    if (argc < 3)
    {
        fprintf(stderr, "usage: <FILE> <QUERY>\n");
        return EXIT_FAILURE;
    }
    // Assign arguments
    char* fname = argv[0];
    char* query = argv[1];

    // Open file
    FILE *fp;
    if (!(fp = fopen(fname, "r")))
    {
        fprintf(stderr, "invalid file: %s\n", fname);
        return EXIT_FAILURE;
    }

    if (f_search(fp, query))
    {
        return EXIT_FAILURE;
    }

    return 0;
}


int f_search(FILE *fp, char *query)
{
    // for keeping track of number of characters read
    int num_char_read = 0;

    // for reading in the next line
    size_t buf_size = 32;
    char *line = (char *) malloc(buf_size);

    // Keep reading in characters while there are still more lines to process
    while ((num_char_read = read_line(fp, line, &buf_size)) > 0)
    {
        // search each line for occurrences of query
        if (search_line(line, buf_size))
        {
            puts(line);
        }

        free(line);
        buf_size = 32;

        char *next_line = (char*) realloc(line, buf_size);
        if (!next_line)
        {
            fprintf(stderr, "%s:%s:%d error reallocating line buffer\n", __FILE__, __FUNCTION__, __LINE__);
            return -1;
        }

        line = next_line;
    }

    if (num_char_read < 0)
        return num_char_read;

    return 0;
}

int read_line(FILE *fp, char *buf, size_t *buf_size)
{
    int cur_char = 0;
    size_t i = 0;

    while ((cur_char = fgetc(fp)) != EOF)
    {
        if (i == *buf_size)
        {
            *buf_size *= 2;
            char *neo_buf = realloc(buf, *buf_size);
            if (neo_buf)
                buf = neo_buf;
            else
            {
                fprintf(stderr, "%s:%s:%d error reallocating buffer\n", __FILE__, __FUNCTION__, __LINE__,  );
                return -1;
            }
        }

        if (cur_char != '\n')
            buf[i++] = (char) cur_char;
        else
            break;
    }

    if (ferror(fp))
    {
        fprintf(stderr, "%s:%s:%d error reading from file into buffer\n", __FILE__, __FUNCTION__, __LINE__);
        clearerr(fp);
        return -1;
    }

    // Reallocate buffer
    char *neo_buf = realloc(buf, i + 1);
    if (neo_buf)
        buf = neo_buf;
    else
    {
        fprintf(stderr, "%s:%s:%d error reallocating buffer\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }

    buf[i] = '\0';
    //TODO: debug statement
    printf("%s\n", buf);
    return 0;
}

bool search_line(char *buf, size_t buf_size)
{

}
