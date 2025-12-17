/*
 * (C) Crown Copyright 2024 Met Office
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#include "ufo/operators/Meteor_Radar_Windspeed/ObsMeteor_Radar_Windspeed.h"

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
static ObsOperatorMaker<ObsMeteor_Radar_Windspeed> makerMeteor_Radar_Windspeed_("Meteor_Radar_Windspeed");
// -----------------------------------------------------------------------------

ObsMeteor_Radar_Windspeed::ObsMeteor_Radar_Windspeed(const ioda::ObsSpace & odb,
                 const Parameters_ &params)
  : ObsOperatorBase(odb), model_eastward_wind_{params.model_eastward_wind.value()},
    model_northward_wind_{params.model_northward_wind.value()},
    varin_{{model_eastward_wind_, model_northward_wind_}}
{
  oops::Log::trace() << "ObsMeteor_Radar_Windspeed created." << std::endl;
}

// -----------------------------------------------------------------------------

ObsMeteor_Radar_Windspeed::~ObsMeteor_Radar_Windspeed() {
  oops::Log::trace() << "ObsMeteor_Radar_Windspeed destructed" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsMeteor_Radar_Windspeed::simulateObs(const GeoVaLs & geovals, ioda::ObsVector & hofx,
                             ObsDiagnostics &, const QCFlags_t&) const {
// Observation operator

  // Check hofx size and initialise hofx to zero:
  ASSERT(geovals.nlocs() == hofx.nlocs());
  hofx.zero();
  // Get number of locations
  std::size_t nwinds = geovals.nlocs()/2;

  // Get u and v winds

  std::vector<double> u_wind(nwinds);  // u wind component
  geovals.get(u_wind, model_eastward_wind_);
  std::vector<double> v_wind(nwinds);  // v wind component
  geovals.get(v_wind, model_northward_wind_);

  // calculate wind speed
  for (size_t w = 0; w < nwinds; w++) {
    hofx[2*w] = u_wind[w];
    hofx[2*w+1] = v_wind[w];
  }

  oops::Log::trace() << "ObsMeteor_Radar_Windspeed: observation operator run" << std::endl;
}

// -----------------------------------------------------------------------------

void ObsMeteor_Radar_Windspeed::print(std::ostream & os) const {
  os << "ObsMeteor_Radar_Windspeed::print not implemented";
}

// -----------------------------------------------------------------------------

}  // namespace ufo
