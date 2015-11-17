

#include "FloatVec4FilterParameter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4FilterParameter::FloatVec4FilterParameter() :
FilterParameter()
{}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4FilterParameter::~FloatVec4FilterParameter()
{}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FloatVec4FilterParameter::Pointer FloatVec4FilterParameter::New(const QString& humanLabel, const QString& propertyName,
  const FloatVec4_t& defaultValue, Category category, int groupIndex)
{

  FloatVec4FilterParameter::Pointer ptr = FloatVec4FilterParameter::New();
  ptr->setHumanLabel(humanLabel);
  ptr->setPropertyName(propertyName);
  QVariant v;
  v.setValue(defaultValue);
  ptr->setDefaultValue(v);
  ptr->setCategory(category);
  ptr->setGroupIndex(groupIndex);

  return ptr;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QString FloatVec4FilterParameter::getWidgetType()
{
  return QString("FloatVec4Widget");
}

