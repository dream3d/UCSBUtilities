#--////////////////////////////////////////////////////////////////////////////
#-- Your License or copyright can go here
#--////////////////////////////////////////////////////////////////////////////

set(_filterGroupName UCSBUtilitiesFilters)
set(${_filterGroupName}_FILTERS_HDRS "")

#--------
# This macro must come first before we start adding any filters
START_FILTER_GROUP(${UCSBUtilities_BINARY_DIR} "${_filterGroupName}" "UCSBUtilities")


#---------
# List your public filters here
set(_PublicFilters
  ClearDataMask
  CopyAttributeArray
  CopyAttributeMatrix
  CopyDataContainer
  FindBoundaryAreas
  FindDirectionalModuli
  FindModulusMismatch
  GenerateMisorientationColors
  InputCrystalCompliances
)


#--------------
# Loop on all the filters adding each one. In this loop we default to making each filter exposed in the user
# interface in DREAM3D. If you want to have the filter compiled but NOT exposed to the user then use the next loop
foreach(f ${_PublicFilters} )
  ADD_DREAM3D_FILTER(  "UCSBUtilities" "UCSBUtilities"
                        ${_filterGroupName} ${f}
                        ${UCSBUtilities_SOURCE_DIR}/Documentation/${_filterGroupName}/${f}.md TRUE)
endforeach()


#---------------
# This is the list of Private Filters. These filters are available from other filters but the user will not
# be able to use them from the DREAM3D user interface.
set(_PrivateFilters

)

#-----------------
# Loop on the Private Filters adding each one to the DREAM3DLib project so that it gets compiled.
foreach(f ${_PrivateFilters} )
  ADD_DREAM3D_FILTER(  "UCSBUtilities" "UCSBUtilities"
                        ${_filterGroupName} ${f}
                        ${SIMPLib_FILTER_DOC_DIR}/${_filterGroupName}/${f}.md FALSE)
endforeach()

ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} util/UCSBHelpers)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/CubicLowOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/CubicOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/HexagonalLowOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/HexagonalOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/TriclinicOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/TetragonalLowOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/TrigonalLowOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/TrigonalOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/TetragonalOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/OrthoRhombicOpsMisoColor)
ADD_DREAM3D_SUPPORT_CLASS(${UCSBUtilities_SOURCE_DIR} ${_filterGroupName} SpaceGroupOps/MonoclinicOpsMisoColor)


#---------------------
# This macro must come last after we are done adding all the filters and support files.
END_FILTER_GROUP(${UCSBUtilities_BINARY_DIR} "${_filterGroupName}" "UCSBUtilities")

