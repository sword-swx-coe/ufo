
#ifndef UFO_OPERATORS_GOLD_TDISK_OBSGOLD_TDISKPARAMETERS_H_
#define UFO_OPERATORS_GOLD_TDISK_OBSGOLD_TDISKPARAMETERS_H_

#include <string>
#include <vector>

#include "oops/util/parameters/OptionalParameter.h"
#include "oops/util/parameters/RequiredParameter.h"
#include "ufo/ObsOperatorParametersBase.h"

namespace ufo {

/// Configuration options recognized by the Wind Speed operator.
class ObsGOLD_TDISKParameters : public ObsOperatorParametersBase {
  OOPS_CONCRETE_PARAMETERS(ObsGOLD_TDISKParameters, ObsOperatorParametersBase)

 public:
  oops::RequiredParameter<std::string> model_eastward_wind{
    "model_temperature", "air_temp", this};
};

}  // namespace ufo
#endif  // UFO_OPERATORS_GOLD_TDISK_OBSGOLD_TDISKPARAMETERS_H_
