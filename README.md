*This project has been created as part of the 42 curriculum by dgankhuy.*

# Description

## Get Next Line

`get_next_line` is a 42 project written in C. Its goal is to implement a function that reads from a file descriptor and returns one line per call, instead of requiring the whole file to be read at once.

The main function is:

```c
char *get_next_line(int fd);
```

Each call returns a newly allocated string containing the next line. When a newline is present, the returned string includes `\n`. If the last line reaches EOF without a newline, its remaining characters are returned. Once there is no content left to return, the function returns `NULL`.

The key idea is to keep unread characters in a persistent static buffer between calls. The function reads chunks of `BUFFER_SIZE` until it finds a newline or reaches EOF, then returns the first line and saves the rest for the next call.

## Files

- `get_next_line.c` — the main function and the functions that read, append, split, and handle EOF.
- `get_next_line_utils.c` — helper functions for finding a newline, measuring strings, and creating the returned line and remaining buffer.
- `get_next_line.h` — system includes, the default `BUFFER_SIZE`, and function prototypes.

# Instructions

## Requirements

- A C compiler such as `cc` or `clang`.
- A standart POSIX/C library is used for `read()`, `free()`, `malloc()`.
- `read()` from `<unistd.h>`.
- `malloc()`, `free()` from `<stdlib.h>`.

## Compilation

Compile the source files together with your own test file. For example, if your test file is named `main.c`:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
  get_next_line.c get_next_line_utils.c main.c -o gnl_test
```

Run the program with:

```sh
./gnl_test
```

`BUFFER_SIZE` controls how many bytes are requested from `read()` at a time. If it is not defined during compilation, `get_next_line.h` sets it to `1`. You can use a different positive value to test how the function behaves with different chunk sizes.

## Example usage

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

Include `<unistd.h>` for `close()` if it is not already declared by the headers used in your test file. The caller owns each returned string and must release it with `free()`.

## Return behaviour

- Returns an allocated line, including `\n` when a newline is found.
- Returns the last remaining characters when EOF is reached without a final newline.
- Returns `NULL` when there is no more data or when a read/allocation error occurs.
- The caller must `free()` each returned line.

# Functions

## Main functions — `get_next_line.c`

### `get_next_line(int fd)`

This is the entry point called by the user. It keeps a `static char *extra`, so any characters that have been read but not yet returned survive between calls. It checks for invalid arguments, searches the saved data for a newline, and keeps reading and appending chunks until it finds one. It then delegates line extraction to `sep_extra()`. If a read returns an empty string at EOF, it delegates to `endfile_check()`.

### `sep_extra(char **extra_p)`

This function splits the saved string into two pieces: the line to return and the characters after the newline. It calls `build_new_line()` to create the returned line and `build_new_extra()` to create the remainder. It frees the old buffer and updates `*extra_p` to point to the remainder. If either allocation fails, it frees the temporary allocations and clears the saved buffer.

### `reading_ft(int fd, size_t buffer_size)`

This function allocates enough space for `buffer_size` bytes plus the terminating null character, then calls `read()`. It null-terminates the data read and returns the allocated buffer. If allocation or `read()` fails, it frees what it allocated and returns `NULL`.

### `join_str(char *extra, char *buf)`

This function creates a new string by concatenating the previously saved characters (`extra`) with the newly read chunk (`buf`). It frees the old `extra` after copying it into the new allocation. The caller frees `buf` after joining. If allocation fails, `join_str()` frees `extra` and returns `NULL`.

### `endfile_check(char *buf, char **extra_p)`

This function handles the EOF case, which is detected when `read()` returns zero bytes and `reading_ft()` therefore returns an empty string. It frees that empty buffer. If `extra` still contains text, it returns that text as the final line and clears the saved pointer. If no text remains, it returns `NULL`.

## Helper functions — `get_next_line_utils.c`

### `find_new_line(char *s)`

Searches the string for the first `\n` and returns its index. It returns `-1` if the string is `NULL` or contains no newline. `get_next_line()` uses this result to decide whether it needs to read more data.

### `build_new_line(char *s)`

Copies the characters from the beginning of `s` through the first newline into a newly allocated string. The newline is included, and the returned string is null-terminated. It is called by `sep_extra()` only after a newline has been found.

### `len_str(char *s)`

Counts the characters in a null-terminated string. It returns `0` for a `NULL` pointer. It is used when allocating and combining strings.

### `build_new_extra(char *s)`

Copies the characters after the first newline into a new string. These are the characters that belong to later lines and must be kept for the next call. If there are no characters after the newline, it creates an empty string.

# Algorithm

This implementation uses a **read, accumulate, and split** algorithm with a persistent remainder.

## 1. Keep unread data between calls

Inside `get_next_line()`, `static char *extra` stores the characters that have been read but not yet returned. Because it is `static`, it remains available after the function returns. On a later call, the function checks this saved data before reading again.

## 2. Read in chunks until a newline is found

If `extra` does not contain `\n`, `reading_ft()` requests up to `BUFFER_SIZE` bytes from the file descriptor. `join_str()` appends the new chunk to the saved text. This repeats until a newline is found or the input reaches EOF.

A `read()` call does not necessarily stop at a line boundary. A chunk can contain part of a line, one whole line, or several lines. That is why each chunk must be accumulated and checked instead of being returned directly.

## 3. Return one line and save the rest

When `find_new_line()` finds a newline, `sep_extra()` calls `build_new_line()` and `build_new_extra()`. The first function creates the line returned to the caller; the second saves everything after that newline for the next call.

For example, if the saved content is:

```text
first line\nsecond line\n
```

The current call returns `first line\n`, while `second line\n` remains saved for the next call.

## 4. Handle EOF

If `read()` returns zero bytes, `reading_ft()` returns an empty string. `endfile_check()` then returns any remaining characters as the final line, even if that line has no trailing newline. If the saved buffer is empty, there is no line left and the function returns `NULL`.

## 5. Why this algorithm?

The important trick is **preserving the part of a read that comes after the first newline**. The function can return exactly one line per call without losing characters that belong to later lines. The static buffer also means it does not need to reread or reload the entire file for each line.

This approach supports lines longer than `BUFFER_SIZE`, because it keeps appending chunks until it finds a newline or EOF. It is simple and incremental, though repeatedly concatenating strings can copy the accumulated data more than once for very long lines.

The current implementation has one static remainder pointer, so it is intended for sequential reading from one file descriptor at a time. It does not keep separate buffers for interleaved reads from multiple file descriptors.

# Resources

- [42 School](https://42.fr/) — information about the 42 curriculum.
- [`read(2)` manual](https://man7.org/linux/man-pages/man2/read.2.html) — the system call used to read bytes from a file descriptor.
- [`malloc(3)` manual](https://man7.org/linux/man-pages/man3/malloc.3.html) — dynamic memory allocation.
- [`free(3)` manual](https://man7.org/linux/man-pages/man3/free.3.html) — releasing dynamically allocated memory.

## AI usage

AI was used to draft this README. The documentation describes the implementation in the supplied code; the project author remains responsible for understanding, checking, and testing the submitted implementation.
