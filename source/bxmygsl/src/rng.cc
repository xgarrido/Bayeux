// mygsl::rng.c
/*
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
 */

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <ctime>

#include <boost/scoped_ptr.hpp>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
 
#include <datatools/properties.h>
#include <datatools/ioutils.h>

#include <mygsl/rng.h>

namespace mygsl {

  using namespace std;

  /*****************************************************/

  // PIMPL :
  struct prng_wrapper
  {
    prng_wrapper();
    ~prng_wrapper();
    const gsl_rng * get() const;
    gsl_rng * grab();
    void reset(gsl_rng * r_ = 0);
    gsl_rng * _r_; /// The internal GSL random number generator
  };

  void prng_wrapper::reset(gsl_rng * r_)
  {
    if (_r_ == r_) return;
    if (_r_ != 0)
      {
        gsl_rng_free (_r_);
      }
    _r_ = r_;
    return;
  }

  const gsl_rng * prng_wrapper::get() const
  {
    return _r_;
  }

  gsl_rng * prng_wrapper::grab()
  {
    return _r_;
  }
  
  prng_wrapper::prng_wrapper()
  {
    _r_ = 0;
    return;
  }

  prng_wrapper::~prng_wrapper()
  {
    reset(0);
    return;
  }

  /*****************************************************/

  // PIMPL :
  struct rng_initializer
  {
  public:
    rng_initializer ();
    ~rng_initializer ();
    static rng_initializer & instance();
  public:
    std::map<std::string, const gsl_rng_type *> dict;
  };

  // static 
  rng_initializer & rng_initializer::instance()
  {
    static boost::scoped_ptr<rng_initializer> g_scoped_initializer(0);
    if (g_scoped_initializer.get() == 0)
      {
        g_scoped_initializer.reset(new rng_initializer);
      }
    return *g_scoped_initializer;
  }
 
  rng_initializer::rng_initializer ()
  {
    bool devel = false;
    if (devel) 
      {
        std::cerr << "DEVEL: mygsl::rng_initializer::ctor: " 
                  << "Initialize dictionnary." << std::endl;
      }
    const gsl_rng_type ** t0 = gsl_rng_types_setup ();
    for (const gsl_rng_type ** t = t0; *t != 0; t++) 
      {
        std::string id ((*t)->name);
        dict[id] = *t;
      }
    return;
  }

  rng_initializer::~rng_initializer ()
  {
    dict.clear();
    return;
  }

  /*****************************************************/
 
  /* Internal state of a generator like "mt19937"
   * use far more memory than "taus2".
   */
  const std::string rng::DEFAULT_RNG_ID = "taus2"; 
  const std::string rng::DEFAULT_RNG_TYPE = rng::DEFAULT_RNG_ID; 

  void rng::default_setup ()
  {
    gsl_rng_env_setup ();
    return;
  }

  void rng::dump (std::ostream & out_) const
  {
    out_ << "mygsl::rng::dump: " << endl;
    out_ << "|-- " << "Initialized   : " << is_initialized () << endl;
    out_ << "|-- " << "ID            : '" << _id_ << "'" << endl;
    if (is_initialized ()) out_ << "|-- ";
    else out_ << "`-- ";
    out_ << "Seed          : " << _seed_ << endl;
    if (is_initialized ())
      {
        out_ << "|-- " << "GSL PRNG name : '" << name () << "'" << endl;
        out_ << "`-- " << "GSL PRNG internal state size : " 
             << get_internal_state_size ()
             << endl;
      }
    return;
  }

  bool rng::is_id_valid(const std::string &id_)
  {
    std::map<std::string,const gsl_rng_type *>::const_iterator found 
      = rng_initializer::instance().dict.find(id_);
    return found != rng_initializer::instance().dict.end();
  }

  bool rng::is_seed_valid(int32_t seed_)
  {
    return seed_ >= 0;
  }

  bool rng::is_initialized () const
  {
    return _r_ != 0;
  }

  void rng::print_dict (std::ostream & out_)
  {
    bool devel = false;
    if (devel) 
      {
        std::clog << "Available GSL random number generators: " 
                  << std::endl;
      }
    std::map<std::string,const gsl_rng_type *>::const_iterator i;
    for (i = rng_initializer::instance().dict.begin (); 
         i != rng_initializer::instance().dict.end (); 
         i++) 
      {
        out_ << i->first << std::endl; 
      } 
    return;
  }

  const std::string & rng::get_id() const
  {
    return _id_;
  }

  int32_t rng::get_seed() const
  {
    return _seed_;
  }

  void rng::set_id (const std::string & id_)
  {
    if (id_.empty()) { 
      _id_ = DEFAULT_RNG_ID;
    }
    else {
      _id_ = id_;
    }   
    return;
  }

  bool rng::is_id_valid() const
  {
    return ::mygsl::rng::is_id_valid(_id_);
  }

  bool rng::is_seed_time() const
  {
    return _seed_ == random_utils::SEED_TIME;
  }

  bool rng::is_seed_invalid() const
  {
    return ! is_seed_valid ();
  }

  bool rng::is_seed_valid() const
  {
    return ::mygsl::rng::is_seed_valid (_seed_);
  }

  void rng::set_seed (int32_t seed_)
  {
    if (seed_ < 0) {
      _seed_ = random_utils::SEED_INVALID;
    }
    else {
      _seed_ = seed_;
    }
    return;
  }

  void rng::init (const std::string & id_, int32_t seed_)
  {
    this->initialize (id_, seed_);
    return;
  }
  
  void rng::initialize ()
  {
    _initialize_();
    return;
  }
  
  void rng::initialize (int32_t seed_)
  {
    set_seed(seed_);
    _initialize_();
    return;
  }
  
  void rng::initialize (const std::string & id_, int32_t seed_)
  {
    set_id(id_);
    set_seed(seed_);
    _initialize_();
    return;
  }
 
  void rng::initialize (const datatools::properties & config_)
  {
    if (_seed_ < 0)
      {
        if (config_.has_key("prng.seed"))
          {
            int32_t seed = config_.fetch_integer("prng.seed");
            if (! is_seed_valid (seed)) {
              std::ostringstream message;
              message << "mygsl::rng::_initialize_: Invalid seed value '" 
                      << seed << "' from property '" << "prng.seed" << "' !";
              throw std::logic_error (message.str ());              
            }
            set_seed (seed);
          }
      }
    if (config_.has_key("prng.id"))
      {
        const std::string id = config_.fetch_string("prng.id");
        if (! is_id_valid (id)) {
          std::ostringstream message;
          message << "mygsl::rng::_initialize_: Invalid PRND id '" 
                  << id << "' from property '" << "prng.id" << "' !";
          throw std::logic_error (message.str ());              
        }
        set_id(id);
      }
    _initialize_();
    return;
  }
   
  void rng::_init_defaults_()
  {
    _id_   = DEFAULT_RNG_ID;
    _seed_ = random_utils::SEED_INVALID;
    return;
  }

  void rng::_initialize_()
  {
    if (_r_ != 0) 
      {
       _reset_ ();
      }
    if (rng_initializer::instance().dict.find (_id_) == rng_initializer::instance().dict.end ()) 
      {
        std::ostringstream message;
        message << "mygsl::rng::_initialize_: Cannot find the '" 
                << _id_ << "' generator from the GSL library !";
        throw std::logic_error (message.str ());
      }
    _r_ = gsl_rng_alloc (rng_initializer::instance().dict[_id_]);
    if (_r_ == 0) 
      {
        std::ostringstream message;
        message << "mygsl::rng::initialize: GSL cannot allocate the '" 
                << _id_ << "' generator!";
        throw std::logic_error (message.str ());
      }
    if (_seed_ < 0) 
      {
        std::ostringstream message;
        message << "mygsl::rng::_initialize_: Invalid seed '" 
                << _seed_ << "' !";
        throw std::logic_error (message.str ());
      }
    if (is_seed_time()) {
      _seed_ = (int32_t) (time(0) & 0x8FFFFFFF);
      std::clog << datatools::io::warning
                << "mygsl::rng::_initialize_: "
                << "Seed initialized with current time (seed=" << _seed_ << ") ! "
                << "This may be a problem if multiple PRNGs are initialized within the same second."
                << std::endl;
    }
    unsigned long int gsl_seed = (unsigned long int) _seed_;
    gsl_rng_set (_r_, gsl_seed);
    return;
  }


  void rng::_reset_ ()
  {
    if (_r_ != 0) gsl_rng_free (_r_);
    _r_ = 0;
    return;
  }
   

  rng::rng ()
  {
    _init_defaults_();
    _r_ = 0;
    return;
  }


  // ctor:
  rng::rng (int32_t seed_, bool init_)
  {
    _init_defaults_();
    _r_ = 0;
    set_seed(seed_);
    if (init_) _initialize_ ();
    return;
  }


  // ctor:
  rng::rng (const std::string & id_, int32_t seed_, bool init_)
  {
    _init_defaults_();
    _r_ = 0;
    set_id(id_);
    set_seed(seed_);
    if (init_) _initialize_ ();
    return;
  }

  void rng::reset ()
  {
    _reset_ ();
    return;
  }
  
  void rng::clear ()
  {
    _reset_();
    _init_defaults_();
    return;
  }
    
  size_t rng::get_internal_state_size () const
  {
    if (! is_initialized())
      {
        std::ostringstream message;
        message << "mygsl::rng::get_internal_state_size: Generator is not initialized !";
        throw std::logic_error (message.str ());  
      }
    size_t sz = gsl_rng_size (_r_); 
    return sz;
  }

  // dtor:
  rng::~rng ()
  {
    if (is_initialized()) reset ();
    return;
  }

  unsigned long int rng::get ()
  {
    return (int32_t) gsl_rng_get (_r_);
  }

  double rng::uniform ()
  {
    return gsl_rng_uniform (_r_);
  }

  double rng::uniform_pos ()
  {
    return gsl_rng_uniform_pos (_r_);
  }

  unsigned long int rng::uniform_int (unsigned long int n_)
  {
    return gsl_rng_uniform_int (_r_, n_);
  }

  std::string rng::name () const
  {
    if (! is_initialized ())
      {
        std::ostringstream message;
        message << "mygsl::rng::name: Generator is not initialized !";
        throw std::logic_error (message.str ());  
      }
    return (std::string (gsl_rng_name (_r_)));
  }

  unsigned long int rng::min () const
  {
    return (gsl_rng_min (_r_));
  }

  unsigned long int rng::max () const
  {
    return (gsl_rng_max (_r_));
  }

  void rng::store (const std::string & filename_) const
  {
    if (! is_initialized ())
      {
        std::ostringstream message;
        message << "mygsl::rng::store: Generator is not initialized !";
        throw std::logic_error (message.str ());  
      }
    FILE * stream = fopen (filename_.c_str (), "w");
    if (stream == NULL) 
      {
        int errid=errno;
        std::ostringstream message;
        message << "mygsl::rng::store: Cannot open file '"
                << filename_ << "'!";
        throw std::logic_error (message.str ());
      }
    int ret = gsl_rng_fwrite (stream, _r_);
    if (ret == GSL_EFAILED) 
      {
        std::ostringstream message;
        message << "mygsl::rng::store: Cannot store state in file '"
                << filename_ << "'!";
        throw std::logic_error (message.str ());
      }
    fclose (stream);
    return;
  }

  void rng::load (const std::string & filename_)
  {
    if (! is_initialized ())
      {
        std::ostringstream message;
        message << "mygsl::rng::load: Generator is not initialized !";
        throw std::logic_error (message.str ());  
      }
    FILE * stream = fopen (filename_.c_str (), "r");
    if (stream == NULL) 
      {
        int errid=errno;
        std::ostringstream message;
        message << "mygsl::rng::load: Cannot open file '"
                << filename_ << "'!";
        throw std::logic_error (message.str ());
      }
    int ret = gsl_rng_fread (stream, _r_);
    if (ret == GSL_EFAILED)  
      {
        std::ostringstream message;
        message << "mygsl::rng::load: Cannot load state from file '"
                << filename_ << "'!";
        throw std::logic_error (message.str ());
      }
    ret = fclose (stream);
    if (ret != 0) 
      {
        int errid = errno;
        std::ostringstream message;
        message << "mygsl::rng::load: Cannot close file '"
                << filename_ << "'!";
        throw std::logic_error (message.str ());
      }
    return;
  }

  void rng::to_buffer (rng::state_buffer_type & buffer_) const
  {
    if (! is_initialized ())
      {
        std::ostringstream message;
        message << "mygsl::rng::to_buffer: Generator is not initialized !";
        throw std::logic_error (message.str ());
      }    
    void * state = gsl_rng_state (_r_);
    size_t n = gsl_rng_size (_r_);
    const unsigned char * b = (const unsigned char *) state;
    buffer_.reserve (n);
    buffer_.clear ();
    for (int i = 0; i < n; i++) 
      {
        buffer_.push_back (*b);
        b++;
      }
    /*
      std::clog << "DEVEL: " << "mygsl::rng::to_buffer: "
      << "Buffer : " << buffer_.size () << '/' << buffer_.capacity () << std::endl;
    */
    return;
  }

  void rng::to_stream (std::ostream & out_) const
  {
    out_ << _id_ << ' ' << _seed_ << ' ';
    size_t n = 0;       
    void * state = 0;
    if (is_initialized ()) {
      state = gsl_rng_state (_r_);
      n = gsl_rng_size (_r_);
    }
    out_ << n;
    if (state != 0) {
      const unsigned char * b = (const unsigned char *) state;
      for (int i = 0; i < n; i++) {
        unsigned int c = (unsigned int) *b;
        out_ << ' ' << std::dec << c;
        b++;
      }
    }
    return;
  }

  void rng::from_buffer (const rng::state_buffer_type & buffer_)
  {
    if (_r_ == 0) 
      {
        std::ostringstream message;
        message << "mygsl::rng::from_buffer: Generator is not initialized !";
        throw std::logic_error (message.str ());
      }    
    void * state = gsl_rng_state (_r_);
    size_t n = gsl_rng_size (_r_);
    if ((buffer_.size () != 0) && (n != buffer_.size ()))
      {
        std::ostringstream message;
        message << "mygsl::rng::from_buffer: " 
                << "Size of the state buffer does not match the size of the internal state of the '"
                << this->name () << "' generator !";
        throw std::logic_error (message.str ());
      }
    unsigned char * b = static_cast<unsigned char *> (state);
    for (int i = 0; i < buffer_.size (); i++) 
      {
        *b = buffer_[i];
        b++;
      }
    return;
  }

  void rng::from_stream (std::istream & in_) 
  {
    if (is_initialized ())
      {
        reset();
      }
    std::string id;
    int32_t seed;
    size_t n;
    in_ >> id >> seed >> n;
    if (! in_) {
      throw std::logic_error ("mygsl::rng::from_stream: Cannot read generator parameteres 'ID/seed/n' from stream!");      
    }
    set_id(id);
    set_seed(seed);
    if (n > 0) {
      _initialize_();
      void * state = gsl_rng_state (_r_);
      size_t n2 = gsl_rng_size (_r_);
      if (n != n2) {
        throw std::logic_error ("mygsl::rng::from_stream: Unmatching size of the PRNG's internal state !");      
      }
      unsigned char * ps = (unsigned char *) state;
      for (int i = 0; i < (int) n; i++) {
        unsigned int c;
        in_ >> c ;
        if (! in_) {
            throw std::logic_error ("mygsl::rng::from_stream: Cannot read state byte from stream !");      
        }
        *ps = (unsigned char) c;
        ps++;
        in_ >> std::ws;
      }
    }
    return;
  }
  
  // specific distributions:
 
  double rng::flat (double min_, double max_) const
  {
    return gsl_ran_flat (_r_, min_, max_);
  }
  
  double rng::gaussian (double sigma_) const
  {
    return gsl_ran_gaussian_ziggurat (_r_, sigma_);
  }
  
  double rng::gaussian (double mu_, double sigma_) const
  {
    return mu_ + gaussian (sigma_);
  }

  double rng::gaussian_tail (double min_, double sigma_) const
  {
    return gsl_ran_gaussian_tail (_r_, min_, sigma_);
  }

  double rng::exponential (double sigma_) const
  {
    return gsl_ran_exponential (_r_, sigma_);
  }

  double rng::chisquare (double nu_) const
  {
    return gsl_ran_chisq (_r_, nu_);
  }

  unsigned long int rng::poisson (double mu_) const
  {
    return gsl_ran_poisson (_r_, mu_);
  }

  unsigned long int rng::bernoulli (double p_) const
  {
    return gsl_ran_bernoulli (_r_, p_);
  }

  unsigned long int rng::binomial (double p_, 
                                   unsigned long int n_) const
  {
    return gsl_ran_binomial (_r_, p_, n_);
  }
 
  double rng::operator () (void)
  {
    return this->uniform ();
  }

}

// end of mygsl::rng.c
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
