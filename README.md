# MPI TypeMap Display
 Little library allowing to display Typemaps for derived Datatypes in MPI.
 
 ## Quick documentation
Use `#include <typemap.h>` to access the function `void printMapDatatype(MPI_Datatype datatype)`. The typemap is printed on the stdout.
 - `[` means that this is the beginning of and vector typemap. `]` is for the end of it.
 - `<` and `>` are for an indexed.
 - `{` and `}` are for a structure.
 
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
 
