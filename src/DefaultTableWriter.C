
#include "DefaultTableWriter.h"
#include "StringHelper.h"
#include "Reaction.h"
#include "fmt/core.h"

using namespace std;

namespace prism
{
DefaultTableWriter::DefaultTableWriter() : TableWriterBase() {}

void
DefaultTableWriter::beginDocument(const std::vector<std::string> & bib_files)
{
  _table_str << "\\documentclass{article}\n";
  _table_str << "\\usepackage{tabu}\n";
  _table_str << "\\usepackage{float}\n";
  _table_str << "\\usepackage{graphicx}\n";
  _table_str << "\\usepackage{amsmath}\n";
  _table_str << "\\usepackage[top=1cm]{geometry}\n";
  _table_str << "\\tabulinesep = 1.5mm\n\n";
  _table_str << "\\usepackage[\n";
  _table_str << "  backend=biber,\n";
  _table_str << "  style=numeric,\n";
  _table_str << "  sorting=nty,\n";
  _table_str << "]{biblatex}\n\n";
  for (const auto & file : bib_files)
  {
    const auto & bib_file = splitByDelimiter(file, "/");
    _table_str << "\\addbibresource{" + bib_file.back() + "}\n";
  }
  _table_str << "\n\n";
  _table_str << "\\begin{document}\n\n";
}

void
DefaultTableWriter::endDocument()
{
  for (unsigned int i = 1; i < _inverse_note_numbers.size() + 1; ++i)
  {
    _table_str << fmt::format("\\footnotemark[{:d}]", i) + "{" + _inverse_note_numbers[i] +
                      "}\\\\ \n";
  }
  _table_str << "\\newpage\n";
  _table_str << "\\printbibliography\n\n";
  _table_str << "\\end{document}\n";
}

void
DefaultTableWriter::beginTable()
{
  _table_str << "\\begin{table}[H]\n";
  _table_str << "  \\centering\n";
  _table_str << "  \\resizebox{\\columnwidth}{!}{\n";
  _table_str << "    \\begin{tabu}{clcccccccc}\n";
  _table_str << "      No. & Reaction & $A$ & $n_g$ & $E_g$ & $n_e$ & $E_e$ & $\\Delta "
                "\\varepsilon_e$ & $\\Delta \\varepsilon_g$ & Ref.\\\\\n";
  _table_str << "      \\hline\n";
  _table_str << "      \\hline\n";
}
void
DefaultTableWriter::endTable()
{
  _table_str << "    \\end{tabu}\n";
  _table_str << "  }\n";
  _table_str << "\\end{table}\n\n";
}

void
DefaultTableWriter::beginRateBasedSection()
{
  _table_str << "\\newpage\n";
  _table_str << "\\section{Rate Reactions}\n";
}

void
DefaultTableWriter::beginXSecBasedSection()
{
  _table_str << "\\newpage\n";
  _table_str << "\\section{Cross Section Reactions}\n";
}

void
DefaultTableWriter::addFunctionalReaction(const std::shared_ptr<const Reaction> & r)
{
  _rxn_count++;
  _table_str << fmt::format("      {:d}", _rxn_count) + " & ";
  _table_str << r->latexRepresentation() + " & ";
  for (const auto & param : r->functionParams())
  {
    _table_str << formatScientific(param) + " & ";
  }
  _table_str << formatScientific(r->deltaEnergyElectron()) + " & ";
  _table_str << formatScientific(r->deltaEnergyGas()) + " & ";
  _table_str << r->getReferencesAsString() + " ";
  addNotes(r->notes());
  _table_str << "\\\\\n";
}

void
DefaultTableWriter::addTabulatedReaction(const std::shared_ptr<const Reaction> & r)
{
  _rxn_count++;
  _table_str << fmt::format("      {:d}", _rxn_count) + " & ";
  _table_str << r->latexRepresentation() + " & ";
  _table_str << " & & EEDF & & & ";
  _table_str << formatScientific(r->deltaEnergyElectron()) + " & ";
  _table_str << formatScientific(r->deltaEnergyGas()) + " & ";
  _table_str << r->getReferencesAsString() + " ";
  addNotes(r->notes());
  _table_str << "\\\\\n";
}

void
DefaultTableWriter::addNotes(const std::vector<std::string> & notes)
{
  vector<unsigned int> numbers;
  for (auto note : notes)
  {
    auto it = _note_numbers.find(note);
    if (it == _note_numbers.end())
    {
      _note_count++;
      _note_numbers[note] = _note_count;
      _inverse_note_numbers[_note_count] = note;
      numbers.push_back(_note_count);
    }
  }
  sort(numbers.begin(), numbers.end());

  for (auto it = numbers.begin(); it != numbers.end(); ++it)
  {
    _table_str << fmt::format("\\footnotemark[{:d}]", *it);
    _table_str << (next(it) != numbers.end() ? "$^{,}$" : "");
  }
}
}
