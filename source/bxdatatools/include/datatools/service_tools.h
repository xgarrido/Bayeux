/* service_tools.h
 * Author(s)     :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2011-06-07
 * Last modified : 2012-12-15
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
 *   Service tools, typedefs.
 * 
 * History: 
 * 
 */
#ifndef DATATOOLS_SERVICE_TOOLS_H_
#define DATATOOLS_SERVICE_TOOLS_H_

// Standard Library
#include <string>
#include <map>

// Third Party
// - Boost
#include <boost/cstdint.hpp>

// Datatools
#include <datatools/handle.h>
#include <datatools/i_tree_dump.h>
#include <datatools/properties.h>

namespace datatools {

class base_service;
typedef datatools::handle<base_service> service_handle_type;

// Constants to measure the level of dependance between services
enum dependency_level_type {
  DEPENDENCY_NONE     =  0, //!< The service does not depend on the external service
  DEPENDENCY_OPTIONAL =  1, //!< The service can work without the external service
  DEPENDENCY_WEAK     =  2, //!< Not so strong dependency on the external service (however part of the service may be invalidated)
  DEPENDENCY_STRICT   =  3, //!< Strictly depends on the external service
  DEPENDENCY_UNKNOWN  = -1
};

//! \brief Record that stores informations about the dependency between services :
struct dependency_info_type {
  std::string id;      //!< ID of the external service
  std::string version; //!< Version of the external service
  std::string meta;    //!< Auxiliary information 
  int level;           //!< Level of the dependency (see dependency_level_type enum)
  dependency_info_type();
};

// HA! The typedef names are JUST AS LONG!!!!
typedef std::map<std::string, int> dependency_level_dict_type;
typedef std::map<std::string, dependency_info_type> service_dependency_dict_type;

class service_manager;

// Record that handles a dynamically allocated service and additional 
// informations :

//! \brief Internal entry for service objects stored in the service manager class
class service_entry : public datatools::i_tree_dumpable  {
 public:
  enum status_type {
    STATUS_BLANK             = 0x0,
    STATUS_CREATED           = 0x1,
    STATUS_INITIALIZED       = 0x2,
    STATUS_BROKEN_DEPENDENCY = 0x4
  };

 public:

  const std::string & get_service_name () const;

  void set_service_name (const std::string &);

  const std::string & get_service_id () const;

  void set_service_id (const std::string &);

  const datatools::properties & get_service_config () const;

  datatools::properties & grab_service_config ();

  void set_service_config (const datatools::properties &);

  service_entry();

  bool can_be_dropped() const;

  uint32_t get_service_status() const;

  void update_service_status(uint32_t);

  void reset_service_status(uint32_t);

  bool is_created() const;

  bool is_initialized() const;

  bool has_slave(const std::string& name) const;

  void remove_slave(const std::string& name);

  virtual void tree_dump(std::ostream& out = std::clog, 
                         const std::string & title  = "",
                         const std::string & indent = "",
                         bool inherit = false) const;

  const service_handle_type & get_service_handle() const;

  service_handle_type & grab_service_handle();

 public: // TO BE CHANGED
 private: 
  // WHY ARE THESE PUBLIC?? service_entry has functionality, it's
  // not just a bag of data...
  // Either that, or it should be a pImpl of service_manager
  //
  std::string  service_name;    //!< The name of the service
  std::string  service_id;      //!< The ID (type) of the service
  datatools::properties service_config;  //!< The configuration of the service 
  uint32_t service_status;  //!< The status of the service
  service_handle_type service_handle;  //!< The handle for the allocated service
 public:
  service_dependency_dict_type service_masters; //!< The list of services the service depends on (by names)
  dependency_level_dict_type   service_slaves;  //!< The list of depending services (by names)

  //friend class service_manager;
};

typedef std::map<std::string, service_entry> service_dict_type;

}  // end of namespace datatools

#endif // DATATOOLS_SERVICE_TOOLS_H_

