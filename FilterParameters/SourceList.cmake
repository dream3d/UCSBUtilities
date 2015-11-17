
set (${PLUGIN_NAME}_FilterParameters_SRCS
  ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/Symmetric6x6FilterParameter.cpp
  ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/FloatVec4FilterParameter.cpp

)

set (${PLUGIN_NAME}_FilterParameters_HDRS
    ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/Symmetric6x6FilterParameter.h
    ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/FloatVec4FilterParameter.h
)
cmp_IDE_SOURCE_PROPERTIES( "FilterParameters" "${${PLUGIN_NAME}_FilterParameters_HDRS}" "${${PLUGIN_NAME}_FilterParameters_SRCS}" "${PROJECT_INSTALL_HEADERS}")
