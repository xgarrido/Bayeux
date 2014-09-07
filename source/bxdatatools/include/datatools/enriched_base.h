/// \file datatools/enriched_base.h
/* Author(s):     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2013-09-12
 * Last modified: 2013-11-25
 *
 * License:
 *
 * Copyright (C) 2013 Francois Mauger <mauger@lpccaen.in2p3.fr>
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
 *   A base class for complex objects with several
 *   basic attributes.
 *
 */

#ifndef DATATOOLS_ENRICHED_BASE_H
#define DATATOOLS_ENRICHED_BASE_H

// Standard library:
#include <string>

// This project:
#include <datatools/datatools_config.h>
#include <datatools/i_serializable.h>
#include <datatools/i_tree_dump.h>
#include <datatools/logger.h>
#include <datatools/properties.h>
#include <datatools/reflection_macros.h>
#include <datatools/object_configuration_description.h>

namespace datatools {

  /// \brief A base class with useful attributes usable in many contexts
  class enriched_base  :
    DATATOOLS_SERIALIZABLE_CLASS,
    public datatools::i_tree_dumpable {

  public:

      /// Validate a name
      static bool validate_name(const std::string & candidate_name_);

      /// Default constructor
      enriched_base();

      /// Constructor
      enriched_base(const std::string & name_,
		    const std::string & display_name_ = "",
		    const std::string & terse_description_ = "",
		    logger::priority lp_ = logger::PRIO_WARNING);

      /// Destructor
      virtual ~enriched_base();

      /// Check if the name is not empty
      bool has_name() const;

      /// Set the name
      void set_name(const std::string & name_);

      /// Set the name
      enriched_base & set_name_c(const std::string & name_);

      /// Return the name
      const std::string & get_name() const;

      /// Check if the display name is empty
      bool has_display_name() const;

      /// Set the display name
      void set_display_name(const std::string & display_name_);

      /// Set the display name
      enriched_base & set_display_name_c(const std::string & display_name_);

      /// Return the display name
      const std::string & get_display_name() const;

      /// Check if the description is empty
      bool has_terse_description() const;

      /// Set the description
      void set_terse_description(const std::string & terse_description_);

      /// Set the description
      enriched_base & set_terse_description_c(const std::string & terse_description_);

      /// Return the description
      const std::string & get_terse_description() const;

      /// Check if the container of auxiliary properties is empty
      bool has_auxiliaries() const;

      /// Return a reference to the non mutable container of auxiliary properties
      const datatools::properties & get_auxiliaries() const;

      /// Return a reference to the mutable container of auxiliary properties
      datatools::properties & grab_auxiliaries();

      /// Remove all auxiliary properties
      void reset_auxiliaries();

      /// Check the debug status
      bool is_debug() const;

      /// Set the logging priority threshold
      void set_logging_priority(logger::priority lp_);

      /// Set the logging priority threshold
      enriched_base & set_logging_priority_c(logger::priority lp_);

      /// Return the logging priority threshold
      logger::priority get_logging_priority() const;

      /// Clear
      void clear();

      /// Smart print
      virtual void tree_dump(std::ostream& out_ = std::clog,
			     const std::string& title_  = "",
			     const std::string& indent_ = "",
			     bool inherit_ = false) const;

      /// Initialization from a container of properties
      void initialize(const datatools::properties & config_, bool clear_ = false);

      /// OCD support
      static void init_ocd(datatools::object_configuration_description &);

  private:

      std::string _name_; //!< Name
      std::string _display_name_; //!< Display name
      std::string _terse_description_; //!< Description
      logger::priority _logging_priority_; //!< Logging priority threshold
      datatools::properties _auxiliaries_; //!< Container of auxiliary properties

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(enriched_base);

      //! Reflection interface
      DR_CLASS_RTTI();

    };

} // end of namespace datatools

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(datatools::enriched_base, "datatools::enriched_base");
// Activate reflection layer for the 'datatools::enriched_base' class:
DR_CLASS_INIT(::datatools::enriched_base);

#endif // DATATOOLS_ENRICHED_BASE_H
