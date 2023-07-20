In `struct book` you have `prtFunc` declared as 
```C
    int (*prtFunc)(struct book);  // eventually, addr of print function
```

And in `main` you have error on    

```C
    arBookList[0].prtFunc = printBook;
```

But `printBook` is
  
```C
void printBook(struct book arBookList[], int id)
```

So the acual parameters does not match the declaration.

### another issue ### 

A bit *off-topic* but if the function is the same for all books its address should not be replicated in every `book` in the array.

### Example: using a collection instead of an array ###    
```C
typedef struct
{                     // struct data
    int  book_id;     // book id
    char title[50];   // book title
    char author[50];  // book author
} Book; // a single book

typedef struct
{
    size_t limit;  // capacity
    size_t size;   // actual size
    Book   book_list[10];
    int (*print_book)(Book*);
} Books; // a collection
``` 

This way we have a **collection** `Books` that has a `size` and `limit`. And also a pointer to a function that prints a single book. And the books are inside the `struct` so you can use it as a whole.

Your test collection can be declared in a modern way as 

```C
    Books coll = {
        .limit = 10,
        .size  = 2,
        .book[0] =
            {.id     = 42,
             .title  = "This Little Piggy",
             .author = "Bad Wolf"},
        .book[1] =
            {.id     = 4242,
             .title  = "Mary Had a Lamb",
             .author = "Snow Fleece"},
        .print_book =
            printBook  // this function prints a book
    };
```

This is easier to read and test.

### a version of `print_book` ###    
```C
void print_book(Book* book)
{
    printf(
        "\
    book_id       : %d\n\
    title         : %s\n\
    author        : %s\n\
\n",
        book->id, book->title, book->author);
};
``` 

A single `printf` (or `puts`) is way more faster and easier to read and type

### printing all books ###    

It can be convenient to have a function to print the complete `Books` struct:    

```C
int print_all(Books* col)
{
    if (col == NULL) return -1;
    printf(
        "    there are %llu of %llu books\n\n", col->size,
        col->limit);
    for (size_t i = 0; i < col->size; i += 1)
        col->print_book(&col->book[i]);
    printf("\n");
    return 0;
};
```

Since it uses the embedded pointer in the `Books` struct it is easy to change style just by changing the pointer inside the struct

### using the 2 functions ###    

```C
    coll.print_book(&coll.book[0]);
    print_book(&coll.book[1]);
   // print all
    printf("test: print the collection\n\n");
    print_all(&coll);
```

here we call `print_book` to print each book, by name and by reference using the pointer inside collection. Then the collection is printed.

### A complete example ###    

```C
// Looks the same but does not work
#include <stdio.h>
#include <string.h>

typedef struct
{                     // struct data
    int  id;          // book id
    char title[50];   // book title
    char author[50];  // book author
} Book;               // a single book

typedef struct
{
    size_t limit;  // capacity
    size_t size;   // actual size
    Book   book[10];
    void (*print_book)(Book*);
} Books;                          // a collection

void print_book(Book* book);      // single book
void print_book_alt(Book* book);  // single book,one line
int  print_all(Books*);           // collection

int main(void)
{
    Books coll = {
        .limit = 10,
        .size  = 2,
        .book[0] =
            {.id     = 42,
             .title  = "This Little Piggy",
             .author = "Bad Wolf"},
        .book[1] =
            {.id     = 4242,
             .title  = "Mary Had a Lamb",
             .author = "Snow Fleece"},
        .print_book =
            print_book  // this function prints a book
    };

    coll.print_book(&coll.book[0]);
    print_book(&coll.book[1]);
    // print all
    printf("test: print the collection\n\n");
    print_all(&coll);
    // test alternate print f.
    printf("test: alternate print\n\n");
    print_book_alt(&coll.book[0]);
    print_book_alt(&coll.book[1]);
    // adds a new book
    Book other = {
        .id     = 17,
        .title  = "My time with Ms. Lane",
        .author = "Super Man"};
    printf("\ntest: new book\n\n");
    print_book_alt(&other);
    // updates collection
    coll.book[2] = other;
    coll.size += 1;
    print_all(&coll);
    // changes printing
    coll.print_book = print_book_alt;
    printf("\ntest: print the collection again\n\n");
    print_all(&coll);
    return 0;
}

void print_book(Book* book)
{
    printf(
        "\
    book_id       : %d\n\
    title         : %s\n\
    author        : %s\n\
\n",
        book->id, book->title, book->author);
};

int print_all(Books* col)
{
    if (col == NULL) return -1;
    printf(
        "    there are %llu of %llu books\n\n", col->size,
        col->limit);
    for (size_t i = 0; i < col->size; i += 1)
        col->print_book(&col->book[i]);
    printf("\n");
    return 0;
};

void print_book_alt(Book* book)
{
    printf(
        "%8d, \"%s\", [%s]\n", book->id, book->title,
        book->author);
}
```

### the output of the example ###       

```none
    book_id       : 42
    title         : This Little Piggy
    author        : Bad Wolf

    book_id       : 4242
    title         : Mary Had a Lamb
    author        : Snow Fleece

test: print the collection

    there are 2 of 10 books

    book_id       : 42
    title         : This Little Piggy
    author        : Bad Wolf

    book_id       : 4242
    title         : Mary Had a Lamb
    author        : Snow Fleece


test: alternate print

      42, "This Little Piggy", [Bad Wolf]
    4242, "Mary Had a Lamb", [Snow Fleece]

test: new book

      17, "My time with Ms. Lane", [Super Man]
    there are 3 of 10 books

    book_id       : 42
    title         : This Little Piggy
    author        : Bad Wolf

    book_id       : 4242
    title         : Mary Had a Lamb
    author        : Snow Fleece

    book_id       : 17
    title         : My time with Ms. Lane
    author        : Super Man



test: print the collection again

    there are 3 of 10 books

      42, "This Little Piggy", [Bad Wolf]
    4242, "Mary Had a Lamb", [Snow Fleece]
      17, "My time with Ms. Lane", [Super Man]
```

In this example 
  - the printing function is called twice, one by name and other using the pointer in `Books`
  - a book is added to the collection
  - an alternate printing function is added
  - the function pointer inside `Books` is changed
  - the collection is again printed


  ## C with classes: turning the `Books` collection generic using function pointers ##    
  Here is the `struct` for the _collection_ in the previous example:

```C  
typedef struct
{
    size_t limit;  // capacity
    size_t size;   // actual size
    Book   book_list[10];
    int (*print_book)(Book*);
} Books; // a collection
```

It solves many things, since it can be treated as a whole and encapsulates the current and total size limits. 

But:
  - if `Book` allocates memory there is no way to free it.
  - The _collecion's_ items are themselves inside the `struct`
    - resize is difficult
    - delete an item is difficult    
  - It is the same file so a change in the container **OR** in the items triggers a change in the same code 

  >It would be better to have the code for the container independent from the item's code.

  ### Example: a more generic container ###    
  Sure it is a list of items. But could be a linked list, a set, a queue, an array... `C++` calls it a *container*. `java` and others call them *collection*.

  Consider

```C
#include "item.h"
typedef struct
{
    size_t limit;             // capacity
    size_t size;              // actual size
    Item** item;              // an array of pointers to items
    Item* (*copy)(Item*);     // copy
    Item* (*destroy)(Item*);  // destructor
    int (*show)(Item*);       // print
} Container;                  // a collection
```

Now we have an array of pointers to items and:
  - it is easy to resize: we have just references to the item
  - we have no reference to the actual definition of the item: it is left to the `item.h` file
  - we know how to copy an item in order to insert it into the container
  - we know how to destroy when destroying the container or managing the container's contents.
  - we know how to show one item at the screen
  > but now we can implement the container code with no mention  to the items: a container can contain anything: books, SDL screens, SDL renderers, invoices, movie tickets...

  A minimum set of functions:

  ```C
  Container* ctn_create(
    size_t, Item* (*copy)(Item*), Item* (*destroy)(Item*),
    int (*show)(Item*));
Container* ctn_destroy(Container*);
int        ctn_insert(Item*, Container*);
int        ctn_show(Container*);
```

This is the mininum to write an example, this one here.

### `Book` as an `Item` in `item.h` ###    

```C
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
```

The `typedef` is the line that links container to item and it is the single line to change in order to use the container code, unchanged.    

And a change in container code does not lead to a change in the item code, thanks to the function pointers. In `C++` they would be caller *copy constructor*, *destructor* and *operator << overload* of `Item`.

### Implementation 1: the same `Book` ###    

### item.h ###    
```C
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
```

### `item.c` a simple implementation ###    
```C
#include "item.h"

#include <stdlib.h>

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
```

### `main` for a test ###    
```C
#include <stdio.h>

#include "container.h"
#include "item1.h"

int main(void)
{
    const int  test_size = 6;
    Container* books =
        ctn_create(30, copy_book, delete_book, print_book);
    Book one = {0};
    for (size_t id = 1; id <= test_size; id += 1)
    {
        sprintf(one.author, "Author %llu", id);
        sprintf(one.title, "Title %llu", id);
        one.id = id;
        ctn_insert(&one, books);
    };
    ctn_show(books);
    printf("\n\t***** now print again changing layout\n\n");
    books->show = print_book_alt;
    ctn_show(books);             // now using _alt
    books = ctn_destroy(books);  // delete all
    ctn_show(books);             // now empty
    return 0;
}
```

Note 1: 
```C
    ctn_show(books);
    printf("\n\t***** now print again changing layout\n\n");
    books->show = print_book_alt;
    ctn_show(books);             // now using _alt
```

Note 2:
```C
    books = ctn_destroy(books);  // delete all
    ctn_show(books);             // now empty
```

By returning `NULL` from `cnt_destroy` the pointer of the container is invalidaded in the same line of code. This is great, so the call to `ctn->show` on the next line will never get an invalid pointer.

The point of using `books->show` here is that it encapsulates behavior, as an object in `Python` or `java` or `C++`: by changing this, the next call to `cnt_show` inherits the new print layout

### output for test 1 ###    

```none
    there are 6 of 30 items

    book_id       : 1
    title         : Title 1
    author        : Author 1

    book_id       : 2
    title         : Title 2
    author        : Author 2

    book_id       : 3
    title         : Title 3
    author        : Author 3

    book_id       : 4
    title         : Title 4
    author        : Author 4

    book_id       : 5
    title         : Title 5
    author        : Author 5

    book_id       : 6
    title         : Title 6
    author        : Author 6

[end of listing]

        ***** now print again changing layout

    there are 6 of 30 items

       1, "Title 1", [Author 1]
       2, "Title 2", [Author 2]
       3, "Title 3", [Author 3]
       4, "Title 4", [Author 4]
       5, "Title 5", [Author 5]
       6, "Title 6", [Author 6]
[end of listing]
``` 

### Implementation 2: `Book` with dynamic allocation, same container code ###

### `item.h` ###    

```C
#pragma once
#include <stdio.h>

typedef struct
{                   // struct data
    size_t id;      // book id
    char*  title;   // book title
    char*  author;  // book author
    size_t n_pages;   
} Book;             // a single book

typedef Book Item;  // here we have a container for books

int   print_book(Book*);
int   print_book_alt(Book*);  // one liner
Book* copy_book(Book*);
Book* create_book(void);
Book* delete_book(Book*);
```

Now title and author are `char*` so memory must be allocated in the copy function and free'd on delete...

### `item.c` a second implementation ###    

```C
#include <stdlib.h>
#include <string.h>
#include "item2.h"

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
```

Note that a _factory_ function `create_book` was added to create `Book` records as needed for the test...

### `main` for the second case ###    

```C
#include <stdio.h>
#include <stdlib.h>

#include "container.h"
#include "item2.h"

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
```

Due to the factory function the code here is simpler than the code on the first example, but the use of the container is identical...

### Output of the second case ###    

```none
    there are 6 of 30 items

    book_id       : 1000
    title         : Title 1000 [241 pages]
    author        : Author 1000

    book_id       : 1001
    title         : Title 1001 [222 pages]
    author        : Author 1001

    book_id       : 1002
    title         : Title 1002 [334 pages]
    author        : Author 1002

    book_id       : 1003
    title         : Title 1003 [350 pages]
    author        : Author 1003

    book_id       : 1004
    title         : Title 1004 [304 pages]
    author        : Author 1004

    book_id       : 1005
    title         : Title 1005 [269 pages]
    author        : Author 1005

[end of listing]

        ***** now print again, using new layout

    there are 6 of 30 items

    1000, "Title 1000 [241 pages]", [Author 1000]
    1001, "Title 1001 [222 pages]", [Author 1001]
    1002, "Title 1002 [334 pages]", [Author 1002]
    1003, "Title 1003 [350 pages]", [Author 1003]
    1004, "Title 1004 [304 pages]", [Author 1004]
    1005, "Title 1005 [269 pages]", [Author 1005]
[end of listing]

C:\Users\toninho\source\repos\soc23-0720-example\x64\Release\Example2.exe (process 19136) exited with code 0.
Press any key to close this window . . .
```


