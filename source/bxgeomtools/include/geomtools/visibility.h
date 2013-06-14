// -*- mode: c++; -*-
/* visibility.h
 * Author(s):     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2008-05-23
 * Last modified: 2009-03-31
 *
 * License:
 *
 * Description:
 *  Utilities for visualization tools:
 *
 *  Some visibility properties are defined:
 *
 *
 * History:
 *
 */

#ifndef GEOMTOOLS_VISIBILITY_H_
#define GEOMTOOLS_VISIBILITY_H_ 1

#include <string>
#include <iostream>

namespace datatools {
  class properties;
}

namespace geomtools {

  class visibility
  {
  public:

    struct constants
    {
      std::string VISIBILITY_PREFIX;
      std::string VISIBILITY_HIDDEN_FLAG;
      std::string VISIBILITY_HIDDEN_ENVELOPE_FLAG;
      std::string VISIBILITY_DAUGHTERS_HIDDEN_FLAG;
      std::string VISIBILITY_COLOR_PROPERTY;
      std::string VISIBILITY_ENVELOPE_COLOR_PROPERTY;
      std::string VISIBILITY_WIRED_CYLINDER_FLAG;

      constants ();

      static const constants & instance ();

     };

  public:

    static std::string make_key (const std::string & flag_);

    static void extract (const datatools::properties & source_,
                         datatools::properties & target_);

    static bool has_flag (const datatools::properties & config_,
                          const std::string & flag_);

    static bool has_key (const datatools::properties & config_,
                         const std::string & key_);

    static bool is_shown (const datatools::properties & config_);

    static bool is_hidden (const datatools::properties & config_);

    static bool is_hidden_envelope (const datatools::properties & config_);

    static bool is_daughters_shown (const datatools::properties & config_);

    static bool is_daughters_hidden (const datatools::properties & config_);

    static bool has_color (const datatools::properties & config_);

    static std::string get_color (const datatools::properties & config_);

    static void set_color (datatools::properties & config_,
                           const std::string & color_);

    static bool has_envelope_color (const datatools::properties & config_);

    static std::string get_envelope_color (const datatools::properties & config_);

    static void set_envelope_color (datatools::properties & config_,
                                     const std::string & color_);

    static bool is_wired_cylinder (const datatools::properties & config_);

  };

}  // end of namespace geomtools

#endif // GEOMTOOLS_VISIBILITY_H_

// end of visibility.h
