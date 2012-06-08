/* cut_tools.h
 * Author(s)     :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2011-06-07
 * Last modified : 2012-06-08
 * 
 * Copyright (C) 2011 Francois Mauger <mauger@lpccaen.in2p3.fr>
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
 * 
 * 
 * Description: 
 *
 *   Typedefs for the cut factory/manager.
 * 
 * History: 
 * 
 */

#ifndef __cuts__cut_tools_h
#define __cuts__cut_tools_h 1

#include <iostream>
#include <string>
#include <map>
#include <exception>

#include <boost/scoped_ptr.hpp>

#include <datatools/utils/handle.h>
#include <datatools/utils/i_tree_dump.h>
#include <datatools/utils/properties.h>
#include <datatools/utils/bit_mask.h>

namespace datatools {
  namespace utils {
    class properties;
  }
  namespace service {
    class service_manager;
  }
}

namespace cuts {

  class i_cut;
 
  typedef std::map<std::string, i_cut *> cut_dict_type;

  typedef datatools::utils::handle<i_cut> cut_handle_type;

  class cut_manager;

  /** Record that handles a dynamically allocated cut and additional setup/status informations 
   */
  class cut_entry_type : public datatools::utils::i_tree_dumpable 
  {
  public:
    
    /// Status of the cut entry
    enum status_type
      {
        STATUS_BLANK       = 0,
        STATUS_CREATED     = datatools::utils::bit_mask::bit00,
        STATUS_INITIALIZED = datatools::utils::bit_mask::bit01,
      };
    
    std::string                  cut_name;    //!< The name of the cut
    std::string                  cut_id;      //!< The ID (type) of the cut
    datatools::utils::properties cut_config;  //!< The configuration of the cut 
    uint32_t                     cut_status;  //!< The status of the cut
    cut_handle_type              cut_handle;  //!< The handle for the allocated cut


    const datatools::utils::properties & get_cut_config () const;

    datatools::utils::properties & grab_cut_config ();

    void set_cut_config (const datatools::utils::properties &);

    const std::string & get_cut_name () const;

    void set_cut_name (const std::string &);

    const std::string & get_cut_id () const;

    void set_ptr (i_cut *);

    void set_cut_id (const std::string &);
        
    bool has_manager () const;

    void set_manager (cut_manager &);
      
    void set_blank ();
         
    void set_created ();
       
    void set_initialized ();
       
    void set_uninitialized ();

    bool is_created () const;

    bool is_initialized () const;

    /// Constructor
    cut_entry_type ();

    /// Check is the cut is referenced in the embedded handle
    bool has_cut () const;

    const i_cut & get_cut () const;
 
    i_cut & grab_cut ();

    const cut_handle_type & get_cut_handle () const;

    cut_handle_type & grab_cut_handle ();

    cut_handle_type & grab_initialized_cut_handle ();

    /// Smart print
    virtual void tree_dump (std::ostream & out_         = std::clog, 
                            const std::string & title_  = "",
                            const std::string & indent_ = "",
                            bool inherit_               = false) const;

  private:

    std::string                  _cut_name_;   /// Cut name
    std::string                  _cut_id_;     /// Cut class registration ID
    datatools::utils::properties _cut_config_; /// The configuration of the cut 
    uint32_t                     _cut_status_; /// The status of the cut
    cut_handle_type              _cut_handle_; /// The handle for the allocated service
    cut_manager *                _manager_;    /// Handle to the module manager

    friend class cut_manager;
   
  };
  
  typedef std::map<std::string, cut_entry_type> cut_handle_dict_type;

} // end of cuts

#endif // __cuts__cut_tools_h

// end of cut_tools.h
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
