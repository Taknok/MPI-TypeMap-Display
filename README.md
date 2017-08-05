# MPI TypeMap Display
 Little library allowing to display Typemaps for derived Datatypes in MPI.
 
 ## Quick documentation
Use `#include <typemap.h>` to access the function `void printMapDatatype(MPI_Datatype datatype)`. The typemap is printed on the stdout.
 - `[` means that this is the beginning of and vector typemap. `]` is for the end of it.
 - `<` and `>` are for an indexed.
 - `{` and `}` are for a structure.
 
 Output example :
 ```
"(LB, -3), (MPI_INT, 0), (MPI_INT, 9), (UB, 15)"
 ```
 ## Contribution
 If you avec an issue you can report it [here](https://github.com/Taknok/MPI-TypeMap-Display/issues/new).<br>
 [Pull request](https://github.com/Taknok/MPI-TypeMap-Display/pulls) are welcomed.<br>
 You can contact me for questions to [pg.developper.fr@gmail.com](mailto:pg.developper.fr@gmail.com)
 
 ## Run the test
 ```bash
 ~ > cd MPI-TypeMap-Display
 ~/MPI-TypeMap-Display > make
 ~/MPI-TypeMap-Display > ./bin/test
 ```
 
 ## TODO
 - Fix the `,` at the end of typemap
 - Implement `MPI_COMBINER_SUBARRAY` & `MPI_COMBINER_DARRAY`
 - Probably a lot more ...
 

### Sources
> [http://www.cs.kent.edu/~farrell/cc08/lectures/mpi_datatypes.pdf][1]<br>
> [http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node161.htm][2]<br>
> [http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-1.1/node55.htm][3]<br>
> [http://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-data.html][4]<br>

[1]:http://www.cs.kent.edu/~farrell/cc08/lectures/mpi_datatypes.pdf]
[2]:http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-2.0/node161.htm
[3]:http://www.mcs.anl.gov/research/projects/mpi/mpi-standard/mpi-report-1.1/node55.htm
[4]:http://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-data.html
