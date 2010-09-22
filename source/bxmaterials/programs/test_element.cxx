// -*- mode: c++ ; -*- 
// test_element.cxx 

#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

#include <materials/element.h>


using namespace std;

int main (int argc_, char ** argv_)
{

  int error_code = EXIT_SUCCESS;
  
  try
    {
      clog << "Test program for class 'element'!" << endl; 
  
      bool debug = false;

      int iarg = 1;
      while (iarg < argc_)
        {
          string token = argv_[iarg];

          if (token[0] == '-')
            {
               string option = token; 
               if ((option == "-d") || (option == "--debug")) 
                 {
                   debug = true;
                 }
               else 
                 { 
                    clog << "warning: ignoring option '" << option << "'!" << endl; 
                 }
            }
          else
            {
              string argument = token; 
              /* Here you may add more argument handlers... */
              { 
                clog << "warning: ignoring argument '" << argument << "'!" << endl; 
              }
            }
          iarg++;
      }    
 
      mat::isotope iso_H1; 
      iso_H1 = mat::isotope("Hydrogen 1","H",1,0,true);   
	 
      mat::isotope iso_H2 = mat::isotope ("Hydrogen 2", "H", 2, mat::isotope::GROUND_STATE, true);
      mat::isotope iso_H3 = mat::isotope ("Hydrogen 3", "H", 3, mat::isotope::GROUND_STATE, true);
      mat::isotope iso_H4 = mat::isotope ("Hydrogen 4", "H", 4);   
      mat::isotope iso_H5 = mat::isotope ("Hydrogen 5",   1, 5);  
      mat::isotope iso_C12 = mat::isotope ("Carbon 12", "C", 12, mat::isotope::GROUND_STATE,true);  
      iso_H4.build ();   
      //iso_H5.build ();   // not locked for test

      iso_H1.tree_dump (cout, "Isotope Hydrogen 1");   
      iso_H2.tree_dump (cout, "Isotope Hydrogen 2");   
      iso_H3.tree_dump (cout, "Isotope Hydrogen 3");   
      iso_H4.tree_dump (cout, "Isotope Hydrogen 4");   
      iso_H5.tree_dump (cout, "Isotope Hydrogen 5");   
      iso_C12.tree_dump (cout, "Isotope Carbon 12");   
 
      mat::element elt_H("Hydrogen",1);
      //elt_H.add_isotope (iso_C12, 0.1); 
      elt_H.add_isotope (iso_H1, 0.2);
      elt_H.add_isotope (iso_H2, 0.5);
      elt_H.add_isotope (iso_H3, 0.1); 
      //elt_H.add_isotope (iso_C12, 0.1); 
      //elt_H.add_isotope (iso_H5, 0.1); 
      elt_H.build (); 
      elt_H.tree_dump (cout, "my element Hydrogen");   
      
      /*
      elt_H.add_isotope (iso_H4, 0.1); 
      elt_H.add_isotope (iso_H5, 0.1);  
      elt_H.add_isotope (new mat::isotope("Hydrogen 5",1,5,0,true),0.2); 
      elt_H.build (); 
      elt_H.tree_dump (cout, "my element Hydrogen");                 
      */

      mat::isotope iso_Na = mat::isotope ("Sodium", 11, 23, 0, true);
      mat::element elt_Na("Sodium", "Na");
      elt_Na.add_isotope (iso_Na); 
      elt_Na.build (); 
      elt_Na.tree_dump (cout, "my element Sodium");     
    
    }
  catch (exception & x)
    {
      cerr << "error: " << x.what () << endl; 
      error_code = EXIT_FAILURE;
    }
  catch (...)
    {
      cerr << "error: " << "unexpected error!" << endl; 
      error_code = EXIT_FAILURE;
    }
  return (error_code);
}

// end of test_element.cxx
