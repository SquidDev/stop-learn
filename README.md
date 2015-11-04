# Stop-Learn

I thought I'd need Machine Learning to play the game "Stop". I didn't, but the name stuck.

## How to Play

Stop is played in an 8x8 grid, each player starting in opposite directions:

```
  0 1 2 3 4 5 6 7
0| | | | | | | |X|
1| | | | | | | | |
2| | | | | | | | |
3| | | | | | | | |
4| | | | | | | | |
5| | | | | | | | |
6| | | | | | | | |
7|0| | | | | | | |
```

Both players take turns to play. You play by placing a new piece one or two tiles away
from one of your existing pieces. You can only place a tile if you have a clear line of sight:

```
  0 1 2 3 4 5 6 7
1| | | |x| | | | |
2| | | |x| | | | |
3| |x|x|X|O| | | |
4| | | |x| | | | |
5| | | |x| | | | |
```

The game ends when one player (or both) cannot play. The winner is whoever has played the most tiles.
Sometimes the game can end despite both players can play as there are no cells in "conflict". 
In the example below, X has one by 3 points.

```
  0 1 2 3 4 5 6 7
0| | |X|0|X|X| |X|
1| |X|0|0|0|X| | |
2|X|X|0|X|0|X|X| |
3|0|0|0|X|0|X| | |
4| | |0|X|0|X|X| |
5| | |0|X|0|X|X|X|
6| | |0|X|X|X|0|0|
7|0| |0|0|0|X|0|0|
```

## Running:
You'll need to setup CMake to use Clang (this isn't required, I just use Clang flags): 
`cmake -DCMAKE_C_COMPILER=/usr/bin/clang .`

Then just run `./StopLearn`, this by default plays against the Manhattan algorithm with a depth of 2.

You can configure this in `main.cpp`.

To play you can entire coordinates in the form (x, y). There are also an additional set of commands you can
run:

 - `f`: Print a territory map based off the FloodFill algorithm
 - `m`: Print a territory map based off the Manhattan algorithm
 - `s`: Print the board score based off all scoring mechanisms
 - `g`: Generate an SVG graphic of the current board.

## Scoring Mechanisms
The computer plays based off a min-max strategy, based off a score of the final board. 
There are two scoring mechanisms: Manhattan and FloodFill. These both score the board 
through finding each player's territory (who owns each cell).

Both algorithms randomly shuffle the possible moves to show some degree of 
"randomness" - otherwise the same game could be played every time.

### FloodFill 
The first pass of the flood fill algorithm establishes territory based off which tiles are
either occupied, or could be reached within one move. If both players can reach the cell within 
one move, then it is marked as "both".

The second pass then attempts to flood fill the board from every occupied point. If it only finds
tiles of the same player, than the fill is confirmed, and that territory is added, otherwise it
is aborted.

### Manhattan
The Manhattan algorithm takes its name from [Manhattan Distance](https://en.wikipedia.org/wiki/Taxicab_geometry), 
though that is not entirely accurate. Like FloodFill, each tile can be owned by either player, or "both" players.
Instead of filling the board, each tile's owner is determined by which player would take the least number of moves
to get to that cell.

Manhattan ends up being more effective, as it does not depend on an 
enclosed area being considered 'territory'.