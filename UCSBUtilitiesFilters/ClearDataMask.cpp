


#include "ClearDataMask.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"

// Include the MOC generated file for this class
#include "moc_ClearDataMask.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ClearDataMask::ClearDataMask() :
  AbstractFilter(),
  m_MaskArrayPath("", "", ""),
  m_Mask(NULL)
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ClearDataMask::~ClearDataMask()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::setupFilterParameters()
{
  FilterParameterVector parameters;
  {
    DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(DREAM3D::TypeNames::Bool, 1, DREAM3D::AttributeMatrixObjectType::Element);
    parameters.push_back(DataArraySelectionFilterParameter::New("Mask", "MaskArrayPath", getMaskArrayPath(), FilterParameter::RequiredArray, req));
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setMaskArrayPath( reader->readDataArrayPath("MaskArrayPath", getMaskArrayPath() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int ClearDataMask::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  SIMPL_FILTER_WRITE_PARAMETER(FilterVersion)
  SIMPL_FILTER_WRITE_PARAMETER(MaskArrayPath)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::dataCheck()
{
  setErrorCondition(0);

  QVector<size_t> cDims(1, 1);
  m_MaskPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<bool>, AbstractFilter>(this, getMaskArrayPath(), cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if( NULL != m_MaskPtr.lock().get() ) /* Validate the Weak Pointer wraps a non-NULL pointer to a DataArray<T> object */
  { m_Mask = m_MaskPtr.lock()->getPointer(0); } /* Now assign the raw pointer to data from the DataArray<T> object */
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ClearDataMask::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(m_MaskArrayPath.getDataContainerName());
  size_t totalPoints = m_MaskPtr.lock()->getNumberOfTuples();

  // get list of array names
  QString attrMatName = m_MaskArrayPath.getAttributeMatrixName();
  QList<QString> voxelArrayNames = m->getAttributeMatrix(attrMatName)->getAttributeArrayNames();

  // convert to list of pointers
  std::vector<IDataArray::Pointer> arrayList;
  for (QList<QString>::iterator iter = voxelArrayNames.begin(); iter != voxelArrayNames.end(); ++iter)
  {
    IDataArray::Pointer p = m->getAttributeMatrix(attrMatName)->getAttributeArray(*iter);
    arrayList.push_back(p);
  }

  int32_t numArrays = arrayList.size();

  for (size_t i = 0; i < totalPoints; i++)
  {
    if (!m_Mask[i])
    {
      for (int32_t j = 0; j < numArrays; j++)
      {
        arrayList[j]->initializeTuple(i, 0);
      }
    }
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ClearDataMask::newFilterInstance(bool copyFilterParameters)
{
  ClearDataMask::Pointer filter = ClearDataMask::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getCompiledLibraryName()
{ return UCSBUtilitiesConstants::UCSBUtilitiesBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getGroupName()
{ return DREAM3D::FilterGroups::ProcessingFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getSubGroupName()
{ return DREAM3D::FilterSubGroups::CleanupFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ClearDataMask::getHumanLabel()
{ return "Clear Data (Mask)"; }