// -*- mode: c++; -*- 
/* primary_event.h
 * Author(s):     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2010-04-11
 * Last modified: 2010-04-11
 * 
 * License: 
 * Copyright 2007-2011 F. Mauger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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
 * Description: 
 *
 *   Primary event from the GENBB generator.
 *
 * 
 * History: 
 *
 * 2012-06-21 FM : 
 *  - support 'weighted' MC event in case 
 *    of user energy range for DBD events
 *  - serialization version 2 supports new 'weight' attributes
 *
 */

#ifndef __genbb_help__primary_event_h
#define __genbb_help__primary_event_h 1

#include <string>
#include <list>

// Interface base class from datatools to support serialization tools:
#include <datatools/serialization/i_serializable.h>

#include <datatools/utils/units.h>
#include <datatools/utils/i_tree_dump.h>

#include <genbb_help/primary_particle.h>

namespace genbb {

  struct primary_event
    : DATATOOLS_SERIALIZABLE_CLASS,
      public datatools::utils::i_tree_dumpable
  {
    
  public:
    typedef std::list<primary_particle> particles_col_t; // to be removed
    typedef particles_col_t particles_col_type;

  public:

    bool is_valid () const;

    void reset ();

    void set_time (double);

    double get_time () const;

    void set_genbb_weight (double genbb_weight_);

    double get_genbb_weight () const;

    bool is_genbb_weighted () const;

    const particles_col_t & get_particles () const;

    particles_col_t & get_particles ();

    void add_particle (const primary_particle &);

    const std::string & get_label () const;

    void set_label (const std::string & l_);

    void reset_label ();

    const std::string & get_classification () const;

    void set_classification (const std::string & c_);

    void reset_classification ();

    void compute_classification ();

    void rotate (double a_phi, double a_theta, double a_delta);

    double get_total_kinetic_energy () const;

    /// Constructor 
    primary_event ();
    
    /// Destructor
    virtual ~primary_event ();

    virtual void
    tree_dump (std::ostream      & out_    = std::clog,
               const std::string & title_  = "",
               const std::string & indent_ = "",
               bool inherit_               = false) const;
    
    void dump (std::ostream & a_out = std::clog,
               const std::string & a_indent = "") const;

    void dump (std::ostream & a_out,
               const std::string & a_title,
               const std::string & a_indent) const;
    
  public:

    std::string     label;
    double          time;
    particles_col_t particles;
    std::string     classification;
    double          genbb_weight;

    /* interface i_serializable */
    DATATOOLS_SERIALIZATION_DECLARATION();

  public:

    // trick to support old tag :
    static const std::string OLD_SERIAL_TAG;

  };

} // end of namespace genbb

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(genbb::primary_event, "genbb::primary_event")

#endif // __genbb_help__primary_event_h

// end of primary_event.h
