// -*- mode: c++ ; -*- 
/* circle.cc
 */

#include <geomtools/circle.h>

#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <sstream>

#include <gsl/gsl_poly.h>

namespace geomtools {

  using namespace std;

  const string circle::CIRCLE_LABEL = "circle";

  string circle::get_shape_name () const
  {
    return CIRCLE_LABEL;
  }
  
  double circle::get_r () const
  {
    return _radius_;
  }
  
  double circle::get_radius () const
  {
    return get_r ();
  }
 
  void circle::set_diameter (double a_diameter)
  {
    set_r (a_diameter * 0.5);
    return;
  }

  double circle::get_diameter () const
  {
    return (_radius_ + _radius_);
  }
  
  void circle::set_r (double a_radius)
  {
    if (a_radius < 0.0 )
      {
        ostringstream message;
        message << "circle::set_r: Invalid '" << a_radius << "' R value!";
        throw logic_error (message.str ());
      }
    _radius_ = a_radius;
    return;
  }
   
  double circle::get_surface () const
  {
    return M_PI * _radius_ * _radius_;
  }

  double circle::get_circumference () const
  {
    return 2 * M_PI * _radius_;
  }
  
  bool circle::is_valid () const
  {
    return (_radius_ > 0.0);
  }
  
  // ctor:
  circle::circle ()
  {
    _radius_ = -1.0;
    return;
  }

  // ctor:
  circle::circle (double a_radius)
  {
    set_r (a_radius);
    return;
  }
  
  // dtor:
  circle::~circle ()
  {
    return;
  }

  void circle::tree_dump (ostream & a_out, 
                          const string & a_title, 
                          const string & a_indent, 
                          bool a_inherit) const
  {
    string indent;
    if (! a_indent.empty ()) indent = a_indent;
    i_object_3d::tree_dump (a_out, a_title, indent, true);

    a_out << indent << datatools::i_tree_dumpable::inherit_tag (a_inherit)  
         << "Radius : " << get_r () / CLHEP::mm << " mm" << endl;
    return;
  }

  bool circle::is_on_curve (const vector_3d &, 
                            double a_tolerance) const
  {
    bool on_curve = false;
    throw runtime_error ("circle::is_on_curve: Not implemented yet !");
    return on_curve;
  }

  vector_3d circle::get_direction_on_curve (const vector_3d & a_position) const
  {
    vector_3d dir;
    invalidate (dir);
    throw runtime_error ("circle::get_direction_on_curve: Not implemented yet !");
    return dir;
  }

  void circle::generate_wires (std::list<polyline_3d> & lpl_, 
                               const placement & p_, 
                               uint32_t options_) const
  {
    const int nsamples = 36;
    vector_3d vertex[nsamples];
    for (int i = 0; i < nsamples; i++)
      {
        double thetai = i * 2. * M_PI/nsamples;
        vertex[i].set (get_r () * std::cos (thetai),
                       get_r () * std::sin (thetai),
                       0.0);
      }
    {
      polyline_3d dummy;
      lpl_.push_back (dummy);
    }
    polyline_3d & pl = lpl_.back ();
    pl.set_closed (true);
    for (int i = 0; i < 36; i++)
      {
        vector_3d v;
        p_.child_to_mother (vertex[i], v);
        pl.add (v);
      }
    return;
  }
  
} // end of namespace geomtools

// end of circle.cc
