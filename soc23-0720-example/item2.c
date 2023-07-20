#include "item2.h"

#include <stdlib.h>
#include <string.h>

Book* copy_book(Book* book)
{
    if (book == NULL) return NULL;
    Book* one   = (Book*)malloc(sizeof(Book));
    one->id     = book->id;
    one->author = malloc(1 + sizeof(book->author));
    strcpy(one->author, book->author);
    one->title = malloc(1 + sizeof(book->title));
    strcpy(one->title, book->title);
    one->n_pages = book->n_pages;
    return one;
}
Book* create_book(void)
{
    static size_t id         = 1000;
    char          author[40] = {0};
    char          title[40]  = {0};
    Book*         one        = (Book*)malloc(sizeof(Book));
    sprintf(&author[0], "Author %llu", id);
    one->author = malloc(1 + strlen(author));
    strcpy(one->author, author);
    sprintf(&title[0], "Title %llu", id);
    one->title = malloc(1 + strlen(title));
    strcpy(one->title, title);
    one->id      = id++;
    one->n_pages = 200 + rand() % 155;
    return one;
};

Book* delete_book(Book* book)
{
    if (book == NULL) return NULL;
    free(book->author);
    free(book->title);
    free(book);  // simple, Book does not allocate
    return NULL;
}

int print_book(Book* book)
{
    if (book == NULL) return 0;
    printf(
        "\
    book_id       : %llu\n\
    title         : %s [%llu pages]\n\
    author        : %s\n\
\n",
        book->id, book->title, book->n_pages, book->author);
    return 0;
};

int print_book_alt(Book* book)
{
    if (book == NULL) return 0;
    printf(
        "%8llu, \"%s [%llu pages]\", [%s]\n", book->id,
        book->title, book->n_pages, book->author);
    return 0;
}
