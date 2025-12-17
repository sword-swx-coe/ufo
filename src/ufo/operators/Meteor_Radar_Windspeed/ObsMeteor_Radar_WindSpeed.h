/*
 * (C) Crown Copyright 2024 Met Office
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef UFO_OPERATORS_Meteor_Radar_Windspeed_OBSMeteor_Radar_Windspeed_H_
#define UFO_OPERATORS_Meteor_Radar_Windspeed_OBSMeteor_Radar_Windspeed_H_

#include <ostream>
#include <string>

#include "oops/base/Variables.h"
#include "oops/util/ObjectCounter.h"
#include "ufo/ObsOperatorBase.h"
#include "ufo/ObsOperatorParametersBase.h"
#include "ufo/operators/Meteor_Radar_Windspeed/ObsMeteor_Radar_WindspeedParameters.h"

/// Forward declarations
namespace ioda {
  class ObsSpace;
  class ObsVector;
}

namespace ufo {
  class GeoVaLs;
  class ObsDiagnostics;

// -----------------------------------------------------------------------------

class Meteor_Radar_WindspeedParameters : public ObsOperatorParametersBase {
        OOPS_CONCRETE_PARAMETERS(Meteor_Radar_WindspeedParameters, ObsOperatorParametersBase)
  // NO extra parameters needed
};
/// ObsMeteor_Radar_Windspeed10m observation operator class
class ObsMeteor_Radar_Windspeed : public ObsOperatorBase,
               private util::ObjectCounter<ObsMeteor_Radar_Windspeed> {
 public:
  /// The type of parameters accepted by the constructor of this operator.
  /// This typedef is used by the ObsOperatorFactory.
  typedef ObsMeteor_Radar_WindspeedParameters Parameters_;

  static const std::string classname() {return "ufo::ObsMeteor_Radar_Windspeed";}

  // -----------------------------------------------------------------------------
  /*! \brief An operator to compare to wind speeds derived from meteor radar.
  *
  * \details Returns wind speeds in the north and east directions.
  * 
  * \date Nov. 2025: Created by N. Peterson (SWORD)
  *
  */
  // -----------------------------------------------------------------------------

  ObsMeteor_Radar_Windspeed(const ioda::ObsSpace &, const Parameters_ &);
  virtual ~ObsMeteor_Radar_Windspeed();

// Obs Operator
  void simulateObs(const GeoVaLs &, ioda::ObsVector &, ObsDiagnostics &,
                   const QCFlags_t&) const override;

// Other
  const oops::Variables & requiredVars() const override {return varin_;}

 private:
  void print(std::ostream &) const override;
  oops::Variable model_eastward_wind_;
  oops::Variable model_northward_wind_;
  oops::Variables varin_;
};

// -----------------------------------------------------------------------------

}  // namespace ufo
#endif  // UFO_OPERATORS_Meteor_Radar_Windspeed_OBSMeteor_Radar_Windspeed_H_
