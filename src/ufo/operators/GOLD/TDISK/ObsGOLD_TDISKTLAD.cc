/*
 * (C) Copyright 2021- UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "tools/new_obsop/GOLD_TDISK/ObsGOLD_TDISKTLAD.h"

#include "ioda/ObsSpace.h"
#include "ioda/ObsVector.h"

#include "oops/util/Logger.h"

#include "ufo/GOLD_TDISK/ObsGOLD_TDISKTLAD.interface.h"
#include "ufo/GeoVaLs.h"
#include "ufo/ObsDiagnostics.h"

namespace ufo {

// -----------------------------------------------------------------------------
static LinearObsOperatorMaker<ObsGOLD_TDISKTLAD> makerGOLD_TDISKTL_("GOLD_TDISK");
// -----------------------------------------------------------------------------

ObsGOLD_TDISKTLAD::ObsGOLD_TDISKTLAD(const ioda::ObsSpace & odb,
                               const Parameters_ & parameters)
  : LinearObsOperatorBase(odb), keyOper_(0), varin_()
{
  ufo_GOLD_TDISK_tlad_setup_f90(keyOper_, parameters.toConfiguration(), odb.obsvariables(), varin_);
  oops::Log::trace() << "ObsGOLD_TDISKTLAD created" << std::endl;
}

// -----------------------------------------------------------------------------

ObsGOLD_TDISKTLAD::~ObsGOLD_TDISKTLAD() {
  ufo_GOLD_TDISK_tlad_delete_f90(keyOper_);
  oops::Log::trace() << "ObsGOLD_TDISKTLAD destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISKTLAD::setTrajectory(const GeoVaLs & geovals, ObsDiagnostics & ydiags,
                                   const QCFlags_t & qc_flags) {
  ufo_GOLD_TDISK_tlad_settraj_f90(keyOper_, geovals.toFortran(), obsspace(), ydiags.toFortran());
  oops::Log::trace() << "ObsGOLD_TDISKTLAD: trajectory set" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISKTLAD::simulateObsTL(const GeoVaLs & geovals, ioda::ObsVector & ovec,
                                   const QCFlags_t & qc_flags) const {
  ufo_GOLD_TDISK_simobs_tl_f90(keyOper_, geovals.toFortran(), obsspace(),
                            ovec.size(), ovec.toFortran());
  oops::Log::trace() << "ObsGOLD_TDISKTLAD: TL observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISKTLAD::simulateObsAD(GeoVaLs & geovals, const ioda::ObsVector & ovec,
                                   const QCFlags_t & qc_flags) const {
  ufo_GOLD_TDISK_simobs_ad_f90(keyOper_, geovals.toFortran(), obsspace(),
                            ovec.size(), ovec.toFortran());
  oops::Log::trace() << "ObsGOLD_TDISKTLAD: adjoint observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISKTLAD::print(std::ostream & os) const {
  os << "ObsGOLD_TDISKTLAD::print not implemented" << std::endl;
}

// -----------------------------------------------------------------------------

}  // namespace ufo
