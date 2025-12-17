/*
 * (C) Copyright 2021- UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISK_INTERFACE_H_
#define TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISK_INTERFACE_H_

#include "ufo/Fortran.h"

namespace eckit {
  class Configuration;
}

namespace ioda {
  class ObsSpace;
}

namespace oops {
  class ObsVariables;
  class Variables;
}

namespace ufo {

/// Interface to Fortran UFO GOLD_TDISK routines

extern "C" {

// -----------------------------------------------------------------------------

  void ufo_GOLD_TDISK_setup_f90(F90hop &, const eckit::Configuration &,
                             const oops::ObsVariables &, oops::Variables &);
  void ufo_GOLD_TDISK_delete_f90(F90hop &);
  void ufo_GOLD_TDISK_simobs_f90(const F90hop &, const F90goms &, const ioda::ObsSpace &,
                               const int &, const int &, double &);

// -----------------------------------------------------------------------------

}  // extern C

}  // namespace ufo
#endif  // TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISK_INTERFACE_H_
