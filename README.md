waresim
=======

A warehouse simulator.


Build: make

Run: ./waresim-run CONFIG

Two sample config files and their corresponding warehouse layout files are included. warehouse.txt and config.txt correspond to a simple 5x5 warehouse. big__config and big_warehouse.txt correspond to a 69x101 warehouse.

'B' represents a bin, '.' represents an empty location, 'W' represents a worker pod, 'D' represents a drop-off location, and 'X' represents a wall.


Configuration File Parameters
=======
Below is a sample configuration file with all possible parameters specified.

Length:1000

Warehouse:big_warehouse.txt

Seed:1000

Log:logs

View:OpenGL

Dispatch:Rand

Routing:FCFS

Path:Shortest

Order:Rand

Contention:Backoff

Decay:0.99

---

1. _Length_: The length of the simulation in seconds.
2. _Warehouse_: The location of the warehouse layout file.
3. _Seed_: The master seed for the simulation. Must be an integer.
4. _Log_: Directory for the log files.
5. _View_: The simulation view to use. Currently supported values are 'OpenGL' and 'None'.
  * _OpenGL_: A real-time warehouse rendering using OpenGL.
  * _None_: No rendering will be displayed.
6. _Dispatch_: The dispatch algorithm to use. The only supported value is 'Rand'.
  * _Rand_: Orders are randomly assigned to workers.
7. _Routing_: The routing algorithm to use. The only supported value is 'FCFS'
  * _FCFS_: A first-come first served algorithm, where order are processed in the order they were assigned to the worker.
8. _Path_: The pathfinding algorithm to use. The only supported value is 'Shortest'.
  * _Shortest_: An implementation of Dijkstra's algorithm.
9. _Order_: The order generation algorithm to use.
  * _Rand_: An random algorithm that has a 50% chance of generating an order, and selects a random bin from which that order must be fulfilled.
10. _Contention_: The contention handling algorithm to use.
  * _Backoff_: Selects the worker with the smaller ID to back off from the worker with the larger ID.
11. _Decay_: Decay factory for the heat window. Should be (1-c), where c is a small constant such as 10^-6 or 10^-9 (see Mining of Massive Datasets - RLU (2013)). Tune depending on on fast spots should 'cool off'.

Dependencies
-------

C++11

GLFW 2.7

GLEW

OpenGL 2.0+

FreeType



