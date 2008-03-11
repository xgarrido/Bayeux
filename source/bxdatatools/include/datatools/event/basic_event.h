// -*- mode: c++; -*- 
/* basic_event.h
 * Author(s):     Francois Mauger <mauger@lpccaen.in2p3.fr>
 * Creation date: 2008-02-25
 * Last modified: 2008-02-25
 * 
 * License: 
 * 
 * 
 * Description: 
 * 
 * History: 
 * 
 */

#ifndef __datatools__event__basic_event_h
#define __datatools__event__basic_event_h 1

// standard includes:
#include <iostream>
#include <string>


#include <datatools/serialization/i_serializable.h>
#include <datatools/serialization/serialization.h>
#include <datatools/utils/i_tree_dump.h>
#include <datatools/utils/i_clear.h>
#include <datatools/utils/properties.h>
#include <datatools/event/event_id.h>

namespace datatools {

  namespace event {
    
    class basic_event : public datatools::utils::i_tree_dumpable,
                        public datatools::utils::i_clear,
			public datatools::serialization::i_serializable     

    {
    public:
      static const std::string SERIAL_TAG;
    private:
      event_id                     __id;
      datatools::utils::properties __properties;

    public: 

      virtual void clear();

      void id( const event_id & id_ );

      const event_id & id() const;

      event_id & id();

      const datatools::utils::properties & properties() const;

      datatools::utils::properties & properties();

    public: 

      basic_event();
      
      basic_event( const event_id & id_ );
      
      virtual ~basic_event();
      
      virtual void tree_dump( std::ostream & out_         = std::cerr , 
			      const std::string & title_  = "" ,
			      const std::string & indent_ = "",
			      bool inherit_               = false ) const;
      
      virtual const std::string & get_serial_tag();
    private:
      friend class boost::serialization::access; 
      template<class Archive>
      void serialize( Archive            & ar_ , 
		      const unsigned int   version_ )
      {
	ar_ & boost::serialization::make_nvp("id",__id);
	ar_ & boost::serialization::make_nvp("properties",__properties);
      }
      
    };

  } // end of namespace event 

} // end of namespace datatools

#endif // __datatools__event__dummy_datatools_h

// end of dummy_datatools.h
