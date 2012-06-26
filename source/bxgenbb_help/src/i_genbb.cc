// -*- mode: c++; -*- 
// i_genbb.cc
/*
 * Copyright 2007-2011 F. Mauger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Publi * License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Publi * License for more details.
 * 
 * You should have received a copy of the GNU General Publi * License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA.
 *
 */

#include <sstream>
#include <stdexcept>

#include <genbb_help/i_genbb.h>
#include <mygsl/rng.h>

namespace genbb {
 
  i_genbb::i_genbb ()
  {
    _external_random_ = 0;
    return;
  }
  
  i_genbb::~i_genbb ()
  {
    return;
  }

  void i_genbb::load_next (primary_event & event_, 
                           bool compute_classification_)
  { 
    _load_next (event_, compute_classification_);
    return;
  }

  bool i_genbb::has_external_random () const
  {
    return _external_random_ != 0;
  }

  bool i_genbb::can_external_random () const
  {
    return false;
  }

  void i_genbb::reset_external_random ()
  {
    _external_random_ = 0;
    return;
  }

  void i_genbb::set_external_random (mygsl::rng & r_)
  {
    if (! can_external_random ())
      {
        throw std::logic_error ("genbb::i_genbb::set_external_random: Not allowed !");
      }
    if (! r_.is_initialized ())
      {
        std::ostringstream message;
        message << "genbb::i_genbb::set_external_random: External PRNG is not initialized !";
        throw std::logic_error (message.str());      
      }
    _external_random_ = &r_;
    return;
  }
  

  mygsl::rng & i_genbb::grab_external_random ()
  {
    if (! has_external_random ())
      {
        throw std::logic_error ("genbb::i_genbb::grab_external_random: No available external PRNG !");
      }
    return *_external_random_;
  }


  const mygsl::rng & i_genbb::get_external_random () const
  {
    if (! has_external_random ())
      {
        throw std::logic_error ("genbb::i_genbb::get_external_random: No available external PRNG !");
      }
    return *_external_random_;
  }

} // end of namespace genbb

// end of i_genbb.cc
