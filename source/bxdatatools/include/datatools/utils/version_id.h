// -*- mode: c++; -*-
// version_id.h

#ifndef __datatools__utils__version_id_h
#define __datatools__utils__version_id_h 1

#include <iostream>
#include <string>
#include <boost/cstdlib.hpp>

namespace datatools {
  namespace utils {
    /** A class representing a version ID :
     * Supported formats are:
     *  - "tag" a alpha numeric character string not starting with
     *   a digit or '-' and with no punctuation symbols in it, however
     *   '-' and '_' are allowed.
     *  
     *  - "major"
     *  - "major.minor"
     *  - "major.minor.revision"
     *  - "major.minor.revision-tag"
     *     where "tag" have special values:
     *     "alpha" for 'alpha' version
     *     "beta" for 'beta' version
     *     "rcN" for 'release candidate' version
     *        with N a positive integer (<10000)
     *     Examples: "alpha", "beta", "rc0", "rc1"...
     *     "tag" can also be a string reprensenting a positive 
     *     integer: "0", "1"...
     *     or any combinaison of alphanumeric character
     *     excluding punctuation symbols but '-' and '_' are stille allowed.
     *     Examples:
     *      "3.1.4-15", "3.1.4-test", "3.1.4-007"
     *
     *  Version ID objects can be relatively ordered unless :
     *   - they are composed by a single tag 
     *     Example: "Natty"
     *   - they have "major.minor.revision" and a "tag" that cannot be
     *     compared (not "alpha", "beta", "rcX" or pure number "X")
     * 
     *  Version ID objects can also be convert to and parsed from
     *  character strings.
     * 
     *  Version ID can be checked to match version requirements 
     *  (version rules) :
     *  Examples : 
     *    "> 3.12" or ">3.12"
     *    ">= 3.12" or ">=3.12"
     *    "< 3.12" or "<3.12"
     *    "<= 3.12" or "<=3.12"
     *    "! 3.1.4-159" or  "!3.1.4-159" or "!= 3.1.4-159" or "!=3.1.4-159"
     *    "= 3.1.4-159" or  "=3.1.4-159" or "== 3.1.4-159" or "==3.1.4-159"
     * 
     *  Version ID can be invalidated : format is then "?" 
     *
     */
    class version_id {
      // Typedefs and Constants
      public:
        // TODO: could probably all be private or in an PImpl...
        static const int32_t INVALID_NUMBER = -1;
        static const char INVALID_SYMBOL = '?';
        static const char NUMERIC_SEPARATOR = '.';
        static const char TAG_SEPARATOR = '-';
        static const std::string ALPHA_TAG;
        static const std::string BETA_TAG;
        static const std::string RELEASE_CANDIDATE_PREFIX_TAG;
        static const int32_t MAX_RC_NUM   = 9999;
        static const int32_t ALPHA_NUM    = -10003;
        static const int32_t BETA_NUM     = -10002;
        static const int32_t RC_BASE_NUM  = -10001;

      // Public Interface
      public:
        // Constructors/Destructors
        version_id();

        version_id(const std::string& tag);

        version_id(int major, int minor = -1, int revision = -1, 
                   const std::string& tag = "");

        // Global operations
        bool is_valid() const;

        bool has_numbers_only() const;

        void reset();

        // Major number operations
        bool has_major() const;

        bool has_major_only() const;

        int get_major() const;
        
        void set_major(int);

        // Minor number operations
        bool has_minor() const;

        int get_minor() const;

        void set_minor(int);

        // Revision number operations
        bool has_revision() const;

        int get_revision() const;

        void set_revision(int);

        // Tag number operations
        bool has_tag() const;
        
        bool has_tag_number() const;

        bool has_tag_only() const;

        bool has_numeric_tag() const;

        bool has_special_tag() const;

        const std::string& get_tag() const;
        
        int get_tag_number() const;
        
        void set_tag(const std::string&);

        // Comparisons
        bool equals(const version_id&, bool ignore_tag = false) const;

        bool operator==(const version_id&) const;

        int compare(const version_id&, bool major_only_ = false) const;

        // These could/should be free functions
        static int compare(const version_id& vid0, const version_id& vid1, bool major_only_ = false);

        static bool are_orderable(const version_id& vid0, 
                                  const version_id& vid1, 
                                  bool major_only_ = false);

        bool matches(const std::string& version_rules, bool major_only_ = false) const;

        // I/O and Conversions
        friend std::ostream& operator<<(std::ostream&, const version_id&);

        friend std::istream& operator>>(std::istream&, version_id&);

        void to_string(std::string& format) const;

        bool from_string(const std::string&);

        void dump(std::ostream& out = std::clog, 
                  const std::string& title = "", 
                  const std::string& indent = "") const;

      // Private Data
      private:
        int32_t _major_;
        int32_t _minor_;
        int32_t _revision_;
        std::string _tag_;
        int32_t _tag_number_;
    };
  } // namespace utils
} // namespace datatools

#endif // __datatools__utils__version_id_h

// end of version_id.h
