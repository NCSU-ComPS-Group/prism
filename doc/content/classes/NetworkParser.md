## Public Methods

- `NetworkParser()`  The constructor for the Network Parser
- `parseNetwork(const string & file)`  The method which will read in the input file, perform checks and prepare data for use
- `printReactionSummary()`  A method for printing some basic analysis of the network to stdout
- `writeReactionSummary(const string & filepath)`  This method performs the same action as `printReactionSummary()` but writes the summary to `filepath`
- `printSpeciesSummary()`  A method for printing a summary of each species and which reactions it exists in to stdout
- `writeSpeciesSummary(const string & filepath)`  This method performs the same action as `printSpeciesSummary()` but writes the summary to `filepath`
