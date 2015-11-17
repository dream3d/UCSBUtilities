


#include "CopyDataContainer.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/DataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

// Include the MOC generated file for this class
#include "moc_CopyDataContainer.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyDataContainer::CopyDataContainer() :
  AbstractFilter(),
  m_SelectedDataContainerName(""),
  m_NewDataContainerName("")
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyDataContainer::~CopyDataContainer()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    DataContainerSelectionFilterParameter::RequirementType req;
    parameters.push_back(DataContainerSelectionFilterParameter::New("Data Container to Copy", "SelectedDataContainerName", getSelectedDataContainerName(), FilterParameter::RequiredArray, req));
  }

  parameters.push_back(StringFilterParameter::New("Copied Data Container", "NewDataContainerName", getNewDataContainerName(), FilterParameter::CreatedArray));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedDataContainerName( reader->readString("SelectedDataContainerName", getSelectedDataContainerName()) );
  setNewDataContainerName( reader->readString( "NewDataContainerName", getNewDataContainerName() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int CopyDataContainer::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  SIMPL_FILTER_WRITE_PARAMETER(FilterVersion)
  SIMPL_FILTER_WRITE_PARAMETER(SelectedDataContainerName)
  SIMPL_FILTER_WRITE_PARAMETER(NewDataContainerName)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::dataCheck()
{
  setErrorCondition(0);

  if (getNewDataContainerName().isEmpty() == true)
  {
    setErrorCondition(-11001);
    QString ss = QObject::tr("The new Data Container name must be set");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  DataContainer::Pointer dc = getDataContainerArray()->getPrereqDataContainer<AbstractFilter>(this, getSelectedDataContainerName());
  if(getErrorCondition() < 0) { return; }

  DataContainer::Pointer dcNew = dc->deepCopy();
  dcNew->setName(getNewDataContainerName());
  getDataContainerArray()->addDataContainer(dcNew);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyDataContainer::preflight()
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
void CopyDataContainer::execute()
{
  setErrorCondition(0);
  dataCheck(); // calling the dataCheck will copy the array, so nothing is required here
  if(getErrorCondition() < 0) { return; }

  notifyStatusMessage(getHumanLabel(), "Complete");
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CopyDataContainer::newFilterInstance(bool copyFilterParameters)
{
  CopyDataContainer::Pointer filter = CopyDataContainer::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getCompiledLibraryName()
{ return Core::CoreBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getGroupName()
{ return DREAM3D::FilterGroups::CoreFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getSubGroupName()
{ return DREAM3D::FilterSubGroups::MemoryManagementFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyDataContainer::getHumanLabel()
{ return "Copy Data Container"; }
