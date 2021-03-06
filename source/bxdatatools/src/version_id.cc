// version_id.cc

// Ourselves:
#include <datatools/version_id.h>

// Standard Library:
#include <cctype>
#include <sstream>
#include <stdexcept>

// This Project:
#include <datatools/exception.h>

namespace datatools {

  DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(version_id,"datatools::version_id")

  const int32_t version_id::INVALID_NUMBER    = -1;
  const char    version_id::INVALID_SYMBOL    = '?';
  const char    version_id::NUMERIC_SEPARATOR = '.';
  const char    version_id::TAG_SEPARATOR     = '-';
  const int32_t version_id::MAX_RC_NUM        =   9999;
  const int32_t version_id::ALPHA_NUM         = -10003;
  const int32_t version_id::BETA_NUM          = -10002;
  const int32_t version_id::RC_BASE_NUM       = -10001;

  const std::string & version_id::tags::alpha()
  {
    static std::string tag;
    if (tag.empty()) tag = "alpha";
    return tag;
  }

  const std::string & version_id::tags::beta()
  {
    static std::string tag;
    if (tag.empty()) tag = "beta";
    return tag;
  }

  const std::string & version_id::tags::release_candidate_prefix()
  {
    static std::string tag;
    if (tag.empty()) tag = "rc";
    return tag;
  }

  version_id::version_id()
  {
    _major_ = INVALID_NUMBER;
    _minor_ = INVALID_NUMBER;
    _revision_ = INVALID_NUMBER;
    _tag_number_ = INVALID_NUMBER;
    return;
  }


  version_id::version_id(const std::string& tag)
  {
    _major_ = INVALID_NUMBER;
    _minor_ = INVALID_NUMBER;
    _revision_ = INVALID_NUMBER;
    _tag_number_ = INVALID_NUMBER;
    this->set_tag(tag);
    return;
  }


  version_id::version_id(int major,
                         int minor,
                         int revision,
                         const std::string& tag)
  {
    _major_ = INVALID_NUMBER;
    _minor_ = INVALID_NUMBER;
    _revision_ = INVALID_NUMBER;
    _tag_number_ = INVALID_NUMBER;
    this->set_major(major);
    this->set_minor(minor);
    this->set_revision(revision);
    this->set_tag(tag);
    return;
  }


  bool version_id::is_valid() const
  {
    if (!this->has_major()) {
      // no major implies a single valid tag : format = "tag"
      return this->has_tag();
    } else {
      if (this->has_revision() && !this->has_minor()) {
        return false;
      }
      // format = "major.minor.revision-tag"
      if (this->has_tag()) {
        return this->has_revision();
      }
    }
    return true;
  }


  bool version_id::has_numbers_only() const
  {
    return this->has_major() && !this->has_tag();
  }


  bool version_id::has_major() const
  {
    return _major_ != INVALID_NUMBER;
  }


  bool version_id::has_major_only() const
  {
    return this->has_major()
      && !this->has_minor()
      && !this->has_revision()
      && !this->has_tag();
  }


  int version_id::get_major() const
  {
    return _major_;
  }


  void version_id::set_major(int major)
  {
    _major_ = major < 0 ? INVALID_NUMBER : major;
    return;
  }


  bool version_id::has_minor() const
  {
    return _minor_ != INVALID_NUMBER;
  }


  int version_id::get_minor() const
  {
    return _minor_;
  }


  void version_id::set_minor(int minor)
  {
    _minor_ = minor < 0 ? INVALID_NUMBER : minor;
    return;
  }


  bool version_id::has_revision() const
  {
    return _revision_ != INVALID_NUMBER;
  }


  int version_id::get_revision() const
  {
    return _revision_;
  }


  void version_id::set_revision(int revision)
  {
    _revision_ = revision < 0 ? INVALID_NUMBER : revision;
    return;
  }


  bool version_id::has_tag_only() const
  {
    return this->has_tag() && !this->has_major();
  }


  bool version_id::has_tag() const
  {
    return ! _tag_.empty();
  }


  bool version_id::has_special_tag() const
  {
    if (_tag_.empty()) return false;
    return _tag_number_ < INVALID_NUMBER;
  }


  bool version_id::has_numeric_tag() const
  {
    if (_tag_.empty()) return false;

    // Specific test :
    {
      std::istringstream dummy(_tag_);
      int val;
      dummy >> val;
      if (!dummy) return false;

      std::string word;
      dummy >> word;
      if (!word.empty()) return false;
    }
    return true;
  }


  const std::string & version_id::get_tag() const
  {
    return _tag_;
  }


  bool version_id::has_tag_number() const
  {
    return _tag_number_ != INVALID_NUMBER;
  }


  int version_id::get_tag_number() const
  {
    return _tag_number_;
  }


  void version_id::set_tag(const std::string& tag)
  {
    std::istringstream tag_iss(tag);
    std::string trimed_tag;
    tag_iss >> std::ws >> trimed_tag >> std::ws;
    if (trimed_tag.empty()) {
      _tag_.clear();
      return;
    }
    char first_char = trimed_tag[0];
    bool validate = false;
    if (isalnum(first_char)) validate = true;
    DT_THROW_IF (!validate,
                 std::logic_error,
                 "Version ID's tag '"
                 << trimed_tag << "' is not allowed !");
    validate = true;
    for (size_t i = 0; i < trimed_tag.length(); i++) {
      char c = trimed_tag[i];
      if (ispunct(c) && c != '-' && c !=  '_') {
        validate = false;
      }
    }
    DT_THROW_IF (!validate,
                 std::logic_error,
                 "Version ID's tag '"
                 << trimed_tag << "' is not allowed !");
    _tag_ = trimed_tag;
    if (isdigit(_tag_[0])) {
      // check numerical tags :
      std::istringstream num_tag_iss(_tag_);
      int num_tag;
      num_tag_iss >> num_tag;
      if (num_tag_iss && num_tag > 0) {
        _tag_number_ = num_tag;
      }
    } else {
      // check special orderable tags :
      if (_tag_ == tags::alpha()) {
        _tag_number_ = ALPHA_NUM;
      } else if (_tag_ == tags::beta()) {
        _tag_number_ = BETA_NUM;
      } else if (_tag_.length() > 2
                 && _tag_.substr(0,2) == tags::release_candidate_prefix()) {
        std::string rc_tag_tail = _tag_.substr(2);
        if (!rc_tag_tail.empty()) {
          std::istringstream rc_tag_iss(rc_tag_tail);
          int rc_num;
          rc_tag_iss >> rc_num;
          if (rc_tag_iss) {
            if (rc_num >= 0) {
              DT_THROW_IF (rc_num > MAX_RC_NUM,
                           std::logic_error,
                           "Version ID's special RC tag '"
                           << trimed_tag
                           << "' is not allowed (RC number is too large) !");
              _tag_number_ = RC_BASE_NUM + rc_num;
            }
          }
        }
      } // RC
    }
    return;
  }


  void version_id::dump(std::ostream& out,
                        const std::string& title,
                        const std::string& indent) const
  {
    const std::string tag = "|-- ";
    const std::string last_tag = "`-- ";
    if (!title.empty()) out << indent << title << std::endl;

    out << indent << tag << "Has major    : " << this->has_major();

    if (this->has_major()) {
      out << " => Major = " << _major_;
    }

    out << std::endl;
    out << indent << tag << "Has minor    : " << this->has_minor();

    if (this->has_minor()) out << " => Minor = " << _minor_;

    out << std::endl;
    out << indent << tag << "Has revision : " << this->has_revision();

    if (this->has_revision()) out << " => Revision = " << _revision_;

    out << std::endl;
    out << indent << tag << "Has tag      : " << this->has_tag();

    if (this->has_tag()) out << " => Tag = '" << _tag_ << "'";

    if (this->has_tag_number()) {
      out << " with number='" << _tag_number_ << "'";
      if (this->has_special_tag()) {
        out << " (special)";
      }
    }

    out << std::endl;
    out << indent << tag      << "Valid        : " << this->is_valid() << std::endl;
    out << indent << last_tag << "Format       : '" << *this << "'" << std::endl;
    return;
  }


  void version_id::reset()
  {
    _major_ = INVALID_NUMBER;
    _minor_ = INVALID_NUMBER;
    _revision_ = INVALID_NUMBER;
    _tag_number_ = INVALID_NUMBER;
    _tag_.clear();
    return;
  }


  bool version_id::equals(const version_id& vid, bool ignore_tag) const
  {
    bool res = false;
    if (vid._major_ == _major_
        && vid._minor_ == _minor_
        && vid._revision_ == _revision_) {
      res = true;
    }
    if (res && ! ignore_tag) res = (vid._tag_ == _tag_);
    return res;
  }


  bool version_id::operator==(const version_id& vid) const
  {
    return this->equals(vid, false);
  }


  int version_id::compare(const version_id& vid, bool major_only) const
  {
    return compare(*this, vid, major_only);
  }

  void version_id::to_string(std::string& format) const
  {
    std::ostringstream out;
    out << *this;
    format = out.str();
    return;
  }

  std::string version_id::to_string() const
  {
    std::string repr;
    to_string(repr);
    return repr;
  }


  std::ostream & operator<<(std::ostream& out, const version_id& vid)
  {
    if (!vid.is_valid()) {
      out << vid.INVALID_SYMBOL;
    } else {
      if (vid.has_major()) {
        out << vid.get_major();
        if (vid.has_minor()) {
          out << version_id::NUMERIC_SEPARATOR << vid.get_minor();
          if (vid.has_revision()) {
            out << version_id::NUMERIC_SEPARATOR << vid.get_revision();
          }
        }

        if (vid.has_tag()) {
          out << version_id::TAG_SEPARATOR;
          out << vid.get_tag();
        }
      } else {
        if (vid.has_tag()) {
          out << vid.get_tag();
        }
      }
    }
    return out;
  }


  bool version_id::from_string(const std::string& s)
  {
    bool devel = false;
    //devel = true;
    this->reset();
    std::istringstream iss(s);
    std::string token;
    iss >> std::ws >> token >> std::ws;

    if (token.empty()) {
      // empty input :
      return false;
    }

    if (token.size() == 1 && token[0] == INVALID_SYMBOL) {
      // the invalid value is allowed.
      return true;
    }

    if (devel) {
      std::cerr << std::endl
                << "DEVEL: version_id::from_string: string = '"
                << s << "'" << std::endl;
    }
    // Check there is only a tag without leading "major[.minor[.revision[-tag]]]" :
    {
      std::istringstream local_iss(token);
      int val = INVALID_NUMBER;
      local_iss >> val;

      if (!local_iss) {
        // cannot parse an integer :
        try {
          // pure "tag" format :
          this->set_tag(token);
        }
        catch (std::exception& x) {
          this->reset();
          return false;
        }

        if (!this->is_valid()) {
          this->reset();
          return false;
        }
        return true;
      } else {
        if (val < 0) {
          this->reset();
          return false;
        }
      }
    }

    // Now we are in a "major[.minor[.revision[-tag]]]" format :
    {
      std::istringstream local_iss(token);
      bool search_tag = false;
      for (int i = 0; i < 3; i++) {
        int val = INVALID_NUMBER;
        local_iss >> val;

        if (!local_iss) {
          this->reset();
          return false;
        } else if (val < 0) {
          this->reset();
          return false;
        }

        try {
          if (i == 0) this->set_major(val);
          if (i == 1) this->set_minor(val);
          if (i == 2) this->set_revision(val);
        }
        catch (std::exception& x) {
          this->reset();
          return false;
        }
        // get the separator :
        char sep = 0;
        local_iss.get(sep);
        if (devel) {
          std::cerr << "DEVEL: version_id::from_string:   separator = '"
                    << sep << "'" << std::endl;
        }

        if (!local_iss) return true;

        if (sep == TAG_SEPARATOR) {
          if (i < 2) {
            this->reset();
            return false;
          }
          search_tag = true;
          break;
        }

        if ((i < 2) && (sep == NUMERIC_SEPARATOR)) continue;

        this->reset();
        return false;
      }

      if (search_tag) {
        // extract a tag :
        std::string tag;
        local_iss >> tag;
        if (devel) {
          std::cerr << "DEVEL: version_id::from_string:   tag = '"
                    << tag << "'" << std::endl;
        }

        if (tag.empty()) {
          this->reset();
          return false;
        }

        try {
          this->set_tag(tag);
        }
        catch (std::exception& x) {
          if (devel) {
            std::cerr << "DEVEL: version_id::from_string:   set_tag error = '"
                      << x.what() << "'" << std::endl;
          }
          this->reset();
          return false;
        }
        if (!this->is_valid()) {
          this->reset();
          if (devel) {
            std::cerr << "DEVEL: version_id::from_string:   not valid!"
                      << std::endl;
          }
          return false;
        }
      }
    }

    return true;
  }


  std::istream& operator>>(std::istream& in, version_id& vid)
  {
    std::string word;
    in >> word;
    if (!vid.from_string(word)) {
      vid.reset();
      in.setstate(std::ios::failbit);
    }
    return in;
  }


  bool version_id::matches(const std::string& version_rule,
                           bool major_only) const
  {
    std::istringstream vr_iss(version_rule);
    std::string word;
    vr_iss >> word;
    DT_THROW_IF (word.empty(),
                 std::logic_error,
                 "Missing valid version rule !");
    const int INVALID          = -1000;
    const int NOT_EQUAL        = -3;
    const int LESS_OR_EQUAL    = -2;
    const int LESS             = -1;
    const int EQUAL            =  0;
    const int GREATER          =  1;
    const int GREATER_OR_EQUAL =  2;
    int match_mode = INVALID;
    std::string vid_word;
    if (match_mode == INVALID && word.length() >= 2) {
      std::string op = word.substr(0,2);
      if (op == "==") {
        match_mode = EQUAL;
      } else if (op == "!=") {
        match_mode = NOT_EQUAL;
      } else if (op == "<=") {
        match_mode = LESS_OR_EQUAL;
      } else if (op == ">=") {
        match_mode = GREATER_OR_EQUAL;
      }
      if (match_mode != INVALID) vid_word = word.substr(2);
    }

    if (match_mode == INVALID && word.length() >= 1) {
      std::string op = word.substr(0,1);
      if (op == "=") {
        match_mode = EQUAL;
      } else if (op == "!") {
        match_mode = NOT_EQUAL;
      } else if (op == "<") {
        match_mode = LESS;
      } else if (op == ">") {
        match_mode = GREATER;
      }

      if (match_mode != INVALID) vid_word = word.substr(1);
    }

    if (vid_word.empty()) vr_iss >> vid_word;
    DT_THROW_IF (vid_word.empty(),
                 std::logic_error,
                 "Missing version ID !");
    version_id matcher_vid;
    std::istringstream vid_iss(vid_word);
    vid_iss >> matcher_vid;
    DT_THROW_IF (!vid_iss,
                 std::logic_error,
                 "Invalid version ID format ('" << vid_word << "') !");
    if (match_mode == EQUAL) {
      try {
        int res = compare(*this, matcher_vid, major_only);
        return (res == 0);
      }
      catch(std::exception& x) {
      }
      if (*this == matcher_vid) return true;
      else return false;
    } else if (match_mode == NOT_EQUAL) {
      try {
        int res = compare(*this, matcher_vid, major_only);
        return !(res == 0);
      }
      catch(std::exception& x) {
      }
      if (*this == matcher_vid) return false;
      else return true;
    } else {
      DT_THROW_IF (!are_orderable(*this, matcher_vid, major_only),
                   std::logic_error,
                   "Version IDs "
                   << *this << " and " << matcher_vid
                   << " are not orderable !");
      int res = compare(*this, matcher_vid, major_only);
      if (match_mode == LESS) {
        if (res == -1) return true;
        else return false;
      } else if (match_mode == GREATER) {
        if (res == +1) return true;
        else return false;
      } else if (match_mode == GREATER_OR_EQUAL) {
        if (res != -1 ) return true;
        else return false;
      } else if (match_mode == LESS_OR_EQUAL) {
        if (res != +1 ) return true;
        else return false;
      } else {
        DT_THROW_IF(true,
                    std::logic_error,
                    "Invalid rule !");
      }
    }
    return false;
  }


  // static
  int version_id::compare(const version_id& vid0,
                          const version_id& vid1,
                          bool major_only)
  {
    if (vid0 == vid1) return 0;
    const version_id& v1 = vid0;
    const version_id& v2 = vid1;
    DT_THROW_IF (!v1.is_valid(),
                 std::logic_error,
                 "Version ID '" << v1 << "' is not valid !");
    DT_THROW_IF (!v2.is_valid(),
                 std::logic_error,
                 "Version ID '" << v2 << "' is not valid !");
    int maj_num_1 = v1._major_;
    int maj_num_2 = v2._major_;
    int min_num_1 = v1._minor_;
    int min_num_2 = v2._minor_;
    int rev_num_1 = v1._revision_;
    int rev_num_2 = v2._revision_;
    int tag_num_1 = v1._tag_number_;
    int tag_num_2 = v2._tag_number_;

    DT_THROW_IF (maj_num_1 == INVALID_NUMBER,
                 std::logic_error,
                 "Version ID '" << v1 << "' is not orderable !");
    DT_THROW_IF (maj_num_2 == INVALID_NUMBER,
                 std::logic_error,
                 "Version ID '" << v2 << "' is not orderable !");
    if (maj_num_1 < maj_num_2) {
      return -1;
    } else if (maj_num_1 > maj_num_2) {
      return +1;
    } else {
      // maj_num_1 == maj_num_2
      if ((min_num_1 == INVALID_NUMBER) && (min_num_2 != INVALID_NUMBER)) {
        if (major_only) return 0;
        return -1;
      } else if ((min_num_1 != INVALID_NUMBER)
                 && (min_num_2 == INVALID_NUMBER)) {
        if (major_only) return 0;
        return +1;
      } else if ((min_num_1 != INVALID_NUMBER)
                 && (min_num_2 != INVALID_NUMBER)) {
        if (min_num_1 < min_num_2) {
          return -1;
        } else if (min_num_1 > min_num_2) {
          return +1;
        } else {
          // min_num_1 == min_num_2
          if ((rev_num_1 == INVALID_NUMBER)
              && (rev_num_2 != INVALID_NUMBER)) {
            return -1;
          } else if ((rev_num_1 != INVALID_NUMBER)
                     && (rev_num_2 == INVALID_NUMBER)) {
            return +1;
          } else if ((rev_num_1 != INVALID_NUMBER)
                     && (rev_num_2 != INVALID_NUMBER)) {
            if (rev_num_1 < rev_num_2) {
              return -1;
            } else if (rev_num_1 > rev_num_2) {
              return +1;
            } else {
              // rev_num_1 == rev_num_2
              DT_THROW_IF (v1.has_tag() && ! v1.has_tag_number(),
                           std::logic_error,
                           "Version ID '" << v1 << "' tag is not orderable !");
              DT_THROW_IF (v2.has_tag() && ! v2.has_tag_number(),
                           std::logic_error,
                           "Version ID '" << v2 << "' tag is not orderable !");
              if ((tag_num_1 == INVALID_NUMBER)
                  && (tag_num_2 != INVALID_NUMBER)) {
                // if (devel) std::cerr << "DEVEL: datatools::version_id::compare: TEST 1\n";

                if (tag_num_2 < INVALID_NUMBER) {
                  // if (devel) std::cerr << "DEVEL: datatools::version_id::compare: +1\n";
                  return +1;
                } else {
                  // if (devel) std::cerr << "DEVEL: datatools::version_id::compare: -1\n";
                  return -1;
                }
              } else if ((tag_num_1 != INVALID_NUMBER)
                         && (tag_num_2 == INVALID_NUMBER)) {
                // if (devel) std::cerr << "DEVEL: datatools::version_id::compare: TEST 2\n";
                if (tag_num_1 < INVALID_NUMBER) {
                  return -1;
                } else {
                  return +1;
                }
              } else if ((tag_num_1 != INVALID_NUMBER)
                         && (tag_num_2 != INVALID_NUMBER)) {
                // if (devel) {
                //   std::cerr << "DEVEL: datatools::version_id::compare: TEST 3\n";
                // }
                // if (devel) {
                //   std::cerr << "DEVEL: datatools::version_id::compare:   tag_num_1 = "
                //             << tag_num_1 << std::endl;
                // }
                // if (devel) {
                //   std::cerr << "DEVEL: datatools::version_id::compare:   tag_num_2 = "
                //             << tag_num_2 << std::endl;
                // }
                if (tag_num_1 < tag_num_2) {
                  return -1;
                } else if (tag_num_1 > tag_num_2) {
                  return +1;
                }
              }
            }
          }
        }
      }
    }
    return 0;
  }

  // static
  const version_id & version_id::invalid()
  {
    static const version_id _vid;
    return _vid;
  }

  // static
  bool version_id::are_orderable(const version_id& vid0,
                                 const version_id& vid1,
                                 bool major_only)
  {
    const version_id& v1 = vid0;
    const version_id& v2 = vid1;
    if (!v1.is_valid()) return false;
    if (!v2.is_valid()) return false;
    // 2012-05-29 FM: not tested yet
    if ((v1.has_major_only () && v2.has_major ())
        ||(v1.has_major () && v2.has_major_only ())) {
      // if one of both version IDs has only a major number
      if (major_only) {
        // we can always compare using the major number only :
        return true;
      }
    }
    if (v1.has_tag_only()) return false;
    if (v2.has_tag_only()) return false;
    if (v1.equals(v2, true)) {
      if (v1.has_tag() && ! v1.has_tag_number()) return false;
      if (v2.has_tag() && ! v2.has_tag_number()) return false;
    }
    return true;
  }

} // namespace datatools
