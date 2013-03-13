// -*- mode: c++ ; -*-
/* material.h
 * Author (s) :  Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2010-09-22
 * Last modified: 2013-03-11
 *
 * License:
 *
 * Description:
 *   A simple class to handle "abstract" material and their basic properties
 *
 *  Note:
 *   some useful data here:
 *     http://www.nucsafe.com/cms/Selecting+Gamma+Detector/43.html
 *
 * History:
 *
 */

#ifndef MATERIALS_MATERIAL_H_
#define MATERIALS_MATERIAL_H_ 1

#include <iostream>
#include <string>
#include <map>

#include <datatools/i_tree_dump.h>
#include <datatools/properties.h>

namespace materials {

  class material;
  class element;

  struct compound_entry
  {
    bool             owned;
    const element *  elt_ptr;
    const material * mat_ptr;
    double           weight;
    int              nb_of_atoms;

    compound_entry ();
    ~compound_entry ();
    bool is_valid () const;
  };

  typedef std::map<std::string, compound_entry>  composition_map_type;

  class material : public datatools::i_tree_dumpable
  {

  public:

    //! Define the proportion units :
    /*! KP_ATOM (number of atoms by molecule) or KP_MASS (% mass)
     */

 
    enum proportion_unit_type
      {
        KP_UNDEFINED = -1,
        KP_ATOM = 0,
        NUMBER_OF_ATOMS = 0,
        KP_MASS = 1,
        FRACTION_MASS = 1,
        KP_ZA = 2,
        MEAN_ZA = 2
      };

    static const double g_per_cm3;

    /// Defaut Constructor
    material (); //!< Defaut Constructor

    /// Constructor
    /*!
      \param name_ : Name of the material
    */
    material (const std::string & name_);

    /// Destructor
    virtual ~material ();       

    bool is_composed_by_mean_z_a () const;

    bool is_composed_by_number_of_atoms () const;

    bool is_composed_by_fraction_mass () const;

    const composition_map_type &  get_composition () const;   //!<  access to the composition map

    const std::string & get_name () const;

    void set_name (const std::string &);

    double get_mean_z () const;

    double get_mean_a () const;

    void set_mean_z_a (double z_, double a_);

    double get_density () const;

    void set_density (double);

    void add_element_by_mass (const element & elt_ref_ , double weight_  , bool owned_ = false);
    //!<  Add an element with weight in KP_MASS proportion unit

    void add_element_by_mass (const element * elt_ptr_ , double weight_ );
    //!<  Add an element with weight in KP_MASS proportion unit

    void add_element_by_nb_of_atoms (const element & elt_ref_ , int nb_of_atoms_  , bool owned_ = false);
    //!<  Add an element  in KP_ATOM proportion unit

    void add_element_by_nb_of_atoms (const element * elt_ptr_ , int nb_of_atoms_   );
    //!<  Add an element in KP_ATOM proportion unit
    
    void add_material_by_mass (const material & mat_ref_ , double weight_  , bool owned_ = false);
    //!<  Add an material with weight in KP_MASS proportion unit

    void add_material_by_mass (const material * mat_ptr_ , double weight_ );
    //!<  Add an element with weight in KP_MASS proportion unit

    void build (); //!<  Build the material :  compute molar mass and lock (or not).
    
    bool is_locked () const {return _locked_;} //!<  Return true if composition is valid, weights are normalized and molar mass is computed.
    
    const datatools::properties & get_properties () const {return _properties_;} //!< Get reference of datatools::properties private attribute
    
    datatools::properties & grab_properties () {return _properties_;} //!< Get reference of datatools::properties private attribute
    
    virtual void tree_dump (std::ostream & out_  = std::clog,
                            const std::string & title_  = "",
                            const std::string & indent_ = "",
                            bool inherit_ = false) const;  //!<  print info virtual method
 
    bool is_alias() const;

  private :

    void _lock_check_ (const std::string & where_) const;

    void _compute_molar_mass_ ();         //!<  Compute molar mass in[g/mol]

    void _lock_ ();                       //!<  Lock the element : boolean flag '_locked_' is set to true

    void _set_mean_z_ (double);

    void _set_mean_a_ (double);
    
    void _normalize_weights_ ();
    
  private:
    
    std::string _name_;    //!< Name
    double      _density_; //!< Density
    double      _mean_z_;  //!< Mean atomic number
    double      _mean_a_;  //!< Mean mass number

    // composition:

    proportion_unit_type  _proportion_unit_; //!< KP_ATOM (number of atoms by molecule) or KP_MASS (% mass)
    composition_map_type  _composition_;     //!<  composition of the material [std::map<string, compound_entry>]

    datatools::properties _properties_;      //!< datatools properties
    bool                  _locked_;          //!< boolean flags : true when composition is validated & derived properties are computed

  };

} // end of namespace materials

#endif // MATERIALS_MATERIAL_H_

// end of material.h
