waresim
=======

A warehouse simulator.


Build: make

Run: ./waresim.out CONFIG

Two sample config files and their corresponding warehouse layout files are included. warehouse.txt and config.txt correspond to a simple 5x5 warehouse. big__config and big_warehouse.txt correspond to a 92x101 warehouse.

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


_Length_: The length of the simulation in seconds.

_Warehouse_: The location of the warehouse layout file.

_Seed_: The master seed for the simulation.

_Log_: Directory for the log files.

_View_: The simulation view to use. Currently supported values are 'OpenGL' and 'None'.
..* OpenGL: A real-time warehouse rendering using OpenGL.
..* None: No rendering will be displayed.

_Dispatch_: The dispatch algorithm to use. The only supported value is 'Rand'.
..* Rand: Orders are randomly assigned to workers.

_Routing_: The routing algorithm to use. The only supported value is 'FCFS'
..* FCFS: A first-come first served algorithm, where order are processed in the order they were assigned to the worker.

Dependencies
-------

C++11

GLFW 2.7

GLEW

OpenGL 2.0+



