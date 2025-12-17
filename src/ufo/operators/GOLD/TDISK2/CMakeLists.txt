# Crown Copyright 2021 Met Office

# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

set ( GOLD_TDISK_files
    ObsGOLD_TDISK.h
    ObsGOLD_TDISK.cc
    ObsGOLD_TDISKParameters.h

)

PREPEND( _p_GOLD_TDISK_files     "operators/GOLD_TDISK"     ${GOLD_TDISK_files} )

set ( GOLD_TDISK_src_files
      ${_p_GOLD_TDISK_files}
      PARENT_SCOPE
)
