//! \file datatools/caster_utils.h
/* Author(s)     : Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date : 2011-04-01
 * Last modified : 2013-04-22
 *
 * Copyright (C) 2011-2013 Francois Mauger <mauger@lpccaen.in2p3.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Description:
 *
 *  Some templatized caster utilities and associated macros.
 *
 */

#ifndef DATATOOLS_CASTER_UTILS_H
#define DATATOOLS_CASTER_UTILS_H

// Standard Library:
#include <iostream>
#include <typeinfo>
#include <memory>

// Third Party:
// - Boost:
// #include <boost/scoped_ptr.hpp>

namespace datatools {

  /*!<  \struct i_caster
    \brief  Templatized abstract interface class with a cast method using covariant return types .
  */
  template<typename From, typename ToBase>
  struct i_caster
  {
    virtual ToBase * cast(From *) = 0;
  };

  /*!<  \struct caster
    \brief  Templatized concrete caster class for casting pointers from a covariant class hierarchy to some other type.
  */
  template <typename From, typename ToBase, typename ToDaughter>
  struct caster
    : public i_caster<From, ToBase>
  {

    virtual ToDaughter * cast(From * ptr_) {
      return reinterpret_cast<ToDaughter*>(ptr_);
    }

    virtual ~caster() {}
  };

  template<class Base, class Derived>
  bool is_covariant(const Base & b_)
  {
    const Base * pb = &b_;
    const Derived * dummy = dynamic_cast<const Derived *>(pb);
    if (!dummy) {
      return false;
    }
    return true;
  }

} // end of namespace datatools

/// Macro to declare a caster class
#define DATATOOLS_CASTER_DECLARATION(From,ToBase,ToDaughter,CasterId,CasterGetter) \
  private:                                                              \
  static std::unique_ptr<datatools::caster<From,ToBase,ToDaughter> > CasterId; \
public:                                                                 \
 virtual datatools::i_caster<From,ToBase>* CasterGetter() const;        \
 /**/

/// Macro to implement a caster class
#define DATATOOLS_CASTER_IMPLEMENTATION(From,ToBase,ToDaughter,CasterId,CasterGetter) \
  std::unique_ptr<datatools::caster<From,ToBase,ToDaughter> > ToDaughter::CasterId; \
  datatools::i_caster<From,ToBase>* ToDaughter::CasterGetter() const {  \
    if (ToDaughter::CasterId.get() == 0) {                              \
      ToDaughter::CasterId.reset(new datatools::caster<From,ToBase,ToDaughter>); \
    }                                                                   \
    return ToDaughter::CasterId.get();                                  \
  }                                                                     \
  /**/

#endif // DATATOOLS_CASTER_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
