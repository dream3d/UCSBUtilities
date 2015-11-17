


#include "InputCrystalCompliances.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersWriter.h"

#include "SIMPLib/FilterParameters/DataArrayCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/FilterParameters/Symmetric6x6FilterParameter.h"

// Include the MOC generated file for this class
#include "moc_InputCrystalCompliances.cpp"



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InputCrystalCompliances::InputCrystalCompliances() :
  AbstractFilter(),
  m_CrystalCompliancesArrayPath("", "", ""),
  m_CrystalCompliances(NULL)
{
  m_Compliances.v11 = 1;
  m_Compliances.v12 = 1;
  m_Compliances.v13 = 1;
  m_Compliances.v14 = 0;
  m_Compliances.v15 = 0;
  m_Compliances.v16 = 0;

  m_Compliances.v22 = 1;
  m_Compliances.v23 = 1;
  m_Compliances.v24 = 0;
  m_Compliances.v25 = 0;
  m_Compliances.v26 = 0;

  m_Compliances.v33 = 1;
  m_Compliances.v34 = 0;
  m_Compliances.v35 = 0;
  m_Compliances.v36 = 0;

  m_Compliances.v44 = 1;
  m_Compliances.v45 = 0;
  m_Compliances.v46 = 0;

  m_Compliances.v55 = 1;
  m_Compliances.v56 = 0;

  m_Compliances.v66 = 1;

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
InputCrystalCompliances::~InputCrystalCompliances()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(Symmetric6x6FilterParameter::New("Compliance Values (10^-11 Pa^-1)", "Compliances", getCompliances(), FilterParameter::Parameter));
  parameters.push_back(SeparatorFilterParameter::New("Ensemble Data", FilterParameter::CreatedArray));
  {
    DataArrayCreationFilterParameter::RequirementType req = DataArrayCreationFilterParameter::CreateRequirement(DREAM3D::AttributeMatrixObjectType::Ensemble);
    parameters.push_back(DataArrayCreationFilterParameter::New("Crystal Compliances", "CrystalCompliancesArrayPath", getCrystalCompliancesArrayPath(), FilterParameter::CreatedArray, req));
  }
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);

  QVector<float> tmp(21);
  reader->readArray("Compliances", tmp);
  m_Compliances.v11 = tmp[0];
  m_Compliances.v12 = tmp[1];
  m_Compliances.v13 = tmp[2];
  m_Compliances.v14 = tmp[3];
  m_Compliances.v15 = tmp[4];
  m_Compliances.v16 = tmp[5];
  m_Compliances.v22 = tmp[6];
  m_Compliances.v23 = tmp[7];
  m_Compliances.v24 = tmp[8];
  m_Compliances.v25 = tmp[9];
  m_Compliances.v26 = tmp[10];
  m_Compliances.v33 = tmp[11];
  m_Compliances.v34 = tmp[12];
  m_Compliances.v35 = tmp[13];
  m_Compliances.v36 = tmp[14];
  m_Compliances.v44 = tmp[15];
  m_Compliances.v45 = tmp[16];
  m_Compliances.v46 = tmp[17];
  m_Compliances.v55 = tmp[18];
  m_Compliances.v56 = tmp[19];
  m_Compliances.v66 = tmp[20];
  setCrystalCompliancesArrayPath(reader->readDataArrayPath("CrystalCompliancesArrayPath", getCrystalCompliancesArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int InputCrystalCompliances::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
  writer->openFilterGroup(this, index);
  SIMPL_FILTER_WRITE_PARAMETER(FilterVersion)
    QVector<float> tmp(21);
  tmp[0] = m_Compliances.v11;
  tmp[1] = m_Compliances.v12;
  tmp[2] = m_Compliances.v13;
  tmp[3] = m_Compliances.v14;
  tmp[4] = m_Compliances.v15;
  tmp[5] = m_Compliances.v16;
  tmp[6] = m_Compliances.v22;
  tmp[7] = m_Compliances.v23;
  tmp[8] = m_Compliances.v24;
  tmp[9] = m_Compliances.v25;
  tmp[10] = m_Compliances.v26;
  tmp[11] = m_Compliances.v33;
  tmp[12] = m_Compliances.v34;
  tmp[13] = m_Compliances.v35;
  tmp[14] = m_Compliances.v36;
  tmp[15] = m_Compliances.v44;
  tmp[16] = m_Compliances.v45;
  tmp[17] = m_Compliances.v46;
  tmp[18] = m_Compliances.v55;
  tmp[19] = m_Compliances.v56;
  tmp[20] = m_Compliances.v66;
  writer->writeValue("Compliances", tmp);

  SIMPL_FILTER_WRITE_PARAMETER(CrystalCompliancesArrayPath)
  writer->closeFilterGroup();
  return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::dataCheck()
{
  setErrorCondition(0);

  // create compliances
  QVector<size_t> cDims(2, 6); // 6 by 6 array
  m_CrystalCompliancesPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>, AbstractFilter, float>(this, getCrystalCompliancesArrayPath(), 0, cDims);
  if( NULL != m_CrystalCompliancesPtr.lock().get() )
  { m_CrystalCompliances = m_CrystalCompliancesPtr.lock()->getPointer(0); }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void InputCrystalCompliances::preflight()
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
void InputCrystalCompliances::execute()
{
  setErrorCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  // determine number of phases
  size_t numPhases = m_CrystalCompliancesPtr.lock()->getNumberOfTuples();

  // convert from 10^-2 GPa^-1 to Pa^-1
  m_Compliances.v11 /= 100000000000;
  m_Compliances.v12 /= 100000000000;
  m_Compliances.v13 /= 100000000000;
  m_Compliances.v14 /= 100000000000;
  m_Compliances.v15 /= 100000000000;
  m_Compliances.v16 /= 100000000000;

  m_Compliances.v22 /= 100000000000;
  m_Compliances.v23 /= 100000000000;
  m_Compliances.v24 /= 100000000000;
  m_Compliances.v25 /= 100000000000;
  m_Compliances.v26 /= 100000000000;

  m_Compliances.v33 /= 100000000000;
  m_Compliances.v34 /= 100000000000;
  m_Compliances.v35 /= 100000000000;
  m_Compliances.v36 /= 100000000000;

  m_Compliances.v44 /= 100000000000;
  m_Compliances.v45 /= 100000000000;
  m_Compliances.v46 /= 100000000000;

  m_Compliances.v55 /= 100000000000;
  m_Compliances.v56 /= 100000000000;

  m_Compliances.v66 /= 100000000000;

  //loop over each phase
  for (size_t i = 0; i < numPhases; i++)
  {
    //for now just give every phase the same values
    size_t index = 36 * i;
    m_CrystalCompliances[index + 0] = m_Compliances.v11;
    m_CrystalCompliances[index + 1] = m_Compliances.v12;
    m_CrystalCompliances[index + 2] = m_Compliances.v13;
    m_CrystalCompliances[index + 3] = m_Compliances.v14;
    m_CrystalCompliances[index + 4] = m_Compliances.v15;
    m_CrystalCompliances[index + 5] = m_Compliances.v16;

    m_CrystalCompliances[index + 6] = m_Compliances.v12;
    m_CrystalCompliances[index + 7] = m_Compliances.v22;
    m_CrystalCompliances[index + 8] = m_Compliances.v23;
    m_CrystalCompliances[index + 9] = m_Compliances.v24;
    m_CrystalCompliances[index + 10] = m_Compliances.v25;
    m_CrystalCompliances[index + 11] = m_Compliances.v26;

    m_CrystalCompliances[index + 12] = m_Compliances.v13;
    m_CrystalCompliances[index + 13] = m_Compliances.v23;
    m_CrystalCompliances[index + 14] = m_Compliances.v33;
    m_CrystalCompliances[index + 15] = m_Compliances.v34;
    m_CrystalCompliances[index + 16] = m_Compliances.v35;
    m_CrystalCompliances[index + 17] = m_Compliances.v36;

    m_CrystalCompliances[index + 18] = m_Compliances.v14;
    m_CrystalCompliances[index + 19] = m_Compliances.v24;
    m_CrystalCompliances[index + 20] = m_Compliances.v34;
    m_CrystalCompliances[index + 21] = m_Compliances.v44;
    m_CrystalCompliances[index + 22] = m_Compliances.v45;
    m_CrystalCompliances[index + 23] = m_Compliances.v46;

    m_CrystalCompliances[index + 24] = m_Compliances.v15;
    m_CrystalCompliances[index + 25] = m_Compliances.v25;
    m_CrystalCompliances[index + 26] = m_Compliances.v35;
    m_CrystalCompliances[index + 27] = m_Compliances.v45;
    m_CrystalCompliances[index + 28] = m_Compliances.v55;
    m_CrystalCompliances[index + 29] = m_Compliances.v56;

    m_CrystalCompliances[index + 30] = m_Compliances.v16;
    m_CrystalCompliances[index + 31] = m_Compliances.v26;
    m_CrystalCompliances[index + 32] = m_Compliances.v36;
    m_CrystalCompliances[index + 33] = m_Compliances.v46;
    m_CrystalCompliances[index + 34] = m_Compliances.v56;
    m_CrystalCompliances[index + 35] = m_Compliances.v66;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer InputCrystalCompliances::newFilterInstance(bool copyFilterParameters)
{
  InputCrystalCompliances::Pointer filter = InputCrystalCompliances::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString InputCrystalCompliances::getCompiledLibraryName()
{ return UCSBUtilitiesConstants::UCSBUtilitiesBaseName; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString InputCrystalCompliances::getGroupName()
{ return DREAM3D::FilterGroups::GenericFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString InputCrystalCompliances::getSubGroupName()
{ return DREAM3D::FilterSubGroups::CrystallographyFilters; }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString InputCrystalCompliances::getHumanLabel()
{ return "Input Crystal Compliances"; }
