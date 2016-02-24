/* test_enriched_base.cxx */

// Third Party:
// - Boost:
#include <boost/scoped_ptr.hpp>

// This Project:
#include <datatools/datatools_config.h>
#include <datatools/logger.h>
#include <datatools/enriched_base.h>
#include <datatools/properties.h>
#include <datatools/io_factory.h>
#if DATATOOLS_WITH_REFLECTION == 1
//#include <datatools/reflection_guard.h>
#include <datatools/reflection_macros.h>
#endif // DATATOOLS_WITH_REFLECTION == 1

// Ourselves:
#include <datatools/enriched_base.ipp>

void test1();
void test2();
void test_reflection();

int main (int /* argc_ */, char ** /* argv_ */) {
  int error_code = EXIT_SUCCESS;
  try {
    test1();
    test2();
#if DATATOOLS_WITH_REFLECTION == 1
    test_reflection();
#endif // DATATOOLS_WITH_REFLECTION == 1
  }
  catch (std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ERROR, error.what());
    error_code = EXIT_FAILURE;
  }
  catch (...){
    DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "Unexpected error !");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void test1()
{
  datatools::enriched_base obj;
  obj.set_name_c("object1")
    .set_display_name_c("o1")
    .set_terse_description_c("A dummy object.")
    .set_logging_priority_c(datatools::logger::PRIO_DEBUG)
    .grab_auxiliaries().store("color", "blue")
    ;
  obj.tree_dump(std::cout, "An object : ");

  {
    datatools::data_writer writer("test_enriched_base.xml");
    writer.store(obj);
  }
  return;
}

void test2()
{

  datatools::properties config;
  config.store("name", "testing_name");
  config.store("display_name", "Testing");
  config.store("terse_description", "This is a test object");
  config.store("logging.priority", "debug");
  config.store_flag("aux.flag");
  config.store("aux.dummy", "foo");
  config.store("aux.user", "John Doe");

  datatools::enriched_base obj;
  obj.initialize(config);
  obj.tree_dump(std::cout, "An object : ");

  return;
}

#if DATATOOLS_WITH_REFLECTION == 1
void test_reflection()
{
  std::cerr << "\n******** Entering test_reflection... " << std::endl;
  const DR_CLASS & aoMetaClass = DR_CLASS_BY_NAME("datatools::enriched_base");
  std::cerr << "******** Go. " << std::endl;

  //boost::scoped_ptr<datatools::enriched_base> ao;
  DR_OBJECT aoObj = aoMetaClass.construct();
  //ao.reset(eb);
  //DR_OBJECT aoObj = ao.get();
  aoObj.call("smart_print", DR_ARGS(1, "Advanced object: ", "*** "));
  std::cerr << "******** Test 1. " << std::endl;

  // Change its 'name' attribute/property :
  aoObj.set("name", DR_VALUE("MyAdvancedObject"));
  std::cerr << "******** Test 2. " << std::endl;

  // Change its 'terse description' attribute/property :
  aoObj.set("terse_description", DR_VALUE("A dummy object"));
  std::cerr << "******** Test 3. " << std::endl;

  DR_VALUE auxVal = aoObj.DR_CALL("grab_auxiliaries");
  DR_OBJECT auxObj = auxVal.to<DR_OBJECT>();
  auxObj.call("store_string", DR_ARGS("Name", "Arthur", "The name fo a King", false));
  auxObj.call("store_real", DR_ARGS("Pi",3.14159, "The Pi constant", false));
  auxObj.call("store_flag", DR_ARGS("Dummy", "A dummy flag", false));
  auxObj.call("smart_print", DR_ARGS(1, "Auxiliaries: ", ">>> "));
  std::cerr << "******** Test 4. " << std::endl;

  aoObj.set("logging_priority", DR_VALUE("trace"));
  aoObj.set("name", DR_VALUE("Mao"));
  aoObj.set("display_name", DR_VALUE("MAO"));
  aoObj.call("smart_print", DR_ARGS(1, "Advanced object: ", "*** "));

  std::cerr << "******** End. " << std::endl;

  return;
}
#endif // DATATOOLS_WITH_REFLECTION == 1
