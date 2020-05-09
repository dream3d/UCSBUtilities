#pragma once

#include "UCSBUtilities/UCSBUtilitiesPlugin.h"

class UCSBUtilitiesGuiPlugin : public UCSBUtilitiesPlugin
{
  Q_OBJECT
  Q_INTERFACES(ISIMPLibPlugin)
  Q_PLUGIN_METADATA(IID "net.bluequartz.dream3d.UCSBUtilitiesGuiPlugin")

public:
  UCSBUtilitiesGuiPlugin();
  ~UCSBUtilitiesGuiPlugin() override;

  /**
   * @brief Register all the filters with the FilterWidgetFactory
   */
  void registerFilterWidgets(FilterWidgetManager* fwm) override;

public:
  UCSBUtilitiesGuiPlugin(const UCSBUtilitiesGuiPlugin&) = delete;            // Copy Constructor Not Implemented
  UCSBUtilitiesGuiPlugin(UCSBUtilitiesGuiPlugin&&) = delete;                 // Move Constructor Not Implemented
  UCSBUtilitiesGuiPlugin& operator=(const UCSBUtilitiesGuiPlugin&) = delete; // Copy Assignment Not Implemented
  UCSBUtilitiesGuiPlugin& operator=(UCSBUtilitiesGuiPlugin&&) = delete;      // Move Assignment Not Implemented
};
