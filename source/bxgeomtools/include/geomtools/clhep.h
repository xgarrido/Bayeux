// -*- mode: c++; -*-
/* clhep.h
 *
 */

#ifndef GEOMTOOLS_CLHEP_H_
#define GEOMTOOLS_CLHEP_H_ 1

#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

#include <CLHEP/Vector/TwoVector.h>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Vector/Rotation.h>

#include <CLHEP/Random/RandFlat.h>

#include <geomtools/geomtools_config.h>

namespace geomtools {

  typedef CLHEP::Hep2Vector  vector_2d;
  typedef CLHEP::Hep3Vector  vector_3d;
  typedef CLHEP::HepRotation rotation_3d;
  typedef rotation_3d        rotation;

}

// Activate reflection layer for some classes :
#include <datatools/reflection_macros.h>
DR_CLASS_INIT(::geomtools::vector_2d);
DR_CLASS_INIT(::geomtools::vector_3d);
DR_CLASS_INIT(::geomtools::rotation_3d);

namespace geomtools {

  /* basic uniform deviates generator from CLHEP
   * used for special geometry algorithms
   */
  class random_tools
  {
  public:

    static double random_flat ()
    {
      return CLHEP::RandFlat::shoot (0.0, 1.0);
    }

  };

} // end of namespace geomtools

#endif // GEOMTOOLS_CLHEP_H_

// end of clhep.h
