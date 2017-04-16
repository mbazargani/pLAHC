<strong><font size="12">  </font></strong>
<article class="markdown-body entry-content" itemprop="text"><h1><a id="user-content-fast-and-efficient-black-box-optimization-using-the-parameter-less-population-pyramid" class="anchor" href="#Parameter-less-Late-Acceptance-Hill-Climbing" aria-hidden="true"><svg aria-hidden="true" class="octicon octicon-link" height="16" version="1.1" viewBox="0 0 16 16" width="16"><path fill-rule="evenodd" d="M4 9h1v1H4c-1.5 0-3-1.69-3-3.5S2.55 3 4 3h4c1.45 0 3 1.69 3 3.5 0 1.41-.91 2.72-2 3.25V8.59c.58-.45 1-1.27 1-2.09C10 5.22 8.98 4 8 4H4c-.98 0-2 1.22-2 2.5S3 9 4 9zm9-3h-1v1h1c1 0 2 1.22 2 2.5S13.98 12 13 12H9c-.98 0-2-1.22-2-2.5 0-.83.42-1.64 1-2.09V6.25c-1.09.53-2 1.84-2 3.25C6 11.31 7.55 13 9 13h4c1.45 0 3-1.69 3-3.5S14.5 6 13 6z"></path></svg></a>Parameter-less Late Acceptance Hill-Climbing</h1>

<p>To compile you will need C++11. We use gcc version 4.8.1 for our compilation.</p>

<p>Our build system uses Makefiles. You can compile the application by typing "make" at the command line. It will generate the executable file "pLAHC".</p>

<p>All of the source code is available in the 'src' directory. All TSP instances that are used in the "Parameter-less Late Acceptance Hill-Climbing" paper, published in GECCO'17 are in the "problem_instances" directory.</p>

<p>There is an input sample file, "inputfile", that also explains the configuration needed to be set in the input file. Three other sample input files that were used in our experiments for the GECCO'17 paper, calling three different algorithms, i.e., LAHC, pLAHc, and pLAHC-s, for the TSP instance "rat783.tsp" are given in the directory "sample_inputfiles".</p>

<p>To run an experiment, the executable file "pLAHC", TSP instance and input file must be in the same directory. For example by typing the following at the command line:</p>

<p>./pLAHC inputfile</p>

<p>All of the output files will be generated in the same directory as the executable file "pLAHC". The output file that contains the main output data has a name ending with ".log". The application also produces some other files for data analysis purposes.</p>
