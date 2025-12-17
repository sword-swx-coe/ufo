/*
 * (C) Crown Copyright 2024 Met Office
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "ufo/operators/GOLD_TDISK/ObsGOLD_TDISK.h"

#include <cmath>
#include <ostream>
#include <string>
#include <vector>

#include "ioda/ObsVector.h"

#include "oops/base/Variables.h"

#include "ufo/GeoVaLs.h"
#include "ufo/ObsDiagnostics.h"
#include "ufo/ObsOperatorBase.h"

namespace ufo {

// -----------------------------------------------------------------------------
static ObsOperatorMaker<ObsGOLD_TDISK> makerGOLD_TDISK_("GOLD_TDISK");
// -----------------------------------------------------------------------------

ObsGOLD_TDISK::ObsGOLD_TDISK(const ioda::ObsSpace & odb,
                 const Parameters_ &params)
  : ObsOperatorBase(odb), model_eastward_wind_{params.model_eastward_wind.value()},
    model_northward_wind_{params.model_northward_wind.value()},
    varin_{{model_eastward_wind_, model_northward_wind_}}
{
  oops::Log::trace() << "ObsGOLD_TDISK created." << std::endl;
}

// -----------------------------------------------------------------------------

ObsGOLD_TDISK::~ObsGOLD_TDISK() {
  oops::Log::trace() << "ObsGOLD_TDISK destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISK::simulateObs(const GeoVaLs & geovals, ioda::ObsVector & hofx,
                             ObsDiagnostics &, const QCFlags_t&) const {
// Observation operator

  // Check hofx size and initialise hofx to zero:
  ASSERT(geovals.nlocs() == hofx.nlocs());
  hofx.zero();
  // Get number of locations
  std::size_t nTemps = geovals.nlocs();

  // Get u and v winds

  std::vector<double> u_wind(nTemps);  // u wind component
  geovals.get(u_wind, model_eastward_wind_);
  std::vector<double> v_wind(nwinds);  // v wind component
  geovals.get(v_wind, model_northward_wind_);

  // calculate wind speed
  for (size_t w = 0; w < nwinds; w++) {
    hofx[2*w] = u_wind[w];
    hofx[2*w+1] = v_wind[w];
  }

  oops::Log::trace() << "ObsGOLD_TDISK: observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsGOLD_TDISK::print(std::ostream & os) const {
  os << "ObsGOLD_TDISK::print not implemented";
}

// -----------------------------------------------------------------------------

}  // namespace ufo
