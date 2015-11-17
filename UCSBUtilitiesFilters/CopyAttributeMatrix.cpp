


#include "CopyAttributeMatrix.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"

// Include the MOC generated file for this class
#include "moc_CopyAttributeMatrix.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeMatrix::CopyAttributeMatrix() :
  AbstractFilter(),
  m_SelectedAttributeMatrixPath("", "", ""),
  m_NewAttributeMatrix("")
{
  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CopyAttributeMatrix::~CopyAttributeMatrix()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::setupFilterParameters()
{
  FilterParameterVector parameters;

  {
    AttributeMatrixSelectionFilterParameter::RequirementType req;
    parameters.push_back(AttributeMatrixSelectionFilterParameter::New("Attribute Matrix to Copy", "SelectedAttributeMatrixPath", getSelectedAttributeMatrixPath(), FilterParameter::RequiredArray, req));
  }

  parameters.push_back(StringFilterParameter::New("Copied Attribute Matrix", "NewAttributeMatrix", getNewAttributeMatrix(), FilterParameter::CreatedArray));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setSelectedAttributeMatrixPath( reader->readDataArrayPath("SelectedAttributeMatrixPath", getSelectedAttributeMatrixPath()) );
  setNewAttributeMatrix( reader->readString( "NewAttributeMatrix", getNewAttributeMatrix() ) );
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int CopyAttributeMatrix::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  SIMPL_FILTER_WRITE_PARAMETER(FilterVersion)
  SIMPL_FILTER_WRITE_PARAMETER(SelectedAttributeMatrixPath)
  SIMPL_FILTER_WRITE_PARAMETER(NewAttributeMatrix)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::dataCheck()
{
  setErrorCondition(0);

  if (m_NewAttributeMatrix.isEmpty() == true)
  {
    setErrorCondition(-11004);
    QString ss = QObject::tr("The new Attribute Matrix name must be set");
    notifyErrorMessage(getHumanLabel(), ss, getErrorCondition());
  }

  AttributeMatrix::Pointer attrMat = getDataContainerArray()->getPrereqAttributeMatrixFromPath<AbstractFilter>(this, getSelectedAttributeMatrixPath(), -301);
  if(getErrorCondition() < 0 ) { return; }

  DataContainer::Pointer dc = getDataContainerArray()->getDataContainer(getSelectedAttributeMatrixPath().getDataContainerName());

  AttributeMatrix::Pointer p = attrMat->deepCopy();
  p->setName(getNewAttributeMatrix());
  dc->addAttributeMatrix(getNewAttributeMatrix(), p );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CopyAttributeMatrix::preflight()
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
void CopyAttributeMatrix::execute()
{
  setErrorCondition(0);
  dataCheck(); // calling the dataCheck will rename the array, so nothing is required here
  if(getErrorCondition() < 0) { return; }

  notifyStatusMessage(getHumanLabel(), "Complete");
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer CopyAttributeMatrix::newFilterInstance(bool copyFilterParameters)
{
  CopyAttributeMatrix::Pointer filter = CopyAttributeMatrix::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getCompiledLibraryName()
{ return Core::CoreBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getGroupName()
{ return DREAM3D::FilterGroups::CoreFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getSubGroupName()
{ return DREAM3D::FilterSubGroups::MemoryManagementFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString CopyAttributeMatrix::getHumanLabel()
{ return "Copy Attribute Matrix"; }
