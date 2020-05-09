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

#include "FindDirectionalModuli.h"

#include <QtCore/QTextStream>

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Math/MatrixMath.h"
#include "SIMPLib/DataContainers/DataContainerArray.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

#include <Eigen/Dense>
#include <Eigen/StdVector>

#include "EbsdLib/Core/EbsdLibConstants.h"
#include "EbsdLib/LaueOps/CubicOps.h"
#include "EbsdLib/LaueOps/HexagonalOps.h"
#include "EbsdLib/LaueOps/LaueOps.h"
#include "EbsdLib/LaueOps/OrthoRhombicOps.h"

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  DataArrayID30 = 30,
  DataArrayID31 = 31,
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindDirectionalModuli::FindDirectionalModuli()
: m_FeaturePhasesArrayPath("", "", "")
, m_CrystalStructuresArrayPath("", "", "")
, m_CrystalCompliancesArrayPath("", "", "")
, m_AvgQuatsArrayPath("", "", "")
, m_DirectionalModuliArrayName("DirectionalModuli")
{
  m_LoadingDirection[0] = 0.0f;
  m_LoadingDirection[1] = 0.0f;
  m_LoadingDirection[2] = 1.0f;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindDirectionalModuli::~FindDirectionalModuli() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SIMPL_NEW_FLOAT_VEC3_FP("Loading Direction", LoadingDirection, FilterParameter::Parameter, FindDirectionalModuli));

  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("FeaturePhases", FeaturePhasesArrayPath, FilterParameter::RequiredArray, FindDirectionalModuli, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("AvgQuats", AvgQuatsArrayPath, FilterParameter::RequiredArray, FindDirectionalModuli, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Crystal Structures", CrystalStructuresArrayPath, FilterParameter::RequiredArray, FindDirectionalModuli, req));
  }
  {
    DataArraySelectionFilterParameter::RequirementType req;
    parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Single Crystal Compliances", CrystalCompliancesArrayPath, FilterParameter::RequiredArray, FindDirectionalModuli, req));
  }

  parameters.push_back(
      SIMPL_NEW_DA_WITH_LINKED_AM_FP("DirectionalModuli", DirectionalModuliArrayName, FeaturePhasesArrayPath, FeaturePhasesArrayPath, FilterParameter::CreatedArray, FindDirectionalModuli));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setLoadingDirection(reader->readFloatVec3("LoadingDirection", getLoadingDirection()));
  setFeaturePhasesArrayPath(reader->readDataArrayPath("FeaturePhasesArrayPath", getFeaturePhasesArrayPath()));
  setAvgQuatsArrayPath(reader->readDataArrayPath("AvgQuatsArrayPath", getAvgQuatsArrayPath()));
  setCrystalStructuresArrayPath(reader->readDataArrayPath("CrystalStructuresArrayPath", getCrystalStructuresArrayPath()));
  setCrystalCompliancesArrayPath(reader->readDataArrayPath("CrystalCompliancesArrayPath", getCrystalCompliancesArrayPath()));
  setDirectionalModuliArrayName(reader->readString("DirectionalModuliArrayName", getDirectionalModuliArrayName()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::dataCheck()
{
  DataArrayPath tempPath;
  clearErrorCode();
  clearWarningCode();

  // create moduli
  std::vector<size_t> dims(1, 1);
  tempPath.update(getFeaturePhasesArrayPath().getDataContainerName(), getFeaturePhasesArrayPath().getAttributeMatrixName(), getDirectionalModuliArrayName());
  m_DirectionalModuliPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>>(this, tempPath, 0, dims, "", DataArrayID31);
  if(nullptr != m_DirectionalModuliPtr.lock())
  {
    m_DirectionalModuli = m_DirectionalModuliPtr.lock()->getPointer(0);
  }

  // check feature phases
  m_FeaturePhasesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>>(this, getFeaturePhasesArrayPath(), dims);
  if(nullptr != m_FeaturePhasesPtr.lock())
  {
    m_FeaturePhases = m_FeaturePhasesPtr.lock()->getPointer(0);
  }

  // check crystal structures
  m_CrystalStructuresPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<unsigned int>>(this, getCrystalStructuresArrayPath(), dims);
  if(nullptr != m_CrystalStructuresPtr.lock())
  {
    m_CrystalStructures = m_CrystalStructuresPtr.lock()->getPointer(0);
  }
  dims[0] = 3;

  // check quats
  dims[0] = 4;
  m_AvgQuatsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>>(this, getAvgQuatsArrayPath(), dims);
  if(nullptr != m_AvgQuatsPtr.lock())
  {
    m_AvgQuats = m_AvgQuatsPtr.lock()->getPointer(0);
  }

  // check compliances
  std::vector<size_t> complianceDims(2, 6);
  m_CrystalCompliancesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>>(this, getCrystalCompliancesArrayPath(), complianceDims);
  if(nullptr != m_CrystalCompliancesPtr.lock())
  {
    m_CrystalCompliances = m_CrystalCompliancesPtr.lock()->getPointer(0);
  }

  // make sure the direction isn't undefined
  if(0 == m_LoadingDirection[0] && 0 == m_LoadingDirection[1] && 0 == m_LoadingDirection[2])
  {
    setErrorCondition(-1, "A non-zero direction must be choosen");
  }

  // make sure quats + phases are from same attribute matrix + data container
  if(!getFeaturePhasesArrayPath().hasSameAttributeMatrixPath(getAvgQuatsArrayPath()))
  {
    setErrorCondition(-2, "Feature Phases and Average Quats must belong to the same DataContainer / AtributreMatrix");
  }

  // make sure compliances + crystal structures are from the same attribute matrix + data container
  if(!getCrystalStructuresArrayPath().hasSameAttributeMatrixPath(getCrystalCompliancesArrayPath()))
  {
    setErrorCondition(-2, "Crystal Structures and Crystal Compliances must belong to the same DataContainer / AtributreMatrix");
  }

  // make sure everything is in the same data container (may not be true for synthetic volumes using a stats gen container but the user can copy the ensemble attribute matrix over)
  if(!getAvgQuatsArrayPath().hasSameDataContainer(getCrystalStructuresArrayPath()))
  {
    setErrorCondition(-2, "Crystal Structures and Average Quaternions must belong to the same DataContainer");
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::execute()
{
  clearErrorCode();
  clearWarningCode();
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }

  // get number of features
  size_t totalFeatures = m_DirectionalModuliPtr.lock()->getNumberOfTuples();

  QuatF q1, q2, qTotal;
  float sampleLoading[3];
  // float crystalLoading[3];

  // normalize loading direction
  sampleLoading[0] = m_LoadingDirection[0];
  sampleLoading[1] = m_LoadingDirection[1];
  sampleLoading[2] = m_LoadingDirection[2];
  MatrixMath::Normalize3x1(sampleLoading);

  // determine a rotation that aligns the loading with the sample 100 direction (as quaternion)
  if(sampleLoading[0] >= 1.0f - std::numeric_limits<float>::epsilon())
  {
    // already 100 aligned
    q2.identity();
  }
  else if(sampleLoading[0] <= -1.0f + std::numeric_limits<float>::epsilon())
  {
    //-100 aligned (rotate 180 deg about any non 100 axis)
    q2[0] = 0.0f;
    q2[1] = 0.0f;
    q2[2] = 1.0f;
    q2[3] = 0.0f;
  }
  else
  {
    // not a special case, get appropriate quaternion manually
    /* for two unit vectors u and v the quaternion defining the rotation (u -> v)
     *  n = u x v
     *  q[0] = n[0]
     *  q[1] = n[1]
     *  q[2] = n[2]
     *  q.w = 1 + u.v
     *  normalize q
     */
    q2[0] = 0.0f;
    q2[1] = sampleLoading[2];
    q2[2] = -sampleLoading[1];
    q2[3] = 1.0f + sampleLoading[0];
    q2 = q2.unitQuaternion();
  }

  // loop over all grains
  for(size_t i = 0; i < totalFeatures; i++)
  {
    // default to 0
    m_DirectionalModuli[i] = 0;

    // get phase and crystal structure
    size_t phase = m_FeaturePhases[i];
    size_t xtal = m_CrystalStructures[phase];
    if(xtal < EbsdLib::CrystalStructure::LaueGroupEnd)
    {
      // concatenate rotation with crystal orientation (determine rotation from crystal frame to sample loading direction)
      q1 = QuatF(m_AvgQuats + i * 4);
      QuatF qTotal = q1 * q2;

      /*
      This method is straightforward but computationally very expensive/wasteful (since it computes the full rotated compliance matrix and we only need s'11)

      //convert orientation to rotation matrix and invert (need rotation from crystal -> sample frame)
      QuaternionMathF::Conjugate(qTotal);
      OrientationMath::QuattoMat(q1, g);

      //construct 6x6 compliance rotation matrix (after Applied Mechanics of Solids, A.F. Bower: pg. 80)
        //compute quadrants of stiffness rotation matrix
        float k1[3][3];
        float k2[3][3];
        float k3[3][3];
        float k4[3][3];
        for(int j = 0; j < 3; j++)
        {
          int j1mod3 = (j + 1) % 3;
          int j2mod3 = (j + 2) % 3;
          for(int k = 0; k < 3; k++)
          {
            int k1mod3 = (k + 1) % 3;
            int k2mod3 = (k + 2) % 3;
            k1[j][k] = g[j][k] * g[j][k];
            k2[j][k] = 2.0f * g[j][k1mod3] * g[j][k2mod3];
            k3[j][k] = g[j1mod3][k] * g[j2mod3][k];
            k4[j][k] = g[j1mod3][k1mod3] * g[j2mod3][k2mod3] + g[j1mod3][k2mod3] * g[j2mod3][k1mod3];
          }
        }

        //assemble stiffness rotation matrix
        Matrix6f k;
        k << k1[0][0], k1[0][1], k1[0][2], k2[0][0], k2[0][1], k2[0][2],
             k1[1][0], k1[1][1], k1[1][2], k2[1][0], k2[1][1], k2[1][2],
             k1[2][0], k1[2][1], k1[2][2], k2[2][0], k2[2][1], k2[2][2],
             k3[0][0], k3[0][1], k3[0][2], k4[0][0], k4[0][1], k4[0][2],
             k3[1][0], k3[1][1], k3[1][2], k4[1][0], k4[1][1], k4[1][2],
             k3[2][0], k3[2][1], k3[2][2], k4[2][0], k4[2][1], k4[2][2];

        //compute compliance rotation matrix
        k = k.inverse();

      //rotate compliance to sample frame
      Matrix6f sampleCompliance = (k.transpose() * compliance[phase]) * k;

      //compute modulus
      m_DirectionalModuli[i] = 1.0f / sampleCompliance(0, 0);

      */

      // Instead I've solved the above symbolically for the quaternion derivted rotation matrix
      // float g00 = (1.0f - (2.0f * q1[1] * q1[1]) - (2.0f * q1[2] * q1[2]));
      // float g01 = ((2.0f * q1[0] * q1[1]) - (2.0f * q1[2] * q1.w));
      // float g02 = ((2.0f * q1[0] * q1[2]) + (2.0f * q1[1] * q1.w));

      // float g10 = ((2.0f * q1[0] * q1[1]) + (2.0f * q1[2] * q1.w));
      // float g11 = (1.0f - (2.0f * q1[0] * q1[0]) - (2.0f * q1[2] * q1[2]));
      // float g12 = (2.0f * q1[1] * q1[2]) - (2.0f * q1[0] * q1.w);

      // float g20 = ((2.0f * q1[0] * q1[2]) - (2.0f * q1[1] * q1.w));
      // float g21 = (2.0f * q1[1] * q1[2]) + (2.0f * q1[0] * q1.w);
      // float g22 = (1.0f - (2.0f * q1[0] * q1[0]) - (2.0f * q1[1] * q1[1]));

      // everything simplifies to a function of 4 factors
      // float a = g11 * g22 - g12 * g21;
      // float b = g12 * g20 - g10 * g22;
      // float c = g10 * g21 - g11 * g20;
      // float denom = powf(g02 * g11 * g20 - g01 * g12 * g20 - g02 * g10 * g21 + g00 * g12 * g21 + g01 * g10 * g22 - g00 * g11 * g22, 4);

      // these can be expressed more compactly directly from quaternions (especially if a unit quaternion is assumed)
      float a = 1.0f - 2.0f * (qTotal[1] * qTotal[1] + qTotal[2] * qTotal[2]);
      float b = 2.0f * (qTotal[0] * qTotal[1] - qTotal[2] * qTotal[3]);
      float c = 2.0f * (qTotal[0] * qTotal[2] + qTotal[1] * qTotal[3]);
      // denom = 1.0

      // squares are used extensively, compute once
      float a2 = a * a;
      float b2 = b * b;
      float c2 = c * c;

      // compute rotated compliance weightings
      float s11Coef = a2 * a2; // a^4
      float s22Coef = b2 * b2; // b^4
      float s33Coef = c2 * c2; // c^4
      float s44Coef = b2 * c2; // b^2 c^2
      float s55Coef = a2 * c2; // a^2 c^2
      float s66Coef = a2 * b2; // a^2 b^2

      float s12Coef = 2.0f * a2 * b2; // 2 a^2 b^2
      float s23Coef = 2.0f * b2 * c2; // 2 b^2 c^2
      float s13Coef = 2.0f * a2 * c2; // 2 a^2 c^2

      float s14Coef = 2.0f * a2 * b * c; // 2 a^2 b c
      float s15Coef = 2.0f * a2 * a * c; // 2 a^3 c
      float s16Coef = 2.0f * a2 * a * b; // 2 a^3 b

      float s24Coef = 2.0f * b2 * b * c; // 2 b^3 c
      float s25Coef = 2.0f * b2 * a * c; // 2 a b^2 c
      float s26Coef = 2.0f * b2 * a * b; // 2 a b^3
      float s34Coef = 2.0f * c2 * b * c; // 2 b c^3
      float s35Coef = 2.0f * c2 * a * c; // 2 a c^3
      float s36Coef = 2.0f * c2 * a * b; // 2 a b c^2

      float s45Coef = 2.0f * a * b * c2; // 2 a b c^2
      float s46Coef = 2.0f * a * b2 * c; // 2 a b^2 c
      float s56Coef = 2.0f * a2 * b * c; // 2 a^2 b c

      // compute rotated compliance
      size_t index = 36 * phase;
      float s11prime = s11Coef * m_CrystalCompliances[index + 0] + s12Coef * m_CrystalCompliances[index + 1] + s13Coef * m_CrystalCompliances[index + 2] + s14Coef * m_CrystalCompliances[index + 3] +
                       s15Coef * m_CrystalCompliances[index + 4] + s16Coef * m_CrystalCompliances[index + 5] +

                       s22Coef * m_CrystalCompliances[index + 7] + s23Coef * m_CrystalCompliances[index + 8] + s24Coef * m_CrystalCompliances[index + 9] + s25Coef * m_CrystalCompliances[index + 10] +
                       s26Coef * m_CrystalCompliances[index + 11] +

                       s33Coef * m_CrystalCompliances[index + 14] + s34Coef * m_CrystalCompliances[index + 15] + s35Coef * m_CrystalCompliances[index + 16] +
                       s36Coef * m_CrystalCompliances[index + 17] +

                       s44Coef * m_CrystalCompliances[index + 21] + s45Coef * m_CrystalCompliances[index + 22] + s46Coef * m_CrystalCompliances[index + 23] +

                       s55Coef * m_CrystalCompliances[index + 28] + s56Coef * m_CrystalCompliances[index + 29] +

                       s66Coef * m_CrystalCompliances[index + 35];

      // compute modulus
      m_DirectionalModuli[i] = 1.0f / s11prime;
    }
  }

  notifyStatusMessage("Completed");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FindDirectionalModuli::newFilterInstance(bool copyFilterParameters) const
{
  FindDirectionalModuli::Pointer filter = FindDirectionalModuli::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getGroupName() const
{
  return SIMPL::FilterGroups::StatisticsFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QUuid FindDirectionalModuli::getUuid() const
{
  return QUuid("{ce4e8767-d74e-52a1-b34c-7fe0d1efa3b9}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getSubGroupName() const
{
  return SIMPL::FilterSubGroups::CrystallographyFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getHumanLabel() const
{
  return "Find Directional Moduli";
}

// -----------------------------------------------------------------------------
FindDirectionalModuli::Pointer FindDirectionalModuli::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
std::shared_ptr<FindDirectionalModuli> FindDirectionalModuli::New()
{
  struct make_shared_enabler : public FindDirectionalModuli
  {
  };
  std::shared_ptr<make_shared_enabler> val = std::make_shared<make_shared_enabler>();
  val->setupFilterParameters();
  return val;
}

// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getNameOfClass() const
{
  return QString("FindDirectionalModuli");
}

// -----------------------------------------------------------------------------
QString FindDirectionalModuli::ClassName()
{
  return QString("FindDirectionalModuli");
}

// -----------------------------------------------------------------------------
void FindDirectionalModuli::setFeaturePhasesArrayPath(const DataArrayPath& value)
{
  m_FeaturePhasesArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindDirectionalModuli::getFeaturePhasesArrayPath() const
{
  return m_FeaturePhasesArrayPath;
}

// -----------------------------------------------------------------------------
void FindDirectionalModuli::setCrystalStructuresArrayPath(const DataArrayPath& value)
{
  m_CrystalStructuresArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindDirectionalModuli::getCrystalStructuresArrayPath() const
{
  return m_CrystalStructuresArrayPath;
}

// -----------------------------------------------------------------------------
void FindDirectionalModuli::setCrystalCompliancesArrayPath(const DataArrayPath& value)
{
  m_CrystalCompliancesArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindDirectionalModuli::getCrystalCompliancesArrayPath() const
{
  return m_CrystalCompliancesArrayPath;
}

// -----------------------------------------------------------------------------
void FindDirectionalModuli::setAvgQuatsArrayPath(const DataArrayPath& value)
{
  m_AvgQuatsArrayPath = value;
}

// -----------------------------------------------------------------------------
DataArrayPath FindDirectionalModuli::getAvgQuatsArrayPath() const
{
  return m_AvgQuatsArrayPath;
}

// -----------------------------------------------------------------------------
void FindDirectionalModuli::setDirectionalModuliArrayName(const QString& value)
{
  m_DirectionalModuliArrayName = value;
}

// -----------------------------------------------------------------------------
QString FindDirectionalModuli::getDirectionalModuliArrayName() const
{
  return m_DirectionalModuliArrayName;
}

// -----------------------------------------------------------------------------
void FindDirectionalModuli::setLoadingDirection(const FloatVec3Type& value)
{
  m_LoadingDirection = value;
}

// -----------------------------------------------------------------------------
FloatVec3Type FindDirectionalModuli::getLoadingDirection() const
{
  return m_LoadingDirection;
}
