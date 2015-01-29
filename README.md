# resistance
Resistance Game Solver by Matthew Brennan and Vincent Kee

This includes 2 programs: one that works for all valid games of resistance and one that provides much deeper analysis but only works on games of resistance with 5 players due to the complexity of the analysis.
Navigate to the directory containing the code.

The Basic Version

g++ simple_main.cpp -o simple_main

To run:
./simple_main


The Complex Version

Note: You may need to compile the header files each time you pull code.
Run the following commands in terminal:
g++ -c r_game.cpp -o rglib.o
g++ -c game_spec.cpp -o gslib.o
g++ -c game_tree.cpp -o gtlib.o

To compile the executable:
g++ main.cpp gslib.o gtlib.o rglib.o -o main

To run:
./main


