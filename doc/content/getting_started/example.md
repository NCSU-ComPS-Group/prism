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

# Example $\LaTeX$ table

One of the capabilities of this program is that a $\LaTeX$ for the reaction mechanism is automatically generated. This is generated by calling the `writeReactionTable` method of the `NetworkParser`. There is a default implementation for this table generation which is available for customization if needed. In this system we generate a footnote for each unique note in the input file and every reaction that has notes references these footnotes in sorted order. This table will compile in a $\LaTeX$ project so long as your project is configured with the proper bib files and you have provided $\LaTeX$ overrides for all species whose names cannot be directly used in a $\LaTeX$ table.

!listing prism/example/table.tex

# Example Species Summary

Another capability of this system is that we provide a species summary with some important information about the reactions that species are involved in. At the beginning of this file we list all the species that have been lumped and the state that they have been lumped into. Then a small summary of the species which: only have sources, sinks, or which are only involved in reactions which lead to no change in their population. After this a summary of the species and all the reactions which is listed. Finally, the summary that has been written to file is again read in. This demontrates that your are able load output from this summary for further analysis using any standard yaml reader tool.

!listing prism/example/summary.yaml