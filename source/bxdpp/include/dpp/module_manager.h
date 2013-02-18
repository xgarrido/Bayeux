/* module_manager.h
 * Author(s)     :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2011-06-22
 * Last modified : 2013-02-15
 *
 * Copyright (C) 2011-2013 Francois Mauger <mauger@lpccaen.in2p3.fr>
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
 *   A manager/factory class for event record processing modules.
 *
 * History:
 *
 */

#ifndef DPP_MODULE_MANAGER_H_
#define DPP_MODULE_MANAGER_H_ 1

#include <string>

#include <boost/cstdint.hpp>

#include <dpp/base_module.h>
#include <dpp/module_tools.h>

namespace dpp {

  /// \brief A manager for data processing modules
  class module_manager : public DPP_DU::i_tree_dumpable
  {
  public:

    enum ctor_flag_type
      {
        BLANK             = 0,
        DEBUG             = DPP_DU::bit_mask::bit00,
        FACTORY_DEBUG     = DPP_DU::bit_mask::bit01,
        FACTORY_NOPRELOAD = DPP_DU::bit_mask::bit02,
        FACTORY_INITIALIZATION_AT_LOAD = DPP_DU::bit_mask::bit03,
        VERBOSE           = DPP_DU::bit_mask::bit04
      };

    bool is_debug () const;

    void set_debug (bool debug_);

    bool is_verbose () const;

    void set_verbose (bool verbose_);

    bool is_no_preload () const;

    bool is_initialization_at_load () const;

    bool has (const std::string & module_name_) const;

    void remove (const std::string & module_name_);

    base_module & grab (const std::string & module_name_);

    const base_module & get (const std::string & module_name_) const;

    module_handle_dict_type & get_modules ();

    const module_handle_dict_type & get_modules () const ;

    bool is_initialized () const;

    void initialize (const DPP_DU::properties & setup_);

    void reset ();

    module_manager (uint32_t flags_ = BLANK);

    virtual ~module_manager ();

    bool has_service_manager () const;

    const DPP_DS::service_manager & get_service_manager () const;

    DPP_DS::service_manager & grab_service_manager ();

    void set_service_manager (DPP_DS::service_manager & service_manager_);

    void install_service_manager (const DPP_DU::properties & service_manager_configuration_);

    virtual void tree_dump (std::ostream & out_         = std::clog,
                            const std::string & title_  = "",
                            const std::string & indent_ = "",
                            bool inherit_               = false) const;

    void load_module (const std::string & module_name_,
                      const std::string & module_id_,
                      const DPP_DU::properties & module_config_);
         
    void create_module (module_entry_type & module_entry_);
        
    void initialize_module (module_entry_type & module_entry_);
 
    void load_modules (const DPP_DU::multi_properties & modules_config_);
       
  protected:

    void _load_module (const std::string & module_name_,
                       const std::string & module_id_,
                       const DPP_DU::properties & module_config_);

    void _load_modules (const DPP_DU::multi_properties & modules_config_);

    void _create_module (module_entry_type & module_entry_);
        
    void _initialize_module (module_entry_type & module_entry_);
        
    void _reset_module (module_entry_type & module_entry_);

    bool has_module_type (const std::string & module_id_) const;
        
    template <class ModuleClass>
    void register_module_type (const std::string & module_id_)
    {
      _factory_register_.registration (module_id_, boost::factory<ModuleClass*>());
      return;
    }
        
    void unregister_module_type (const std::string & module_id_);

  protected:

    void _preload_global_dict ();
        
  private:

    bool                    _initialized_; //!< Initialization status
    uint32_t                _flags_;       //!< Some flags
    base_module::factory_register_type _factory_register_;      //!< Factory register
    module_handle_dict_type            _modules_;               //!< Dictionnary of modules
    bool                               _service_manager_owner_; //!< Owner flag for the embedded service manager
    DPP_DS::service_manager * _service_manager_;    //!< Handle to the embedded service manager

  };

}  // end of namespace dpp

#endif // DPP_MODULE_MANAGER_H_

// end of module_manager.h
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
