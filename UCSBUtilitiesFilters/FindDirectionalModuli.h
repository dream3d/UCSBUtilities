/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 * Copyright (c) 2015 The Regents of the University of California, Author: William Lenthe        *
 *                                                                                               *
 * This program is free software: you can redistribute it and/or modify it under the terms of    *
 * the GNU Lesser General Public License as published by the Free Software Foundation, either    *
 * version 2 of the License, or (at your option) any later version.                              *
 *                                                                                               *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 * See the GNU Lesser General Public License for more details.                                   *
 *                                                                                               *
 * You should have received a copy of the GNU Lesser General Public License                      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.                         *
 *                                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _FindDirectionalModuli_H_
#define _FindDirectionalModuli_H_

#include <vector>
#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/DataContainers/DataContainer.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"

#include "OrientationLib/SpaceGroupOps/CubicOps.h"
#include "OrientationLib/SpaceGroupOps/HexagonalOps.h"
#include "OrientationLib/SpaceGroupOps/SpaceGroupOps.h"
#include "OrientationLib/SpaceGroupOps/OrthoRhombicOps.h"

/**
 * @class FindDirectionalModuli FindDirectionalModuli.h DREAM3DLib/GenericFilters/FindDirectionalModuli.h
 * @brief
 * @author
 * @date Nov 19, 2011
 * @version 1.0
 */
class FindDirectionalModuli : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    SIMPL_SHARED_POINTERS(FindDirectionalModuli)
    SIMPL_STATIC_NEW_MACRO(FindDirectionalModuli)
    SIMPL_TYPE_MACRO_SUPER(FindDirectionalModuli, AbstractFilter)

    virtual ~FindDirectionalModuli();

    SIMPL_FILTER_PARAMETER(DataArrayPath, FeaturePhasesArrayPath)
    Q_PROPERTY(DataArrayPath FeaturePhasesArrayPath READ getFeaturePhasesArrayPath WRITE setFeaturePhasesArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, CrystalStructuresArrayPath)
    Q_PROPERTY(DataArrayPath CrystalStructuresArrayPath READ getCrystalStructuresArrayPath WRITE setCrystalStructuresArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, CrystalCompliancesArrayPath)
    Q_PROPERTY(DataArrayPath CrystalCompliancesArrayPath READ getCrystalCompliancesArrayPath WRITE setCrystalCompliancesArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, AvgQuatsArrayPath)
    Q_PROPERTY(DataArrayPath AvgQuatsArrayPath READ getAvgQuatsArrayPath WRITE setAvgQuatsArrayPath)

    SIMPL_FILTER_PARAMETER(QString, DirectionalModuliArrayName)
    Q_PROPERTY(QString DirectionalModuliArrayName READ getDirectionalModuliArrayName WRITE setDirectionalModuliArrayName)

    SIMPL_FILTER_PARAMETER(FloatVec3_t, LoadingDirection)
    Q_PROPERTY(FloatVec3_t LoadingDirection READ getLoadingDirection WRITE setLoadingDirection)

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getCompiledLibraryName();

    /**
     * @brief getBrandingString Returns the branding string for the filter, which is a tag
     * used to denote the filter's association with specific plugins
     * @return Branding string
    */
    virtual const QString getBrandingString();

    /**
     * @brief getFilterVersion Returns a version string for this filter. Default
     * value is an empty string.
     * @return
     */
    virtual const QString getFilterVersion();

    /**
     * @brief newFilterInstance Reimplemented from @see AbstractFilter class
     */
    virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters);

    /**
     * @brief getGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getGroupName();

    /**
     * @brief getSubGroupName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getSubGroupName();

    /**
     * @brief getHumanLabel Reimplemented from @see AbstractFilter class
     */
    virtual const QString getHumanLabel();

    /**
     * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void setupFilterParameters();

    /**
     * @brief writeFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual int writeFilterParameters(AbstractFilterParametersWriter* writer, int index);

    /**
     * @brief readFilterParameters Reimplemented from @see AbstractFilter class
     */
    virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

    /**
     * @brief execute Reimplemented from @see AbstractFilter class
     */
    virtual void execute();

    /**
    * @brief preflight Reimplemented from @see AbstractFilter class
    */
    virtual void preflight();

  signals:
    /**
     * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
     * be pushed from a user-facing control (such as a widget)
     * @param filter Filter instance pointer
     */
    void updateFilterParameters(AbstractFilter* filter);

    /**
     * @brief parametersChanged Emitted when any Filter parameter is changed internally
     */
    void parametersChanged();

    /**
     * @brief preflightAboutToExecute Emitted just before calling dataCheck()
     */
    void preflightAboutToExecute();

    /**
     * @brief preflightExecuted Emitted just after calling dataCheck()
     */
    void preflightExecuted();

  protected:
    FindDirectionalModuli();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();

  private:
    std::vector<SpaceGroupOps::Pointer> m_OrientationOps;
    CubicOps::Pointer m_CubicOps;
    HexagonalOps::Pointer m_HexOps;
    OrthoRhombicOps::Pointer m_OrthoOps;

    DEFINE_DATAARRAY_VARIABLE(float, DirectionalModuli)
    DEFINE_DATAARRAY_VARIABLE(int32_t, FeaturePhases)
    DEFINE_DATAARRAY_VARIABLE(float, AvgQuats)

    DEFINE_DATAARRAY_VARIABLE(unsigned int, CrystalStructures)
    DEFINE_DATAARRAY_VARIABLE(float, CrystalCompliances)

    FindDirectionalModuli(const FindDirectionalModuli&); // Copy Constructor Not Implemented
    void operator=(const FindDirectionalModuli&); // Operator '=' Not Implemented
};

#endif /* FindDirectionalModuli_H_ */
