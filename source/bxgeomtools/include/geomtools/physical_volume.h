// -*- mode: c++ ; -*- 
/* physical_volume.h
 * Author (s):     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2010-02-08
 * Last modified: 2010-02-08
 * 
 * License: 
 * 
 * Description: 
 *   Geometry ID
 * 
 * History: 
 * 
 */

#ifndef GEOMTOOLS_PHYSICAL_VOLUME_H_
#define GEOMTOOLS_PHYSICAL_VOLUME_H_ 1

#include <iostream>
#include <string>
#include <map>

#include <datatools/properties.h>
#include <datatools/i_tree_dump.h>

namespace geomtools {
 
  class i_placement;
  class logical_volume;

  class physical_volume
    : public datatools::i_tree_dumpable
  {

  private:
    
    void _clear_logical_ ();
    
    void _clear_placement_ ();
    
  public:

    bool is_locked () const;

    void lock ();

    void unlock ();

    const std::string & get_name () const;

    void set_name (const std::string &);

    const datatools::properties & parameters () const;

    datatools::properties & parameters ();

    bool has_placement () const;

    void set_placement (const i_placement &);

    void set_placement (const i_placement *);

    const i_placement & get_placement () const;

    bool has_logical () const;

    void set_logical (const logical_volume &);

    void set_logical (const logical_volume *);

    const logical_volume & get_logical () const;

    bool has_mother () const;

    void set_mother (const logical_volume &);

    const logical_volume & get_mother () const;

    physical_volume ();

    physical_volume (const std::string & name_);

    physical_volume (const std::string & name_, 
                     const logical_volume & logical_,
                     const logical_volume & mother_,
                     const i_placement    & placement_);

    physical_volume (const std::string & name_, 
                     const logical_volume * logical_,
                     const logical_volume & mother_,
                     const i_placement    & placement_);
    
    physical_volume (const std::string & name_,   
                     const logical_volume & logical_,
                     const logical_volume & mother_,
                     const i_placement    * placement_);
    
    physical_volume (const std::string & name_,  
                     const logical_volume * logical_,
                     const logical_volume & mother_,
                     const i_placement    * placement_);

    virtual ~physical_volume ();

    virtual void tree_dump (std::ostream & out_         = std::clog, 
                            const std::string & title_  = "", 
                            const std::string & indent_ = "", 
                            bool inherit_               = false) const;
  private:

    std::string            _name_;
    bool                   _locked_;
    datatools::properties  _parameters_;
    bool                   _own_placement_;
    const i_placement *    _placement_;
    bool                   _own_logical_;
    const logical_volume * _logical_;
    const logical_volume * _mother_;

  public:

    typedef std::map<std::string, const physical_volume *> dict_type;
      
  };

} // end of namespace geomtools

#endif // GEOMTOOLS_PHYSICAL_VOLUME_H_

// end of physical_volume.h
