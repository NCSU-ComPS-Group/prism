# Input File Syntax

The rxn-cpp project utilizes the yaml file format to store reaction networks. The yaml file format was selected for its minimal syntax and wide support accross programming languages. The input file is broken up into blocks, the following are supported by rxn-cpp.

- Data Path Block
- Custom Species Block
- Lumped Species Block
- $\LaTeX$ Overrides Block
- Custom Equations Block
- Rate Based Reactions Block
- Cross Section Based Reactions Block

It is important to note that since this project uses the yaml file format, projects which adopt the rxn-cpp format standard can also include additional blocks which will be ignored by the rxn-cpp project.

# Data Path Block

To reduce the amount of user input requried the rxn-cpp project also supports the definition of a data path. This should be used if all the data files required for your network are stored in the same location. When defined the data path will be prepended to the file parameter used in the reaction block. It can be defined in the following manner.

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

If no `data-path` parameter is defined we provide the default data path of `data/`.

# Custom Species Block

The rxn-cpp project provides the mass of neutral and ionized species for all species which are based on elements in the periodic table. This data has been collected from the following sources [!cite](baum2010nuclides) and [!cite](pubchem).

However, we understand that in some cases users may need and or want to define a custom species or override the default mass provided for an element. When defining a custom species simply define the name and mass of the species with the following syntax.

```yaml
custom-species:
  - name: A
    mass: 1
```

You may also define an arbitrary number of custom species. This is true for all blocks. In the sense that you can and often will have repeated entries in each block.

```yaml
custom-species:
  - name: A
    mass: 1
  - name: A
    mass: 2
```

# Lumped Species Block

It a relatively common practice in plasma simulations to lump multiple excited states of a species into a single species. In this block you can define species present in reactions to be substituted with your lumped species. This allows users to retain all of the information in a reaction network while easily lumping excited states during calculations.

```yaml
lumped-species:
  - lumped: N2*
    actual: [N2(rotation), N2(vibration)]
```

With the lumped species block above the states `N2(rotational)` and `N2(vibrational)` will be substituded with `N2*` when parsing the reactions which contain those excited states.

# $\LaTeX$ Override Block

Since we automatically generate $\LaTeX$ tables for the reaction networks in the input file we understand that you may not be satisfied with the default $\LaTeX$ representation of the species. In this block we allow you to define a custom $\LaTeX$ representation that will override the default representation.

```yaml
latex-overrides:
  - species: N2(excited)
    latex: N$_{2}\Delta)$
```

# Custom Equation Block

From time to time an analytic form is required for input that is not in arrehnius form. For this use case users can define custom equation types with the custom equation block. When defining a custom equation 4 parameters are requried.


| Parameter | Description | Data Type |
| - | - | - |
| eqn-type | The alias for the analytic form which will be used in the reaction block | string |
| params | The parameters which are requried to evaluate the custom equation | list of floats |
latex-form | The $\LaTeX$ representation of the equation, used for creating the network table | string |


```yaml
custom-equation:
  - eqn-type: custom
    params: [A, B, C]
    latex-form: $A + B + C$
```

# Reaction Block

The main workhorse of this standard is the reaction block. This is really two blocks to allow for cross section based reactions and rate based reactions in the same network. When using both the following syntax will be used. Note that this exact syntax is invalid and full reaction inputs must be provided.

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

The rxn-cpp project supports two main types of reactions. The first is reactions which have cross sections or reaction rates tabulated in files. The second is reactions which have an analytic expression for their cross section or reaction rate.

| Parameter | Description | Data Type | Required? | Default Value |
| - | - | - | - | - |
| reaction | The expression of the reaction see, [reaction_parsing.md] for the expected form |  string | always | N/A |
| delta-eps-e | The change in energy of electrons | float | no | 0.00 |
| delta-eps-g | The change in energy of the background gas | float | no | 0.00 |
| file | The file where the tabulated data is stored | string | yes, if params is not provided | "" |
| eqn-type | The name of the type of analytic express the rates or cross sections take | string | no | arrhenius |
| params | The parameters required for evaluation of the analytic expression | A float or a list of floats | yes, if eqn-type is provided | [] |
| reference | The reference for where the reaction came from | string | always | N/A |


## Reaction With Data File

```yaml
 - reaction: e + Ar -> e + Ar*
   delta-eps-e: -11.56
   delta-eps-g: 0.00
   file: data/location.txt
   reference: source
```

## Reaction With Analytic Equation

```yaml
 - reaction:  e + Ar -> e + Ar*
   delta-eps-e: -11.56
   delta-eps-g: 0.00
   eqn-type: custom
   params: [1, 2, 3]
   reference: source
```

If no other equation type is defined we assume that any reaction which uses an analytic form for its cross section data or rate data take the following form.

\begin{equation}
  A \:\: T_e^{\:n}\:\exp\left(-\frac{E_e}{k_b T_e}\right) T_g^{\:m} \exp\left(-\frac{E_g}{k_b T_g}\right)
\end{equation}

This form required at most 5 input parameters with the index of the `params` input corrisponding to the following variables.

| Index | Parameter |
| - | - |
| 0 | $A$ |
| 1 | $n$ |
| 2 | $E_e$ |
| 3 | $m$ |
| 4 | $E_g$ |

If less than 5 parameters are supplied when using the default arrhenius form the remaining parameters which are not supplied will be set to 0. Constant rates are also supported by default. When you would like to use a constant rate equation simply provide a single float that is not in list form.


```yaml
 - reaction:  e + Ar -> e + Ar*
   delta-eps-e: -11.56
   delta-eps-g: 0.00
   eqn-type: custom
   params: 1
   reference: source
```

A note for developers interested in using this proejct. When a single float is supplied here we still treat this an an arrhenius rate will return a vector of 5 floats where the first is the value of interest and the rest are simply zeros.
