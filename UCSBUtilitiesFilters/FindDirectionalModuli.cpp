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

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/FloatVec3FilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Math/MatrixMath.h"

#include "UCSBUtilities/UCSBUtilitiesConstants.h"
#include "UCSBUtilities/UCSBUtilitiesVersion.h"

#include <Eigen/Dense>
#include <Eigen/StdVector>

#include "EbsdLib/EbsdConstants.h"

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
  m_LoadingDirection.x = 0.0f;
  m_LoadingDirection.y = 0.0f;
  m_LoadingDirection.z = 1.0f;

  m_OrientationOps = LaueOps::getOrientationOpsVector();

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
  FilterParameterVector parameters;

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

  parameters.push_back(SIMPL_NEW_STRING_FP("DirectionalModuli", DirectionalModuliArrayName, FilterParameter::CreatedArray, FindDirectionalModuli));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setLoadingDirection( reader->readFloatVec3("LoadingDirection", getLoadingDirection() ) );
  setFeaturePhasesArrayPath(reader->readDataArrayPath("FeaturePhasesArrayPath", getFeaturePhasesArrayPath() ) );
  setAvgQuatsArrayPath(reader->readDataArrayPath("AvgQuatsArrayPath", getAvgQuatsArrayPath() ) );
  setCrystalStructuresArrayPath(reader->readDataArrayPath("CrystalStructuresArrayPath", getCrystalStructuresArrayPath() ) );
  setCrystalCompliancesArrayPath(reader->readDataArrayPath("CrystalCompliancesArrayPath", getCrystalCompliancesArrayPath() ) );
  setDirectionalModuliArrayName(reader->readString("DirectionalModuliArrayName", getDirectionalModuliArrayName() ) );
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
  setErrorCondition(0);
  setWarningCondition(0);

  //create moduli
  QVector<size_t> dims(1, 1);
  tempPath.update(getFeaturePhasesArrayPath().getDataContainerName(), getFeaturePhasesArrayPath().getAttributeMatrixName(), getDirectionalModuliArrayName() );
  m_DirectionalModuliPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>, AbstractFilter, float>(this, tempPath, 0, dims);
  if(nullptr != m_DirectionalModuliPtr.lock())
  { m_DirectionalModuli = m_DirectionalModuliPtr.lock()->getPointer(0); }

  //check feature phases
  m_FeaturePhasesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter>(this, getFeaturePhasesArrayPath(), dims);
  if(nullptr != m_FeaturePhasesPtr.lock())
  { m_FeaturePhases = m_FeaturePhasesPtr.lock()->getPointer(0); }

  //check crystal structures
  m_CrystalStructuresPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<unsigned int>, AbstractFilter>(this, getCrystalStructuresArrayPath(), dims);
  if(nullptr != m_CrystalStructuresPtr.lock())
  { m_CrystalStructures = m_CrystalStructuresPtr.lock()->getPointer(0); }
  dims[0] = 3;

  //check quats
  dims[0] = 4;
  m_AvgQuatsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>, AbstractFilter>(this, getAvgQuatsArrayPath(), dims);
  if(nullptr != m_AvgQuatsPtr.lock())
  { m_AvgQuats = m_AvgQuatsPtr.lock()->getPointer(0); }

  //check compliances
  QVector<size_t> complianceDims(2, 6);
  m_CrystalCompliancesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>, AbstractFilter>(this, getCrystalCompliancesArrayPath(), complianceDims);
  if(nullptr != m_CrystalCompliancesPtr.lock())
  { m_CrystalCompliances = m_CrystalCompliancesPtr.lock()->getPointer(0); }

  //make sure the direction isn't undefined
  if(0 == m_LoadingDirection.x && 0 == m_LoadingDirection.y && 0 == m_LoadingDirection.z)
  {
    setErrorCondition(-1);
    notifyErrorMessage(getHumanLabel(), "A non-zero direction must be choosen", getErrorCondition());
  }

  //make sure quats + phases are from same attribute matrix + data container
  if( !getFeaturePhasesArrayPath().hasSameAttributeMatrixPath(getAvgQuatsArrayPath()) )
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "Feature Phases and Average Quats must belong to the same DataContainer / AtributreMatrix", getErrorCondition());
  }

  //make sure compliances + crystal structures are from the same attribute matrix + data container
  if( !getCrystalStructuresArrayPath().hasSameAttributeMatrixPath(getCrystalCompliancesArrayPath()) )
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "Crystal Structures and Crystal Compliances must belong to the same DataContainer / AtributreMatrix", getErrorCondition());
  }

  //make sure everything is in the same data container (may not be true for synthetic volumes using a stats gen container but the user can copy the ensemble attribute matrix over)
  if( !getAvgQuatsArrayPath().hasSameDataContainer(getCrystalStructuresArrayPath()) )
  {
    setErrorCondition(-2);
    notifyErrorMessage(getHumanLabel(), "Crystal Structures and Average Quaternions must belong to the same DataContainer", getErrorCondition());
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::preflight()
{
  setInPreflight(true);
  Q_EMIT preflightAboutToExecute();
  Q_EMIT updateFilterParameters(this);
  dataCheck();
  Q_EMIT preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindDirectionalModuli::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0) { return; }

  //get number of features
  size_t totalFeatures = m_DirectionalModuliPtr.lock()->getNumberOfTuples();

  //wrap quats
  QuatF* avgQuats = reinterpret_cast<QuatF*>(m_AvgQuats);

  // //fill compliance matrix for each phase
  // typedef Eigen::Matrix<float, 6, 6> Matrix6f;
  // size_t totalPhases = m_CrystalCompliancesPtr.lock()->getNumberOfTuples();
  // std::vector<Matrix6f, Eigen::aligned_allocator<Matrix6f> > compliance(totalPhases, Matrix6f::Zero());
  // for(int i = 0; i < totalPhases; i++)
  // {
  //   size_t index = i * 36;
  //   compliance[i] << m_CrystalCompliances[index + 0],  m_CrystalCompliances[index + 1],  m_CrystalCompliances[index + 2],  m_CrystalCompliances[index + 3],  m_CrystalCompliances[index + 4],  m_CrystalCompliances[index + 5],
  //                    m_CrystalCompliances[index + 6],  m_CrystalCompliances[index + 7],  m_CrystalCompliances[index + 8],  m_CrystalCompliances[index + 9],  m_CrystalCompliances[index + 10], m_CrystalCompliances[index + 11],
  //                    m_CrystalCompliances[index + 12], m_CrystalCompliances[index + 13], m_CrystalCompliances[index + 14], m_CrystalCompliances[index + 15], m_CrystalCompliances[index + 16], m_CrystalCompliances[index + 17],
  //                    m_CrystalCompliances[index + 18], m_CrystalCompliances[index + 19], m_CrystalCompliances[index + 20], m_CrystalCompliances[index + 21], m_CrystalCompliances[index + 22], m_CrystalCompliances[index + 23],
  //                    m_CrystalCompliances[index + 24], m_CrystalCompliances[index + 25], m_CrystalCompliances[index + 26], m_CrystalCompliances[index + 27], m_CrystalCompliances[index + 28], m_CrystalCompliances[index + 29],
  //                    m_CrystalCompliances[index + 30], m_CrystalCompliances[index + 31], m_CrystalCompliances[index + 32], m_CrystalCompliances[index + 33], m_CrystalCompliances[index + 34], m_CrystalCompliances[index + 35] ;
  // }

  QuatF q1, q2, qTotal;
  float sampleLoading[3];
  //float crystalLoading[3];

  //normalize loading direction
  sampleLoading[0] = m_LoadingDirection.x;
  sampleLoading[1] = m_LoadingDirection.y;
  sampleLoading[2] = m_LoadingDirection.z;
  MatrixMath::Normalize3x1(sampleLoading);

  //determine a rotation that aligns the loading with the sample 100 direction (as quaternion)
  if(sampleLoading[0] >= 1.0f - std::numeric_limits<float>::epsilon())
  {
    //already 100 aligned
    QuaternionMathF::Identity(q2);
  }
  else if(sampleLoading[0] <= -1.0f + std::numeric_limits<float>::epsilon())
  {
    //-100 aligned (rotate 180 deg about any non 100 axis)
    q2.x = 0.0f;
    q2.y = 0.0f;
    q2.z = 1.0f;
    q2.w = 0.0f;
  }
  else
  {
    //not a special case, get appropriate quaternion manually
    /* for two unit vectors u and v the quaternion defining the rotation (u -> v)
     *  n = u x v
     *  q.x = n.x
     *  q.y = n.y
     *  q.z = n.z
     *  q.w = 1 + u.v
     *  normalize q
     */
    q2.x = 0.0f;
    q2.y = sampleLoading[2];
    q2.z = -sampleLoading[1];
    q2.w = 1.0f + sampleLoading[0];
    QuaternionMathF::UnitQuaternion(q2);
  }

  //loop over all grains
  for (size_t i = 0; i < totalFeatures; i++)
  {
    //default to 0
    m_DirectionalModuli[i] = 0;

    //get phase and crystal structure
    size_t phase = m_FeaturePhases[i];
    size_t xtal = m_CrystalStructures[phase];
    if(xtal < Ebsd::CrystalStructure::LaueGroupEnd)
    {
      //concatenate rotation with crystal orientation (determine rotation from crystal frame to sample loading direction)
      QuaternionMathF::Copy(avgQuats[i], q1);
      QuaternionMathF::Multiply(q1, q2, qTotal);

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

      //Instead I've solved the above symbolically for the quaternion derivted rotation matrix
      // float g00 = (1.0f - (2.0f * q1.y * q1.y) - (2.0f * q1.z * q1.z));
      // float g01 = ((2.0f * q1.x * q1.y) - (2.0f * q1.z * q1.w));
      // float g02 = ((2.0f * q1.x * q1.z) + (2.0f * q1.y * q1.w));

      // float g10 = ((2.0f * q1.x * q1.y) + (2.0f * q1.z * q1.w));
      // float g11 = (1.0f - (2.0f * q1.x * q1.x) - (2.0f * q1.z * q1.z));
      // float g12 = (2.0f * q1.y * q1.z) - (2.0f * q1.x * q1.w);

      // float g20 = ((2.0f * q1.x * q1.z) - (2.0f * q1.y * q1.w));
      // float g21 = (2.0f * q1.y * q1.z) + (2.0f * q1.x * q1.w);
      // float g22 = (1.0f - (2.0f * q1.x * q1.x) - (2.0f * q1.y * q1.y));

      //everything simplifies to a function of 4 factors
      // float a = g11 * g22 - g12 * g21;
      // float b = g12 * g20 - g10 * g22;
      // float c = g10 * g21 - g11 * g20;
      // float denom = powf(g02 * g11 * g20 - g01 * g12 * g20 - g02 * g10 * g21 + g00 * g12 * g21 + g01 * g10 * g22 - g00 * g11 * g22, 4);

      //these can be expressed more compactly directly from quaternions (especially if a unit quaternion is assumed)
      float a = 1.0f - 2.0f * (qTotal.y * qTotal.y + qTotal.z * qTotal.z);
      float b = 2.0f * (qTotal.x * qTotal.y - qTotal.z * qTotal.w);
      float c = 2.0f * (qTotal.x * qTotal.z + qTotal.y * qTotal.w);
      //denom = 1.0

      //squares are used extensively, compute once
      float a2 = a * a;
      float b2 = b * b;
      float c2 = c * c;

      //compute rotated compliance weightings
      float s11Coef = a2 * a2;//a^4
      float s22Coef = b2 * b2;//b^4
      float s33Coef = c2 * c2;//c^4
      float s44Coef = b2 * c2;//b^2 c^2
      float s55Coef = a2 * c2;//a^2 c^2
      float s66Coef = a2 * b2;//a^2 b^2

      float s12Coef = 2.0f * a2 * b2;//2 a^2 b^2
      float s23Coef = 2.0f * b2 * c2;//2 b^2 c^2
      float s13Coef = 2.0f * a2 * c2;//2 a^2 c^2

      float s14Coef = 2.0f * a2 * b * c;//2 a^2 b c
      float s15Coef = 2.0f * a2 * a * c;//2 a^3 c
      float s16Coef = 2.0f * a2 * a * b;//2 a^3 b

      float s24Coef = 2.0f * b2 * b * c;//2 b^3 c
      float s25Coef = 2.0f * b2 * a * c;//2 a b^2 c
      float s26Coef = 2.0f * b2 * a * b;//2 a b^3
      float s34Coef = 2.0f * c2 * b * c;//2 b c^3
      float s35Coef = 2.0f * c2 * a * c;//2 a c^3
      float s36Coef = 2.0f * c2 * a * b;//2 a b c^2

      float s45Coef = 2.0f * a * b * c2;//2 a b c^2
      float s46Coef = 2.0f * a * b2 * c;//2 a b^2 c
      float s56Coef = 2.0f * a2 * b * c;//2 a^2 b c

      //compute rotated compliance
      size_t index = 36 * phase;
      float s11prime = s11Coef * m_CrystalCompliances[index + 0] +
                       s12Coef * m_CrystalCompliances[index + 1] +
                       s13Coef * m_CrystalCompliances[index + 2] +
                       s14Coef * m_CrystalCompliances[index + 3] +
                       s15Coef * m_CrystalCompliances[index + 4] +
                       s16Coef * m_CrystalCompliances[index + 5] +

                       s22Coef * m_CrystalCompliances[index + 7] +
                       s23Coef * m_CrystalCompliances[index + 8] +
                       s24Coef * m_CrystalCompliances[index + 9] +
                       s25Coef * m_CrystalCompliances[index + 10] +
                       s26Coef * m_CrystalCompliances[index + 11] +

                       s33Coef * m_CrystalCompliances[index + 14] +
                       s34Coef * m_CrystalCompliances[index + 15] +
                       s35Coef * m_CrystalCompliances[index + 16] +
                       s36Coef * m_CrystalCompliances[index + 17] +

                       s44Coef * m_CrystalCompliances[index + 21] +
                       s45Coef * m_CrystalCompliances[index + 22] +
                       s46Coef * m_CrystalCompliances[index + 23] +

                       s55Coef * m_CrystalCompliances[index + 28] +
                       s56Coef * m_CrystalCompliances[index + 29] +

                       s66Coef * m_CrystalCompliances[index + 35];

      //compute modulus
      m_DirectionalModuli[i] = 1.0f / s11prime;
    }
  }

  notifyStatusMessage(getHumanLabel(), "Completed");
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
const QString FindDirectionalModuli::getCompiledLibraryName() const
{
  return UCSBUtilitiesConstants::UCSBUtilitiesBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindDirectionalModuli::getBrandingString() const
{
  return "UCSBUtilities";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindDirectionalModuli::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream <<  UCSBUtilities::Version::Major() << "." << UCSBUtilities::Version::Minor() << "." << UCSBUtilities::Version::Patch();
  return version;
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindDirectionalModuli::getGroupName() const
{ return SIMPL::FilterGroups::StatisticsFilters; }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid FindDirectionalModuli::getUuid()
{
  return QUuid("{ce4e8767-d74e-52a1-b34c-7fe0d1efa3b9}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindDirectionalModuli::getSubGroupName() const
{ return SIMPL::FilterSubGroups::CrystallographyFilters; }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindDirectionalModuli::getHumanLabel() const
{ return "Find Directional Moduli"; }

