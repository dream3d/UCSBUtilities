


#ifndef _FindBoundaryAreas_H_
#define _FindBoundaryAreas_H_

#include <QtCore/QString>

#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/DataArrays/IDataArray.h"
#include "SIMPLib/Common/AbstractFilter.h"

/**
 * @class FindBoundaryAreas FindBoundaryAreas.h /FilterCategoryFilters/FindBoundaryAreas.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class FindBoundaryAreas : public AbstractFilter
{
    Q_OBJECT /* Need this for Qt's signals and slots mechanism to work */
  public:
    SIMPL_SHARED_POINTERS(FindBoundaryAreas)
    SIMPL_STATIC_NEW_MACRO(FindBoundaryAreas)
    SIMPL_TYPE_MACRO_SUPER(FindBoundaryAreas, AbstractFilter)

    virtual ~FindBoundaryAreas();

    //required arrays
    SIMPL_FILTER_PARAMETER(DataArrayPath, SurfaceMeshTriangleAreasArrayPath)
    Q_PROPERTY(DataArrayPath SurfaceMeshTriangleAreasArrayPath READ getSurfaceMeshTriangleAreasArrayPath WRITE setSurfaceMeshTriangleAreasArrayPath)

    SIMPL_FILTER_PARAMETER(DataArrayPath, SurfaceMeshFeatureFaceIdsArrayPath)
    Q_PROPERTY(DataArrayPath SurfaceMeshFeatureFaceIdsArrayPath READ getSurfaceMeshFeatureFaceIdsArrayPath WRITE setSurfaceMeshFeatureFaceIdsArrayPath)

    //created array path
    SIMPL_FILTER_PARAMETER(DataArrayPath, SurfaceMeshBoundaryAreasArrayPath)
    Q_PROPERTY(DataArrayPath SurfaceMeshBoundaryAreasArrayPath READ getSurfaceMeshBoundaryAreasArrayPath WRITE setSurfaceMeshBoundaryAreasArrayPath)

    /**
     * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
     */
    virtual const QString getCompiledLibraryName();

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
    FindBoundaryAreas();

    /**
     * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
     */
    void dataCheck();

  private:
    DEFINE_DATAARRAY_VARIABLE(double, SurfaceMeshTriangleAreas)
    DEFINE_DATAARRAY_VARIABLE(int32_t, SurfaceMeshFeatureFaceIds)
    DEFINE_DATAARRAY_VARIABLE(double, SurfaceMeshBoundaryAreas)

    FindBoundaryAreas(const FindBoundaryAreas&); // Copy Constructor Not Implemented
    void operator=(const FindBoundaryAreas&); // Operator '=' Not Implemented
};

#endif /* FindBoundaryAreas_H_ */

