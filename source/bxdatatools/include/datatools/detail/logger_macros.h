//! \file  datatools/detail/logger_macros.h
//! \brief Macros associated to logging utilities
//
// Copyright (c) 2013-2016 by Ben Morgan <bmorgan.warwick@gmail.com>
// Copyright (c) 2013-2016 by The University of Warwick
//
// This file is part of datatools.
//
// datatools is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// datatools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with datatools.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DATATOOLS_DETAIL_LOGGER_MACROS_H
#define DATATOOLS_DETAIL_LOGGER_MACROS_H

// Standard Library:
#include <iostream>

// Third Party:
// - Boost:
#include <boost/current_function.hpp>

// This project:
#include <datatools/logger.h>

//! Log Message if Priority is greater or equal to PRIO_FATAL
//!
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_FATAL;
//! ...
//! DT_LOG_FATAL(loglevel, "Invalid indentifier '" << bad_id << "'!");
//! \endcode
#define DT_LOG_FATAL(Priority, Message)                                 \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_FATAL) { \
      std::cerr << "[fatal:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_CRITICAL
//!
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_CRITICAL;
//! ...
//! DT_LOG_CRITICAL(loglevel, "Possible mismatch with address '" << strange_address << "'!");
//! \endcode
#define DT_LOG_CRITICAL(Priority, Message)                              \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_CRITICAL) { \
      std::cerr << "[critical:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_ERROR
//!
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_ERROR;
//! ...
//! DT_LOG_ERROR(loglevel, "Parsing failed at line #" << line_no << "!");
//! \endcode
#define DT_LOG_ERROR(Priority, Message)                                 \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p  >= ::datatools::logger::PRIO_ERROR) { \
      std::cerr << "[error:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_WARNING
//!
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_WARNING;
//! ...
//! DT_LOG_WARNING(loglevel, "GUI is not available in this context! Falling back to command line interface!");
//! \endcode
#define DT_LOG_WARNING(Priority, Message)                               \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_WARNING) { \
      std::cerr << "[warning:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_NOTICE
//!
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_NOTICE;
//! ...
//! DT_LOG_NOTICE(loglevel, "Starting server...");
//! \endcode
#define DT_LOG_NOTICE(Priority, Message)                                \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_NOTICE) { \
      std::clog << "[notice:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_INFORMATION
//!
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_INFORMATION;
//! ...
//! DT_LOG_INFORMATION(loglevel, "Processing event #" << event_id << "...");
//! \endcode
#define DT_LOG_INFORMATION(Priority, Message)                           \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_INFORMATION) { \
      std::clog << "[information:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }
//! Log Message if Priority is greater or equal to PRIO_DEBUG
#define DT_LOG_DEBUG(Priority, Message)                                 \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_DEBUG) { \
      std::clog << "[debug:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_DEBUG (short format)
//!
//! Log message short format is expected to be used after a call to
//! the *normal* DT_LOG_DEBUG macro.
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_DEBUG;
//! ...
//! DT_LOG_DEBUG(loglevel, "List of items:");
//! for (const auto & item : items) {
//!   DT_LOG_DEBUG_SHORT(loglevel, "Item = " << item);
//! }
//! \endcode
#define DT_LOG_DEBUG_SHORT(Priority, Message)                           \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_DEBUG) { \
      std::clog << "[debug] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_TRACE
#define DT_LOG_TRACE(Priority, Message)                                 \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_TRACE) { \
      std::cerr << "[trace:" << BOOST_CURRENT_FUNCTION << ":" << __LINE__ << "] " << Message << std::endl; \
    }                                                                   \
  }

//! Log Message if Priority is greater or equal to PRIO_TRACE (short format)
//!
//! Log message short format is expected to be used after a call to
//! the *normal* DT_LOG_TRACE macro.
//! Example:
//! \code
//! datatools::logger::priority loglevel = datatools::logger::PRIO_TRACE;
//! ...
//! DT_LOG_TRACE(loglevel, "Devel details:");
//! for (const auto & detail : details) {
//!   DT_LOG_TRACE_SHORT(loglevel, "Devel detail: " << detail);
//! }
//! \endcode
#define DT_LOG_TRACE_SHORT(Priority, Message)                           \
  {                                                                     \
    ::datatools::logger::priority _dt_xxx_p = Priority;                 \
    if (_dt_xxx_p == ::datatools::logger::PRIO_ALWAYS || _dt_xxx_p >= ::datatools::logger::PRIO_TRACE) { \
      std::clog << "[trace] " << Message << std::endl; \
    }                                                                   \
  }

//! Log a fonction entering message if Priority is greater or equal to PRIO_TRACE
#define DT_LOG_TRACE_ENTERING(Priority)         \
  {                                             \
    DT_LOG_TRACE(Priority, "Entering...");      \
  }

//! Log a fonction exiting message if Priority is greater or equal to PRIO_TRACE
#define DT_LOG_TRACE_EXITING(Priority)          \
  {                                             \
    DT_LOG_TRACE(Priority, "Exiting.");         \
  }

#endif // DATATOOLS_DETAIL_LOGGER_MACROS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
