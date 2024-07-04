# Input File Syntax

The PRISM project utilizes the yaml file format to store reaction networks. The yaml file format was selected for its minimal syntax and wide support accross programming languages. The input file is broken up into blocks, the following are supported by PRISM.

- Bibliography Block
- Data Path Block
- Custom Species Block
- Lumped Species Block
- $\LaTeX$ Overrides Block
- Rate Based Reactions Block
- Cross Section Based Reactions Block

## Bibliography Block

In the PRISM format every reaction is required to have at least one cite key associated with it. This blocks allows you to specify a bibliography file in which these cite keys exist.

!alert note
The bibliograpy file must be in the [BibTex](https://www.bibtex.com) format in order to better support documenting a reaction mechanism in $\LaTeX$.

This block only requires a single string which is the path from the location of the executable which is reading the input file to the bibliography file

```yaml
  bibliography: path/to/bib.bib
```

## Data Path Block

To reduce the amount of user input requried, the PRISM project also supports the definition of a data path. This should be used if all the data files required for your network are stored in the same location. When defined the data path will be prepended to the `file` parameter of every reaction in the file. It can be defined in the following manner.

```yaml
  data-path: path/to/data/
```

If we have a reaction which takes the file parameter like this

```yaml
  file: data.txt
```

The reaction object that is created will prvoide the path to the data file in the following manner.

```c++
  "path/to/data/data.txt"
```

## Custom Species Block

The PRISM project provides the mass of all elements in the periodic table, and electrons. PRISM can compute the mass for any species (neutral or ionized) which is composed for elements in the periodic table based on this data. This data has been collected from the following sources [!cite](baum2010nuclides) and [!cite](pubchem).

However, we understand that in some cases users may need and or want to define a custom species or override the default mass provided for an element. When defining a custom species simply define the name and the molar mass of the neutral species with the following syntax.

```yaml
custom-species:
  - name: A
    mass: 1
```

You may also define an arbitrary number of custom species. This is true for all blocks. In the sense that you can and often will have repeated entries in each block.

```yaml
custom-species:
  - name: [A, B, C]
    mass: [1, 2, 3]
```

!alert note
All custom species must have a name which begins with a capital letter.

## Lumped Species Block

It a relatively common practice in plasma simulations to lump multiple excited states of a species into a single species. In this block you can define species present in reactions to be substituted with your lumped species. This allows users to retain all of the information in a reaction network while easily lumping excited states during calculations.

```yaml
lumped-species:
  - lumped: N2*
    actual: [N2(rotation), N2(vibration)]
```

With the lumped species block above the states `N2(rotational)` and `N2(vibrational)` will be substituded with `N2*` when parsing the reactions which contain those excited states. You can also define an arbitrary number of lumped states.

```yaml
lumped-species:
  - lumped: N2*
    actual: [N2(rotation), N2(vibration)]
  - lumped: O2*
    actual: [O2(rotation), O2(vibration)]
```

When this is performed PRISM will first validate that the original reaction that you have provided is valid, then substitute the lumped states into the reaction and then re-validate with the lumped states in the reaction.

## $\LaTeX$ Override Block

Since we automatically generate $\LaTeX$ tables for the reaction networks in the input file we understand that you may not be satisfied with the default $\LaTeX$ representation of the species. In this block we allow you to define a custom $\LaTeX$ representation that will override the default representation.

```yaml
latex-overrides:
  - species: N2(excited)
    latex: N$_{2}\Delta)$
```

You can also define an arbitrary number of these overrides.

```yaml
latex-overrides:
  - species: [O^m, He^m, He2^m]
    latex: ["O$^\\text{m}$", "He$^\\text{m}$", "He$_2$$^\\text{m}$"]
```

## Reaction Block

The bulk of the inputs for a simulation will be in the reaction block. This is really two blocks to allow for cross section based reactions and rate based reactions in the same network. When using both the following syntax will be used. Note that this exact syntax is invalid and full reaction inputs must be provided.

```yaml
rate-based:
  - reaction: ...
  - reaction: ...

xsec-based:
  - reaction: ...
  - reaction: ...
```

Inputs can include both the `rate-based` block, and the `xsec-based` block. Either can also be used on its own. There is no need to define a `rate-based` block if you do not have any rate based reactions in the network.

## Reaction Input Parameters

| Parameter | Description | Data Type | Required? | Default Value |
| - | - | - | - | - |
| reaction | The expression of the reaction see, [reaction_parsing.md] for the expected form |  string | always | N/A |
| delta-eps-e | The change in energy of electrons | float | no | 0.00 |
| delta-eps-g | The change in energy of the background gas | float | no | 0.00 |
| file | The file where the tabulated data is stored | string | yes, if params is not provided | "" |
| eqn-type | The name of the type of analytic express the rates or cross sections take | string | no | arrhenius |
| params | The parameters required for evaluation of the analytic expression | A float or a list of floats | yes, if eqn-type is provided | [] |
| database | The database where you got the cross sections from | string | no | N/A |
| reference | The reference for where the reaction came from | string or a list of strings | always | N/A |


### Reaction Rate/Cross Section Data

The PRISM project supports two main types of reactions. The first is reactions which have cross sections or reaction rates tabulated in files.

```yaml
  - reaction: Ar + e -> Ar* + e
    delta-eps-e: 11.56
    file: ar_excitation.txt
    references: lymberopoulos1993fluid
    notes: This is a test
```

The second is reactions which have an Arrhenius expression for their cross section or reaction rates given by

\begin{equation}
  A \:\:
  \left(
    \frac{T_e}{0.025 \;\;[\text{eV}]}
  \right)^{\:n_e}\:
  \exp\left(-\frac{E_e}{k_b T_e}\right)
  \left(
    \frac{T_g}{0.025 \;\;[\text{eV}]}
  \right)^{\:n_g}\:
  \exp\left(-\frac{E_g}{k_b T_g}\right)
\end{equation}


| Parameter | Index | Units |
| - | - | - |
| $A$ | 0 | m$^{3 (n-1)}$ |
| $n_e$ | 1 | Unitless |
| $E_e$ | 2 | eV |
| $n_g$ | 3 | Unitless |
| $E_g$ | 4 | eV |

Where $n$ in the parameter $A$ row is the number of reactants in a reaction.

When using the provided sampling functions it is expected that the electron temperature $T_e$ and the gas temperature $T_g$ are given in \[eV\]. This is expected to be given in the form

```yaml
  - reaction: 2Ar* -> Ar+ + Ar + e
    params: [1, 2, 3, 4, 5]
    references: lymberopoulos1993fluid
```

!alert note
You do not have to expliticty provide all of the parameters for a reaction which has data in an Arrhenius form. Any parameters which are not provided are assumed to be zero.


## Example input files

For more complete examples of input files please checkout the [inputs](https://github.com/NCSU-ComPS-Group/prism/tree/devel/test/inputs) that are used for testing PRISM.
