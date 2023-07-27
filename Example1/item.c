#include <stdlib.h>
#include "item.h"

Book* copy_book(Book* book)
{
    if (book == NULL) return NULL;
    Book* one = (Book*)malloc(sizeof(Book));
    *one      = *book;  // plain copy
    return one;
};

Book* delete_book(Book* book)
{
    if (book == NULL) return NULL;
    free(book);  // simple, Book does not allocate
    return NULL;
}

int print_book(Book* book)
{
    if (book == NULL) return 0;
    printf(
        "\
    book_id       : %llu\n\
    title         : %s\n\
    author        : %s\n\
\n",
        book->id, book->title, book->author);
    return 0;
};

int print_book_alt(Book* book)
{
    if (book == NULL) return 0;
    printf(
        "%8llu, \"%s\", [%s]\n", book->id, book->title,
        book->author);
    return 0;
}
