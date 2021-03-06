// -*- mode: c++; -*-
// genbb_writer.cc
/*
 * Copyright 2007-2011 F. Mauger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public  License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <genbb_help/genbb_writer.h>

#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include <list>

#include <datatools/exception.h>

#include <genbb_help/primary_event.h>

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Vector/ThreeVector.h>

namespace genbb {

  void genbb_writer::set_seed ( int seed_ )
  {
    _seed_ = seed_;
  }

  int genbb_writer::get_seed() const
  {
    return _seed_;
  }

  void genbb_writer::set_npart ( int npart_ )
  {
    _npart_ = npart_;
  }

  int genbb_writer::get_npart() const
  {
    return _npart_;
  }

  void genbb_writer::set_part_per_evt ( int part_per_evt_ )
  {
    _part_per_evt_ = part_per_evt_;
  }

  int genbb_writer::get_part_per_evt() const
  {
    return _part_per_evt_;
  }

  void genbb_writer::set_type ( int type_ )
  {
    _type_ = type_;
  }

  int genbb_writer::get_type() const
  {
    return _type_;
  }

  void genbb_writer::set_Emin ( double Emin_ )
  {
    _emin_ = Emin_;
  }

  double genbb_writer::get_Emin() const
  {
    return _emin_;
  }

  void genbb_writer::set_Emax ( double Emax_ )
  {
    _emax_ = Emax_;
  }

  double genbb_writer::get_Emax() const
  {
    return _emax_;
  }

  void genbb_writer::set_phi_min ( double phi_min_ )
  {
    _phi_min_ = phi_min_;
  }

  double genbb_writer::get_phi_min() const
  {
    return _phi_min_;
  }

  void genbb_writer::set_phi_max ( double phi_max_ )
  {
    _phi_max_ = phi_max_;
  }

  double genbb_writer::get_phi_max() const
  {
    return _phi_max_;
  }

  void genbb_writer::set_outfile ( std::string fout_ )
  {
    _fout_ = fout_;
  }

  std::string genbb_writer::get_outfile() const
  {
    return _fout_;
  }

  void genbb_writer::set_debug ( bool d_ )
  {
    _debug_ = d_;
  }

  bool genbb_writer::is_debug () const
  {
    return _debug_;
  }

  void genbb_writer::run ()
  {
    init ();

    std::ofstream out ( _fout_.c_str() );

    double ElectronMass = CLHEP::electron_mass_c2;
    double AlphaMass    = 3.72738 *CLHEP::GeV;

    for ( int i = 0; i < _npart_; i++ )
      {
        out << i << " "
            << 0 << " "
            << _part_per_evt_
            << std::endl;

        for ( int j = 0; j < _part_per_evt_; j++ )
          {

            double KineticEnergy      = myrandom ( _emin_, _emax_ );
            double cos_phi            = myrandom ( std::cos ( _phi_min_ ), std::cos ( _phi_max_ ) );
            double phi                = std::acos ( cos_phi );
            // double phi                = myrandom ( _phi_min_, _phi_max_ );

            double theta              = myrandom ( 0., 2*M_PI );
            double RelativisticEnergy = 0.;
            double TotalMomentum      = 0.;

            if ( _type_ == 2 || _type_ == 3 ) // e+ / e-
              {
                RelativisticEnergy = KineticEnergy + ElectronMass;
                TotalMomentum = std::sqrt( RelativisticEnergy * RelativisticEnergy
                                           - ElectronMass * ElectronMass );
              }
            else if ( _type_ == 47 )          // alpha
              {
                RelativisticEnergy = KineticEnergy + AlphaMass;
                TotalMomentum = std::sqrt( RelativisticEnergy * RelativisticEnergy
                                           - AlphaMass * AlphaMass );
              }
            else                              // gamma
              {
                TotalMomentum = KineticEnergy;
              }

            double x_mom = TotalMomentum * cos ( theta ) * sin ( phi );
            double y_mom = TotalMomentum * sin ( theta ) * sin ( phi );
            double z_mom = TotalMomentum * cos ( phi );

            out << _type_ << " "
                << x_mom << " "
                << y_mom << " "
                << z_mom << " "
                << 0
                << std::endl;
          }
      }
    return;
  }


  double genbb_writer::myrandom ( double min_, double max_ )
  {
    return ( max_ - min_ ) * drand48() + min_;
  }


  void genbb_writer::init ()
  {
    DT_THROW_IF (((_npart_ < 0)
                  || (_part_per_evt_ < 0)
                  || (_type_ == -1)
                  || (_emin_ < 0)
                  || (_emax_ < 0)
                  || (_phi_min_ < 0)
                      || (_phi_max_ > M_PI)),
                 std::logic_error,
                 "All the parameters of the writer are not well defined. Abort!");
    return;
  }


  void genbb_writer::dump ( std::ostream & out_ ) const
  {
    out_ << "genbb_writer::dump: " << std::endl;
    out_ << "|-- debug:         " << _debug_        << std::endl;
    out_ << "|-- outfile:       " << _fout_         << std::endl;
    out_ << "|-- Nb particles:  " << _npart_        << std::endl;
    out_ << "|-- part / evt:    " << _part_per_evt_ << std::endl;
    out_ << "|-- particle type: " << _type_         << std::endl;
    out_ << "|-- energy min:    " << _emin_         << std::endl;
    out_ << "|-- energy max:    " << _emax_         << std::endl;
    out_ << "|-- phi_min:       " << _phi_min_    << std::endl;
    out_ << "`-- phi_max:       " << _phi_max_    << std::endl;
    return;
  }


  // ctor:
  genbb_writer::genbb_writer ()
  {
    _debug_        = false;
    _seed_         = 314159;
    _npart_        = -1;
    _part_per_evt_ =  1;
    _type_         = -1;
    _emin_         = -1.;
    _emax_         = -1.;
    _phi_min_      =  1.;
    _phi_max_      = -1.;
    srand48 ( _seed_ );
    return;
  }

  // dtor:
  genbb_writer::~genbb_writer ()
  {
    return;
  }

} // end of namespace genbb

// end of genbb_writer.cc
