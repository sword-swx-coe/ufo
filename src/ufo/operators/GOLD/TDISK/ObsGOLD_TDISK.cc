/*
 * (C) Copyright 2021- UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "tools/new_obsop/GOLD_TDISK/ObsGOLD_TDISK.h"

#include <ostream>

#include "ioda/ObsVector.h"

#include "ufo/GOLD_TDISK/ObsGOLD_TDISK.interface.h"
#include "ufo/GeoVaLs.h"
#include "ufo/ObsDiagnostics.h"

namespace ufo {

// -----------------------------------------------------------------------------
static ObsOperatorMaker<ObsGOLD_TDISK> makerGOLD_TDISK_("GOLD_TDISK");
// -----------------------------------------------------------------------------

ObsGOLD_TDISK::ObsGOLD_TDISK(const ioda::ObsSpace & odb,
                       const Parameters_ & parameters)
  : ObsOperatorBase(odb), keyOper_(0), odb_(odb), varin_()
{
  ufo_GOLD_TDISK_setup_f90(keyOper_, parameters.toConfiguration(), odb.obsvariables(), varin_);
  oops::Log::trace() << "ObsGOLD_TDISK created." << std::endl;
}

// -----------------------------------------------------------------------------

ObsGOLD_TDISK::~ObsGOLD_TDISK() {
  ufo_GOLD_TDISK_delete_f90(keyOper_);
  oops::Log::trace() << "ObsGOLD_TDISK destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISK::simulateObs(const GeoVaLs & gv, ioda::ObsVector & ovec,
                             ObsDiagnostics & d, const QCFlags_t & qc_flags) const {
  ufo_GOLD_TDISK_simobs_f90(keyOper_, gv.toFortran(), odb_, ovec.nvars(), ovec.nlocs(),
                         ovec.toFortran());
  oops::Log::trace() << "ObsGOLD_TDISK: observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISK::print(std::ostream & os) const {
  os << "ObsGOLD_TDISK::print not implemented";
}

// -----------------------------------------------------------------------------

}  // namespace ufo
