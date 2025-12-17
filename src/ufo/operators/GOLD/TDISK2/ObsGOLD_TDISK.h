/*
 * (C) Crown Copyright 2024 Met Office
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef UFO_OPERATORS_GOLD_TDISK_OBSGOLD_TDISK_H_
#define UFO_OPERATORS_GOLD_TDISK_OBSGOLD_TDISK_H_

#include <ostream>
#include <string>

#include "oops/base/Variables.h"
#include "oops/util/ObjectCounter.h"
#include "ufo/ObsOperatorBase.h"
#include "ufo/ObsOperatorParametersBase.h"
#include "ufo/operators/GOLD_TDISK/ObsGOLD_TDISKParameters.h"

/// Forward declarations
namespace ioda {
  class ObsSpace;
  class ObsVector;
}

namespace ufo {
  class GeoVaLs;
  class ObsDiagnostics;

// -----------------------------------------------------------------------------

class GOLD_TDISKParameters : public ObsOperatorParametersBase {
        OOPS_CONCRETE_PARAMETERS(GOLD_TDISKParameters, ObsOperatorParametersBase)
  // NO extra parameters needed
};
/// ObsGOLD_TDISK10m observation operator class
class ObsGOLD_TDISK : public ObsOperatorBase,
               private util::ObjectCounter<ObsGOLD_TDISK> {
 public:
  /// The type of parameters accepted by the constructor of this operator.
  /// This typedef is used by the ObsOperatorFactory.
  typedef ObsGOLD_TDISKParameters Parameters_;

  static const std::string classname() {return "ufo::ObsGOLD_TDISK";}

  // -----------------------------------------------------------------------------
  /*! \brief An operator to compare to wind speeds derived from meteor radar.
  *
  * \details Returns wind speeds in the north and east directions.
  * 
  * \date Nov. 2025: Created by N. Peterson (SWORD)
  *
  */
  // -----------------------------------------------------------------------------

  ObsGOLD_TDISK(const ioda::ObsSpace &, const Parameters_ &);
  virtual ~ObsGOLD_TDISK();

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
#endif  // UFO_OPERATORS_GOLD_TDISK_OBSGOLD_TDISK_H_
