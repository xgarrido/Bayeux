#ifndef GENBB_DECAY0_GAUSS_H_
#define GENBB_DECAY0_GAUSS_H_ 1

// This project:
#include <genbb_help/decay0/common.h>

namespace genbb {

  class primary_event;

  namespace decay0 {

    /// \brief Gauss quadrature of a function f between min and max
    double decay0_gauss(func_type f, double min, double max, double epsrel,
                        void * params);


  } // end of namespace decay0
} // end of namespace genbb

#endif // GENBB_DECAY0_GAUSS_H_
//
// Local Variables: --
// mode: c++ --
// End: --
