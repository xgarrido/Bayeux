// -*- mode: c++ ; -*-
// test_brio.cxx

#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

#include <brio/brio_config.h>
#include <brio/version.h>

using namespace std;

int main (int /* argc_ */, char ** /* argv_ */)
{
  int error_code = EXIT_SUCCESS;
  try {
    clog << "Test program for the 'brio' program library." << endl;

    /*
    bool debug = false;
    bool verbose = false;

    int iarg = 1;
    while (iarg < argc_) {
      string token = argv_[iarg];

      if (token[0] == '-') {
        string option = token;
        if ((option == "-d") || (option == "--debug")) {
          debug = true;
        } else if ((option == "-v") || (option == "--verbose")) {
          verbose = true;
        } else {
          clog << "warning: ignoring option '" << option << "'!" << endl;
        }
      } else {
        string argument = token;
        {
          clog << "warning: ignoring argument '" << argument << "'!" << endl;
        }
      }
      iarg++;
    }
    */

    clog << "brio version : " << BRIO_LIB_VERSION << endl;

    clog << "The end." << endl;
  }
  catch (exception & x) {
    cerr << "error: " << x.what () << endl;
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    cerr << "error: " << "unexpected error !" << endl;
    error_code = EXIT_FAILURE;
  }
  return (error_code);
}

// end of test_brio.cxx
