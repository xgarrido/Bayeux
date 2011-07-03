// -*- mode: c++; -*- 
// test_library_loader_1.cxx

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include <boost/algorithm/string.hpp>

#include <kwsys/DynamicLoader.hxx>

using namespace std;

int main (int argc_ , char ** argv_)
{
  int error_code = EXIT_SUCCESS;
  try 
    { 
      clog << "Test of the 'datatools::utils::library_loader' class." << endl; 
  
      bool   debug = false;

      int iarg = 1;
      while (iarg <  argc_) 
        {
          string arg = argv_[iarg];
          if (boost::algorithm::starts_with (arg, "-"))
            {
              if (arg == "-d" || arg == "--debug") 
                {
                  debug =  true;
                }
            }
	  iarg++;
        }
      
      vector<kwsys::DynamicLoader::LibraryHandle> h;

      clog << "************* Loading 'ssl'..." << endl;
      h.push_back (kwsys::DynamicLoader::OpenLibrary ("libssl.so"));
 
      clog << "************* Loading 'mygsl'..." << endl;
      h.push_back (kwsys::DynamicLoader::OpenLibrary ("libmygsl.so"));
 
      clog << "************* Loading 'geomtools'..." << endl;
      h.push_back (kwsys::DynamicLoader::OpenLibrary ("libgeomtools.so"));

      clog << "************* Loading 'crypto'..." << endl;
      h.push_back (kwsys::DynamicLoader::OpenLibrary ("libcrypto.so"));

      clog << "************* Load is done." << endl;

      for (int i = (h.size () - 1) ; i >= 0 ; --i)
	{
	  clog << "************* Closing library i=" << i << endl;
	  kwsys::DynamicLoader::CloseLibrary (h[i]);
	}

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
  return error_code;
}

// end of test_library_loader_1.cxx
