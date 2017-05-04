// placement.cc

// Ourselves:
#include <geomtools/placement.h>

// Standard library:
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <limits>

// Third party:
// - Bayeux/datatools:
#include <datatools/units.h>
#include <datatools/utils.h>

// This project:
#include <geomtools/utils.h>

namespace geomtools {

  using namespace std;

  DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(placement, "geomtools::placement")


  bool placement::is_replica () const
  {
    return false;
  }

  size_t placement::get_dimension () const
  {
    return 0;
  }

  bool placement::has_only_one_rotation () const
  {
    return true;
  }

  bool placement::is_valid () const
  {
    return
      geomtools::is_valid (_translation_)
      && geomtools::is_valid (_rotation_)
      && geomtools::is_valid (_inverse_rotation_);
  }

  void placement::invalidate ()
  {
    geomtools::invalidate (_translation_);
    geomtools::invalidate (_rotation_);
    geomtools::invalidate (_inverse_rotation_);
    _rotation_axis_ = ROTATION_AXIS_INVALID;
    _rotation_angle_ = std::numeric_limits<double>::quiet_NaN ();
    _phi_ = _theta_ = _delta_ = std::numeric_limits<double>::quiet_NaN ();
    return;
  }

  const vector_3d &
  placement::get_translation () const
  {
    return _translation_;
  }

  vector_3d &
  placement::grab_translation ()
  {
    return _translation_;
  }

  void
  placement::set_translation (double x_, double y_, double z_)
  {
    set_translation (vector_3d (x_, y_, z_));
    return;
  }

  void
  placement::set_translation_xyz (double x_, double y_, double z_)
  {
    set_translation (vector_3d (x_, y_, z_));
    return;
  }

  void
  placement::set_translation (const vector_3d & new_value_)
  {
    _translation_ = new_value_;
    return;
  }

  void
  placement::set_translation_v (const vector_3d & new_value_)
  {
    _translation_ = new_value_;
    return;
  }

  void placement::translate(double dx_, double dy_, double dz_)
  {
    translate(vector_3d(dx_, dy_, dz_));
    return;
  }


  void placement::translate(const vector_3d & shift_)
  {
    _translation_ += shift_;
    return;
  }

  const rotation_3d &
  placement::get_rotation () const
  {
    return _rotation_;
  }

  bool placement::is_simple_rotation () const
  {
    return is_rotation_x () || is_rotation_y () || is_rotation_z ();
  }

  bool placement::is_zyz_rotation () const
  {
    return ! is_simple_rotation ();
  }

  bool placement::is_rotation_x () const
  {
    return _rotation_axis_ == ROTATION_AXIS_X;
  }

  bool placement::is_rotation_y () const
  {
    return _rotation_axis_ == ROTATION_AXIS_Y;
  }

  bool placement::is_rotation_z () const
  {
    return _rotation_axis_ == ROTATION_AXIS_Z;
  }

  int placement::get_rotation_axis () const
  {
    return _rotation_axis_;
  }

  double placement::get_rotation_angle () const
  {
    return _rotation_angle_;
  }

  double placement::get_phi () const
  {
    return _phi_;
  }

  double placement::get_theta () const
  {
    return _theta_;
  }

  double placement::get_delta () const
  {
    return _delta_;
  }

  void placement::_compute_orientation_ ()
  {
    if (is_simple_rotation ()) {
      // X, Y or Z rotations:
      geomtools::create_rotation_from_axis (_rotation_, _rotation_axis_, _rotation_angle_);
    } else {
      // rotation by (Z,Y,Z) Euler angles:
      geomtools::create_rotation_from_zyz_euler_angles (_rotation_, _phi_, _theta_, _delta_);
    }
    geomtools::rectify (_rotation_);
    _inverse_rotation_ = _rotation_.inverse ();
    return;
  }

  void placement::_compute_orientation_xyz_ ()
  {
    if (is_simple_rotation ()) {
      // X, Y or Z rotations:
      geomtools::create_rotation_from_axis(_rotation_,
                                           _rotation_axis_,
                                           _rotation_angle_);
    } else {
      // rotation by (X,Y,Z) Euler angles:
      geomtools::create_xyz(_rotation_, _phi_, _theta_, _delta_);
    }
    geomtools::rectify (_rotation_);
    _inverse_rotation_ = _rotation_.inverse ();
    return;
  }

  bool placement::has_angles () const
  {
    if (is_simple_rotation ()) {
      return _rotation_angle_ == _rotation_angle_;
    }
    // else:
    return ((_phi_ == _phi_) && (_theta_ == _theta_) && (_delta_ == _delta_));
  }

  void placement::set (double x_, double y_, double z_,
                       double phi_, double theta_, double delta_)
  {
    set_translation (x_, y_, z_);
    set_orientation (phi_, theta_, delta_);
    return;
  }

  void placement::set(double x_, double y_, double z_,
                      double angle0_, double angle1_, double angle2_,
                      euler_angles_type euler_angle_)
  {
    set_translation (x_, y_, z_);
    set_orientation (angle0_, angle1_, angle2_, euler_angle_);
    return;
  }

  void placement::set (const vector_3d & t_,
                       double phi_, double theta_, double delta_)
  {
    set_translation (t_);
    set_orientation (phi_, theta_, delta_);
    return;
  }

  void placement::set (const vector_3d & t_,
                       double angle0_, double angle1_, double angle2_,
                       euler_angles_type euler_angle_)
  {
    set_translation (t_);
    set_orientation (angle0_, angle1_, angle2_, euler_angle_);
    return;
  }

  void placement::set (double x_, double y_, double z_,
                       int axis_, double angle_)
  {
    set_translation (x_, y_, z_);
    set_orientation (axis_, angle_);
    return;
  }

  void placement::set (const vector_3d & t_,
                       int axis_, double angle_)
  {
    set_translation (t_);
    set_orientation (axis_, angle_);
    return;
  }

  void placement::set_orientation_axis_angle (int axis_, double angle_)
  {
    set_orientation(axis_,angle_);
  }

  void placement::set_orientation (int axis_, double angle_)
  {
    DT_THROW_IF (! check_rotation_axis (axis_), std::logic_error, "Invalid rotation axis !");
    _rotation_axis_ = axis_;
    _rotation_angle_ = angle_;
    _phi_ = std::numeric_limits<double>::quiet_NaN ();
    _theta_ = std::numeric_limits<double>::quiet_NaN ();
    _delta_ = std::numeric_limits<double>::quiet_NaN ();
    _compute_orientation_ ();
    return;
  }

  void placement::set_orientation (double phi_, double theta_, double delta_)
  {
    _rotation_axis_ = ROTATION_AXIS_INVALID;
    _rotation_angle_ = std::numeric_limits<double>::quiet_NaN ();
    _phi_ = phi_;
    _theta_ = theta_;
    _delta_ = delta_;
    _compute_orientation_ ();
    return;
  }

  void placement::set_orientation(double angle0_, double angle1_, double angle2_,
                                  euler_angles_type euler_angle_)
  {
    _rotation_axis_ = ROTATION_AXIS_INVALID;
    _rotation_angle_ = std::numeric_limits<double>::quiet_NaN();
    _phi_   = std::numeric_limits<double>::quiet_NaN();
    _theta_ = std::numeric_limits<double>::quiet_NaN();
    _delta_ = std::numeric_limits<double>::quiet_NaN();
    create(_rotation_, angle0_, angle1_, angle2_, euler_angle_);
    _inverse_rotation_ = _rotation_.inverse();
    // extract_zyz_euler_angle_from_rotation(_rotation_,
    //                                       _phi_,
    //                                       _theta_,
    //                                       _delta_);
    return;
  }

  void placement::set_orientation_zyz (double phi_, double theta_, double delta_)
  {
    set_orientation(phi_, theta_, delta_);
  }

  // Not recommended at all:
  void placement::set_orientation (const rotation_3d & r_)
  {
    _rotation_axis_ = ROTATION_AXIS_INVALID;
    _rotation_angle_ = std::numeric_limits<double>::quiet_NaN();
    _phi_   = std::numeric_limits<double>::quiet_NaN();
    _theta_ = std::numeric_limits<double>::quiet_NaN();
    _delta_ = std::numeric_limits<double>::quiet_NaN();
    _rotation_ = r_;
    rectify(_rotation_);
    _inverse_rotation_ = _rotation_.inverse();
    extract_zyz_euler_angle_from_rotation(_rotation_,
                                          _phi_,
                                          _theta_,
                                          _delta_);
    return;
  }

  void placement::set_orientation_xyz (double phi_, double theta_, double delta_)
  {
    _rotation_axis_ = ROTATION_AXIS_INVALID;
    _rotation_angle_ = std::numeric_limits<double>::quiet_NaN ();
    _phi_ = phi_;
    _theta_ = theta_;
    _delta_ = delta_;
    _compute_orientation_xyz_ ();
    extract_zyz_euler_angle_from_rotation (_rotation_,
                                           _phi_,
                                           _theta_,
                                           _delta_);
    return;
  }

  const rotation_3d & placement::get_inverse_rotation () const
  {
    return _inverse_rotation_;
  }

  size_t placement::get_number_of_items () const
  {
    return 1;
  }

  size_t placement::compute_index_map (vector<uint32_t> & map_,
                                       int item_) const
  {
    DT_THROW_IF ((item_ < 0) || (item_ >= (int)get_number_of_items ()), std::domain_error,
                 "Invalid item index '" << item_ << "' !");
    map_.clear ();
    map_.push_back (item_);
    return map_.size ();
  }

  void placement::get_placement(int item_, placement & p_) const
  {
    DT_THROW_IF (item_ != 0, std::logic_error, "Invalid item index '" << item_ << "' !");
    p_ = *this;
  }

  bool placement::is_identity() const
  {
    if (! ::geomtools::is_valid(_translation_)) {
      return false;
    }
    if (_translation_.mag() > 0.0) {
      return false;
    }
    if (_rotation_axis_ != ROTATION_AXIS_INVALID) {
      if (_rotation_angle_ != 0.0) {
        return false;
      }
    } else {
      if (_phi_ != 0.0) return false;
      if (_theta_ != 0.0) return false;
      if (_delta_ != 0.0) return false;
    }
    return true;
  }

  void placement::set_identity()
  {
    set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    return;
  }

  placement::placement () : i_placement ()
  {
    set_identity();
    return;
  }

  placement::placement (const vector_3d & translation_) : i_placement ()
  {
    set_translation(translation_);
    set_orientation(0.0, 0.0, 0.0);
    return;
  }

  placement::placement (double x_,
                        double y_,
                        double z_) : i_placement ()
  {
    set_translation(x_, y_, z_);
    set_orientation(0.0, 0.0, 0.0);
    return;
  }

  placement::placement (const vector_3d & translation_,
                        double phi_,
                        double theta_,
                        double delta_) : i_placement ()
  {
    set_translation (translation_);
    set_orientation (phi_, theta_, delta_);
    return;
  }

  placement::placement (const vector_3d & translation_,
                        int axis_,
                        double angle_) : i_placement ()
  {
    set_translation(translation_);
    set_orientation(axis_, angle_);
    return;
  }

  placement::placement (double x_,
                        double y_,
                        double z_,
                        double phi_,
                        double theta_,
                        double delta_) : i_placement ()
  {
    set_translation(x_, y_, z_);
    set_orientation(phi_, theta_, delta_);
    return;
  }

  placement::placement (double x_,
                        double y_,
                        double z_,
                        int axis_,
                        double angle_) : i_placement ()
  {
    set_translation(x_, y_, z_);
    set_orientation(axis_, angle_);
    return;
  }

  placement::placement(const vector_3d & translation_,
                       const rotation_3d & rotation_)
  {
    set_translation(translation_);
    set_orientation(rotation_);
    return;
  }

  placement::placement(double x_,
                       double y_,
                       double z_,
                       double angle0_,
                       double angle1_,
                       double angle2_,
                       euler_angles_type euler_angles_)
  {
    set_translation(x_, y_, z_);
    set_orientation(angle0_, angle1_, angle2_, euler_angles_);
    return;
  }

  placement::placement(const vector_3d & translation_,
                       double angle0_,
                       double angle1_,
                       double angle2_,
                       euler_angles_type euler_angles_)
  {
    set_translation(translation_);
    set_orientation(angle0_, angle1_, angle2_, euler_angles_);
    return;
  }

  placement::~placement ()
  {
    return;
  }

  void placement::reset ()
  {
    set_identity();
    return;
  }

  // transformation methods:

  void placement::mother_to_child (const vector_3d & mother_pos_,
                                   vector_3d & child_pos_) const
  {
    vector_3d v = mother_pos_ - _translation_;
    child_pos_ = v.transform (_rotation_);
    return;
  }

  vector_3d placement::mother_to_child (const vector_3d & mother_pos_) const
  {
    vector_3d v;
    mother_to_child (mother_pos_, v);
    return v;
  }

  void placement::child_to_mother (const vector_3d & child_pos_ ,
                                   vector_3d & mother_pos_) const
  {
    vector_3d v (child_pos_);
    mother_pos_ = v.transform (_inverse_rotation_) + _translation_;
    return;
  }

  vector_3d placement::child_to_mother (const vector_3d & child_pos_) const
  {
    vector_3d v;
    child_to_mother (child_pos_, v);
    return v;
  }

  void placement::mother_to_child_direction (const vector_3d & mother_dir_,
                                             vector_3d & child_dir_) const
  {
    vector_3d a;
    mother_to_child (vector_3d ( 0., 0., 0.), a);
    vector_3d b;
    mother_to_child (mother_dir_, b);
    child_dir_= b - a;
    return;
  }

  vector_3d placement::mother_to_child_direction (const vector_3d & mother_dir_) const
  {
    vector_3d v;
    mother_to_child_direction (mother_dir_,v);
    return v;
  }

  void placement::child_to_mother_direction (const vector_3d & child_dir_ ,
                                             vector_3d & mother_dir_) const
  {
    vector_3d a;
    child_to_mother (vector_3d (0., 0., 0.), a);
    vector_3d b;
    child_to_mother (child_dir_, b);
    mother_dir_= b - a;
    return;
  }

  vector_3d placement::child_to_mother_direction (const vector_3d & child_dir_) const
  {
    vector_3d v;
    child_to_mother_direction (child_dir_,v);
    return v;
  }

  /*
    uti  void placement::mother_to_child (const placement & pm_, placement & pc_) const
    {
    mother_to_child (pm_._translation_, pc_._translation_);
    pc_._phi_ = pc_._theta_ = pc_._delta_ = std::numeric_limits<double>::quiet_NaN ();
    pm_._rotation_ = pc_._rotation_ * pc_._rotation_;
    pc_._inverse_rotation_ = pc_._rotation_.inverse ();
    }
  */

  void placement::child_to_mother (const placement & pc_, placement & pm_) const
  {
    child_to_mother (pc_._translation_, pm_._translation_);
    pm_._phi_ = pm_._theta_ = pm_._delta_ = std::numeric_limits<double>::quiet_NaN ();
    pm_._inverse_rotation_ = _inverse_rotation_ * pc_._inverse_rotation_;
    pm_._rotation_ = pm_._inverse_rotation_.inverse ();

    // reconvert angles in ZYZ Euler representation from _rotation_:
    extract_zyz_euler_angle_from_rotation (pm_._rotation_,
                                           pm_._phi_,
                                           pm_._theta_,
                                           pm_._delta_);
    return;
  }

  void placement::relocate (const placement & wp2_, placement & lp2_) const
  {
    const placement & wp1 = *this;
    //clog << "DEVEL: placement::relocate: T1      = " <<  wp1.get_translation () << endl;
    //clog << "DEVEL: placement::relocate: T2      = " <<  wp2_.get_translation () << endl;
    vector_3d ref = wp2_.get_translation ();
    //clog << "DEVEL: placement::relocate: ref     = " << ref << endl;
    vector_3d new_ref;
    wp1.mother_to_child (ref, new_ref);
    //clog << "DEVEL: placement::relocate: new_ref = " << new_ref << endl;
    lp2_.set_translation (new_ref);
    lp2_._phi_ = lp2_._theta_ = lp2_._delta_ = std::numeric_limits<double>::quiet_NaN ();

    lp2_._rotation_         = wp2_._rotation_ * wp1._inverse_rotation_;
    lp2_._inverse_rotation_ = lp2_._rotation_.inverse ();

    // reconvert angles in ZYZ Euler representation from _rotation_:
    extract_zyz_euler_angle_from_rotation (lp2_._rotation_,
                                           lp2_._phi_,
                                           lp2_._theta_,
                                           lp2_._delta_);
    return;
  }

  void placement::tree_dump (ostream & out_,
                             const string & title_,
                             const string & indent_,
                             bool inherit_) const
  {
    string indent;
    if (! indent_.empty ()) indent = indent_;
    this->i_placement::tree_dump (out_, title_, indent, true);

    out_ << indent << datatools::i_tree_dumpable::tag << "Valid : "
         << is_valid ()  << endl;

    out_ << indent << datatools::i_tree_dumpable::tag << "Translation : "
         << _translation_  << endl;

    if (is_zyz_rotation ())
      {
        out_ << indent << datatools::i_tree_dumpable::tag << "Euler Angles (ZYZ): "
             << (has_angles ()? "Yes": "No") << endl;
        if (has_angles ())
          {
            out_ << indent << datatools::i_tree_dumpable::skip_tag
                 << datatools::i_tree_dumpable::tag << "Phi :   "
                 << _phi_ / CLHEP::degree << " degree" << endl;
            out_ << indent << datatools::i_tree_dumpable::skip_tag
                 << datatools::i_tree_dumpable::tag << "Theta : "
                 << _theta_ / CLHEP::degree << " degree"  << endl;
            out_ << indent << datatools::i_tree_dumpable::skip_tag
                 << datatools::i_tree_dumpable::last_tag << "Delta : "
                 << _delta_ / CLHEP::degree << " degree"  << endl;
          }
      }
    else
      {
        out_ << indent << datatools::i_tree_dumpable::tag << "Rotation axis : "
             << _rotation_axis_ << endl;
        out_ << indent << datatools::i_tree_dumpable::tag << "Rotation angle : "
             << _rotation_angle_ / CLHEP::degree << " degree" << endl;
      }

    {
      ostringstream oss_title;
      oss_title << indent << datatools::i_tree_dumpable::tag << "Rotation :";
      ostringstream oss_indent;
      oss_indent << indent << datatools::i_tree_dumpable::skip_tag;
      geomtools::tree_dump (_rotation_,
                            out_,
                            oss_title.str (),
                            oss_indent.str ());
    }

    {
      ostringstream oss_title;
      oss_title << indent << datatools::i_tree_dumpable::inherit_tag (inherit_) << "Inverse rotation :";
      ostringstream oss_indent;
      oss_indent << indent << datatools::i_tree_dumpable::inherit_skip_tag (inherit_);
      geomtools::tree_dump (_inverse_rotation_,
                            out_,
                            oss_title.str (),
                            oss_indent.str ());
    }

    return;
  }

  void
  placement::dump (ostream      & out_ ,
                   const string & title_,
                   const string & indent_) const
  {
    tree_dump (out_, title_, indent_);
  }

  void placement::to_string (string & str_, const placement & pl_)
  {
    ostringstream oss;
    double lunit = CLHEP::mm;
    double aunit = CLHEP::degree;
    oss << pl_.get_translation ().x () / lunit;
    oss << ' ' << pl_.get_translation ().y () / lunit;
    oss << ' ' << pl_.get_translation ().z () / lunit;
    oss << " (mm)";
    if (pl_.is_simple_rotation ())
      {
        oss << " / ";
        oss << pl_.get_rotation_axis ();
        oss << ' ' << pl_. get_rotation_angle () / aunit;
      }
    else
      {
        oss << " @ ";
        oss << pl_.get_phi () / aunit;
        oss << ' ' << pl_.get_theta () / aunit;
        oss << ' ' << pl_.get_delta () / aunit;
      }
    oss << " (deg)";
    str_ = oss.str ();
    return;
  }

  std::ostream & operator<< (std::ostream & out_, const placement & pl_)
  {
    std::string s;
    placement::to_string (s, pl_);
    out_ << s;
    return out_;
  }

  bool placement::from_string (const std::string & str_, placement & pl_)
  {
    pl_.reset ();
    std::istringstream iss (str_);
    double x (0.0), y (0.0), z (0.0);
    double length_unit = CLHEP::mm;
    double phi (0.0), theta (0.0), delta (0.0);
    double angle_unit = CLHEP::degree;

    iss >> std::ws;
    if (iss.eof ()) {
      return false;
    }

    // Position:
    iss >> x >> y >> z >> std::ws;
    // extract length unit:
    if (! iss.eof ()) {
        char open = iss.peek ();
        if (open == '(') {
            std::string length_unit_str;
            iss.get ();
            getline (iss, length_unit_str, ')');
            if (! iss) {
              return false;
            }
            length_unit = datatools::units::get_length_unit_from (length_unit_str);
          }
        iss >> std::ws;
      }
    x *= length_unit;
    y *= length_unit;
    z *= length_unit;

    std::string rotation_axis_label;
    int rotation_axis = -1;
    bool simple_rotation = false;
    iss >> std::ws;
    if (! iss.eof ()) {
        char pr_sep = iss.peek ();
        if (pr_sep != '@' && pr_sep != '/') {
            return false;
          }
        iss.get ();
        if (pr_sep == '@' ) {
            iss >> phi >> theta >> delta >> std::ws;
          }
        if (pr_sep == '/' )
          {
            simple_rotation = true;
            iss >> rotation_axis_label >> phi >> std::ws;
            rotation_axis = get_rotation_axis_from_label (rotation_axis_label);
          }
        // extract angle unit:
        if (! iss.eof ()) {
            char open = iss.peek ();
            if (open == '(') {
                std::string angle_unit_str;
                iss.get ();
                getline (iss, angle_unit_str, ')');
                if (! iss) {
                    return false;
                  }
                angle_unit = datatools::units::get_angle_unit_from (angle_unit_str);
              }
            iss >> std::ws;
          }

      }
    phi *= angle_unit;
    if (! simple_rotation) {
        theta *= angle_unit;
        delta *= angle_unit;
        pl_.set (x, y, z, phi, theta, delta);
      } else {
        pl_.set (x, y, z, rotation_axis, phi);
      }
    return true;
  }

  void placement::test () const
  {
    clog << "DEVEL: placement::test: " << endl;
    double phi, theta, delta;
    extract_zyz_euler_angle_from_rotation (_rotation_, phi, theta, delta);
    clog << "|-- phi   = " << get_phi () / CLHEP::degree << " deg" << endl;
    clog << "|-- theta = " << get_theta () / CLHEP::degree << " deg" << endl;
    clog << "|-- delta = " << get_delta () / CLHEP::degree << " deg" << endl;
    clog << "|-- test ZYZ: " << endl;
    clog << "|   |-- phi   = " << phi / CLHEP::degree << " deg" << endl;
    clog << "|   |-- theta = " << theta / CLHEP::degree << " deg" << endl;
    clog << "|   `-- delta = " << delta / CLHEP::degree << " deg" << endl;
    clog << "|--rotation: ";
    geomtools::tree_dump (_rotation_, clog, "", "|   ");
    clog << "|--Inverse rotation: ";
    geomtools::tree_dump (_inverse_rotation_, clog, "", "|   ");
    double a, b, c;
    extract_xyz_euler_angle_from_rotation (_rotation_, a, b, c);
    clog << "|-- test XYZ: " << endl;
    clog << "|   |-- a = " << a / CLHEP::degree << " deg" << endl;
    clog << "|   |-- b = " << b / CLHEP::degree << " deg" << endl;
    clog << "|   `-- c = " << c / CLHEP::degree << " deg" << endl;
    clog << "|-- placement XYZ: " << endl;
    placement pxyz;
    pxyz.set_translation (0.,0.,0.);
    pxyz.set_orientation_xyz (a, b, c);
    pxyz.tree_dump (clog, "", "|   ");
    clog << "`-- Placement: " << pxyz << endl;
    clog << endl;

    return;
  }

} // end of namespace geomtools

// end of placement.cc