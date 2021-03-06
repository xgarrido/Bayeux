// disk.cc

// Ourselves:
#include <geomtools/disk.h>

// Standard library:
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>

// Third party:
// - GSL:
#include <gsl/gsl_math.h>
#include <gsl/gsl_poly.h>
// - Bayeux/datatools:
#include <datatools/utils.h>
#include <datatools/units.h>

// This project:
#include <geomtools/circle.h>
#include <geomtools/line_3d.h>

namespace geomtools {

  // Registration :
  GEOMTOOLS_OBJECT_3D_REGISTRATION_IMPLEMENT(disk, "geomtools::disk")

  const std::string & disk::disk_label()
  {
    static std::string label;
    if (label.empty()) {
      label = "disk";
    }
    return label;
  }

  void disk::_set_defaults()
  {
    datatools::invalidate(_inner_radius_);
    datatools::invalidate(_outer_radius_);
    _angle_domain_.set_type(angular_range::RANGE_TYPE_AZIMUTHAL);
    _angle_domain_.reset_partial_angles();
    return;
  }

  std::string disk::get_shape_name () const
  {
    return disk_label();
  }

  void disk::set_outer_r(double new_value_)
  {
    DT_THROW_IF (new_value_ < 0.0,
                 std::domain_error,
                 "Invalid '" << new_value_ << "' R value !");
    if (has_inner_r()) {
      DT_THROW_IF (new_value_ < _inner_radius_,
                   std::range_error,
                   "Invalid '" << new_value_ << "' outer radius value, lower than inner radius !");
    }
    _outer_radius_ = new_value_;
    return;
  }

  double disk::get_outer_r() const
  {
    return _outer_radius_;
  }

  void disk::set_r(double new_value_)
  {
    set_outer_r(new_value_);
    return;
  }

  double disk::get_r () const
  {
    return get_outer_r();
  }

  double disk::get_radius () const
  {
    return get_outer_r();
  }

  void disk::set_diameter(double new_value_)
  {
    set_outer_r(new_value_ * 0.5);
    return;
  }

  double disk::get_diameter() const
  {
    return (_outer_radius_ + _outer_radius_);
  }

  bool disk::has_inner_r() const
  {
    return datatools::is_valid(_inner_radius_);
  }

  double disk::get_inner_r () const
  {
    return _inner_radius_;
  }

  void disk::set_inner_r (double new_value_)
  {
    DT_THROW_IF (new_value_ < 0.0,
                 std::domain_error,
                 "Invalid '" << new_value_ << "' inner radius value !");
    if (datatools::is_valid(_outer_radius_)) {
      DT_THROW_IF (new_value_ > _outer_radius_,
                   std::range_error,
                   "Invalid '" << new_value_ << "' inner radius value, greater than outer radius !");
    }
    _inner_radius_ = new_value_;
    return;
  }

  const angular_range & disk::get_angle_domain() const
  {
    return _angle_domain_;
  }

  bool disk::has_partial_angle() const
  {
    return _angle_domain_.is_partial();
  }

  bool disk::has_start_angle() const
  {
    return _angle_domain_.has_start_angle();
  }

  void disk::set_start_angle(double new_value_)
  {
    _angle_domain_.set_start_angle(new_value_);
    return;
  }

  double disk::get_start_angle() const
  {
    return _angle_domain_.get_start_angle();
  }

  bool disk::has_delta_angle() const
  {
    return _angle_domain_.has_delta_angle();
  }

  void disk::set_delta_angle(double new_value_)
  {
    _angle_domain_.set_delta_angle(new_value_);
    return;
  }

  double disk::get_delta_angle() const
  {
    return _angle_domain_.get_delta_angle();
  }

  double disk::get_surface (uint32_t /* flags_ */) const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    double surface;
    datatools::invalidate(surface);
    double s_ext = _outer_radius_ * _outer_radius_;
    double s_int = 0.0;
    if (has_inner_r()) {
      s_int = _inner_radius_ * _inner_radius_;
    }
    surface = 0.5 * _angle_domain_.get_angle_spread() * (s_ext - s_int);
    return surface;
  }

  double disk::get_perimeter(uint32_t /* flags_ */) const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    return get_circumference();
  }

  double disk::get_circumference() const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    double circum = datatools::invalid_real();
    double circ_ext = _outer_radius_;
    double circ_int = 0.0;
    double rad_start_stop = 0.0;
    if (has_inner_r()) {
      circ_int = _inner_radius_;
    }
    double angle = _angle_domain_.get_angle_spread();
    if (has_partial_angle()) {
      if (has_inner_r()) {
        rad_start_stop = circ_ext - circ_int;
      }
    }
    circum = angle * (circ_ext + circ_int) + 2 * rad_start_stop;
    return circum;
  }

  bool disk::is_valid () const
  {
    if (!datatools::is_valid(_outer_radius_)) {
      return false;
    }
    return _angle_domain_.is_valid();
  }

  void disk::initialize(const datatools::properties & config_, const handle_dict_type * objects_)
  {
    if (!is_valid()) {
      this->i_object_3d::_initialize(config_, objects_);

      double lunit = CLHEP::mm;
      if (config_.has_key("length_unit")) {
        const std::string lunit_str = config_.fetch_string("length_unit");
        lunit = datatools::units::get_length_unit_from(lunit_str);
      }

      DT_THROW_IF(! config_.has_key("inner_radius"), std::logic_error, "Missing disk 'inner_radius' property !");
      double inner_radius = config_.fetch_real("inner_radius");
      if (! config_.has_explicit_unit("inner_radius")) {
        inner_radius *= lunit;
      }

      DT_THROW_IF(! config_.has_key("outer_radius"), std::logic_error, "Missing disk 'outer_radius' property !");
      double outer_radius = config_.fetch_real("outer_radius");
      if (! config_.has_explicit_unit("outer_radius")) {
        outer_radius *= lunit;
      }

      set(inner_radius, outer_radius);

      datatools::properties angle_config;
      config_.export_and_rename_starting_with(angle_config, "angle.", "");
      if (angle_config.size()) {
        _angle_domain_.initialize(angle_config);
      } else {
        // deprecated ;
        double aunit = CLHEP::degree;
        if (config_.has_key("angle_unit")) {
          const std::string aunit_str = config_.fetch_string("angle_unit");
          aunit = datatools::units::get_angle_unit_from(aunit_str);
        }

        double start_angle = 0.0;
        double delta_angle = 2 * M_PI * CLHEP::radian;
        bool not_full_angle = false;
        if (config_.has_key ("start_angle")) {
          start_angle = config_.fetch_real ("start_angle");
          if (! config_.has_explicit_unit ("start_angle")) {
            start_angle *= aunit;
          }
          not_full_angle = true;
        }
        if (config_.has_key ("delta_angle")) {
          delta_angle = config_.fetch_real ("delta_angle");
          if (! config_.has_explicit_unit ("delta_angle")) {
            delta_angle *= aunit;
          }
          not_full_angle = true;
        }
        if (not_full_angle) {
          set_start_angle(start_angle);
          set_delta_angle(delta_angle);
        }
      }
    }

    return;
  }

  void disk::reset()
  {
    _set_defaults();
    this->i_object_3d::reset();
    return;
  }

  void disk::set(double inner_r_, double outer_r_)
  {
    reset();
    set_outer_r(outer_r_);
    set_inner_r(inner_r_);
    return;
  }

  void disk::set(double inner_r_, double outer_r_, double start_angle_, double delta_angle_)
  {
    reset();
    set_outer_r(outer_r_);
    set_inner_r(inner_r_);
    _angle_domain_.set_partial_angles(start_angle_, delta_angle_);
    return;
  }

  disk::disk ()
  {
    _set_defaults();
    return;
  }

  disk::disk (double outer_r_)
  {
    _set_defaults();
    set_outer_r(outer_r_);
    return;
  }

  disk::disk(double inner_r_, double outer_r_)
  {
    _set_defaults();
    set_outer_r(outer_r_);
    set_inner_r(inner_r_);
    return;
  }

  disk::disk(double inner_r_, double outer_r_, double start_angle_, double delta_angle_)
  {
    _set_defaults();
    set_outer_r(outer_r_);
    set_inner_r(inner_r_);
    _angle_domain_.set_partial_angles(start_angle_, delta_angle_);
    return;
  }

  disk::~disk ()
  {
    return;
  }

  bool disk::is_on_surface(const vector_3d & position_,
                           double tolerance_) const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    double z_tolerance = compute_tolerance(tolerance_);
    double half_z_tolerance = 0.5 * z_tolerance;
    double angular_tolerance = get_angular_tolerance ();
    double z = position_.z();
    if (std::abs(z) > half_z_tolerance) {
      return false;
    }
    double x = position_.x();
    double y = position_.y();
    if (has_partial_angle()) {
      double theta = std::atan2(y, x);
      if (!::geomtools::angle_is_in(theta,
                                    _angle_domain_.get_first_angle(),
                                    _angle_domain_.get_angle_spread(),
                                    angular_tolerance,
                                    true)) {
        return false;
      }
    }
    double r2 = gsl_pow_2(_outer_radius_);
    double rho2 = x * x + y * y;
    if (rho2 > r2) {
      return false;
    }
    if (has_inner_r()) {
      double r02 = gsl_pow_2(_inner_radius_);
      if (rho2 < r02) {
        return false;
      }
    }
    return true;
  }

  vector_3d disk::get_normal_on_surface(const vector_3d & position_,
                                        bool check_,
                                        double skin_) const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    vector_3d normal;
    geomtools::invalidate(normal);
    DT_THROW_IF(check_ && !is_on_surface(position_, skin_), std::logic_error,
                "Position " << position_ << " is not on surface!");
    normal.set(0.0, 0.0, 1.0);
    return normal;
  }

  bool disk::find_intercept (const vector_3d & from_,
                             const vector_3d & direction_,
                             face_intercept_info & intercept_,
                             double tolerance_) const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    intercept_.reset();
    double ux = direction_.x();
    double uy = direction_.y();
    double uz = direction_.z();
    double xf = from_.x();
    double yf = from_.y();
    double zf = from_.z();
    double z_tolerance = compute_tolerance(tolerance_);

    if (uz == 0.0) {
      return intercept_.is_ok();
    }

    intercept_.reset ();
    double lambda = - zf / uz;
    if (lambda < 0.0) {
      return intercept_.is_ok ();
    }
    double xi, yi, zi;
    zi = 0.0;
    xi = xf + lambda * ux;
    yi = yf + lambda * uy;
    vector_3d impact (xi, yi, zi);
    if (! is_on_surface(impact, z_tolerance)) {
      intercept_.is_ok ();
    } else {
      // int face = FACE_UP;
      // if (from_.z()  < 0.0) {
      //   face = FACE_DOWN;
      // }
      intercept_.grab_face_id().set_face_bit(FACE_UNIQUE);
      intercept_.set_impact(vector_3d(xi, yi, zi));
    }
    return intercept_.is_ok();
  }

  void disk::tree_dump (std::ostream & out_,
                        const std::string & title_,
                        const std::string & indent_,
                        bool inherit_) const
  {
    i_object_3d::tree_dump (out_, title_, indent_, true);

    out_ << indent_ << datatools::i_tree_dumpable::tag
         << "Inner radius : ";
    if (has_inner_r()) {
      out_ << get_inner_r() / CLHEP::mm << " mm";
    } else {
      out_ << "<none>";
    }
    out_ << std::endl;

    out_ << indent_ << datatools::i_tree_dumpable::tag
         << "Outer radius : " << get_outer_r() / CLHEP::mm << " mm" << std::endl;

    {
      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Angle domain : " << std::endl;
      std::ostringstream indent2;
      indent2 << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
      _angle_domain_.tree_dump(out_, "", indent2.str());
    }

    return;
  }

  void disk::generate_wires_self(wires_type & wires_,
                                 uint32_t options_) const
  {
    DT_THROW_IF(! is_valid(), std::logic_error, "Invalid disk!");
    bool no_ext_edge = (options_ & WR_DISK_NO_EXT_EDGE);
    bool no_int_edge = (options_ & WR_DISK_NO_INT_EDGE);
    bool no_start_angle_edge = (options_ & WR_DISK_NO_START_ANGLE_EDGE);
    bool no_stop_angle_edge  = (options_ & WR_DISK_NO_STOP_ANGLE_EDGE);

    uint32_t base_options = options_ & WR_BASE_MASK;

    uint32_t nsamples_r  = linear_sampling_from_options(base_options);
    double rmin = 0.0;
    double rmax = get_outer_r();
    if (has_inner_r()) {
      rmin = get_inner_r();
    }
    double dr = (rmax - rmin) / (nsamples_r - 1);

    for (size_t ir = 0; ir < nsamples_r; ir++) {
      if (has_inner_r()) {
        if (no_int_edge && (ir == 0)) continue;
      } else {
        if (ir == 0) continue;
      }
      if (no_ext_edge && (ir == (nsamples_r-1))) continue;
      double ri = rmin + ir * dr;
      circle arc(ri);
      if (has_partial_angle()) {
        arc.set_start_angle(get_start_angle());
        arc.set_delta_angle(get_delta_angle());
      }
      arc.generate_wires_self(wires_, base_options);
    }

    uint32_t nsamples_th = 0;
    if (has_start_angle()) {
      nsamples_th = 1;
    }
    if (options_ & i_wires_3d_rendering::WR_BASE_GRID) {
      nsamples_th = i_wires_3d_rendering::angular_sampling_from_options(base_options);
    }
    if (nsamples_th > 0) {
      uint32_t nth_max = nsamples_th;
      if (has_start_angle()) {
        nth_max++;
      }

      for (angular_range::iterator iter(_angle_domain_, nsamples_th);
           !iter;
           ++iter) {
        if (iter.is_at_first()) {
          if (!has_partial_angle()) continue;
          else if (no_start_angle_edge) continue;
        }
        if (iter.is_at_last()) {
          if (!has_partial_angle()) continue;
          else if (no_stop_angle_edge) continue;
        }
        int    it = iter.get_current_step();
        double thetai = iter.get_current_angle();
        double r_min = rmin;
        if (! has_inner_r() || r_min < 2 * dr) {
          size_t nth = nsamples_th;
          if (nth > angular_sampling(SL_LOW)) {
            int f0 = nth / angular_sampling(SL_LOW);
            int f1 = nth / (2 * angular_sampling(SL_LOW));
            int f2 = nth / (4 * angular_sampling(SL_LOW));
            int itest0 = (it % f0);
            if (itest0 != 0) {
              r_min = rmin + dr;
            }
            int itest1 = (it % f1);
            if (itest1 != 0) {
              r_min = rmin + 2 * dr;
            }
            int itest2 = (it % f2);
            if (itest2 != 0) {
              r_min = rmin + 3 * dr;
            }
          }
        }
        vector_3d v1(r_min * std::cos(thetai),
                     r_min * std::sin(thetai),
                     0.0);
        vector_3d v2(get_r() * std::cos(thetai),
                     get_r() * std::sin(thetai),
                     0.0);
        line_3d radial(v1, v2);
        radial.generate_wires_self(wires_, 0);
      }
    }
    return;
  }

} // end of namespace geomtools
