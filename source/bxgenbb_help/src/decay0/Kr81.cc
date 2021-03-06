
#include <sstream>
#include <stdexcept>
#include <cmath>

#include <mygsl/rng.h>

#include <genbb_help/decay0/Kr81.h>
#include <genbb_help/primary_event.h>
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
#include <genbb_help/decay0/beta1.h>
#include <genbb_help/decay0/beta2.h>
#include <genbb_help/decay0/beta_1fu.h>
#include <genbb_help/decay0/PbAtShell.h>

namespace genbb {
  namespace decay0 {

    // Kr81.f 
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

    void Kr81(mygsl::rng & prng_, genbb::primary_event & event_, double tcnuc, double & tdnuc)
    {
      double t;
      double tdlev;
      double pdecay;
      double pklm;
      double tclev;
      double thlev;
      double thnuc;
      // Scheme of Kr81 decay (NDS 79(1996)447 and ENSDF at NNDC site on 9.12.2007).
      // Input : tcnuc - time of creation of nucleus (sec)
      // Output: tdnuc - time of decay of nucleus (sec)
      // // common/genevent/tevst,npfull,npgeant(100),pmoment(3,100),// ptime(100).
      // VIT, 9.12.2007.
      thnuc=7.226493e12;
      tdnuc=tcnuc-thnuc/std::log(2.)*std::log(prng_());
      tclev=0.;
      pdecay=100.*prng_();
      if (pdecay <= 0.30) goto label_276;
      goto label_10000;
    label_276  :
      decay0_gamma(prng_, event_, 0.013,0.,0.,t)  ;// capture from only K shell is supposed
      thlev=9.7e-12;
      decay0_nucltransKLM(prng_, event_, 0.276,0.013,7.3e-3,0.002,7.8e-4,0.000,2.6e-4,
			  0.,tclev,thlev,tdlev);
      return;
    label_10000  :
      pklm=100.*prng_();
      if ( pklm <= 84.73) decay0_gamma(prng_, event_, 0.013,0.,0.,t);
      if (pklm > 84.73  &&  pklm <= 97.44) decay0_gamma(prng_, event_, 0.002,0.,0.,t);
      if (pklm > 97.44 ) decay0_gamma(prng_, event_, 0.000,0.,0.,t);
      return;
    }
    // end of Kr81.f




  } // end of namespace decay0 
} // end of namespace genbb 

// end of Kr81.cc
// Local Variables: --
// mode: c++ --
// End: --
