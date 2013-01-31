// -*- mode: c++ ; -*- 
/* geomtools_test_model_2
 * Author (s) :     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2010-02-24
 * Last modified: 2010-02-24
 * 
 * License: 
 * 
 * Description: 
 *   Factory for geometry models
 * 
 * History: 
 * 
 */

#ifndef GEOMTOOLS_TEST_TEST_MODEL_2_H_
#define GEOMTOOLS_TEST_TEST_MODEL_2_H_ 1

#include <iostream>
#include <string> 
#include <exception>

#include <geomtools/i_model.h>

#include <geomtools/box.h>
#include <geomtools/placement.h>
#include <geomtools/physical_volume.h>
#include <geomtools/visibility.h>

#include <geomtools_test_model_1.h>

namespace geomtools {

  using namespace std;

  // define a geometry model with a single box: 
  class test_model_2 : public geomtools::i_model 
    {

    public: 

      const geomtools::box & get_solid () const;
  
      test_model_2 ();
  
      virtual ~test_model_2 ();

      virtual string get_model_id () const;
 
      virtual void tree_dump (ostream & out_         = clog, 
                              const string & title_  = "", 
                              const string & indent_ = "", 
                              bool inherit_          = false) const;

    protected:
  
      virtual void _at_construct (const string & name_,
                                  const datatools::properties & config_,
                                  models_col_type * models_ = 0);
     private:

      double                      __distance;
      double                      __phi;
      double                      __theta;
      const test_model_1 *        __sub1_model;
      const test_model_1 *        __sub2_model;
      geomtools::placement        __sub1_placement;
      geomtools::placement        __sub2_placement;
      geomtools::physical_volume  __sub1_phys;
      geomtools::physical_volume  __sub2_phys;
      geomtools::box              __solid;

      // registration interface :
      GEOMTOOLS_MODEL_REGISTRATION_INTERFACE(test_model_2);
   
    };

} // end of namespace geomtools

#endif // GEOMTOOLS_TEST_TEST_MODEL_2_H_

// end of geomtools_test_model_2.h
