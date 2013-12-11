// -*- mode: c++; -*-
//! \file datatools/multi_properties-reflect.h
#ifndef DATATOOLS_MULTI_PROPERTIES_REFLECT_H_
#define DATATOOLS_MULTI_PROPERTIES_REFLECT_H_

// Standard Library
#include <string>

// Datatools
#include <datatools/multi_properties.h>

// Export the reflection code for the 'datatools::multi_properties' class:
DR_CLASS_EXPORT (::datatools::multi_properties, "datatools::multi_properties")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::datatools::multi_properties, /*tag_*/)
      {
        // Declare and populate the reflection metaclass for
        // the 'datatools::multi_properties' class :
        DR_CLASS_DECLARE(::datatools::multi_properties)
          .DR_CLASS_BASE(::datatools::i_serializable)
          .DR_CLASS_BASE(::datatools::i_tree_dumpable)
          .DR_CLASS_CONSTRUCTOR()
          .DR_CLASS_CONSTRUCTOR2(std::string, std::string)
          .DR_CLASS_CONSTRUCTOR4(std::string, std::string, std::string, bool)

          .DR_CLASS_PROPERTY_GETTER_SETTER ("debug",
                                            ::datatools::multi_properties::is_debug,
                                            ::datatools::multi_properties::set_debug)
          .DR_CLASS_PROPERTY_GETTER_SETTER ("description",
                                            ::datatools::multi_properties::get_description,
                                            ::datatools::multi_properties::set_description)
          .DR_CLASS_PROPERTY_GETTER_SETTER ("key_label",
                                            ::datatools::multi_properties::get_key_label,
                                            ::datatools::multi_properties::set_key_label)
          .DR_CLASS_PROPERTY_GETTER_SETTER ("meta_label",
                                            ::datatools::multi_properties::get_meta_label,
                                            ::datatools::multi_properties::set_meta_label)
          .DR_CLASS_METHOD_CONST("is_debug",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::is_debug,
                                 bool)
          .DR_CLASS_VOID_METHOD1("set_debug",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::set_debug,
                                 bool)
          .DR_CLASS_VOID_METHOD1("set_description",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::set_description,
                                 const std::string &)
          .DR_CLASS_METHOD_CONST("get_description",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::get_description,
                                 const std::string &)
          .DR_CLASS_VOID_METHOD1("set_key_label",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::set_key_label,
                                 const std::string &)
          .DR_CLASS_METHOD_CONST("get_key_label",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::get_key_label,
                                 const std::string &)
          .DR_CLASS_VOID_METHOD1("set_meta_label",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::set_meta_label,
                                 const std::string &)
          .DR_CLASS_METHOD_CONST("get_meta_label",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::get_meta_label,
                                 const std::string &)
          .DR_CLASS_METHOD_CONST("size",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::size,
                                 int32_t)
          .DR_CLASS_METHOD_CONST("empty",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::empty,
                                 bool)
          .DR_CLASS_VOID_METHOD("reset",
                                ::datatools::multi_properties,
                                ::datatools::multi_properties::reset)
          .DR_CLASS_VOID_METHOD("clear",
                                ::datatools::multi_properties,
                                ::datatools::multi_properties::clear)
          .DR_CLASS_METHOD1_CONST("has_key",
                                  ::datatools::multi_properties,
                                  ::datatools::multi_properties::has_key,
                                  bool,
                                  const std::string &)
          .DR_CLASS_METHOD1_CONST("has_section",
                                  ::datatools::multi_properties,
                                  ::datatools::multi_properties::has_section,
                                  bool,
                                  const std::string &)
          .DR_CLASS_METHOD1_CONST("key",
                                  ::datatools::multi_properties,
                                  ::datatools::multi_properties::key,
                                  const std::string &, int)
          .DR_CLASS_METHOD1_CONST("ordered_key",
                                  ::datatools::multi_properties,
                                  ::datatools::multi_properties::ordered_key,
                                  const std::string &, int)
          .DR_CLASS_METHOD1_CONST("get_section",
                                  ::datatools::multi_properties,
                                  ::datatools::multi_properties::get_section_const,
                                  const ::datatools::properties &,
                                  const std::string &)
          .DR_CLASS_METHOD2("add_section",
                            ::datatools::multi_properties,
                            ::datatools::multi_properties::add_section,
                            ::datatools::properties &,
                            const std::string &, const std::string &)
          .DR_CLASS_METHOD1("grab_section",
                            ::datatools::multi_properties,
                            ::datatools::multi_properties::grab_section,
                            ::datatools::properties &,
                            const std::string &)
          .DR_CLASS_VOID_METHOD1("remove",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::remove,
                                 const std::string &)
          .DR_CLASS_VOID_METHOD3_CONST("write",
                                       ::datatools::multi_properties,
                                       ::datatools::multi_properties::write,
                                       const std::string &, bool, bool)
          .DR_CLASS_VOID_METHOD2("read",
                                 ::datatools::multi_properties,
                                 ::datatools::multi_properties::read,
                                 const std::string &, bool)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // DATATOOLS_MULTI_PROPERTIES_REFLECT_H_
