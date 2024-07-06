#include "DefaultSpeciesSummaryWriter.h"
#include "Constants.h"
#include "fmt/core.h"
#include "NetworkParser.h"
#include "Species.h"
#include "Reaction.h"

using namespace std;
namespace prism
{

DefaultSpeciesSummaryWriter::DefaultSpeciesSummaryWriter() : SpeciesSummaryWriterBase() {}

void
DefaultSpeciesSummaryWriter::addMiscSummary()
{
  const auto & np = NetworkParser::instance();
  const auto & species = np.species();

  const string const_warning =
      "# species which are only in balanced reactions\nconstant-species:\n";
  const string all_sinks = "# species which are only consumed\nall-sinks:\n";
  const string all_srcs = "# species which are never consumed\nall-sources:\n";

  vector<string> const_s;
  vector<string> all_sinks_s;
  vector<string> all_srcs_s;

  for (const auto & s : species)
  {
    const auto & rate_data = s->rateBasedReactionData();
    const auto & xsec_data = s->xsecBasedReactionData();
    const auto rxn_count = rate_data.size() + xsec_data.size();

    if (s->unbalancedRateBasedReactionData().size() + s->unbalancedXSecBasedReactionData().size() ==
        0)
    {
      const_s.push_back(s->name());
      continue;
    }

    unsigned int sink_count = 0;
    unsigned int src_count = 0;

    for (const auto & r : rate_data)
    {
      if (r.stoic_coeff < 0)
        sink_count++;
      else
        src_count++;
    }

    for (const auto & r : xsec_data)
    {
      if (r.stoic_coeff < 0)
        sink_count++;
      else
        src_count++;
    }

    if (sink_count == rxn_count)
      all_sinks_s.push_back(s->name());

    if (src_count == rxn_count)
      all_srcs_s.push_back(s->name());
  }

  auto species_lister = [](vector<string> & species, std::stringstream & _summary_str)
  {
    _summary_str << "  - species: [";

    for (const auto & s : species)
      _summary_str << s << (s == species.back() ? "" : ", ");

    _summary_str << "]\n";
  };

  _summary_str << all_srcs;
  species_lister(all_srcs_s, _summary_str);

  _summary_str << const_warning;
  species_lister(const_s, _summary_str);

  _summary_str << all_sinks;
  species_lister(all_sinks_s, _summary_str);

  _summary_str << endl << endl;
}

void
DefaultSpeciesSummaryWriter::addLumpedSummary(
    std::map<std::string, std::vector<std::string>> lumped_map)
{
  unsigned int total_lumped = 0;

  for (const auto & it : lumped_map)
  {
    total_lumped += it.second.size();
  }

  _summary_str << fmt::format("# {:d} species lumped into {:d}\n", total_lumped, lumped_map.size());
  _summary_str << LUMPED_SPECIES << ":\n";

  for (const auto & it : lumped_map)
  {
    _summary_str << fmt::format("  - {:s}: {:s}\n", LUMPED_KEY, it.first);
    _summary_str << fmt::format("    {:s}: [", ACTUAL_KEY);
    for (const auto & s : it.second)
    {
      _summary_str << s << (s == it.second.back() ? "" : ", ");
    }
    _summary_str << "]\n";
  }
  _summary_str << "\n";
}

void
DefaultSpeciesSummaryWriter::addSpeciesSummary()
{
  auto reaction_lister = [](vector<string> & srcs,
                            vector<string> & balanced,
                            vector<string> & sinks,
                            std::stringstream & _summary_str)
  {
    _summary_str << fmt::format("      sources:\n");
    _summary_str << fmt::format("        - count: {:d}\n", srcs.size());
    for (const auto & r : srcs)
      _summary_str << fmt::format("        - {:s}\n", r);

    _summary_str << fmt::format("\n      balanced:\n");
    _summary_str << fmt::format("        - count: {:d}\n", balanced.size());

    for (const auto & r : balanced)
      _summary_str << fmt::format("        - {:s}\n", r);

    _summary_str << fmt::format("\n      sinks:\n");
    _summary_str << fmt::format("        - count: {:d}\n", sinks.size());

    for (const auto & r : sinks)
      _summary_str << fmt::format("        - {:s}\n", r);

    _summary_str << endl;
  };

  auto species_summary = [reaction_lister](const vector<shared_ptr<Reaction>> & rxn_list,
                                           const vector<ReactionData> & data,
                                           std::stringstream & _summary_str)
  {
    vector<string> srcs;
    vector<string> sinks;
    vector<string> balanced;
    for (const auto & d : data)
    {
      const auto stoic_coeff = d.stoic_coeff;
      const auto & expression = rxn_list[d.id]->expression();

      if (stoic_coeff == 0)
        balanced.push_back(expression);
      else if (stoic_coeff < 0)
        sinks.push_back(expression);
      else
        srcs.push_back(expression);
    }
    reaction_lister(srcs, balanced, sinks, _summary_str);
  };

  const auto & np = NetworkParser::instance();
  const auto & species = np.species();

  _summary_str << "unique-species:" << endl;
  _summary_str << "  - count: " << species.size() << endl;
  for (const auto & s : species)
  {
    _summary_str << fmt::format("  - {:s}\n", s->name());
  }
  _summary_str << endl << endl;

  _summary_str << "reacion-summary:" << endl;

  const auto & rate_based = np.rateBasedReactions();
  const auto & xsec_based = np.xsecBasedReactions();

  for (const auto & s : species)
  {
    _summary_str << fmt::format("  - {:s}:\n", s->name());
    _summary_str << fmt::format(
        "    - {:s}: {:d}\n", RATE_BASED, s->rateBasedReactionData().size());
    species_summary(rate_based, s->rateBasedReactionData(), _summary_str);

    _summary_str << fmt::format(
        "    - {:s}: {:d}\n", XSEC_BASED, s->xsecBasedReactionData().size());
    species_summary(xsec_based, s->xsecBasedReactionData(), _summary_str);
  }
}
}
