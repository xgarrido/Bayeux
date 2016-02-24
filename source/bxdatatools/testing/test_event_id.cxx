// test_event_id.cxx

// Ourselves:
#include <datatools/event_id.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <list>

// This project:
#include <datatools/io_factory.h>

// Some pre-processor guard about Boost I/O usage and linkage :
//#include <datatools/bio_guard.h>

int main (int /* argc_ */, char ** /* argv_ */)
{
  using namespace std;
  int error_code = EXIT_SUCCESS;
  try
    {
      clog << "Hello, this is a test program for class 'datatools::event_id'!" << endl;

      datatools::event_id my_id;

      my_id.set (666, 34);

      clog << "Event id = " << my_id << endl;

      try {
        std::istringstream ins("666_7635");
        clog << "Enter event id [format=XXX_YYY] ? ";
        ins >> my_id;
        if (! ins) {
          throw runtime_error ("Format error!");
        }
        clog << "Event id = " << my_id << endl;
        my_id.tree_dump (clog, "datatools::event_id:");
      }
      catch (exception & x) {
        cerr << "Format error !" << endl;
      }

      string filename = "test_event_id.xml";
      {
        clog << "Storing..." << endl;
        list<datatools::event_id> ids;
        datatools::data_writer writer(filename);

        for (int i = 0; i < 4; i++)
          {
            ids.push_back (datatools::event_id (666, i));
            ids.back ().tree_dump (clog, "event_id","<<< ");
            writer.store (ids.back ());
          }
      }

      {
        clog << "Loading..." << endl;
        list<datatools::event_id> ids;
        datatools::data_reader reader (filename);
        while (reader.has_record_tag ())
          {
            if (reader.record_tag_is (datatools::event_id::SERIAL_TAG))
              {
                datatools::event_id id;
                ids.push_back (id);
                reader.load (ids.back ());
                ids.back ().tree_dump (clog, "event_id",">>> ");
              }
            else
              {
                break;
              }
          }
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
