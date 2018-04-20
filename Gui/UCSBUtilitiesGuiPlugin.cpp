

#include "UCSBUtilitiesGuiPlugin.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UCSBUtilitiesGuiPlugin::UCSBUtilitiesGuiPlugin()
: UCSBUtilitiesPlugin()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UCSBUtilitiesGuiPlugin::~UCSBUtilitiesGuiPlugin() = default;

#include "UCSBUtilities/Gui/FilterParameterWidgets/RegisterKnownFilterParameterWidgets.cpp"
