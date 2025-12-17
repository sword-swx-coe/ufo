/*
 * (C) Copyright 2021- UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISK_H_
#define TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISK_H_

#include <ostream>

#include "ioda/ObsDataVector.h"

#include "oops/base/Variables.h"

#include "ufo/GOLD_TDISK/ObsGOLD_TDISKParameters.h"
#include "ufo/Fortran.h"
#include "ufo/ObsOperatorBase.h"

/// Forward declarations
namespace ioda {
  class ObsSpace;
  class ObsVector;
}

namespace ufo {
  class GeoVaLs;
  class ObsDiagnostics;

// -----------------------------------------------------------------------------
/// GOLD_TDISK observation operator class
class ObsGOLD_TDISK : public ObsOperatorBase {
 public:
  /// The type of parameters accepted by the constructor of this operator.
  /// This typedef is used by the ObsOperatorFactory.
  typedef ObsGOLD_TDISKParameters Parameters_;
  typedef ioda::ObsDataVector<int> QCFlags_t; 

  ObsGOLD_TDISK(const ioda::ObsSpace &, const Parameters_ &);
  virtual ~ObsGOLD_TDISK();

// Obs Operator
  void simulateObs(const GeoVaLs &, ioda::ObsVector &, ObsDiagnostics &,
                   const QCFlags_t &) const override;

// Other
  const oops::Variables & requiredVars() const override {return varin_;}

  int & toFortran() {return keyOper_;}
  const int & toFortran() const {return keyOper_;}

 private:
  void print(std::ostream &) const override;
  F90hop keyOper_;
  const ioda::ObsSpace& odb_;
  oops::Variables varin_;
};

// -----------------------------------------------------------------------------

}  // namespace ufo
#endif  // TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISK_H_
