#include "item.h"

#include <stdlib.h>
#include <string.h>

void* copy_book(void* book)
{
    if (book == NULL) return NULL;
    Book* one     = (Book*)book;
    Book* other   = (void*)malloc(sizeof(Book));
    other->id     = one->id;
    other->author = malloc(1 + sizeof(one->author));
    strcpy(other->author, one->author);
    other->title = malloc(1 + sizeof(one->title));
    strcpy(other->title, one->title);
    other->n_pages = one->n_pages;
    return (void*)other;
};

void* create_book(void)
{
    static size_t id         = 1000;
    char          author[40] = {0};
    char          title[40]  = {0};
    Book*         one        = (void*)malloc(sizeof(Book));

    sprintf(&author[0], "Author %llu", id);
    one->author = malloc(1 + strlen(author));
    strcpy(one->author, author);

    sprintf(&title[0], "Title %llu", id);
    one->title = malloc(1 + strlen(title));
    strcpy(one->title, title);

    one->id      = id++;
    one->n_pages = 200 + rand() % 155;
    return (void*) one;
};

void* delete_book(void* book)
{
    if (book == NULL) return NULL;
    Book* one = (Book*)book;
    free(one->author);
    free(one->title);
    free(one);  // simple, void does not allocate
    return NULL;
}

int print_book(void* book)
{
    if (book == NULL) return 0;
    Book* one = (Book*)book;
    printf(
        "\
    Book ID       : %llu\n\
    title         : %s [%llu pages]\n\
    author        : %s\n\
\n",
        one->id, one->title, one->n_pages, one->author);
    return 0;
};

int print_book_alt(void* book)
{
    if (book == NULL) return 0;
    Book* one = (Book*)book;
    printf(
        "%8llu, \"%s [%llu pages]\", [%s]\n", one->id,
        one->title, one->n_pages, one->author);
    return 0;
}
