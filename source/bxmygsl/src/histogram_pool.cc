// mygsl::histogram_pool.cc

#include <mygsl/histogram_pool.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>

#include <datatools/utils.h>
#include <datatools/units.h>

namespace mygsl {

  using namespace std;

  DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(histogram_pool::histogram_entry_type,"mygsl::histogram_pool::histogram_entry_type")

  DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(histogram_pool,"mygsl::histogram_pool")

  const std::string histogram_pool::_INITIALIZED_FLAG_KEY_ = "__initialized";

  histogram_pool::histogram_entry_type::histogram_entry_type ()
  {
    dimension = HISTOGRAM_DIM_UNDEFINED;
    return;
  }

  histogram_pool::histogram_entry_type::~histogram_entry_type ()
  {
    return;
  }

  void histogram_pool::histogram_entry_type::tree_dump (std::ostream& out_,
                                                        const std::string& title_,
                                                        const std::string& indent_,
                                                        bool inherit_) const
  {
    std::string indent;
    if (!indent_.empty()) indent = indent_;
    if (!title_.empty()) out_ << indent << title_ << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
         << "Name  : '" << name << "'" << std::endl;
    out_ << indent << datatools::i_tree_dumpable::tag
         << "Title  : '" << title << "'" << std::endl;
    out_ << indent << datatools::i_tree_dumpable::tag
         << "Dimension  : " << dimension << std::endl;
    if (dimension == HISTOGRAM_DIM_1D)
      {
        out_ << indent << datatools::i_tree_dumpable::tag
             << "Histogram 1D : " << &(hh1d.get ());
        if (hh1d.get ().is_initialized ())
          {
            out_ << " has " << hh1d.get ().counts () << " counts within "
                 << hh1d.get ().bins () << " bins" << std::endl;;
            //hh1d.get ().tree_dump (out_, "", indent);
          }
        else
          {
            out_ << " (not initialized)" << std::endl;;
          }
      }
    if (dimension == HISTOGRAM_DIM_2D)
      {
        out_ << indent << datatools::i_tree_dumpable::tag
             << "Histogram 2D : " << &(hh2d.get ());
        if (hh2d.get ().is_initialized ())
          {
            out_ << " has " << hh2d.get ().counts () << " counts within "
                 << hh2d.get ().xbins () << '*' <<  hh2d.get ().ybins () << " bins" << std::endl;
            //hh2d.get ().tree_dump (out_, "", indent);
          }
        else
          {
            out_ << " (not initialized)" << std::endl;
          }
      }

    out_ << indent << datatools::i_tree_dumpable::inherit_tag (inherit_)
         << "Group  : '" << group << "'" << std::endl;

    // out_ << indent << datatools::i_tree_dumpable::inherit_tag (inherit_)
    //       << "Auxiliaries : ";
    // if (auxiliaries.empty ())
    //   {
    //     out_ << "<empty>";
    //   }
    // out_ << endl;
    // {
    //   ostringstream indent_oss;
    //   indent_oss << indent;
    //   indent_oss << datatools::i_tree_dumpable::inherit_skip_tag (inherit_) ;
    //   auxiliaries.tree_dump (out_, "", indent_oss.str ());
    // }

    return;
  }

  /************************************************************************/

  bool histogram_pool::is_initialized () const
  {
    return _auxiliaries_.has_flag (_INITIALIZED_FLAG_KEY_);
  }

  const std::string & histogram_pool::get_description () const
  {
    return _description_;
  }

  void histogram_pool::set_description (const std::string & desc_)
  {
    _description_ = desc_;
    return;
  }

  const datatools::properties & histogram_pool::get_auxiliaries () const
  {
    return _auxiliaries_;
  }

  datatools::properties & histogram_pool::grab_auxiliaries ()
  {
    return _auxiliaries_;
  }

  histogram_pool::histogram_pool ()
  {
    return;
  }

  histogram_pool::histogram_pool (const std::string & desc_)
  {
    set_description (desc_);
    return;
  }

  histogram_pool::~histogram_pool ()
  {
    return;
  }

  void histogram_pool::load (const std::string & histo_setups_filename_)
  {
    datatools::multi_properties histo_setups;
    std::string hsf = histo_setups_filename_;
    datatools::fetch_path_with_env (hsf);
    histo_setups.read (hsf);
    load (histo_setups);
    return;
  }

  void histogram_pool::load (const datatools::multi_properties & histo_setups_)
  {
    const datatools::multi_properties::entries_ordered_col_type& hentries = histo_setups_.ordered_entries();
    for (datatools::multi_properties::entries_ordered_col_type::const_iterator iter = hentries.begin ();
         iter != hentries.end ();
         iter++)
      {
        const datatools::multi_properties::entry & hentry = *(*iter);
        std::string histo_name = hentry.get_key();
        std::string histo_type = hentry.get_meta();
        const datatools::properties& histo_params = hentry.get_properties();
        std::string histo_title;
        std::string histo_group;
        if (histo_params.has_key ("group"))
          {
            histo_group = histo_params.fetch_string ("group");
          }
        if (histo_params.has_key ("title"))
          {
            histo_title = histo_params.fetch_string ("title");
          }
        if (histo_type == "mygsl::histogram_1d")
          {
            mygsl::histogram_1d & h_1d = this->add_1d (histo_name);
            init_histo_1d (h_1d, histo_params, this);
            if (! histo_title.empty ())
              {
                h_1d.grab_auxiliaries ().update ("display.title", histo_title);
              }
          }
        else if (histo_type == "mygsl::histogram_2d")
          {
            mygsl::histogram_2d & h_2d = this->add_2d (histo_name);
            init_histo_2d (h_2d, histo_params, this);
            if (! histo_title.empty ())
              {
                h_2d.grab_auxiliaries ().update ("display.title", histo_title);
              }
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::load: "
                    << "Invalid histogram class name '" << histo_type << "' !";
            throw std::logic_error (message.str ());
          }
        // Add traits :
        this->set_title (histo_name, histo_title);
        this->set_group (histo_name, histo_group);
      }
    return;
  }

  // static
  void histogram_pool::init_histo_1d (histogram_1d & h1_,
                                      const datatools::properties& h1_setup_,
                                      const histogram_pool* histo_pool_ )
  {
    if (h1_.is_initialized ())
      {
        h1_.reset ();
      }

    std::string mode;
    if (h1_setup_.has_key ("mode"))
      {
        mode = h1_setup_.fetch_string ("mode");
        if (mode != "regular" && mode != "table" && mode != "mimic")
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Invalid 1D-histogram build mode '" << mode << "' !";
            throw std::logic_error (message.str ());
          }
      }
    if (mode.empty ())
      {
        mode = "regular";
      }

    std::string xunit_type;  // The physical dimension of the X axis binning
    std::string xunit_str;   // The unit symbol of the X axis
    double      xunit = 1.0; // The unit intrinsic value of the X axis
    
    // Store all display properties :
    h1_setup_.export_starting_with (h1_.grab_auxiliaries (), "display.");
 
    // Extract the unit used for X axis bins :
    if (h1_setup_.has_key ("unit"))
      {
        xunit_str = h1_setup_.fetch_string ("unit");
        if (! datatools::units::find_unit(xunit_str, xunit, xunit_type))
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Invalid X binning unit ('" << xunit_str << "') !";
            throw std::logic_error (message.str ());
          }
       }
 
    if (mode == "regular" || mode == "table")
      {
        // Extract the physical dimension for the X axis :
        if (h1_setup_.has_key ("unit.type"))
          {
            std::string xunit_type2 = h1_setup_.fetch_string ("unit.type");
            if (! datatools::units::is_unit_label_valid (xunit_type2))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Invalid binning unit type ('" << xunit_type << "') !";
                throw std::logic_error (message.str ());
              }
            if (! xunit_type.empty () && xunit_type2 != xunit_type)
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Incompatible unit type ('" << xunit_type2
                        << "') with bins unit ('" << xunit_type << "') !";
                throw std::logic_error (message.str ());
              }
          }
        
        // Extract the optional display unit for X axis and check it :
        if (h1_setup_.has_key ("display.xaxis.unit"))
          {
            std::string display_xunit_str = h1_setup_.fetch_string ("display.xaxis.unit");
            std::string display_xunit_type;
            double display_xunit = 1.0;
            if (datatools::units::find_unit(display_xunit_str, display_xunit, display_xunit_type))
              {
                if (xunit_type.empty ())
                  {
                    xunit_type = display_xunit_type;
                  }
                else
                  {
                    if (display_xunit_type != xunit_type)
                      {
                        std::ostringstream message;
                        message << "mygsl::histogram_pool::init_histo_1d: "
                                << "X axis display unit type ('" << display_xunit_type << "') does not match the X axis unit !";
                        throw std::logic_error (message.str ());
                      }
                  }
              }
          }
        else
          {
            // Default display unit for X axis :
            if (! xunit_str.empty ())
              {
                h1_.grab_auxiliaries ().update ("display.xaxis.unit", xunit_str);
              }
          }
     }

    if (! xunit_type.empty ())
      {
        h1_.grab_auxiliaries ().store ("unit.type", xunit_type);
      }

    if (mode == "regular")
      {
        double xmin, xmax;
        int nxbins = -1;
        int xbinmode = BIN_MODE_INVALID;
        datatools::invalidate (xmin);
        datatools::invalidate (xmax);

        if (h1_setup_.has_flag ("linear"))
          {
            xbinmode = BIN_MODE_LINEAR;
          }

        if (h1_setup_.has_flag ("logarithmic"))
          {
            xbinmode = BIN_MODE_LOG;
          }

        if (xbinmode == BIN_MODE_INVALID)
          {
            xbinmode = BIN_MODE_LINEAR;
          }

        if (h1_setup_.has_key ("min"))
          {
            xmin = h1_setup_.fetch_real ("min");
          }

        if (h1_setup_.has_key ("max"))
          {
            xmax = h1_setup_.fetch_real ("max");
          }

        if (! datatools::is_valid (xmin))
          {
            if (xmax > 0.0)
              {
                xmin = 0.0;
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Invalid 1D-histogram X range [" << xmin << ":" << xmax << "[ !";
                throw std::logic_error (message.str ());
              }
          }

        if (! datatools::is_valid (xmax))
          {
            if (xmin < 0.0)
              {
                xmax = 0.0;
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Invalid 1D-histogram X range [" << xmin << ":" << xmax << "[ !";
                throw std::logic_error (message.str ());
              }
          }
        if (! h1_setup_.has_explicit_unit ("min")) {
          xmin *= xunit;
        }
        if (! h1_setup_.has_explicit_unit ("max")) {
          xmax *= xunit;
        }

        if (h1_setup_.has_key ("number_of_bins"))
          {
            nxbins = h1_setup_.fetch_integer ("number_of_bins");
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Missing number of bins in 1D-histogram !";
            throw std::logic_error (message.str ());
          }
        if (nxbins < 1)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Invalid number of bins ('" << nxbins << "') in 1D-histogram !";
            throw std::logic_error (message.str ());
          }
        h1_.initialize (nxbins, xmin, xmax, xbinmode);
      }
    else if (mode == "table")
      {
        std::vector<double> xranges;
        if (h1_setup_.has_key ("bounds"))
          {
            h1_setup_.fetch ("bounds", xranges);
          }
        else if (h1_setup_.has_key ("bounds.file"))
          {
            std::string bounds_file;
            bounds_file = h1_setup_.fetch_string ("bounds.file");

            std::string bounds_file2 = bounds_file;
            datatools::fetch_path_with_env (bounds_file2);
            std::ifstream bounds_ifs (bounds_file2.c_str ());
            if (! bounds_ifs)
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Cannot open file '" << bounds_file2 << "' !";
                throw std::logic_error (message.str ());
              }
            while (bounds_ifs)
              {
                double bound;
                bounds_ifs >> bound;
                if (! bounds_ifs)
                  {
                    std::ostringstream message;
                    message << "mygsl::histogram_pool::init_histo_1d: "
                            << "Invalid format in file '" << bounds_file << "' !";
                    throw std::logic_error (message.str ());
                  }
                xranges.push_back (bound * xunit);
                bounds_ifs >> std::ws;
                if (bounds_ifs.eof ())
                  {
                    break;
                  }
              }
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "No rule was given to setup the bins (bounds, bounds.file)!";
            throw std::logic_error (message.str ());
          }
        if (xranges.size () < 2)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Invalid number of bins ('" << xranges.size () << "') in 1D-histogram !";
            throw std::logic_error (message.str ());
          }
        h1_.initialize (xranges);
        h1_.grab_auxiliaries ().tree_dump (std::cerr, "H1D : ", "DEVEL: *********** ");
      }
    else if (mode == "mimic")
      {
        // extract information about another histogram :
        // 1D clone
        // 2D use X or Y
        if (histo_pool_ == 0)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Missing pool of histograms !";
            throw std::logic_error (message.str ());
          }

        if (h1_setup_.has_key ("mimic.histogram_1d"))
          {
            // Copy the structure of another 1D histogram :
            std::string h1d_name = h1_setup_.fetch_string ("mimic.histogram_1d");

            if (! histo_pool_->has (h1d_name, histogram_pool::HISTOGRAM_DIM_1D))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "No 1D-histogram named '" << h1d_name << "' does exist !";
                throw std::logic_error (message.str ());
              }
            const histogram & mimic_h1 = histo_pool_->get_1d (h1d_name);
            std::vector<std::string> import_prop_prefixes;
            if (h1_setup_.has_flag ("mimic.histogram_1d.import_aux"))
              {
                h1_setup_.fetch ("mimic.histogram_1d.import_aux", import_prop_prefixes);
              }
            h1_.initialize (mimic_h1, import_prop_prefixes);
          }
        else if (h1_setup_.has_key ("mimic.histogram_2d"))
          {
            // Copy the structure of one of the axis of a 2D histogram :
             std::string h2d_name = h1_setup_.fetch_string ("mimic.histogram_2d");

            if (! histo_pool_->has (h2d_name, histogram_pool::HISTOGRAM_DIM_2D))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "No 2D-histogram named '" << h2d_name << "' does exist !";
                throw std::logic_error (message.str ());
              }
            const histogram_2d & mimic_h2 = histo_pool_->get_2d (h2d_name);
            int bin_axis = BIN_AXIS_INVALID;
            // Extract the axis of the 2D histogram to copy :
            if (h1_setup_.has_key ("mimic.histogram_2d.axis"))
              {
                std::string axis_label = h1_setup_.fetch_string ("mimic.histogram_2d.axis");
                if (axis_label == "x")
                  {
                    bin_axis = BIN_AXIS_X;
                  }
                else if (axis_label == "y")
                  {
                    bin_axis = BIN_AXIS_Y;
                  }
                else
                  {
                    std::ostringstream message;
                    message << "mygsl::histogram_pool::init_histo_1d: "
                            << "Invalid bin axis label '" << axis_label << "' !";
                    throw std::logic_error (message.str ());
                  }
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Missing bin axis property '" << "mimic.histogram_2d.axis" << "' !";
                throw std::logic_error (message.str ());
              }
            std::vector<std::string> import_aux_prefixes;
            if (h1_setup_.has_flag ("mimic.histogram_2d.import_aux"))
              {
                h1_setup_.fetch ("mimic.histogram_2d.import_aux", import_aux_prefixes);
              }
            h1_.initialize (mimic_h2, bin_axis, import_aux_prefixes);
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_1d: "
                    << "Missing mimic mode !";
            throw std::logic_error (message.str ());
          }
      }

    return;
  }

  // static
  void histogram_pool::init_histo_2d (histogram_2d & h2_,
                                      const datatools::properties& h2_setup_,
                                      const histogram_pool* histo_pool_ )
  {
    if (h2_.is_initialized ())
      {
        h2_.reset ();
      }

    std::string mode;
    if (h2_setup_.has_key ("mode"))
      {
        mode = h2_setup_.fetch_string ("mode");
        if (mode != "regular" && mode != "table" && mode != "mimic")
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid 2D-histogram build mode '" << mode << "' !";
            throw std::logic_error (message.str ());
          }
      }
    if (mode.empty ())
      {
        mode = "regular";
      }

    std::string xunit_type;  // The physical dimension of the X axis binning
    std::string xunit_str;   // The unit symbol of the X axis
    double      xunit = 1.0; // The unit intrinsic value of the X axis

    std::string yunit_type;  // The physical dimension of the Y axis binning
    std::string yunit_str;   // The unit symbol of the Y axis
    double      yunit = 1.0; // The unit intrinsic value of the Y axis
  
    // Store all display properties :
    h2_setup_.export_starting_with (h2_.grab_auxiliaries (), "display.");

    // Extract the unit used for X axis bins :
    if (h2_setup_.has_key ("x.unit"))
      {
        xunit_str = h2_setup_.fetch_string ("x.unit");
        if (! datatools::units::find_unit(xunit_str, xunit, xunit_type))
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid X binning unit ('" << xunit_str << "') !";
            throw std::logic_error (message.str ());
          }
      }
    // Extract the unit used for Y axis bins :
    if (h2_setup_.has_key ("y.unit"))
      {
        yunit_str = h2_setup_.fetch_string ("y.unit");
        if (! datatools::units::find_unit(yunit_str, yunit, yunit_type))
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid Y binning unit ('" << yunit_str << "') !";
            throw std::logic_error (message.str ());
          }
      }
 
    if (mode == "regular" || mode == "table")
      {
        // Extract the physical dimension for the X axis :
        if (h2_setup_.has_key ("x.unit.type"))
          {
            std::string xunit_type2 = h2_setup_.fetch_string ("x.unit.type");
            if (! datatools::units::is_unit_label_valid (xunit_type2))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "Invalid X binning unit type ('" << xunit_type << "') !";
                throw std::logic_error (message.str ());
              }
            if (! xunit_type.empty () && xunit_type2 != xunit_type)
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Incompatible X unit type ('" << xunit_type2
                        << "') with X bins unit ('" << xunit_type << "') !";
                throw std::logic_error (message.str ());
              }
          }

        // Extract the physical dimension for the Y axis :
        if (h2_setup_.has_key ("y.unit.type"))
          {
            std::string yunit_type2 = h2_setup_.fetch_string ("y.unit.type");
            if (! datatools::units::is_unit_label_valid (yunit_type2))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "Invalid Y binning unit type ('" << yunit_type << "') !";
                throw std::logic_error (message.str ());
              }
            if (! yunit_type.empty () && yunit_type2 != yunit_type)
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_1d: "
                        << "Incompatible Y unit type ('" << yunit_type2
                        << "') with Y bins unit ('" << yunit_type << "') !";
                throw std::logic_error (message.str ());
              }
          }
         
        // Extract the optional display unit for X axis and check it :
        if (h2_setup_.has_key ("display.xaxis.unit"))
          {
            std::string display_xunit_str = h2_setup_.fetch_string ("display.xaxis.unit");
            std::string display_xunit_type;
            double display_xunit = 1.0;
            if (datatools::units::find_unit(display_xunit_str, display_xunit, display_xunit_type))
              {
                 if (xunit_type.empty ())
                  {
                    xunit_type = display_xunit_type;
                  }
                else
                  {
                    if (display_xunit_type != xunit_type)
                      {
                        std::ostringstream message;
                        message << "mygsl::histogram_pool::init_histo_1d: "
                                << "X axis display unit type ('" << display_xunit_type << "') does not match the X axis unit !";
                        throw std::logic_error (message.str ());
                      }
                  }
              }
          }
        else
          {
            // Default display unit for X axis :
            if (! xunit_str.empty ())
              {
                h2_.grab_auxiliaries ().update ("display.xaxis.unit", xunit_str);
              }
          }
        
        // Extract the optional display unit for Y axis and check it :
        if (h2_setup_.has_key ("display.yaxis.unit"))
          {
            std::string display_yunit_str = h2_setup_.fetch_string ("display.yaxis.unit");
            std::string display_yunit_type;
            double display_yunit = 1.0;
            if (datatools::units::find_unit(display_yunit_str, display_yunit, display_yunit_type))
              {
                 if (yunit_type.empty ())
                  {
                    yunit_type = display_yunit_type;
                  }
                else
                  {
                    if (display_yunit_type != yunit_type)
                      {
                        std::ostringstream message;
                        message << "mygsl::histogram_pool::init_histo_1d: "
                                << "Y axis display unit type ('" << display_yunit_type << "') does not match the Y axis unit !";
                        throw std::logic_error (message.str ());
                      }
                  }
              }
          }
        else
          {
            // Default display unit for Y axis :
            if (! yunit_str.empty ())
              {
                h2_.grab_auxiliaries ().update ("display.yaxis.unit", yunit_str);
              }
          }
     }

    if (! xunit_type.empty ())
      {
        h2_.grab_auxiliaries ().store ("x.unit.type", xunit_type);
      }

    if (! yunit_type.empty ())
      {
        h2_.grab_auxiliaries ().store ("y.unit.type", yunit_type);
      }

    if (mode == "regular")
      {
        double xmin, xmax;
        int nxbins = -1;
        int xbinmode = BIN_MODE_INVALID;
        datatools::invalidate (xmin);
        datatools::invalidate (xmax);
        double ymin, ymax;
        int nybins = -1;
        int ybinmode = BIN_MODE_INVALID;
        datatools::invalidate (ymin);
        datatools::invalidate (ymax);

        if (h2_setup_.has_flag ("x.linear"))
          {
            xbinmode = BIN_MODE_LINEAR;
          }
        if (h2_setup_.has_flag ("x.logarithmic"))
          {
            xbinmode = BIN_MODE_LOG;
          }
        if (xbinmode == BIN_MODE_INVALID)
          {
            xbinmode = BIN_MODE_LINEAR;
          }

        if (h2_setup_.has_flag ("y.linear"))
          {
            ybinmode = BIN_MODE_LINEAR;
          }
        if (h2_setup_.has_flag ("y.logarithmic"))
          {
            ybinmode = BIN_MODE_LOG;
          }
        if (ybinmode == BIN_MODE_INVALID)
          {
            ybinmode = BIN_MODE_LINEAR;
          }


        if (h2_setup_.has_key ("x.min"))
          {
            xmin = h2_setup_.fetch_real ("x.min");
          }

        if (h2_setup_.has_key ("x.max"))
          {
            xmax = h2_setup_.fetch_real ("x.max");
          }

        if (! datatools::is_valid (xmin))
          {
            if (xmax > 0.0)
              {
                xmin = 0.0;
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "Invalid 2D-histogram X range [" << xmin << ":" << xmax << "[ !";
                throw std::logic_error (message.str ());
              }
          }

        if (! datatools::is_valid (xmax))
          {
            if (xmin < 0.0)
              {
                xmax = 0.0;
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "Invalid 2D-histogram X range [" << xmin << ":" << xmax << "[ !";
                throw std::logic_error (message.str ());
              }
          }

        if (h2_setup_.has_key ("y.min"))
          {
            ymin = h2_setup_.fetch_real ("y.min");
          }

        if (h2_setup_.has_key ("y.max"))
          {
            ymax = h2_setup_.fetch_real ("y.max");
          }

        if (! datatools::is_valid (ymin))
          {
            if (ymax > 0.0)
              {
                ymin = 0.0;
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "Invalid 2D-histogram Y range [" << ymin << ":" << ymax << "[ !";
                throw std::logic_error (message.str ());
              }
          }

        if (! datatools::is_valid (ymax))
          {
            if (ymin < 0.0)
              {
                ymax = 0.0;
              }
            else
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "Invalid 2D-histogram Y range [" << ymin << ":" << ymax << "[ !";
                throw std::logic_error (message.str ());
              }
          }

        if (!h2_setup_.has_explicit_unit("x.max")) {
          xmax *= xunit;
        }
        if (!h2_setup_.has_explicit_unit("x.min")) {
          xmin *= xunit;
        }
        if (!h2_setup_.has_explicit_unit("y.min")) {
          ymin *= yunit;
        }
        if (!h2_setup_.has_explicit_unit("y.max")) {
          ymax *= yunit;
        }

        if (h2_setup_.has_key ("x.number_of_bins"))
          {
            nxbins = h2_setup_.fetch_integer ("x.number_of_bins");
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Missing number of X bins in 2D-histogram !";
            throw std::logic_error (message.str ());
          }
        if (nxbins < 1)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid number of X bins ('" << nxbins << "') in 2D-histogram !";
            throw std::logic_error (message.str ());
          }

        if (h2_setup_.has_key ("y.number_of_bins"))
          {
            nybins = h2_setup_.fetch_integer ("y.number_of_bins");
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Missing number of Y bins in 2D-histogram !";
            throw std::logic_error (message.str ());
          }
        if (nybins < 1)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid number of Y bins ('" << nybins << "') in 2D-histogram !";
            throw std::logic_error (message.str ());
          }

        h2_.initialize (nxbins, xmin, xmax,
                        nybins, ymin, ymax,
                        xbinmode, ybinmode);
      }
    else if (mode == "table")
      {
        std::vector<double> xranges;
        std::vector<double> yranges;
        if (h2_setup_.has_key ("x.bounds"))
          {
            h2_setup_.fetch ("x.bounds", xranges);
          }
        if (h2_setup_.has_key ("y.bounds"))
          {
            h2_setup_.fetch ("y.bounds", yranges);
          }
        if (xranges.size() == 0 && h2_setup_.has_key ("x.bounds.file"))
          {
            std::string xbounds_file;
            xbounds_file = h2_setup_.fetch_string ("x.bounds.file");

            std::string xbounds_file2 = xbounds_file;
            datatools::fetch_path_with_env (xbounds_file2);
            std::ifstream xbounds_ifs (xbounds_file2.c_str ());
            while (xbounds_ifs)
              {
                double xbound;
                xbounds_ifs >> xbound;
                if (! xbounds_ifs)
                  {
                    std::ostringstream message;
                    message << "mygsl::histogram_pool::init_histo_2d: "
                            << "Invalid format in file  '" << xbounds_file << "' !";
                    throw std::logic_error (message.str ());
                  }
                xranges.push_back (xbound * xunit);
                xbounds_ifs >> std::ws;
                if (xbounds_ifs.eof ())
                  {
                    break;
                  }
              }
          }
        if (yranges.size() == 0 && h2_setup_.has_key ("y.bounds.file"))
          {
            std::string ybounds_file;
            ybounds_file = h2_setup_.fetch_string ("y.bounds.file");

            std::string ybounds_file2 = ybounds_file;
            datatools::fetch_path_with_env (ybounds_file2);
            std::ifstream ybounds_ifs (ybounds_file2.c_str ());
            while (ybounds_ifs)
              {
                double ybound;
                ybounds_ifs >> ybound;
                if (! ybounds_ifs)
                  {
                    std::ostringstream message;
                    message << "mygsl::histogram_pool::init_histo_2d: "
                            << "Invalid format in file  '" << ybounds_file << "' !";
                    throw std::logic_error (message.str ());
                  }
                yranges.push_back (ybound * yunit);
                ybounds_ifs >> std::ws;
                if (ybounds_ifs.eof ())
                  {
                    break;
                  }
              }
          }
        if (xranges.size() == 0)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "No rule was given to setup the X bins (xbounds, xbounds.file)!";
            throw std::logic_error (message.str ());
          }
        if (yranges.size() == 0)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "No rule was given to setup the Y bins (ybounds, ybounds.file)!";
            throw std::logic_error (message.str ());
          }
        if (xranges.size () < 2)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid number of X bins ('" << xranges.size () << "') in 2D-histogram !";
            throw std::logic_error (message.str ());
          }
        if (yranges.size () < 2)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Invalid number of Y bins ('" << yranges.size () << "') in 2D-histogram !";
            throw std::logic_error (message.str ());
          }
        h2_.initialize (xranges, yranges);
      }
    else if (mode == "mimic")
      {
        // Extract information about other histograms :
        // 1D clone x 1D clone
        // 2D clone
        if (histo_pool_ == 0)
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Missing pool of histograms !";
            throw std::logic_error (message.str ());
          }

        if (h2_setup_.has_key ("mimic.x.histogram_1d") && h2_setup_.has_key ("mimic.y.histogram_1d"))
          {
            // Copy the structure of another 1D histogram :
            std::string h2d_x_name = h2_setup_.fetch_string ("mimic.x.histogram_1d");
            std::string h2d_y_name = h2_setup_.fetch_string ("mimic.y.histogram_1d");

            if (! histo_pool_->has (h2d_x_name, histogram_pool::HISTOGRAM_DIM_1D))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "No 1D-histogram named '" << h2d_x_name << "' does exist !";
                throw std::logic_error (message.str ());
              }

            if (! histo_pool_->has (h2d_y_name, histogram_pool::HISTOGRAM_DIM_1D))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "No 1D-histogram named '" << h2d_y_name << "' does exist !";
                throw std::logic_error (message.str ());
              }
            const histogram & mimic_x_h1 = histo_pool_->get_1d (h2d_x_name);
            const histogram & mimic_y_h1 = histo_pool_->get_1d (h2d_y_name);
            std::vector<std::string> import_prop_prefixes;
            if (h2_setup_.has_flag ("mimic.histogram_2d.import_aux"))
              {
                h2_setup_.fetch ("mimic.histogram_2d.import_aux", import_prop_prefixes);
              }
            h2_.initialize (mimic_x_h1, mimic_y_h1, import_prop_prefixes);
          }
        else if (h2_setup_.has_key ("mimic.histogram_2d"))
          {
            // Copy the structure of another 2D histogram :
            std::string h2d_name = h2_setup_.fetch_string ("mimic.histogram_2d");

            if (! histo_pool_->has (h2d_name, histogram_pool::HISTOGRAM_DIM_2D))
              {
                std::ostringstream message;
                message << "mygsl::histogram_pool::init_histo_2d: "
                        << "No 2D-histogram named '" << h2d_name << "' does exist !";
                throw std::logic_error (message.str ());
              }
            const histogram_2d & mimic_h2 = histo_pool_->get_2d (h2d_name);
            std::vector<std::string> import_prop_prefixes;
            if (h2_setup_.has_flag ("mimic.histogram_2d.import_aux"))
              {
                h2_setup_.fetch ("mimic.histogram_2d.import_aux", import_prop_prefixes);
              }
            h2_.initialize (mimic_h2, import_prop_prefixes);
          }
        else
          {
            std::ostringstream message;
            message << "mygsl::histogram_pool::init_histo_2d: "
                    << "Missing mimic mode !";
            throw std::logic_error (message.str ());
          }
      }

    return;
  }

  void histogram_pool::initialize (const datatools::properties & setup_)
  {
    if (is_initialized ()) {
      std::ostringstream message;
      message << "mygsl::histogram_pool::initialize: "
              << "Pool is already initialized  !";
      throw std::logic_error (message.str ());
    }
    
    if (_description_.empty()) {
      if (setup_.has_key ("description")) {
        set_description (setup_.fetch_string ("description"));
      }
    }
    
    std::vector<std::string> histo_setup_filenames;
    if (setup_.has_key ("histo.setups")) {
      setup_.fetch ("histo.setups", histo_setup_filenames);
    }
    
    for (int i = 0; i < histo_setup_filenames.size (); i++)
      {
        load (histo_setup_filenames[i]);
      }

    _auxiliaries_.set_flag (_INITIALIZED_FLAG_KEY_);

    return;
  }

  void histogram_pool::reset ()
  {
    if (is_initialized ())
      {
        remove_all ();
        clear_auxiliaries ();
      }
    return;
  }

  void histogram_pool::remove_all ()
  {
    _dict_.clear ();
    return;
  }

  void histogram_pool::clear_auxiliaries ()
  {
    _auxiliaries_.clear ();
    return;
  }

  void histogram_pool::remove (const std::string & name_)
  {
    dict_type::iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        return;
      }
    _dict_.erase (found);
    return;
  }

  void histogram_pool::rename (const std::string & name_, const std::string & new_name_)
  {
    dict_type::iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::rename: "
                << "No histogram named '" << name_ << "' exists !";
        throw std::logic_error (message.str ());
      }
    dict_type::iterator found2 = _dict_.find (new_name_);
    if (found2 != _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::rename: "
                << "An histogram named '" << name_ << "' already exists !";
        throw std::logic_error (message.str ());
      }
    histogram_entry_type hentry = found->second;
    _dict_.erase (found);
    hentry.name = new_name_;
    _dict_[new_name_] = hentry;
    return;
  }

  const std::string &
  histogram_pool::get_group (const std::string & name_) const
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::get_group: "
                << "No histogram named '" << name_ << "' exists !";
        throw std::logic_error (message.str ());
      }
    return found->second.group;
  }

  const std::string &
  histogram_pool::get_title (const std::string & name_) const
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::get_title: "
                << "No histogram named '" << name_ << "' exists !";
        throw std::logic_error (message.str ());
      }
    return found->second.title;
  }

  void histogram_pool::set_title (const std::string & name_, const std::string & title_)
  {
    dict_type::iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::set_title: "
                << "No histogram named '" << name_ << "' exists !";
        throw std::logic_error (message.str ());
      }
    found->second.title = title_;
    return;
  }

  void histogram_pool::set_group (const std::string & name_, const std::string & group_)
  {
    dict_type::iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::set_group: "
                << "No histogram named '" << name_ << "' exists !";
        throw std::logic_error (message.str ());
      }
    found->second.group = group_;
    return;
  }

  bool histogram_pool::has_1d (const std::string & name_) const
  {
    return has (name_, HISTOGRAM_DIM_1D);
  }

  bool histogram_pool::has_2d (const std::string & name_) const
  {
    return has (name_, HISTOGRAM_DIM_2D);
  }

  bool histogram_pool::has (const std::string & name_, int dim_) const
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        return false;
      }
    if (dim_ == HISTOGRAM_DIM_UNDEFINED)
      {
        return true;
      }
    const histogram_entry_type & hentry = found->second;
    if (hentry.dimension == dim_)
      {
        return true;
      }
    return false;
  }

  histogram & histogram_pool::grab_1d (const std::string & name_)
  {
    dict_type::iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::grab_1d: "
                << "No histogram named '" << name_ << "' !";
        throw std::logic_error (message.str ());
      }
    histogram_entry_type & hentry = found->second;
    if (hentry.dimension != HISTOGRAM_DIM_1D)
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::grab_1d: "
                << "Histogram '" << name_ << "' is not a 1D histogram !";
        throw std::logic_error (message.str ());
      }
    return hentry.hh1d.grab ();
  }

  const histogram & histogram_pool::get_1d (const std::string & name_) const
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::get_1d: "
                << "No histogram named '" << name_ << "' !";
        throw std::logic_error (message.str ());
      }
    const histogram_entry_type & hentry = found->second;
    if (hentry.dimension != HISTOGRAM_DIM_1D)
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::get_1d: "
                << "Histogram '" << name_ << "' is not a 1D histogram !";
        throw std::logic_error (message.str ());
      }
    return hentry.hh1d.get ();
  }


  histogram_2d & histogram_pool::grab_2d (const std::string & name_)
  {
    dict_type::iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::grab_2d: "
                << "No histogram named '" << name_ << "' !";
        throw std::logic_error (message.str ());
      }
    histogram_entry_type & hentry = found->second;
    if (hentry.dimension != HISTOGRAM_DIM_2D)
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::grab_2d: "
                << "Histogram '" << name_ << "' is not a 2D histogram !";
        throw std::logic_error (message.str ());
      }
    return hentry.hh2d.grab ();
  }

  const histogram_2d & histogram_pool::get_2d (const std::string & name_) const
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found == _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::get_2d: "
                << "No histogram named '" << name_ << "' !";
        throw std::logic_error (message.str ());
      }
    const histogram_entry_type & hentry = found->second;
    if (hentry.dimension != HISTOGRAM_DIM_2D)
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::get_2d: "
                << "Histogram '" << name_ << "' is not a 2D histogram !";
        throw std::logic_error (message.str ());
      }
    return hentry.hh2d.get ();
  }

  histogram & histogram_pool::add_1d (const std::string & name_,
                                      const std::string & title_,
                                      const std::string & group_)
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found != _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::add_1d: "
                << "Histogram named '" << name_ << "' already exists !";
        throw std::logic_error (message.str ());
      }
    {
      histogram_entry_type dummy;
      _dict_[name_] = dummy;
    }
    histogram_entry_type & he = _dict_[name_];
    he.name = name_;
    he.title = title_;
    he.group = group_;
    he.dimension = HISTOGRAM_DIM_1D;
    he.hh1d.reset (new histogram ());
    return he.hh1d.grab ();
  }

  histogram_2d & histogram_pool::add_2d (const std::string & name_,
                                         const std::string & title_,
                                         const std::string & group_)
  {
    dict_type::const_iterator found = _dict_.find (name_);
    if (found != _dict_.end ())
      {
        std::ostringstream message;
        message << "mygsl::histogram_pool::add_2d: "
                << "Histogram named '" << name_ << "' already exists !";
        throw std::logic_error (message.str ());
      }
    histogram_entry_type & he = _dict_[name_];
    he.name = name_;
    he.title = title_;
    he.group = group_;
    he.dimension = HISTOGRAM_DIM_2D;
    he.hh2d.reset (new histogram_2d ());
    return he.hh2d.grab ();
  }

  bool histogram_pool::empty () const
  {
    return _dict_.size () == 0;
  }

  unsigned int histogram_pool::size () const
  {
    return _dict_.size ();
  }

  void histogram_pool::names (std::vector<std::string> &names_, const std::string & filter_, bool reverse_) const
  {
    if (names_.size () == 0) names_.reserve (_dict_.size ());
    for (dict_type::const_iterator i = _dict_.begin ();
         i != _dict_.end ();
         i++)
      {
        const std::string& key = i->first;
        bool select = false;

        // here, all histogram are selected, we make no use of the 'filter_' argument :
        const histogram_entry_type& he = i->second;
        if (filter_ == "" || filter_ == "*")
          {
            select = true;
          }
        else if (filter_ == "dim=1")
          {
            if (he.dimension == HISTOGRAM_DIM_1D)
              {
                select = true;
              }
          }
        else if (filter_ == "dim=2")
          {
            if (he.dimension == HISTOGRAM_DIM_2D)
              {
                select = true;
              }
          }
        else if (filter_.substr(0,6) == "group=")
          {
            std::string group_name = filter_.substr(6);
            if (he.group == group_name)
              {
                select = true;
              }
          }
        else if (filter_.substr(0,5) == "flag=")
          {
            std::string flag_name = filter_.substr(5);

            if (he.dimension == HISTOGRAM_DIM_1D)
              {
                if (he.hh1d.get ().get_auxiliaries ().has_flag (flag_name))
                  {
                    select = true;
                  }
              }
            if (he.dimension == HISTOGRAM_DIM_2D)
              {
                if (he.hh2d.get ().get_auxiliaries ().has_flag (flag_name))
                  {
                    select = true;
                  }
              }
          }

        bool push_it = false;
        if (select)
          {
            if (! reverse_) push_it = true;
          }
        else
          {
            if (reverse_) push_it = true;
          }
        if (push_it)
          {
            if (std::find(names_.begin(), names_.end(), key) == names_.end())
              {
                names_.push_back (key);
              }
          }
      }
    return;
  }

  void histogram_pool::tree_dump(std::ostream& out_,
                                 const std::string& title_,
                                 const std::string& indent_,
                                 bool inherit_) const
  {
    std::string indent;
    if (!indent_.empty()) indent = indent_;
    if (!title_.empty()) out_ << indent << title_ << std::endl;

    out_ << indent << datatools::i_tree_dumpable::tag
         << "Description : '" <<  this->get_description() << "'" << std::endl;


    out_ << indent << datatools::i_tree_dumpable::tag
         << "Histograms  : ";
    if (_dict_.empty ())
      {
        out_ << "<empty>";
      }
    out_ << endl;
    for (dict_type::const_iterator i = _dict_.begin ();
         i != _dict_.end ();
         i++)
      {
        const std::string& key = i->first;
        const histogram_entry_type& he = i->second;
        out_ << indent << datatools::i_tree_dumpable::skip_tag;
        std::ostringstream indent_oss;
        indent_oss << indent << datatools::i_tree_dumpable::skip_tag;
        dict_type::const_iterator j = i;
        j++;
        if (j == _dict_.end()) {
          out_ << datatools::i_tree_dumpable::inherit_tag(inherit_);
          indent_oss << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        } else {
          out_ << datatools::i_tree_dumpable::tag;
          indent_oss << datatools::i_tree_dumpable::skip_tag;
        }
        out_ << "Name : " << '"' << key << '"' << std::endl;
        he.tree_dump(out_, "", indent_oss.str());
      }


    out_ << indent << datatools::i_tree_dumpable::inherit_tag (inherit_)
         << "Auxiliaries : ";
    if (_auxiliaries_.empty ())
      {
        out_ << "<empty>";
      }
    out_ << endl;
    {
      ostringstream indent_oss;
      indent_oss << indent;
      indent_oss << datatools::i_tree_dumpable::inherit_skip_tag (inherit_) ;
      _auxiliaries_.tree_dump (out_, "", indent_oss.str ());
    }

    return;
  }

} // end of namespace mygsl

/***************
 * OCD support *
 ***************/

#include <datatools/ocd_macros.h>

// OCD support for class '::mygsl::histogram_pool' :
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(::mygsl::histogram_pool,ocd_)
{
  ocd_.set_class_name ("mygsl::histogram_pool");
  ocd_.set_class_description ("A pool of histograms");
  
   
  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("description")
      .set_terse_description("The embeded description string")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description(
                            "Superseded by a former call of :                  \n"
                            "  mygsl::histogram_pool::set_description(...)     \n"
                            )
      ;
  }  
  
  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("histo.setups")
      .set_terse_description("A list of configuration file names for embeded histograms")
      .set_traits(datatools::TYPE_STRING, 
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(false)
      .set_path(true)
      .set_long_description(
                            "A list of filenames from where the histogram pool      \n"
                            "loads the directives to dynamically instantiate        \n"
                            "new embeded histogram objects. The filenames may contains\n"
                            "some environment variables.                            \n"
                            "Example:                                               \n"
                            "  \"${CONFIG_REPOSITORY_DIR}/histo_1d.conf\"           \n"
                            "The target files must use the format of the            \n"
                            "'datatools::multi_properties' class.                   \n"
                            "The loading order of the files is critical             \n"
                            "because some histograms may depend on other ones       \n"
                            "which should thus be defined *before* their            \n"
                            "dependers.                                             \n"
                            "Extends the instantiation of histograms triggered by   \n"
                            "former calls to :                                      \n"
                            "  mygsl::histogram_pool::load(...)                     \n"
                            "See OCD support for 'mygsl::histogram_1d'              \n"
                            "and 'mygsl::histogram_2d' classes.                     \n"
                            )
    ;
  }  

  ocd_.set_configuration_hints ("The histogram pool uses a 'datatools::properties' object        \n"
                                "to initialize its behaviour and contents.                       \n"
                                "                                                                \n"
                                "Example of configuration for the histogram pool :               \n"
                                "  |                                                             \n"
                                "  | description  : string = \"The data analysis histogram pool\"\n"
                                "  | histo.setups : string[1] as path = \"histos.conf\"          \n"
                                "  |                                                              \n"
                                "                                                                \n"
                                "Example of associated file 'histos.conf' for automated          \n"
                                "construction of four histograms :                               \n"
                                "  |                                                             \n"
                                "  | #@description A sample list of histograms build rules       \n"
                                "  | #@key_label   \"name\"                                      \n"
                                "  | #@meta_label  \"type\"                                      \n"
                                "  |                                                             \n"
                                "  | [name=\"E1\" type=\"mygsl::histogram_1d\"]                  \n"
                                "  | mode   : string = \"regular\"                               \n"
                                "  | unit   : string = \"MeV\"                                   \n"
                                "  | linear : boolean = 1                                        \n"
                                "  | min    : real = 0.0                                         \n"
                                "  | max    : real = 3.0                                         \n"
                                "  | number_of_bins : integer = 120                              \n"
                                "  | unit.type : string = \"energy\"                             \n"
                                "  | display.xaxis.unit  : string = \"keV\"                      \n"
                                "  | display.xaxis.label : string = \"E1\"                       \n"
                                "  |                                                             \n"
                                "  | [name=\"E2\" type=\"mygsl::histogram_1d\"]                  \n"
                                "  | mode : string = \"mimic\"                                   \n"
                                "  | display.xaxis.unit  : string = \"keV\"                      \n"
                                "  | display.xaxis.label : string = \"E2\"                       \n"
                                "  | mimic.histogram_1d  : string = \"E1\"                       \n"                       
                                "  |                                                             \n"                    
                                "  | [name=\"E1+E2\" type=\"mygsl::histogram_1d\"]               \n"
                                "  | mode : string = \"mimic\"                                   \n"
                                "  | unit : string = \"keV\"                                     \n"
                                "  | unit.type : string = \"energy\"                             \n"
                                "  | display.xaxis.unit : string = \"MeV\"                       \n"
                                "  | mimic.histogram_1d : string = \"E1\"                        \n"                       
                                "  |                                                             \n"                    
                                "  | [name=\"E1xE2\" type=\"mygsl::histogram_2d\"]               \n"
                                "  | mode : string = \"regular\"                                 \n"
                                "  | x.unit : string = \"keV\"                                   \n"
                                "  | x.unit.type : string = \"energy\"                           \n"
                                "  | x.linear : boolean = 1                                      \n"                       
                                "  | x.min : real = 0.0                                          \n"
                                "  | x.max : real = 3.0                                          \n"
                                "  | x.number_of_bins : integer = 120                            \n"
                                "  | display.xaxis.unit  : string = \"keV\"                      \n"
                                "  | display.xaxis.label : string = \"E1\"                       \n"
                                "  | y.unit : string = \"keV\"                                   \n"
                                "  | y.unit.type : string = \"energy\"                           \n"
                                "  | y.linear : boolean = 1                                      \n"                       
                                "  | y.min : real = 0.0                                          \n"
                                "  | y.max : real = 3.0                                          \n"
                                "  | y.number_of_bins : integer = 120                            \n"
                                "  | display.yaxis.unit  : string = \"keV\"                      \n"
                                "  | display.yaxis.label : string = \"E2\"                       \n"
                                "  |                                                              \n"
                                );
  ocd_.set_validation_support(true);
  ocd_.lock(); 
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()


// OCD support for class '::mygsl::histogram' :
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(::mygsl::histogram,ocd_)
{
  ocd_.set_class_name ("mygsl::histogram_1d");
  ocd_.set_class_description ("One dimension histogram as built by the 'mygsl::histogram_pool' class");

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("title")
      .set_terse_description("The histogram title mode")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("A human friendly title for the histogram (handled by the histogram pool)")
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("group")
      .set_terse_description("The name of the group the histogram is assigned to by the histogram pool")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("The group may be used for selection operations on histograms")
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode")
      .set_terse_description("The histogram build mode")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("The mode can take the following values :             \n"
                            " - \"regular\" (default)                             \n"
                            " - \"table\"                                         \n"
                            " - \"mimic\"                                         \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("unit")
      .set_terse_description("The histogram unit symbol on the X-axis")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("The unit symbol on X-axis should be known from \n"
                            "the 'datatools::units' module.                 \n"
                            "Example:                                       \n"
                            "  | unit : string = \"keV\"                    \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("unit.type")
      .set_terse_description("The histogram physical dimension on the X-axis")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "regular;table")
      //.set_complex_triggering_conditions(true)
      .set_long_description(
                            "The unit type should be known from the 'datatools::units' module. \n"
                            "The 'unit.type' property should be compatible with the 'unit'     \n"
                            "property if given previously.                                     \n"
                            "Example:                                                          \n"
                            "  | unit.type : string = \"energy\"                               \n"
                            "This property is only defined for modes 'regular' and 'table'.    \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("display.xaxis.unit")
      .set_terse_description("The histogram display physical unit on the X-axis")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "regular;table")
      .set_long_description(
                            "The display unit type should be known from the 'datatools::units' \n"
                            "module and of the same physical dimension the unit.               \n"
                            "The 'display.xaxis.unit' property should be compatible with the   \n"
                            "'unit' property if given previously.                              \n"
                            "Example:                                                          \n"
                            "  | display.xaxis.unit : string = \"MeV\"                         \n"
                            "This property is only defined for modes 'regular' and 'table'.    \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("linear")
      .set_terse_description("The flag for linear regular binning")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "regular")
      .set_long_description("The 'linear' flag setups the linear regular binning of the histogram. \n"
                            "                                                                      \n"
                            "Example:                                                              \n"
                            "  | linear : boolean = 1                                              \n"
                            "This property is only defined for mode 'regular'.                     \n"
                            "This property is not compatible with the 'logarithmic' flag.          \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("logarithmic")
      .set_terse_description("The flag for logarithmic regular binning")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "regular")
      .set_long_description(
                            "The 'logarithmic' property sets the logarithmic regular binning  \n"
                            " of the histogram.                                               \n"
                            "Example:                                                         \n"
                            "  | logarithmic : boolean = 1                                    \n"
                            "This property is only defined for mode 'regular'.                \n"
                            "This property is not compatible with the 'linear' flag.          \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("min")
      .set_terse_description("The minimal bound of the histogram")
      .set_traits(datatools::TYPE_REAL)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "regular")
      .set_long_description("The 'min' property sets the lower bound of the regular histogram.\n"
                            "Example:                                                         \n"
                            "  | min : real = 0.0                                             \n"
                            "This property should be lower than the 'max' value.              \n"
                            "This property is only defined for mode 'regular'.                \n"
                           )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("max")
      .set_terse_description("The maximal bound of the histogram")
      .set_traits(datatools::TYPE_REAL)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "regular")
      .set_long_description("The 'max' property sets the upper bound of the regular histogram.\n"
                            "Example:                                                         \n"
                            "  | max : real = 10.0                                            \n"
                            "This property should be greater than the 'min' value.            \n"
                            "This property is only defined for mode 'regular'.                \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("number_of_bins")
      .set_terse_description("The number of bins of the histogram")
      .set_traits(datatools::TYPE_INTEGER)
      .set_mandatory(true)
      .set_triggered_by_label("mode", "regular")
      .set_long_description("The 'number_of_bins' property sets the total number of bins.     \n"
                            "Example:                                                         \n"
                            " | number_of_bins : integer = 20                                 \n"
                            "This property is only defined for mode 'regular'.                \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("bounds")
      .set_terse_description("The bounds of tabulated bins")
      .set_traits(datatools::TYPE_REAL, 
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "table")
      .set_long_description("The 'bounds' of the histogram's bins.                            \n"
                            "Example:                                                         \n"
                            " | bounds : real[6] = 0.0 1.0 3.0 6.0 10.0 15.0                  \n"
                            "This property is only defined for mode 'table'.                  \n"
                            "This property is incompatible with 'bounds.file'.                \n"
                          )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("bounds.file")
      .set_terse_description("The name of a file that stores the bounds of tabulated bins")
      .set_traits(datatools::TYPE_STRING)
      .set_path(true)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "table")
      .set_long_description("The name of a file where the bins' bounds are stored.            \n"
                            "Example:                                                         \n"
                            " | bounds.file : string as path = \"bins.data\"                  \n"
                            "This property is only defined for mode 'table'.                  \n"
                            "This property is incompatible with 'bounds'.                     \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mimic.histogram_1d")
      .set_terse_description("The name of a 1D-histogram to be cloned")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "mimic")
      .set_long_description("Example:                                                         \n"
                            "  | mimic.histogram_1d : string = \"hx\"                         \n"
                            "This property is only defined for mode 'mimic'.                  \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mimic.histogram_1d.import_aux")
      .set_terse_description("The list of prefixes for auxiliary properties to be imported from the cloned 1D-histogram")
      .set_traits(datatools::TYPE_STRING, 
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "mimic")
      .set_long_description("Example:                                                         \n"
                            "  | mimic.histogram_1d.import_aux : string[1] = \"display.\"     \n"
                            "This property is only defined for mode 'mimic'.                  \n"
                            "It is used with the 'mimic.histogram_1d' property.               \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mimic.histogram_2d")
      .set_terse_description("The name of a 2D-histogram from which one of the axis is to be cloned")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "mimic")
      .set_long_description("Example:                                                         \n"
                            "  | mimic.histogram_2d : string = \"hxy\"                        \n"
                            "This property is only defined for mode 'mimic'.                  \n"
                            )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mimic.histogram_2d.axis")
      .set_terse_description("Select the axis of the original 2D-histogram to be used")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(true)
      .set_triggered_by_label("mode", "mimic")
      .set_long_description("Example:                                                         \n"
                            "  | mimic.histogram_2d.axis : string = \"x\"                     \n"
                            "Possible values are 'x' and 'y'.                                 \n"
                            "This property is only defined for mode 'mimic'.                  \n"
                            "It is used with the 'mimic.histogram_2d' property.               \n"
                           )
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mimic.histogram_2d.import_aux")
      .set_terse_description("The list of prefixes for auxiliary properties to be imported from the cloned 2D-histogram")
      .set_traits(datatools::TYPE_STRING, 
                  datatools::configuration_property_description::ARRAY)
      .set_mandatory(false)
      .set_triggered_by_label("mode", "mimic")
      .set_long_description("Example:                                                         \n"
                            "  |                                                              \n"
                            "  | mimic.histogram_1d.import_aux : string[1] = \"display.\"     \n"
                            "  |                                                              \n"
                            "This property is only defined for mode 'mimic'.                  \n"
                            "It is used with the 'mimic.histogram_2d' property.               \n"
                             )
      ;
  }  

  ocd_.set_configuration_hints ("Display properties:                                             \n"
                                "  All properties the name of which starts with the prefix       \n"
                                " \"display.\" are preserved and passed to the histogram object. \n"
                                "                                                                \n"
                                "Here are typical examples:                                      \n"
                                "                                                                \n"
                                "A 1D-histogram with linear regular bins :                       \n"
                                "  |                                                             \n"
                                "  | #@key_label  \"name\"                                       \n"
                                "  | #@meta_label \"type\"                                       \n"
                                "  |                                                             \n"
                                "  | [name=\"E1\" type=\"mygsl::histogram_1d\"]                  \n"
                                "  | mode                : string  = \"regular\"                 \n"
                                "  | linear              : boolean = 1                           \n"
                                "  | unit                : string  = \"keV\"                     \n"
                                "  | min                 : real    = 0.0                         \n"
                                "  | max                 : real    = 3000.0                      \n"
                                "  | number_of_bins      : integer = 120                         \n"
                                "  | unit.type           : string = \"energy\"                   \n"
                                "  | display.xaxis.unit  : string = \"MeV\"                      \n"
                                "  | display.xaxis.label : string = \"Energy\"                   \n"
                                "  | display.color       : string = \"red\"                      \n"
                                "  |                                                             \n"
                                "                                                                \n"
                                "A 1D-histogram build from explicit binning :                    \n"
                                "  |                                                             \n"
                                "  | [name=\"E2\" type=\"mygsl::histogram_1d\"]                  \n"
                                "  | mode               : string  = \"table\"                    \n"
                                "  | bounds             : real[5] = 0.0 1.0 2.5 4.5 7.0          \n"
                                "  | unit.type          : string  = \"energy\"                   \n"
                                "  | display.xaxis.unit : string = \"MeV\"                       \n"
                                "  |                                                             \n"
                                "                                                                \n"
                                "A 1D-histogram build from a 1D histogram :                      \n"
                                "  |                                                             \n"
                                "  | [name=\"E3\" type=\"mygsl::histogram_1d\"]                  \n"
                                "  | mode               : string = \"mimic\"                     \n"
                                "  | mimic.histogram_1d : string = \"E1\"                        \n"
                                "  | mimic.histogram_1d.import_aux : string[1] = \"display.\"    \n"
                                "  |                                                             \n"
                                "                                                                \n"
                                "A 1D-histogram build from the Y-layout of a 2D histogram :      \n"
                                "  |                                                             \n"
                                "  | [name=\"E3\" type=\"mygsl::histogram_1d\"]                  \n"
                                "  | mode                    : string = \"mimic\"                \n"
                                "  | mimic.histogram_2d      : string = \"E1xE2\"                \n"
                                "  | mimic.histogram_2d.axis : string = \"y\"                    \n"
                                "  |                                                             \n"
                                "                                                                \n"
                                ); 
  ocd_.set_validation_support(true);
  ocd_.lock(); 
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()


// OCD support for class '::mygsl::histogram_2d' :
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(::mygsl::histogram_2d,ocd_)
{
  ocd_.set_class_name ("mygsl::histogram_2d");
  ocd_.set_class_description ("Two dimension histogram as built by the 'mygsl::histogram_pool' class");

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("title")
      .set_terse_description("The histogram title mode")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("A human friendly title for the histogram (handled by the histogram pool)")
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("group")
      .set_terse_description("The name of the group the histogram is assigned to by the histogram pool")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("The group may be used for selection operations on histograms")
      ;
  }  

  {
    configuration_property_description & cpd = ocd_.add_property_info();
    cpd.set_name_pattern("mode")
      .set_terse_description("The histogram build mode")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("The mode can take the following values :             \n"
                            " - \"regular\" (default)                             \n"
                            " - \"table\"                                         \n"
                            " - \"mimic\"                                         \n"
                            )
      ;
  }  
  

  ocd_.set_configuration_hints ("Display properties:                                             \n"
                                "  All properties the name of which starts with the prefix       \n"
                                "  \"display.\" are preserved and passed to the histogram object.\n"
                                "                                                                \n"
                                "Examples :                                                      \n"
                                "  Not implemented yet.                                          \n"
                                );

  ocd_.set_validation_support(true);
  ocd_.lock(); 
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()

DOCD_CLASS_SYSTEM_REGISTRATION(::mygsl::histogram_pool,"mygsl::histogram_pool")
DOCD_CLASS_SYSTEM_REGISTRATION(::mygsl::histogram_1d,  "mygsl::histogram_1d")
DOCD_CLASS_SYSTEM_REGISTRATION(::mygsl::histogram_2d,  "mygsl::histogram_2d")

// end of mygsl::histogram_pool.cc

