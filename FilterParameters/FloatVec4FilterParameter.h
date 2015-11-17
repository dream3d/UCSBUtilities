

#ifndef _FloatVec4FilterParameter_H_
#define _FloatVec4FilterParameter_H_

#include <QtCore/QJsonObject>

#include "SIMPLib/FilterParameters/FilterParameter.h"

typedef struct {
  float a; float b; float c; float d;

  void writeJson(QJsonObject &json)
  {
    json["a"] = a;
    json["b"] = b;
    json["c"] = c;
    json["d"] = d;
  }

  bool readJson(QJsonObject &json)
  {
    if (json["a"].isDouble() && json["b"].isDouble() && json["c"].isDouble() && json["d"].isDouble())
    {
      a = static_cast<float>(json["a"].toDouble());
      b = static_cast<float>(json["b"].toDouble());
      c = static_cast<float>(json["c"].toDouble());
      d = static_cast<float>(json["d"].toDouble());
      return true;
    }
    return false;
  }
} FloatVec4_t;

Q_DECLARE_METATYPE(FloatVec4_t)

class SIMPLib_EXPORT FloatVec4FilterParameter : public FilterParameter
{
public:
  SIMPL_SHARED_POINTERS(FloatVec4FilterParameter)
    SIMPL_STATIC_NEW_MACRO(FloatVec4FilterParameter)
    SIMPL_TYPE_MACRO(FloatVec4FilterParameter)

    static Pointer New(const QString& humanLabel, const QString& propertyName,
    const FloatVec4_t& defaultValue, Category category,
     int groupIndex = -1);

    virtual ~FloatVec4FilterParameter();

  /**
   * @brief getWidgetType Returns the type of widget that displays and controls
   * this FilterParameter subclass
   * @return
   */
  QString getWidgetType();


protected:
  FloatVec4FilterParameter();

private:
  FloatVec4FilterParameter(const FloatVec4FilterParameter&); // Copy Constructor Not Implemented
  void operator=(const FloatVec4FilterParameter&); // Operator '=' Not Implemented
};

#endif /* _FloatVec4FilterParameter_H_ */
