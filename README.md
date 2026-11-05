# MP3 Tag Reader & Editor

A command-line MP3 Tag Reader and Editor developed using C. This application allows users to view and modify metadata information stored in MP3 files.

## Overview

The project focuses on understanding file handling and binary data manipulation in C. It reads MP3 metadata and provides options to view or edit selected ID3 tag information.

## Features

- View MP3 metadata.
- Edit song title.
- Edit artist name.
- Edit album name.
- Edit release year.
- Edit genre.
- Edit comments.
- Accept updated values through command-line arguments or interactive input.
- Preserve the remaining MP3 file data during metadata modification.

## Metadata Supported

| Option | Description |
|---|---|
| `-t` | Edit song title |
| `-a` | Edit artist |
| `-A` | Edit album |
| `-y` | Edit year |
| `-c` | Edit genre |
| `-C` | Edit comment |

## Technologies & Concepts Used

- C Programming
- Structures
- Pointers
- File Handling (`fopen`, `fread`, `fwrite`, `fseek`)
- Binary File Processing
- Dynamic Memory Allocation
- Command-Line Argument Handling
- String Manipulation

## Project Structure

```text
MP3-Tag-Reader-and-Editor/
│
├── mp3_main.c
├── mp3_header.h
├── functions.c
├── mp3_view.c
├── mp3_edit.c
├── sample.mp3
└── README.md
```

## Compilation

Compile the source files using GCC:

```bash
gcc -Wall -Wextra mp3_main.c functions.c mp3_view.c mp3_edit.c -o mp3_editor
```

## How to Run

### 1. View MP3 metadata

```bash
./mp3_editor -v sample.mp3
```

### 2. Edit metadata interactively

```bash
./mp3_editor -e -t sample.mp3
```

The program prompts you to enter the new title.

### 3. Edit metadata using command-line arguments

```bash
./mp3_editor -e -t "New Song Title" sample.mp3
```

Other examples:

```bash
./mp3_editor -e -a "New Artist" sample.mp3

./mp3_editor -e -A "New Album" sample.mp3

./mp3_editor -e -y 2026 sample.mp3

./mp3_editor -e -c "Rock" sample.mp3

./mp3_editor -e -C "Updated Comment" sample.mp3
```

### 4. Display help

```bash
./mp3_editor --help
```

## Learning Outcomes

Working on this project helped strengthen my understanding of binary file handling, metadata processing, file pointers, and modifying specific sections of a file while preserving the remaining data.

## Author

**Adithyan S**
