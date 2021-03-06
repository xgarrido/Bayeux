/// \file geomtools/geomtools_driver.h
/* Author(s) :    Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2013-08-24
 * Last modified: 2016-09-07
 *
 * License:
 *
 * Description:
 *
 *   Geomtools model factory and manager driver.
 *
 * History:
 *
 */

#ifndef GEOMTOOLS_GEOMTOOLS_DRIVER_H
#define GEOMTOOLS_GEOMTOOLS_DRIVER_H

// Standard Library:
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Third Party:
// - Boost:
#include <boost/cstdint.hpp>
// #include <boost/scoped_ptr.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>

// This project:
#include <geomtools/geomtools_config.h>
#include <geomtools/display_data.h>

/// Top-level namespace of the Bayeux/geomtools module library
namespace geomtools {

  class model_factory;
  class manager;
  class id_mgr;

  struct geomtools_driver_params {
  public:
    geomtools_driver_params();
    void reset();
    int parse(const std::vector<std::string> & argv_,
              std::ostream & out_ = std::clog);
    bool can_initialize() const;

  public:

    datatools::logger::priority logging;
    bool mute;
    bool interactive;
    std::vector<std::string> LL_dlls;
    std::string LL_config;
    std::string geo_mgr_config_file;
    std::vector<std::string> setup_filenames;
    bool mapping_requested;
    std::string categories_filename;
    std::string top_mapping_model_name;
    std::vector<std::string> mapping_only_categories;
    std::vector<std::string> mapping_excluded_categories;
    int32_t mapping_max_depth;
    std::string materials_plugin_name;

#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
    bool        visu;
    std::string visu_drawer_view;
    std::string visu_object_name;
    bool        visu_drawer_labels;
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

  };

  class geomtools_driver {

  public:

    /// Constructor
    geomtools_driver();

    /// Destructor
    virtual ~geomtools_driver();

    /// Check initialization flag
    bool is_initialized() const;

    /// Initialization from a dedicated structure
    void initialize(const geomtools_driver_params &);

    /// Reset
    void reset();

    /// Check for an embeded manager
    bool has_manager() const;

    /// Clear the error message stack
    void clear_error_message_stack();

    /// Return the error message stack
    const std::vector<std::string> & get_error_message_stack() const;

  private:

    void _error_message_stack_push_(const std::string & message_);

    void _error_message_stack_backtrace_();

  public:
    /*
    // Common command line interface ?
    int exec_command(const std::string & command_,
    const std::vector<std::string> & argv_,
    std::ostream & out_ = std::cout,
    std::ostream & log_ = std::clog);
    */

    // Initialization from a list of arguments
    int command_initialize(const std::vector<std::string> & argv_,
                           std::ostream & out_ = std::clog);

    /// Reset
    int command_reset();

    /// Print driver status
    int command_status(std::ostream & out_) const;

    /// Print manager status
    int command_print_manager(std::ostream & out_) const;

    /// Print factory status
    int command_print_factory(std::ostream & out_) const;

    /// Print mapping status
    int command_print_mapping(std::ostream & out_,
                              const std::string & mapping_name_ = "") const;

    /// Print the list of geometry models
    int command_print_list_of_models(std::ostream & out_, const std::string & print_models_options_ = "") const;

    /// Print the list of logicals volumes
    int command_print_list_of_logicals(std::ostream & out_, const std::string & print_logicals_options_ = "") const;

    /// Print the list of registered GIDs
    int command_print_list_of_gids(std::ostream & out_, const std::string & print_gids_options_ = "") const;

    /// Print the list of registered geometry categories
    int command_print_list_of_categories(std::ostream & out_, const std::vector<std::string> & argv_) const;

    /// Print a geometry model given by name
    int command_print_model(const std::string & model_name_, std::ostream & out_) const;

    /// Print a logical volume given by name
    int command_print_logical(const std::string & logical_name_, std::ostream & out_) const;

    /// Print the list of embedded display data
    int command_print_list_of_display_data(std::ostream & out_, const std::vector<std::string> & argv_) const;

    /// Load a display data
    int command_load_display_data(const std::vector<std::string> & argv_,
                                  std::ostream & out_ = std::clog);

    /// Unload a display data
    int command_unload_display_data(const std::vector<std::string> & argv_,
                                    std::ostream & out_ = std::clog);

    /// Unload several/all display data
    int command_clear_display_data(const std::vector<std::string> & argv_,
                                   std::ostream & out_ = std::clog);

    // /// Set the rendering options attached to a logical volume given by name
    // int command_set_rendering_options(const std::vector<std::string> & argv_,
    //                                   std::ostream & out_) const;

    /// Export to GDML format
    int command_export_gdml(const std::vector<std::string> & argv_,
                            std::ostream & out_ = std::clog) const;

#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
    /// Gnuplot 2D/3D display of the geometry setup
    int command_gnuplot_display(const std::vector<std::string> & argv_,
                                std::ostream & out_ = std::clog);
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
    /// Parse the gnuplot 2D/3D view
    static std::string get_drawer_view (const std::string & view_label_);
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

    /// Export to GDML format
    static int export_gdml(const geomtools::manager & geo_mgr_,
                           const std::string & materials_plugin_name_ = "",
                           const std::string & top_mapping_model_name_ = "",
                           const std::string & gdml_file_= "",
                           bool replica_support_ = true,
                           bool root_display_ = false,
                           int root_vis_option_ = 1,
                           int root_vis_level_ = 100,
                           const std::string & root_top_volume_ = "",
                           datatools::logger::priority logging_ = datatools::logger::PRIO_WARNING);

  private:

    bool _initialized_ = false; //!< Initialization flag
    geomtools_driver_params          _params_;   //!< Configuration and dynamic parameters

    // Geometry setup model:
    std::unique_ptr<manager>       _geo_mgr_;         //!< Geometry manager handle
    std::unique_ptr<model_factory> _geo_factory_;     //!< Geometry model factory handle
    const model_factory *          _geo_factory_ref_ = nullptr; //!< Geometry model factory effective handle
    bool                           _has_world_ = false;       //!< Flag for a 'world' top-level volume/model
    // std::map<std::string, datatools::properties> _ropts_; //!< Rendering options associated to displayed objects
    std::map<std::string, display_data> _dds_; //!< Embedded display data objects
    std::vector<std::string> _error_message_stack_;


  };

} // end of namespace geomtools

#endif // GEOMTOOLS_GEOMTOOLS_DRIVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
