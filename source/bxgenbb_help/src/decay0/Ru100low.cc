
#include <sstream>
#include <stdexcept>

#include <gsl/gsl_math.h>

#include <mygsl/rng.h>

#include <genbb_help/decay0/Ru100low.h>
#include <genbb_help/primary_event.h>
#include <genbb_help/primary_particle.h>
#include <genbb_help/decay0/alpha.h>
#include <genbb_help/decay0/gamma.h>
#include <genbb_help/decay0/electron.h>
#include <genbb_help/decay0/positron.h>
#include <genbb_help/decay0/particle.h>
#include <genbb_help/decay0/pair.h>
#include <genbb_help/decay0/nucltransK.h>
#include <genbb_help/decay0/nucltransKL.h>
#include <genbb_help/decay0/nucltransKLM.h>
#include <genbb_help/decay0/nucltransKLM_Pb.h>
#include <genbb_help/decay0/beta.h>

namespace genbb
{
  namespace decay0
  {
    // Ru100low.f
    // This file was extracted from the 'decay0' program by V.I. Tretyak
    // Copyright 1995-2011 V.I. Tretyak
    // This program is free software
    // it under the terms of the GNU General Public License as published by
    // the Free Software Foundation
    // your option) any later version.
    //
    // This program is distributed in the hope that it will be useful, but
    // WITHOUT ANY WARRANTY
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    // General Public License for more details.
    //
    // You should have received a copy of the GNU General Public License
    // along with this program
    // Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
    //
    void Ru100low (mygsl::rng & prng_, genbb::primary_event & event_,
                   int levelkev)
    {
      //static const double pi = M_PI;
      static const double twopi = 2 * M_PI;
      //double t;
      double tdlev;
      double cg;
      double cK;
      //double ctet;
      double EbindK;
      double Egamma;
      double p;
      double tclev;
      double thlev;
      primary_particle * ipg540 = 0;
      primary_particle * ipg591 = 0;
      // Subroutine describes the deexcitation process in Ru100 nucleus
      // after 2b-decay of Mo100 to ground and excited 0+ and 2+ levels
      // of Ru100 ("Table of Isotopes", 7th ed., 1978).
      // Call : call Ru100low(levelkev)
      // Input : levelkev - energy of Ru100 level (integer in keV) occupied
      // initially
      // 0+(gs) - 0 keV,
      // 2+(1) - 540 keV,
      // 0+(1) - 1130 keV,
      // 2+(2) - 1362 keV,
      // 0+(2) - 1741 keV.
      // Output: common/genevent/tevst,npfull,npgeant(100),pmoment(3,100),ptime(100).
      // VIT, 28.06.1993, 22.10.1995.
      // VIT, 19.05.2009: angular correlation of 591 and 540 keV gamma quanta
      // if 1130 keV level is populated.
      tclev = 0.;
      if (levelkev == 1741)
        goto label_1741;
      if (levelkev == 1362)
        goto label_1362;
      if (levelkev == 1130)
        goto label_1130;
      if (levelkev == 540)
        goto label_540;
      if (levelkev == 0)
        goto label_10000;
      goto label_20000;
    label_1741:
      thlev = 0.;
      p = 100. * prng_ ();
      if (p <= 0.05)
        goto label_17411;
      if (p <= 59.00)
        goto label_17412;
      if (p <= 59.03)
        goto label_17413;
      goto label_17414;
    label_17411:
      decay0_electron (prng_, event_, 1.741 - 0.022, tclev, thlev, tdlev); // only ec
      decay0_gamma (prng_, event_, 0.022, 0., 0., tdlev);
      return;
    label_17412:
      decay0_nucltransK (prng_, event_, 1.201, 0.022, 6.2e-4,
                         0.1e-4, tclev, thlev, tdlev);
      goto label_540;
    label_17413:
      decay0_electron (prng_, event_, 0.611 - 0.022, tclev, thlev, tdlev); // only ec
      decay0_gamma (prng_, event_, 0.022, 0., 0., tdlev);
      goto label_1130;
    label_17414:
      decay0_nucltransK (prng_, event_, 0.379, 0.022, 1.3e-2,
                         0., tclev, thlev, tdlev);
      goto label_1362;
    label_1362:
      thlev = 1.2e-12;
      p = 100. * prng_ ();
      if (p <= 43.)
        goto label_13621;
      goto label_13622;
    label_13621:
      decay0_nucltransK (prng_, event_, 1.362, 0.022, 4.2e-4,
                         0.2e-4, tclev, thlev, tdlev);
      return;
    label_13622:
      decay0_nucltransK (prng_, event_, 0.822, 0.022, 1.7e-3,
                         0., tclev, thlev, tdlev);
      goto label_540;
    label_1130:
      thlev = 0.;
      p = 100. * prng_ ();
      if (p <= 0.02)
        goto label_11301;
      goto label_11302;
    label_11301:
      decay0_electron (prng_, event_, 1.130 - 0.022, tclev, thlev, tdlev); // only ec
      decay0_gamma (prng_, event_, 0.022, 0., 0., tdlev);
      return;
    label_11302:
      Egamma = 0.591;
      EbindK = 0.022;
      cg = 1.;
      cK = 3.3e-3;
      p = prng_ () * (cg + cK);
      if (p <= cg)
        {
          decay0_gamma (prng_, event_, Egamma, tclev, thlev, tdlev);
          ipg591 = &event_.grab_particles().back();
        }
      else
        {
          decay0_electron (prng_, event_, Egamma - EbindK, tclev, thlev,
                           tdlev);
          decay0_gamma (prng_, event_, EbindK, 0., 0., tdlev);
        }
      goto label_540;
    label_540:
      thlev = 11.e-12;
      // decay0_nucltransK(prng_, event_, 0.540,0.022,4.4e-3,0.,tclev,thlev,tdlev)
      Egamma = 0.540;
      EbindK = 0.022;
      cg = 1.;
      cK = 4.4e-3;
      p = prng_ () * (cg + cK);
      if (p <= cg)
        {
          decay0_gamma (prng_, event_, Egamma, tclev, thlev, tdlev);
          ipg540 = &event_.grab_particles().back();
        }
      else
        {
          decay0_electron (prng_, event_, Egamma - EbindK, tclev, thlev,
                           tdlev);
          decay0_gamma (prng_, event_, EbindK, 0., 0., tdlev);
        }
      // Angular correlation between gammas 591 and 540 keV
      if (ipg591 != 0 && ipg540 != 0) {
        double p591 = ipg591->get_momentum().mag();
        double p540 = ipg540->get_momentum().mag();
        // std::sqrt (pmoment (1, npg591) ** 2 + pmoment (2, npg591) ** 2 +;
        //            pmoment (3, npg591) ** 2);
        // npg591 p540 =
        // std::sqrt (pmoment (1, npg540) ** 2 + pmoment (2, npg540) ** 2 +;
        //            pmoment (3, npg540) ** 2);
        // Coefficients in formula 1+a2*ctet**2+a4*ctet**4 are from:
        // R.D.Evans, "The Atomic Nucleus", Krieger Publ. Comp., 1985, p. 240,
        // 0(2)2(2)0 cascade.
        double a2 = -3.0;
        double a4 = 4.0;
        // Rejection method :
        double phi1, ctet1, stet1;
        double phi2, ctet2, stet2;
        double ctet;
      label_1:
        phi1 = twopi * prng_ ();
        ctet1 = 1. - 2. * prng_ ();
        stet1 = std::sqrt (1. - ctet1 * ctet1);
        phi2 = twopi * prng_ ();
        ctet2 = 1. - 2. * prng_ ();
        stet2 = std::sqrt (1. - ctet2 * ctet2);
        ctet = ctet1 * ctet2 + stet1 * stet2 * cos (phi1 - phi2);
        if (prng_ () * (1. + std::abs (a2) + std::abs (a4)) >
            1. + a2 * gsl_pow_2(ctet) + a4 *  gsl_pow_4(ctet)) {
          goto label_1;
        }
        ipg591->grab_momentum().set (p591 * stet1 * cos (phi1),
                                     p591 * stet1 * sin (phi1),
                                     p591 * ctet1);
        ipg540->grab_momentum().set (p540 * stet2 * cos (phi2),
                                     p540 * stet2 * sin (phi2),
                                     p540 * ctet2);
      }
      return;
    label_10000:
      return;
    label_20000:
      {
        std::ostringstream message;
        message << "genbb::decay0::Ru100low: Wrong energy level El="
                << levelkev << " [keV] !";
        throw std::logic_error (message.str());
      }
      return;
    }

  } // end of namespace decay0
} // end of namespace genbb
// end of Ru100low.cc
// Local Variables: --
// mode: c++ --
// End: --
