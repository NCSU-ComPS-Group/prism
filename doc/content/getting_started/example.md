# Example Usage

One way you can use this library is for evaluating the derivatives needed for a 0D plasma species model. Below shows how you would access all of the data required for a simple 0D model.

!listing prism/main.C

An important thing to note in this example is that we are iterating over all of the rate based reactions. This file includes rate based reactions which have rates that come from an arrhenius form and ones that come from data read in from a tabulated data file. Sampling this data is handled with different methods based on your input but there is a single API for ease of use. Reactions which have data from an arrhenius form call a function which evaluates this function and for Reactions which have data read from a file we use linear interpolation to sample data from the file.

!alert note
Ensure that the data is in tabulated data file encompasses the entire sample space you will be probing. Extrapolatory sampling is not supported.

The way this single API is enabled is by assigning the correct sampling function to a function pointer when the Reaction is created and then this function pointer is called on every evaluation, so overhead for the generality of this API should be minimal.

# Example Input File

!listing prism/example/simple_argon_rate.yaml

# Example Program Output

!listing prism/example/output.txt

In the output from the example program you can first see that the reactions are validated by the parser. After this there are 3 main columns under each species section. The first column is the stoichiometric coefficient for the reactions. The second column is the reaction rate sampled at a value of 10. The third column is a list of all of the reactants for each reaction which is either a source a sink for the given species where each species name is raised to the power corrisponding to the number of times it appears on the left hand side of the reaction.
