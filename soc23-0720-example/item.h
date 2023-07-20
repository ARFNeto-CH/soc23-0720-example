#pragma once
#include <stdio.h>
typedef struct
{                       // struct data
    size_t id;          // book id
    char   title[50];   // book title
    char   author[50];  // book author
} Book;                 // a single book

typedef Book Item;  // here we have a container for books

int   print_book(Book*);
int   print_book_alt(Book*);  // one liner
Book* copy_book(Book*);
Book* delete_book(Book*);
