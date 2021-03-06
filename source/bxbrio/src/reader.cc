// reader.cc

// Ourselves:
#include <brio/reader.h>

// Standard Library:
#include <cstdlib>

// Third Party:
// - Boost:
#include <boost/filesystem.hpp>
// - ROOT:
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++11-long-long"
#endif
#include <TTree.h>
#include <TFile.h>
#include <TKey.h>
#ifdef __clang__
#pragma clang diagnostic pop
#endif

// - Bayeux/datatools:
#include <datatools/utils.h>
#include <datatools/exception.h>

namespace brio {

  reader::reader()
    : detail::base_io(RW_READ)
  {
    reader::_set_default();
    return;
  }


  reader::reader(const std::string & filename_,
                 datatools::logger::priority p_)
    : detail::base_io (RW_READ, p_)
  {
    reader::_set_default();
    std::string ext = boost::filesystem::extension(filename_);
    if (ext == store_info::constants::trio_file_extension()) {
      this->set_format(detail::base_io::text_label());
    } else {
      this->set_format(detail::base_io::pba_label());
    }
    this->open(filename_);
    return;
  }


  reader::reader(const std::string & filename_,
                 const std::string & format_str_,
                 datatools::logger::priority p_)
    : detail::base_io (RW_READ, p_)
  {
    reader::_set_default();
    set_format(format_str_);
    open(filename_);
    return;
  }

  reader::~reader()
  {
    if (this->is_opened()) {
      this->close();
    }
    this->detail::base_io::_reset();
    return;
  }

  void reader::set_check_serial_tag(bool new_value_)
  {
    _check_serial_tag_ = new_value_;
    return;
  }

  bool reader::is_check_serial_tag() const
  {
    return _check_serial_tag_;
  }

  void reader::_set_default()
  {
    detail::base_io::_set_default();
    _allow_mixed_types_in_stores_ = false;
    _allow_automatic_store_ = true;
    _check_serial_tag_ = true;
    _automatic_store_ = 0;
    return;
  }

  void reader::print_info(std::ostream & out_) const
  {
    this->tree_dump(out_, "brio::reader:");
    return;
  }

  void reader::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
  {
    std::string indent;
    if (!indent_.empty()) indent = indent_;
    detail::base_io::tree_dump(out_, title_, indent_, true);

    out_ <<  indent << datatools::i_tree_dumpable::tag
         << "Allow automatic store: " << _allow_automatic_store_
         << std::endl;

    if (_automatic_store_ != 0) {
      out_ <<  indent << datatools::i_tree_dumpable::tag
           << "Automatic store: '" << _automatic_store_->label << "'"
           << std::endl;
    }

    out_ <<  indent << datatools::i_tree_dumpable::tag
         << "Allow mixed types in stores: "
         << _allow_mixed_types_in_stores_ << std::endl;

    out_ <<  indent << datatools::i_tree_dumpable::inherit_tag(inherit_)
         << "Check serial tag: " << _check_serial_tag_ << std::endl;
    return;
  }

  bool reader::has_previous(const std::string & label_) const
  {
    const store_info *ptr_si = this->get_store_or_throw(label_);
    if (ptr_si->number_of_entries == 0){
      return false;
    } else {
      if (ptr_si->current_entry > 0) {
        return true;
      }
    }
    return false;
  }

  bool reader::has_next(const std::string & label_) const
  {
    const store_info *ptr_si = this->get_store_or_throw(label_);
    if (ptr_si->number_of_entries == 0) {
      return false;
    } else {
      if (ptr_si->current_entry < (ptr_si->number_of_entries - 1)) {
        return true;
      }
    }
    return false;
  }

  void reader::rewind_store(const std::string & label_)
  {
    store_info * ptr_si = this->get_store_or_throw(label_);
    ptr_si->current_entry = -1;
    return;
  }

  void reader::unwind_store(const std::string & label_)
  {
    store_info * ptr_si = this->get_store_or_throw(label_);
    ptr_si->current_entry = ptr_si->number_of_entries;
    return;
  }

  void reader::_at_open(const std::string & filename_)
  {
    _filename = filename_;
    datatools::fetch_path_with_env(_filename);
    DT_THROW_IF(!boost::filesystem::exists(_filename),
                std::runtime_error,
                "File '" << _filename << "' does not exist !");

    std::string default_extension =
      store_info::constants::default_file_extension();
    std::string extension = boost::filesystem::extension(_filename);
    DT_LOG_DEBUG(this->get_logging_priority(),
                 "Extension is `" << extension << "' !");
    std::string expected_extension =
      store_info::constants::default_file_extension();
    if (this->is_format_text()) {
      expected_extension = store_info::constants::trio_file_extension();
    }
    if (extension != expected_extension) {
      DT_LOG_WARNING(this->get_logging_priority(),
                     "Using extension different from `"
                     << expected_extension
                     << "' is not recommended !");
    }
    std::string mode("READ");
    _file = new TFile(_filename.c_str(), mode.c_str());
    DT_THROW_IF(_file == 0,
                std::runtime_error,
                "Cannot open file '" << _filename << "' !");
    if (_file->IsOpen()) {
      DT_LOG_NOTICE(this->get_logging_priority(),
                    "File '" << _filename << "' has been successfully opened !");
    } else {
      DT_THROW_IF(true,
                  std::runtime_error,
                  "File '" << _filename << "' is not opened !");
    }
    _file->cd();

    // extract the stores' informations:
    TList *list_of_keys = _file->GetListOfKeys();
    DT_LOG_TRACE(this->get_logging_priority(),
                 "# entries: " << list_of_keys->GetEntries());
    TIter next(list_of_keys);
    while (TObject *obj_key = next()) {
      TKey *key = dynamic_cast<TKey*>(obj_key);
      std::string class_name = key->ClassName();
      std::string store_label = key->GetName();
      DT_LOG_TRACE(this->get_logging_priority(),
                   "Class_name: `" << class_name << "'");
      DT_LOG_TRACE(this->get_logging_priority(),
                   "Store label is: `" << store_label << "'");
      // This test is probably not useful:
      if (class_name != "TKey") {
        continue;
      }
      if (this->get_logging_priority() >= datatools::logger::PRIO_TRACE) {
        key->Print();
      }
      // create a new store:
      {
        store_info si;
        _store_infos[store_label] = si;
      }
      store_info& the_si = _store_infos.find(store_label)->second;
      the_si.label = store_label;
      the_si.serialization_tag = "";
      the_si.tree = 0;
      the_si.p_record = &the_si.record;

      TTree* the_tree = 0;
      _file->cd();
      _file->GetObject(store_label.c_str(), the_tree);
      DT_THROW_IF(the_tree == 0,
                  std::logic_error,
                  "Cannot retrieve store's tree labelled '"
                  << store_label.c_str ()
                  << "' from  file '" << _filename << "' !");
      the_si.tree = the_tree;
      if (this->get_logging_priority() >= datatools::logger::PRIO_TRACE) {
        the_si.tree->Print();
      }

      TObjArray *obj_array = the_si.tree->GetListOfBranches();
      DT_THROW_IF(obj_array->GetEntries() != 1,
                  std::runtime_error,
                  "The store's tree labelled '" << store_label.c_str()
                  << "' from  file '" << _filename
                  << "' has not an unique branch ! Unsupported format !");

      TObject *obj_branch = obj_array->At(0);
      TBranch *the_branch = dynamic_cast<TBranch*>(obj_branch);
      DT_THROW_IF(the_branch == 0,
                  std::runtime_error,
                  "Cannot retrieve a valid branch from the store's tree labelled '"
                  << store_label.c_str()
                  << "' from  file '" << _filename << "' !");
      std::string branch_name = the_branch->GetName();
      the_si.serialization_tag = branch_name;
      if (branch_name == store_info::constants::no_dedicated_serial_tag_label()) {
        _allow_mixed_types_in_stores_ = true;
      }
      the_si.number_of_entries = the_si.tree->GetEntries();
      the_si.current_entry = -1; // rewind entry  position
      the_si.tree->SetBranchAddress(branch_name.c_str(), &the_si.p_record);
      the_si.tree->SetBranchStatus (branch_name.c_str(), 1);
      the_branch->SetAutoDelete(kFALSE);

      // tag if the automatic store label has been found:
      if (store_label == store_info::constants::automatic_store_label()) {
        _allow_automatic_store_ = true;
        _automatic_store_ = &the_si;
      }

      // The last found store is made the current selected one by default:
      _current_store = &the_si;
    } // while
    // If the conventional automatic store is found, make it the current selected one:
    if (_store_infos.size() >= 1 && _automatic_store_ != 0) {
      _current_store = _automatic_store_;
    }
    DT_LOG_TRACE(this->get_logging_priority(),"Exiting." );
    return;
  }

  const store_info * reader::get_store_or_throw(const std::string & label_) const
  {
    DT_THROW_IF(!this->is_opened(),
                std::logic_error,
                "Operation prohibited; file is not opened !");
    const store_info *ptr_si = this->_get_store_info(label_);
    DT_THROW_IF(ptr_si == 0,
                std::logic_error,
                "Missing explicit and valid store label !");
    return ptr_si;
  }

  store_info * reader::get_store_or_throw(const std::string & label_)
  {
    DT_THROW_IF(!this->is_opened(),
                std::logic_error,
                "Operation prohibited; file is not opened !");
    store_info *ptr_si = this->_get_store_info(label_);
    DT_THROW_IF(ptr_si == 0,
                std::logic_error,
                "Missing explicit and valid store label !");
    return ptr_si;
  }

} // end of namespace brio
