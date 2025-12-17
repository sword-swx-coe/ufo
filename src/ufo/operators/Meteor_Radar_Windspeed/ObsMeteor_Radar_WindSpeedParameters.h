
#ifndef UFO_OPERATORS_Meteor_Radar_Windspeed_OBSMeteor_Radar_WindspeedPARAMETERS_H_
#define UFO_OPERATORS_Meteor_Radar_Windspeed_OBSMeteor_Radar_WindspeedPARAMETERS_H_

#include <string>
#include <vector>

#include "oops/util/parameters/OptionalParameter.h"
#include "oops/util/parameters/RequiredParameter.h"
#include "ufo/ObsOperatorParametersBase.h"

namespace ufo {

/// Configuration options recognized by the Wind Speed operator.
class ObsMeteor_Radar_WindspeedParameters : public ObsOperatorParametersBase {
  OOPS_CONCRETE_PARAMETERS(ObsMeteor_Radar_WindspeedParameters, ObsOperatorParametersBase)

 public:
  oops::RequiredParameter<std::string> model_eastward_wind{
    "model_eastward_wind", "u", this};
  oops::RequiredParameter<std::string> model_northward_wind{
    "model_northward_wind", "v", this};
};

}  // namespace ufo
#endif  // UFO_OPERATORS_Meteor_Radar_Windspeed_OBSMeteor_Radar_WindspeedPARAMETERS_H_
