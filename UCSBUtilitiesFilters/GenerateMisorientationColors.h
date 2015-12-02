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

#ifndef _GenerateMisorientationColors_H_
#define _GenerateMisorientationColors_H_

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/AbstractFilter.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"

/**
 * @brief The GenerateMisorientationColors class. See [Filter documentation](@ref generateipfcolors) for details.
 */
class GenerateMisorientationColors : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    SIMPL_SHARED_POINTERS(GenerateMisorientationColors)
    SIMPL_STATIC_NEW_MACRO(GenerateMisorientationColors)
    SIMPL_TYPE_MACRO_SUPER(GenerateMisorientationColors, AbstractFilter)

    virtual ~GenerateMisorientationColors();

    SIMPL_FILTER_PARAMETER(FloatVec3_t, ReferenceAxis)
    Q_PROPERTY(FloatVec3_t ReferenceAxis READ getReferenceAxis WRITE setReferenceAxis)

    SIMPL_FILTER_PARAMETER(float, ReferenceAngle)
    Q_PROPERTY(float ReferenceAngle READ getReferenceAngle WRITE setReferenceAngle)

    SIMPL_FILTER_PARAMETER(DataArrayPath, CellPhasesArrayPath)
    Q_PROPERTY(DataArrayPath CellPhasesArrayPath READ getCellPhasesArrayPath WRITE setCellPhasesArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, QuatsArrayPath)
    Q_PROPERTY(DataArrayPath QuatsArrayPath READ getQuatsArrayPath WRITE setQuatsArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, CrystalStructuresArrayPath)
    Q_PROPERTY(DataArrayPath CrystalStructuresArrayPath READ getCrystalStructuresArrayPath WRITE setCrystalStructuresArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, GoodVoxelsArrayPath)
    Q_PROPERTY(DataArrayPath GoodVoxelsArrayPath READ getGoodVoxelsArrayPath WRITE setGoodVoxelsArrayPath)

    SIMPL_FILTER_PARAMETER(QString, MisorientationColorArrayName)
    Q_PROPERTY(QString MisorientationColorArrayName READ getMisorientationColorArrayName WRITE setMisorientationColorArrayName)

    SIMPL_FILTER_PARAMETER(bool, UseGoodVoxels)
    Q_PROPERTY(bool UseGoodVoxels READ getUseGoodVoxels WRITE setUseGoodVoxels)

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
    GenerateMisorientationColors();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();

  private:
    DEFINE_DATAARRAY_VARIABLE(int32_t, CellPhases)
    DEFINE_DATAARRAY_VARIABLE(float, Quats)
    DEFINE_DATAARRAY_VARIABLE(uint32_t, CrystalStructures)
    DEFINE_DATAARRAY_VARIABLE(bool, GoodVoxels)

    DEFINE_DATAARRAY_VARIABLE(uint8_t, MisorientationColor)

    GenerateMisorientationColors(const GenerateMisorientationColors&); // Copy Constructor Not Implemented
    void operator=(const GenerateMisorientationColors&); // Operator '=' Not Implemented
};

#endif /* _GenerateMisorientationColors_H_ */
