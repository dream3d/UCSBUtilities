

#include "Symmetric6x6FilterParameter.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::Symmetric6x6FilterParameter() :
FilterParameter()
{}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::~Symmetric6x6FilterParameter()
{}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
Symmetric6x6FilterParameter::Pointer Symmetric6x6FilterParameter::New(const QString& humanLabel, const QString& propertyName,
  const FloatVec21_t& defaultValue, Category category, int groupIndex)
{

  Symmetric6x6FilterParameter::Pointer ptr = Symmetric6x6FilterParameter::New();
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
QString Symmetric6x6FilterParameter::getWidgetType()
{
  return QString("Symmetric6x6Widget");
}

