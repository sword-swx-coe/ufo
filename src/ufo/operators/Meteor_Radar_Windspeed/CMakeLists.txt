# Crown Copyright 2021 Met Office

# This software is licensed under the terms of the Apache Licence Version 2.0
# which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

set ( Meteor_Radar_Windspeed_files
    ObsMeteor_Radar_Windspeed.h
    ObsMeteor_Radar_Windspeed.cc
    ObsMeteor_Radar_WindspeedParameters.h

)

PREPEND( _p_Meteor_Radar_Windspeed_files     "operators/Meteor_Radar_Windspeed"     ${Meteor_Radar_Windspeed_files} )

set ( Meteor_Radar_Windspeed_src_files
      ${_p_Meteor_Radar_Windspeed_files}
      PARENT_SCOPE
)
