#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 100

#define BOOKS_FILE "books.dat"
#define STUDENTS_FILE "students.dat"

typedef struct {
    int book_id;
    char title[100];
    char author[50];
    int is_issued;
    long prn_issued_to;
} Book;

typedef struct {
    long prn;
    char name[50];
    char branch[30];
    int issued_book_id;
} Student;

void printLine() {
    printf("\n--------------------------------------------\n");
}

/* ================= FILE SAVE LOAD ================= */

void saveBooks(Book books[], int count) {

    FILE *f = fopen(BOOKS_FILE, "wb");

    if (f == NULL) {
        printf("Error saving books!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, f);
    fwrite(books, sizeof(Book), count, f);

    fclose(f);
}

int loadBooks(Book books[]) {

    FILE *f = fopen(BOOKS_FILE, "rb");

    if (f == NULL)
        return 0;

    int count;

    fread(&count, sizeof(int), 1, f);
    fread(books, sizeof(Book), count, f);

    fclose(f);

    return count;
}

void saveStudents(Student students[], int count) {

    FILE *f = fopen(STUDENTS_FILE, "wb");

    if (f == NULL) {
        printf("Error saving students!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, f);
    fwrite(students, sizeof(Student), count, f);

    fclose(f);
}

int loadStudents(Student students[]) {

    FILE *f = fopen(STUDENTS_FILE, "rb");

    if (f == NULL)
        return 0;

    int count;

    fread(&count, sizeof(int), 1, f);
    fread(students, sizeof(Student), count, f);

    fclose(f);

    return count;
}

/* ================= SEARCH ================= */

int findBookById(Book books[], int count, int id) {

    for (int i = 0; i < count; i++)
        if (books[i].book_id == id)
            return i;

    return -1;
}

int findStudentByPRN(Student students[], int count, long prn) {

    for (int i = 0; i < count; i++)
        if (students[i].prn == prn)
            return i;

    return -1;
}

/* ================= BOOK FUNCTIONS ================= */

void addBook(Book books[], int *count) {

    if (*count >= MAX_BOOKS) {
        printf("Library Full!\n");
        return;
    }

    Book b;

    b.book_id = *count + 1;

    printf("Enter Book Title: ");
    getchar();
    fgets(b.title, 100, stdin);
    b.title[strcspn(b.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(b.author, 50, stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    b.is_issued = 0;
    b.prn_issued_to = 0;

    books[*count] = b;
    (*count)++;

    printf("Book Added Successfully! ID = %d\n", b.book_id);
}

void viewBooks(Book books[], int count) {

    if (count == 0) {
        printf("No books available!\n");
        return;
    }

    printLine();

    printf("%-5s %-30s %-20s %-10s\n","ID","Title","Author","Status");

    printLine();

    for (int i=0;i<count;i++) {

        printf("%-5d %-30s %-20s",
               books[i].book_id,
               books[i].title,
               books[i].author);

        if (books[i].is_issued)
            printf("Issued\n");
        else
            printf("Available\n");
    }

    printLine();
}

/* ================= STUDENT ================= */

void registerStudent(Student students[], int *count) {

    if (*count >= MAX_STUDENTS) {
        printf("Student limit reached!\n");
        return;
    }

    Student s;

    printf("Enter PRN: ");
    scanf("%ld",&s.prn);

    if (findStudentByPRN(students,*count,s.prn)!=-1) {
        printf("Student already exists!\n");
        return;
    }

    printf("Enter Name: ");
    getchar();
    fgets(s.name,50,stdin);
    s.name[strcspn(s.name,"\n")]=0;

    printf("Enter Branch: ");
    fgets(s.branch,30,stdin);
    s.branch[strcspn(s.branch,"\n")]=0;

    s.issued_book_id=0;

    students[*count]=s;
    (*count)++;

    printf("Student Registered Successfully!\n");
}

/* ================= ISSUE ================= */

void issueBook(Book books[],int bcount,Student students[],int scount){

    long prn;
    int book_id;

    printf("Enter PRN: ");
    scanf("%ld",&prn);

    int si=findStudentByPRN(students,scount,prn);

    if(si==-1){
        printf("Student not found!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d",&book_id);

    int bi=findBookById(books,bcount,book_id);

    if(bi==-1){
        printf("Book not found!\n");
        return;
    }

    if(books[bi].is_issued){
        printf("Book already issued!\n");
        return;
    }

    books[bi].is_issued=1;
    books[bi].prn_issued_to=prn;

    students[si].issued_book_id=book_id;

    printf("Book Issued Successfully!\n");
}

/* ================= RETURN ================= */

void returnBook(Book books[],int bcount,Student students[],int scount){

    long prn;

    printf("Enter PRN: ");
    scanf("%ld",&prn);

    int si=findStudentByPRN(students,scount,prn);

    if(si==-1){
        printf("Student not found!\n");
        return;
    }

    int book_id=students[si].issued_book_id;

    if(book_id==0){
        printf("No book to return!\n");
        return;
    }

    int bi=findBookById(books,bcount,book_id);

    books[bi].is_issued=0;
    books[bi].prn_issued_to=0;

    students[si].issued_book_id=0;

    printf("Book Returned Successfully!\n");
}

/* ================= MAIN ================= */

int main(){

    Book books[MAX_BOOKS];
    Student students[MAX_STUDENTS];

    int bcount=loadBooks(books);
    int scount=loadStudents(students);

    int choice;

    do{

        printf("\n===== LIBRARY SYSTEM =====\n");
        printf("1 Add Book\n");
        printf("2 View Books\n");
        printf("3 Register Student\n");
        printf("4 Issue Book\n");
        printf("5 Return Book\n");
        printf("6 Save Data\n");
        printf("0 Exit\n");

        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice){

            case 1: addBook(books,&bcount); break;

            case 2: viewBooks(books,bcount); break;

            case 3: registerStudent(students,&scount); break;

            case 4: issueBook(books,bcount,students,scount); break;

            case 5: returnBook(books,bcount,students,scount); break;

            case 6:
                saveBooks(books,bcount);
                saveStudents(students,scount);
                printf("Data Saved Successfully!\n");
                break;

            case 0:
                saveBooks(books,bcount);
                saveStudents(students,scount);
                printf("Data Saved. Goodbye!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    }while(choice!=0);

    return 0;
}
