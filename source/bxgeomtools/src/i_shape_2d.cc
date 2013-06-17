// -*- mode: c++; -*-
/* i_shape_2d.cc
 */

#include <geomtools/i_shape_2d.h>

#include <datatools/utils.h>

namespace geomtools {

  using namespace std;

  bool i_shape_2d::has_number_of_sides() const
  {
    return get_number_of_sides() > 0;
  }

  unsigned int i_shape_2d::get_number_of_sides() const
  {
    return 0;
  }

  bool i_shape_2d::has_perimeter(uint32_t flags_) const
  {
    return datatools::is_valid(get_perimeter(flags_));
  }

  double i_shape_2d::get_perimeter(uint32_t flags_) const
  {
    return std::numeric_limits<double>::quiet_NaN();
  }

  bool i_shape_2d::has_surface(uint32_t flags_) const
  {
    return datatools::is_valid(get_surface(flags_));
  }

  double i_shape_2d::get_surface(uint32_t flags_) const
  {
    return std::numeric_limits<double>::quiet_NaN();
  }

  int i_shape_2d::get_dimensional () const
  {
    return DIMENSIONAL_2;
  }

  i_shape_2d::i_shape_2d (double skin_) : i_object_3d (skin_)
  {
    return;
  }

  i_shape_2d::~i_shape_2d ()
  {
    return;
  }

} // end of namespace geomtools

// end of i_shape_2d.cc
