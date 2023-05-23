# NodeTreeVisualiazer
INSPIRED BY TSODING :) https://www.youtube.com/c/Tsoding
Input a list of words or names and the program breaks down the words to characters and creates a node tree of the. Then it visualizes it using the graphviz library.

## INSTALLATION 
Your first have to download the repository files and the build the program.c file using this command:

```bash
cc -Wall -Werror -std=c17 -pedantic -ggdb -o program program.c
```

## USAGE
To use the program just run the program followed by the input file:

```bash
./program input_file
```

## INPUT FILE
The input file must be words with one in each line:

```bash
animal
danger
outlaw
```
