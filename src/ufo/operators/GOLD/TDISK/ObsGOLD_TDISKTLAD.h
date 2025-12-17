/*
 * (C) Copyright 2021- UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISKTLAD_H_
#define TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISKTLAD_H_

#include <ostream>

#include "ioda/ObsDataVector.h"

#include "oops/base/Variables.h"

#include "ufo/GOLD_TDISK/ObsGOLD_TDISKParameters.h"
#include "ufo/Fortran.h"
#include "ufo/LinearObsOperatorBase.h"

// Forward declarations
namespace ioda {
  class ObsSpace;
  class ObsVector;
}

namespace ufo {
  class GeoVaLs;

// -----------------------------------------------------------------------------
/// GOLD_TDISK TL/AD observation operator class
class ObsGOLD_TDISKTLAD : public LinearObsOperatorBase {
 public:
  /// The type of parameters accepted by the constructor of this operator.
  /// This typedef is used by the LinearObsOperatorFactory.
  typedef ObsGOLD_TDISKParameters Parameters_;
  typedef ioda::ObsDataVector<int> QCFlags_t; 

  ObsGOLD_TDISKTLAD(const ioda::ObsSpace &, const Parameters_ &);
  virtual ~ObsGOLD_TDISKTLAD();

  // Obs Operators
  void setTrajectory(const GeoVaLs &, ObsDiagnostics &, const QCFlags_t &) override;
  void simulateObsTL(const GeoVaLs &, ioda::ObsVector &, const QCFlags_t &) const override;
  void simulateObsAD(GeoVaLs &, const ioda::ObsVector &, const QCFlags_t &) const override;

  // Other
  const oops::Variables & requiredVars() const override {return varin_;}

  int & toFortran() {return keyOper_;}
  const int & toFortran() const {return keyOper_;}

 private:
  void print(std::ostream &) const override;
  F90hop keyOper_;
  oops::Variables varin_;
};

// -----------------------------------------------------------------------------

}  // namespace ufo
#endif  // TOOLS_NEW_OBSOP_GOLD_TDISK_OBSGOLD_TDISKTLAD_H_
