/*
 * Your License should go here
 */
#ifndef _UCSBUtilitiesConstants_H_
#define _UCSBUtilitiesConstants_H_

#include <QtCore/QString>

/**
* @brief This namespace is used to define some Constants for the plugin itself.
*/
namespace UCSBUtilitiesConstants
{
  const QString UCSBUtilitiesPluginFile("UCSBUtilitiesPlugin");
  const QString UCSBUtilitiesPluginDisplayName("UCSBUtilitiesPlugin");
  const QString UCSBUtilitiesBaseName("UCSBUtilitiesPlugin");

  namespace FilterGroups
  {
    const QString UCSBUtilitiesFilters("UCSBUtilities");
  }
}

/**
* @brief Use this namespace to define any custom GUI widgets that collect FilterParameters
* for a filter. Do NOT define general reusable widgets here.
*/
namespace FilterParameterWidgetType
{
/* const QString SomeCustomWidget("SomeCustomWidget"); */
}

namespace DREAM3D
{
  namespace FeatureData
  {
    const QString MicroTextureRegionFractionOccupied("MicroTextureRegionFractionOccupied");
    const QString MicroTextureRegionNumCells("MicroTextureRegionNumCells");
  }
}
#endif
