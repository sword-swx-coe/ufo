# (C) Copyright 2017-2018 UCAR.
#
# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

set ( GOLD_TDISK_files
    ObsGOLD_TDISK.h
    ObsGOLD_TDISK.cc
    ObsGOLD_TDISKTLAD.h
    ObsGOLD_TDISKTLAD.cc
    ObsGOLD_TDISK.interface.F90
    ObsGOLD_TDISK.interface.h
    ObsGOLD_TDISKParameters.h
    ObsGOLD_TDISKTLAD.interface.F90
    ObsGOLD_TDISKTLAD.interface.h
    ufo_GOLD_TDISK_mod.F90
    ufo_GOLD_TDISK_tlad_mod.F90
)

PREPEND( _p_GOLD_TDISK_files     "GOLD_TDISK"     ${GOLD_TDISK_files} )

set ( GOLD_TDISK_src_files
      ${_p_GOLD_TDISK_files}
      PARENT_SCOPE
)
