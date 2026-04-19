# Library Management System (C)

A straightforward console-based library system in C that handles books, students, and issue/return flow with basic file storage.

## What it does

* Add & list books
* Register students (PRN)
* Issue and return books
* Saves data to local files

## Run it

```bash
gcc main.c -o library
./library
```

## Notes

* Uses simple `.dat` files (no DB)
* Max 100 books & students
* One book per student at a time
