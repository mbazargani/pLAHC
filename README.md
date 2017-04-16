<strong> Parameter-less Late Acceptance Hill-Climbing </strong>


To compile you will need C++11. We use gcc version 4.8.1 for our compilation.

Our build system uses Makefiles. You can compile the application by typing "make" at the command line.
It will generate the executable file "pLAHC".

All of the source code is available in the 'src' directory. 
All TSP instances that are used in the "Parameter-less Late Acceptance Hill-Climbing" paper,
published in GECCO'17 are in the "problem_instances" directory.

There is an input sample file, "inputfile", that also explains the configuration needed to be set in
the input file. Three other sample input files that were used in our experiments for the GECCO'17 paper,
calling three different algorithms, i.e., LAHC, pLAHc, and pLAHC-s, for the TSP instance "rat783.tsp" are
given in the directory "sample_inputfiles".

To run an experiment, the executable file "pLAHC", TSP instance and input file must be in the same directory.
For example by typing the following at the command line:

./pLAHC inputfile

All of the output files will be generated in the same directory as the executable file "pLAHC".
The output file that contains the main output data has a name ending with ".log". The application also
produces some other files for data analysis purposes.
