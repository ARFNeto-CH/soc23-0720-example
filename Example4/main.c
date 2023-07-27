#include <stdio.h>
#include <stdlib.h>

#include "container.h"
#include "item.h"

int main(void)
{
    const int  test_size = 6;
    Container* books =
        ctn_create(30, copy_book, delete_book, print_book);
    for (size_t id = 1; id <= test_size; id += 1)
    {
        Book* newb = create_book();
        ctn_insert(newb, books);
        delete_book(newb);
    };
    ctn_show(books);
    printf(
        "\n\t***** now print again, using new layout\n\n");
    books->show = print_book_alt;
    ctn_show(books);             // now using _alt
    books = ctn_destroy(books);  // delete all
    ctn_show(books);             // now empty
    return 0;
}
