// multiple_placement.cc

// Ourselves:
#include <geomtools/multiple_placement.h>

// Standard library:
#include <stdexcept>
#include <sstream>

namespace geomtools {

  using namespace std;

  size_t multiple_placement::get_dimension () const
  {
    return 1;
  }

  bool multiple_placement::is_replica () const
  {
    return false;
  }

  bool multiple_placement::is_valid () const
  {
    return _placements_.size () > 0;
  }

  void multiple_placement::invalidate ()
  {
    reset ();
    return;
  }

  void multiple_placement::add (const placement & p_)
  {
    _placements_.push_back (p_);
    return;
  }

  const placement & multiple_placement::get_placement (int index_) const
  {
    return _placements_.at (index_);
  }

  placement & multiple_placement::get_placement (int index_)
  {
    return _placements_.at (index_);
  }

  size_t multiple_placement::get_number_of_items () const
  {
    return _placements_.size ();
  }

  size_t multiple_placement::compute_index_map (vector<uint32_t> & map_,
                                                int item_) const
  {
    DT_THROW_IF ((item_ < 0) || (item_ >= (int)get_number_of_items ()), std::domain_error,
                 "Invalid item index '" << item_ << "' !");
    map_.clear ();
    map_.push_back (item_);
    return map_.size ();
  }

  void multiple_placement::get_placement (int item_, placement & p_) const
  {
    p_ = _placements_[item_];
    return;
  }

  multiple_placement::multiple_placement ()
  {
    return;
  }

  multiple_placement::~multiple_placement ()
  {
    reset ();
    return;
  }

  void multiple_placement::reset ()
  {
    _placements_.clear ();
    return;
  }

  void multiple_placement::tree_dump (std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
  {
    const std::string indent2 = indent_;
    this->i_placement::tree_dump (out_, title_, indent2, true);

    out_ << indent_ << datatools::i_tree_dumpable::tag
         << "Size    :" << get_number_of_items () << std::endl;

    out_ << indent_ << datatools::i_tree_dumpable::inherit_tag (inherit_)
         << "Replica :" << is_replica () << std::endl;

    return;
  }

} // end of namespace geomtools
