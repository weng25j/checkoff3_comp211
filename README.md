# checkoff3_comp211
UNC COMP211 class Checkoff 3 materials

LZ77: A Lossless Data Compression Algorithm
Topics Explored: 
- what lossless data compression means 
- how LZ77 compresses data and what it works best for
    - what a history/sliding window is
    - what a lookahead is
    - what a back reference is 
- interesting discoveries about LZ77
    - how LZ77 serves as the core for many modern data compression algorithms like DEFLATE
    - the idea of a minimum and maximum length of data to compress

Presentation Materials: Presentation file - LZ77.pptx

Coding Examples: LZ77_example.c is a demo of the LZ77 compression algorithm in action
- To run the program, enter this command in your terminal to compile the program:
    - gcc -std=c11 -Wall -Wextra -o LZ77_example LZ77_example.c
    - Then run: ./LZ77_example

Additional Materials:
