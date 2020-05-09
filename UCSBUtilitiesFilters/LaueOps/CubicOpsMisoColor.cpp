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

#include "CubicOpsMisoColor.h"

// Include this FIRST because there is a needed define for some compiles
// to expose some of the constants needed below
#include "SIMPLib/Math/SIMPLibMath.h"
#include "SIMPLib/Math/GeometryMath.h"
#include "SIMPLib/Utilities/ColorUtilities.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CubicOpsMisoColor::CubicOpsMisoColor() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CubicOpsMisoColor::~CubicOpsMisoColor() = default;

/**
 * @brief Sorts the 3 values from low to high
 * @param a Input
 * @param b Input
 * @param c Input
 * @param x Output
 * @param y Output
 * @param z Output
 */
template <typename T>
void _TripletSort(T a, T b, T c, T& x, T& y, T& z)
{
  if(a > b && a > c)
  {
    z = a;
    if(b > c)
    {
      y = b;
      x = c;
    }
    else
    {
      y = c;
      x = b;
    }
  }
  else if(b > a && b > c)
  {
    z = b;
    if(a > c)
    {
      y = a;
      x = c;
    }
    else
    {
      y = c;
      x = a;
    }
  }
  else if(a > b)
  {
    y = a;
    x = b;
    z = c;
  }
  else if(a >= c && b >= c)
  {
    x = c;
    y = a;
    z = b;
  }
  else
  {
    x = a;
    y = b;
    z = c;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SIMPL::Rgb CubicOpsMisoColor::generateMisorientationColor(const QuatType& q, const QuatType& refFrame) const
{
  // double n1, n2, n3, w;
  double x, x1, x2, x3, x4, x5, x6, x7;
  double y, y1, y2, y3, y4, y5, y6, y7;
  double z, z1, z2, z3, z4, z5, z6, z7;
  double k, h, s, v;

  QuatType q1 = q;
  QuatType q2 = refFrame;

  // get disorientation
  OrientationD axisAngle = CubicOps::calculateMisorientation(q1, q2);
  axisAngle[0] = fabs(axisAngle[0]);
  axisAngle[1] = fabs(axisAngle[1]);
  axisAngle[2] = fabs(axisAngle[2]);

  _TripletSort(axisAngle[0], axisAngle[1], axisAngle[2], z, y, x);

  // eq c9.1
  k = tan(axisAngle[3] / 2.0f);
  x = x * k;
  y = y * k;
  z = z * k;

  // eq c9.2
  x1 = x;
  if(x >= SIMPLib::Constants::k_1Over3 && atan2(z, y) >= ((1.0f - 2.0f * x) / x))
  {
    y1 = (x * (y + z)) / (1.0f - x);
    z1 = (x * z * (y + z)) / (y * (1.0f - x));
  }
  else
  {
    y1 = y;
    z1 = z;
  }

  // eq c9.3
  x2 = x1 - SIMPLib::Constants::k_Tan_OneEigthPi;
  y2 = y1 * SIMPLib::Constants::k_Cos_ThreeEightPi - z1 * SIMPLib::Constants::k_Sin_ThreeEightPi;
  z2 = y1 * SIMPLib::Constants::k_Sin_ThreeEightPi + z1 * SIMPLib::Constants::k_Cos_ThreeEightPi;

  // eq c9.4
  x3 = x2;
  y3 = y2 * (1.0f + (y2 / z2) * SIMPLib::Constants::k_Tan_OneEigthPi);
  z3 = z2 + y2 * SIMPLib::Constants::k_Tan_OneEigthPi;

  // eq c9.5
  x4 = x3;
  y4 = (y3 * SIMPLib::Constants::k_Cos_OneEigthPi) / SIMPLib::Constants::k_Tan_OneEigthPi;
  z4 = z3 - x3 / SIMPLib::Constants::k_Cos_OneEigthPi;

  // eq c9.6
  k = atan2(-x4, y4);
  x5 = x4 * (sin(k) + fabs(cos(k)));
  y5 = y4 * (sin(k) + fabs(cos(k)));
  z5 = z4;

  // eq c9.7
  k = atan2(-x5, y5);
  x6 = -sqrt(x5 * x5 + y5 * y5) * sin(2.0f * k);
  y6 = sqrt(x5 * x5 + y5 * y5) * cos(2.0f * k);
  z6 = z5;

  // eq c9.8 these hsv are from 0 to 1 in cartesian coordinates
  x7 = (x6 * SIMPLib::Constants::k_Sqrt3 - y6) / (2.0f * SIMPLib::Constants::k_Tan_OneEigthPi);
  y7 = (x6 + y6 * SIMPLib::Constants::k_Sqrt3) / (2.0f * SIMPLib::Constants::k_Tan_OneEigthPi);
  z7 = z6 * (SIMPLib::Constants::k_Cos_OneEigthPi / SIMPLib::Constants::k_Tan_OneEigthPi);

  // convert to traditional hsv (0-1)
  h = atan2(y7, x7);
  if(h < 0.0f)
  {
    h += SIMPLib::Constants::k_2Pi;
  }
  h /= SIMPLib::Constants::k_2Pi;
  s = sqrt(x7 * x7 + y7 * y7);
  v = z7;
  if(v > 0.0f)
  {
    s = s / v;
  }

  SIMPL::Rgb rgb = ColorUtilities::ConvertHSVtoRgb(h, s, v);

  // now standard 0-255 rgb, needs rotation
  return RgbColor::dRgb(255 - RgbColor::dGreen(rgb), RgbColor::dBlue(rgb), RgbColor::dRed(rgb), 0);
}

#if 0
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
UInt8ArrayType::Pointer CubicOpsMisoColor::generateMisorientationTriangleLegend(double angle, int n1, int n2, int imageDim)
{
  std::vector<size_t> dims(1, 4);
  UInt8ArrayType::Pointer image = UInt8ArrayType::CreateArray(imageDim * imageDim, dims, "Cubic High Misorientation Triangle Legend");

  //uint32_t* pixelPtr = reinterpret_cast<uint32_t*>(image->getPointer(0));

  double maxk = SIMPLib::Constants::k_Sqrt2 - 1;
  double maxdeg = 2 * atan(sqrt(6 * maxk * maxk - 4 * maxk + 1));
  double deg1 = 2 * atan(sqrt(2 * maxk * maxk));

  double A = angle * M_PI / 360;
  std::vector<double> B;
  std::vector< std::vector<double> > C;

  ///Generate regularly spaced array of points in misorientation space
  if(A <= M_PI / 8)
  {
    double theta1 = atan(1 / sin(M_PI_4));
    double theta2 = M_PI_2;
    B = SIMPLibMath::linspace(theta1, theta2, n2);
    for(int i = 0; i < n2; i++)
    {
      C.push_back(SIMPLibMath::linspace(asin(1 / tan(B[i])), M_PI_4, n1));
    }
  }
  else if(A > M_PI / 8 && A <= M_PI / 6)
  {
    double theta1 = atan(1 / sin(M_PI_4));
    double theta2 = acos(-(maxk * (1 / tan(A))) * (maxk * (1 / tan(A)))) / 2;
    double theta3 = M_PI_2;
    int frac1 = floor((n2 - 3) * (theta2 - theta1) / (theta3 - theta1));
    int frac2 = (n2 - 3) - frac1;
    std::vector<double> temptheta1 = SIMPLibMath::linspace(theta1, theta2, frac1 + 2);
    std::vector<double> temptheta2 = SIMPLibMath::linspace(theta2, theta3, frac2 + 2);
    temptheta2.erase(temptheta2.begin());
    B.insert(B.end(), temptheta1.begin(), temptheta1.end());
    B.insert(B.end(), temptheta2.begin(), temptheta2.end());
    for(int i = 0; i < n2; i++)
    {
      double theta = B[i];
      if(theta >= theta1 && theta <= theta2)
      {
        C.push_back(SIMPLibMath::linspace(asin(1 / tan(theta)), M_PI_4, n1));
      }
      else
      {
        C.push_back(SIMPLibMath::linspace(acos(maxk / (tan(A)*sin(theta))), M_PI_4, n1));
      }
    }
  }
  else if(A > M_PI / 6 && A <= deg1 / 2)
  {
    std::vector<double> thetaSort;
    double thetaa = acos((1 - sqrt(6 * tan(A) * tan(A) - 2)) / (3 * tan(A)));
    thetaSort.push_back(thetaa);
    thetaSort.push_back(M_PI_2);
    thetaSort.push_back(acos((2 - sqrt(6 * tan(A)*tan(A) - 2)) / (6 * tan(A))));
    double thetad = (acos(-(maxk / tan(A)) * (maxk / tan(A)))) / 2;
    thetaSort.push_back(thetad);
    std::sort(thetaSort.begin(), thetaSort.end());
    double theta1 = thetaSort[0];
    double theta2 = thetaSort[1];
    double theta3 = thetaSort[2];
    double theta4 = thetaSort[3];
    int frac1 = (floor((n2 - 4) * (theta2 - theta1) / (theta4 - theta1)));
    int frac2 = (floor((n2 - 4) * (theta3 - theta2) / (theta4 - theta1)));
    int frac3 = n2 - 4 - frac1 - frac2;
    std::vector<double> temptheta3 = SIMPLibMath::linspace(theta1, theta2, (frac1 + 2));
    std::vector<double> temptheta4 = SIMPLibMath::linspace(theta2, theta3, (frac2 + 2));
    temptheta4.erase(temptheta4.begin());
    std::vector<double>temptheta5 = SIMPLibMath::linspace(theta3, theta4, (frac3 + 2));
    temptheta5.erase(temptheta5.begin());
    B.insert(B.end(), temptheta3.begin(), temptheta3.end());
    B.insert(B.end(), temptheta4.begin(), temptheta4.end());
    B.insert(B.end(), temptheta5.begin(), temptheta5.end());

    for(int i = 0; i < n2; i++)
    {
      double theta = B[i];
      double phi1, phi2;
      if(thetaa <= thetad)
      {
        if(theta <= theta2)
        {
          phi1 = asin(1 / tan(theta));
          double k = (1 - tan(A) * cos(theta)) / (SIMPLib::Constants::k_Sqrt2 * (tan(A) * sin(theta)));
          if(k > 1)
          {
            k = 1;
          }
          if(k < -1)
          {
            k = -1;
          }
          phi2 = asin(k) - M_PI_4;
        }
        else if(theta > theta2 && theta < theta3)
        {
          phi1 = acos((SIMPLib::Constants::k_Sqrt2 - 1) / (tan(A) * sin(theta)));
          phi2 = M_PI_4;
        }
      }
      else
      {
        if(theta <= theta2)
        {
          phi1 = asin(1 / tan(theta));
        }
        else if(theta > theta2 && theta <= theta3)
        {
          phi1 = acos(maxk / (tan(A) * sin(theta)));
        }

        double d3 = tan(A) * cos(theta);
        double k = ((1 - d3) + sqrt(2 * (tan(A) * tan(A) - d3 * d3) - (1 - d3) * (1 - d3))) / (2 * tan(A) * sin(theta));
        if(k > 1)
        {
          k = 1;
        }
        if(k < -1)
        {
          k = -1;
        }
        phi2 = acos(k);
      }
      if(theta > theta3)
      {
        phi1 = acos(maxk / (tan(A) * sin(theta)));
        phi2 = M_PI_4;
      }
      C.push_back(SIMPLibMath::linspace(phi1, phi2, n1));
    }
  }
  else if(A >= deg1 / 2 && A <= maxdeg / 2)
  {
    double theta1 = acos(((1 - maxk) - sqrt(2 * (tan(A) * tan(A) - maxk * maxk) - (1 - maxk) * (1 - maxk))) / (2 * tan(A)));
    double theta2 = acos((1 - sqrt(6 * (tan(A) * tan(A)) - 2)) / (3 * tan(A)));
    double theta3 = acos((sqrt(tan(A) * tan(A) - 2 * (maxk * maxk))) / (tan(A)));
    int frac1 = (floor((n2 - 3) * (theta2 - theta1) / (theta3 - theta1)));
    int frac2 = (n2 - 3) - frac1;
    std::vector<double> temptheta1 = SIMPLibMath::linspace(theta1, theta2, (frac1 + 2));
    std::vector<double> temptheta2 = SIMPLibMath::linspace(theta2, theta3, (frac2 + 2));
    temptheta2.erase(temptheta2.begin());
    B.insert(B.end(), temptheta1.begin(), temptheta1.end());
    B.insert(B.end(), temptheta2.begin(), temptheta2.end());
    for(int i = 0; i < n2; i++)
    {
      double theta = B[i];
      double phi1, phi2;
      phi1 = acos(maxk / (tan(A) * sin(theta)));
      if(theta >= theta1 && theta < theta2)
      {
        double d3 = tan(A) * cos(theta);
        double k = ((1 - d3) + sqrt(2 * (tan(A) * tan(A) - d3 * d3) - (1 - d3) * (1 - d3))) / (2 * tan(A) * sin(theta));
        phi2 = acos(k);
      }
      else
      {
        phi2 = M_PI_4;
      }
      C.push_back(SIMPLibMath::linspace(phi1, phi2, n1));
    }
  }

  ///create image, fill with empty pixels, setup painter
  // int width = 1000;
  //double scale = width / tan(M_PI / 8);
  /*
  int height = ceil(0.349159 * scale);
  QPainter painter;
  image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
  image.fill(0x00000000);
  painter.begin(&image);
  painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
  */

  ///draw standard sterographic triangle border (dashed)
  /*
  QPen pen;
  pen.setColor(Qt::black);
  pen.setWidth(2);
  pen.setStyle(Qt::DotLine);
  painter.setPen(pen);
  */
  double r = tan(A);
  std::vector<double> x, y, z;
  y = SIMPLibMath::linspace(0, r / SIMPLib::Constants::k_Sqrt2, 100);
  for(std::vector<double>::size_type i = 0; i < y.size(); i++)
  {
    double k = r * r - y[i] * y[i];
    if(k < 0)
    {
      k = 0;
    }
    x.push_back(sqrt(k));
    z.push_back(0);
  }
  std::vector< std::pair<double, double> > ptsa = rodri2pair(x, y, z);
  //std::vector< std::pair<int, int> > pointlist=scalePoints(ptsa, scale);
  //std::vector<QPoint> qpointlist = pairToQPoint(pointlist);
  //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

  x.clear();
  y.clear();
  z.clear();
  x = SIMPLibMath::linspace(r / SIMPLib::Constants::k_Sqrt3, r, 100);
  for(std::vector<double>::size_type i = 0; i < x.size(); i++)
  {
    double k = r * r - x[i] * x[i];
    if(k < 0)
    {
      k = 0;
    }
    y.push_back(sqrt((k) / 2));
    z.push_back(y[i]);
  }
  ptsa = rodri2pair(x, y, z);
  //pointlist=scalePoints(ptsa, scale);
  //qpointlist = pairToQPoint(pointlist);
  //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

  x.clear();
  y.clear();
  z.clear();
  x = SIMPLibMath::linspace(r / SIMPLib::Constants::k_Sqrt3, r / SIMPLib::Constants::k_Sqrt2, 100);
  for(std::vector<double>::size_type i = 0; i < x.size(); i++)
  {
    y.push_back(x[i]);
    double k = r * r - 2 * x[i] * x[i];
    if(k < 0)
    {
      k = 0;
    }
    z.push_back(sqrt(k));
  }
  ptsa = rodri2pair(x, y, z);
  //pointlist=scalePoints(ptsa, scale);
  //qpointlist = pairToQPoint(pointlist);
  //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

  ///find triangle vertices by converting misorientation space grid to pixels
  std::vector< std::pair<double, double> > ba;
  std::vector<double> d0, d1, d2, d3;
  //int k = 0;
  for(int i = 0; i < n1; i++)
  {
    for(int j = 0; j < n2; j++)
    {
      d3.push_back(tan(A)*cos(B[j]));
      d2.push_back(tan(A)*sin(B[j])*sin(C[j][i]));
      d1.push_back(tan(A)*sin(B[j])*cos(C[j][i]));
      //      double d = 1 - d1[k] * d1[k] - d2[k] * d2[k] - d3[k] * d3[k];
      //      k++;
    }
  }

  ///add corner vertices if the sst isn't filled
  if(A > M_PI / 8)
  {
    d1.push_back(0);
    d2.push_back(0);
    d3.push_back(0);

    if(A > M_PI / 8 && A <= M_PI / 6)
    {
    }
    else
    {
      d1.push_back(r / SIMPLib::Constants::k_Sqrt3);
      d2.push_back(r / SIMPLib::Constants::k_Sqrt3);
      d3.push_back(r / SIMPLib::Constants::k_Sqrt3);
    }
  }
  ba = rodri2pair(d1, d2, d3);

  ///find triangles
  std::vector< std::pair<int, int> > scaledba;//=scalePoints(ba, scale);
  //std::vector<QPoint> qpointba = pairToQPoint(scaledba);


  std::vector< std::vector<int> > triList;
  /*
  tpp::Delaunay::Point tempP;
  vector<tpp::Delaunay::Point> v;
  for(int i=0; i<ba.size(); i++)
  {
    tempP[0]=scaledba[i].first;
    tempP[1]=scaledba[i].second;
    v.push_back(tempP);
  }
  tpp::Delaunay delobject(v);
  delobject.Triangulate();

  for(tpp::Delaunay::fIterator fit = delobject.fbegin(); fit != delobject.fend(); ++fit)
  {
    std::vector<int> triangle;
    triangle.push_back(delobject.Org(fit));
    triangle.push_back(delobject.Dest(fit));
    triangle.push_back(delobject.Apex(fit));
    triList.push_back(triangle);
  }
  */

  ///fill triangles
  for(std::vector< std::pair<double, double> >::size_type i = 0; i < ba.size(); i++)
  {
    QuatType quat, refQuat;
    refQuat.x = 0;
    refQuat.y = 0;
    refQuat.z = 0;
    refQuat.w = 1;
    //have rodrigues vector, need quat
    double tanAng = sqrt(d1[i] * d1[i] + d2[i] * d2[i] + d3[i] * d3[i]);
    double cosAng = cosf(atanf(tanAng));
    quat.x = d1[i] * cosAng * tanAng;
    quat.y = d2[i] * cosAng * tanAng;
    quat.z = d3[i] * cosAng * tanAng;
    quat.w = cosAng;
    SIMPL::Rgb pix = generateMisorientationColor(quat, refQuat);
    //image.setPixel(qpointba[i].x(), qpointba[i].y(), pix);
  }

  std::pair<int, int> vert1, vert2, vert3;
  std::vector<int> triangle;

  const double k_PiOver8 = M_PI / 8.0;
  const double k_PiOver6 = M_PI / 6.0;
  qint32 baSizeMinus1 = static_cast<qint32>(ba.size() - 1);
  qint32 baSizeMinus2 = static_cast<qint32>(ba.size() - 2);

  for(std::vector< std::vector<int> >::size_type k = 0; k < triList.size(); k++)
  {
    triangle = triList[k];
    vert1 = scaledba[triangle[0]];
    vert2 = scaledba[triangle[1]];
    vert3 = scaledba[triangle[2]];

    //check that none of verticies are special spots
    bool color = true;
    if(A > k_PiOver8)
    {
      if( A <= k_PiOver6)
      {
        //1 extra point at end
        if(triangle[0] == baSizeMinus1)
        {
          color = false;
        }
        if(triangle[1] == baSizeMinus1)
        {
          color = false;
        }
        if(triangle[2] == baSizeMinus1)
        {
          color = false;
        }
      }
      else
      {
        //2 extra points at end
        if(triangle[0] == baSizeMinus1)
        {
          color = false;
        }
        if(triangle[1] == baSizeMinus1)
        {
          color = false;
        }
        if(triangle[2] == baSizeMinus1)
        {
          color = false;
        }
        if(triangle[0] == baSizeMinus2)
        {
          color = false;
        }
        if(triangle[1] == baSizeMinus2)
        {
          color = false;
        }
        if(triangle[2] == baSizeMinus2)
        {
          color = false;
        }
      }
    }

    if(color)
    {
      double x1 = 0.0, x2 = 0.0, x3 = 0.0, y1 = 0.0, y2 = 0.0, y3 = 0.0;
      int r1 = 0, r2 = 0, r3 = 0, g1 = 0, g2 = 0, g3 = 0, b1 = 0, b2 = 0, b3 = 0;
      x1 = vert1.first;
      x2 = vert2.first;
      x3 = vert3.first;
      y1 = vert1.second;
      y2 = vert2.second;
      y3 = vert3.second;

      //find rectangle bounding triangle
      int xMin, xMax, yMin, yMax;
      xMin = std::min(std::min(x1, x2), x3);
      xMax = std::max(std::max(x1, x2), x3);
      yMin = std::min(std::min(y1, y2), y3);
      yMax = std::max(std::max(y1, y2), y3);

      //get colors of vertices
      /*
        QRgb pixval1=image.pixel(x1, y1);
        QRgb pixval2=image.pixel(x2, y2);
        QRgb pixval3=image.pixel(x3, y3);
        r1 = qRed(pixval1);
        r2 = qRed(pixval2);
        r3 = qRed(pixval3);
        g1 = qGreen(pixval1);
        g2 = qGreen(pixval2);
        g3 = qGreen(pixval3);
        b1 = qBlue(pixval1);
        b2 = qBlue(pixval2);
        b3 = qBlue(pixval3);
        */

      //loop over pixels in rectangle
      for(int i = xMin; i <= xMax; i++)
      {
        for(int j = yMin; j <= yMax; j++)
        {
          //determine barycentric coordinates
          double gamma1, gamma2, gamma3;
          double det = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
          gamma1 = ((y2 - y3) * ((double)i - x3) + (x3 - x2) * ((double)j - y3)) / det;
          gamma2 = ((y3 - y1) * ((double)i - x3) + (x1 - x3) * ((double)j - y3)) / det;
          gamma3 = 1.0 - gamma1 - gamma2;

          //check if pixel is inside triangle
          double minval = -0.0000000000000000001;//misses some boundary points for >=0
          if(gamma1 >= minval && gamma2 >= minval && gamma3 >= minval && gamma1 <= 1.0 && gamma2 <= 1.0 && gamma3 <= 1.0 )
          {
            if(gamma1 == 1.0 || gamma2 == 1.0 || gamma3 == 1.0)
            {
              //vertex
            }
            else
            {
              //edge or inside
#if 0
              uint8_t red = r1 * gamma1 + r2 * gamma2 + r3 * gamma3;
              uint8_t green = g1 * gamma1 + g2 * gamma2 + g3 * gamma3;
              uint8_t blue = b1 * gamma1 + b2 * gamma2 + b3 * gamma3;
              uint8_t alpha = 255;
              unsigned int pix = (alpha << 24) | (red << 16) | (green << 8) | blue;
              image.setPixel(i, j, pix);
#endif
            }
          }
          else
          {
            //outside triangle
          }
        }
      }
    }
  }

  ///Draw Solid Border
  //pen.setStyle(Qt::SolidLine);
  //painter.setPen(pen);

  if(A <= M_PI / 8)
  {
    double k = 0.0;
    x.clear();
    y.clear();
    z.clear();
    y = SIMPLibMath::linspace(0.0f, r / sqrt(2.0f), 100);
    for(std::vector<double>::size_type i = 0; i < y.size(); i++)
    {
      z.push_back(0);
      k = r * r - y[i] * y[i];
      if(k < 0)
      {
        k = 0;
      }
      x.push_back(sqrt(k));
    }
    ptsa = rodri2pair(x, y, z);
    //std::vector< std::pair<int, int> >pointlist=scalePoints(ptsa, scale);
    //std::vector<QPoint> qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    x = SIMPLibMath::linspace(r / SIMPLib::Constants::k_Sqrt3, r, 100);
    for(std::vector<double>::size_type i = 0; i < x.size(); i++)
    {
      k = (r * r - x[i] * x[i]) / 2;
      if(k < 0)
      {
        k = 0;
      }
      y.push_back(sqrt(k));
      z.push_back(y[i]);
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    x = SIMPLibMath::linspace(r / SIMPLib::Constants::k_Sqrt3, r / SIMPLib::Constants::k_Sqrt2, 100);
    for(std::vector<double>::size_type i = 0; i < x.size(); i++)
    {
      y.push_back(x[i]);
      k = r * r - 2 * x[i] * x[i];
      if(k < 0)
      {
        k = 0;
      }
      z.push_back(sqrt(k));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));
  }

  else if(A > M_PI / 8 && A <= M_PI / 6)
  {
    x.clear();
    y.clear();
    z.clear();
    double theta1 = atan(1 / sin(M_PI_4));
    double theta2 = acos(-(maxk * maxk) / (tan(A) * tan(A))) / 2;
    double theta3 = M_PI_2;
    double phi3 = acos(maxk / (tan(A) * sin(theta3)));

    y = SIMPLibMath::linspace(r * sin(phi3), r / (SIMPLib::Constants::k_Sqrt2), 100);
    for(std::vector<double>::size_type i = 0; i < y.size(); i++)
    {
      x.push_back(sqrt(r * r - y[i]*y[i]));
      z.push_back(0);
    }
    ptsa = rodri2pair(x, y, z);
    //std::vector< std::pair<int, int> >pointlist=scalePoints(ptsa, scale);
    //std::vector<QPoint> qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(theta1), r * cos(theta2), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      y.push_back(z[i]);
      x.push_back(sqrt(r * r - 2 * z[i]*z[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(theta1), r * cos(theta3), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      x.push_back(sqrt((r * r - z[i]*z[i]) / 2));
      y.push_back(x[i]);
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(theta2), r * cos(theta3), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      x.push_back(maxk);
      y.push_back(sqrt(r * r - maxk * maxk - z[i]*z[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

  }
  else if(A > M_PI / 6 && A <= deg1 / 2)
  {
    x.clear();
    y.clear();
    z.clear();
    double thetac = acos((2 - sqrt(6 * (tan(A) * tan(A)) - 2)) / (6 * tan(A)));
    double thetaa = acos((1 - sqrt(6 * (tan(A) * tan(A)) - 2)) / (3 * tan(A)));
    double thetad = acos(-(maxk * maxk) / (tan(A) * tan(A))) / 2;
    double thetab = M_PI_2;
    double phi3 = acos(maxk / (tan(A) * sin(thetab)));

    y = SIMPLibMath::linspace(r * sin(phi3), r / (SIMPLib::Constants::k_Sqrt2), 100);
    for(std::vector<double>::size_type i = 0; i < y.size(); i++)
    {
      z.push_back(0);
      x.push_back(sqrt(r * r - y[i]*y[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //std::vector< std::pair<int, int> >pointlist=scalePoints(ptsa, scale);
    //std::vector<QPoint> qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(thetac), r * cos(thetad), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      y.push_back(z[i]);
      double k = r * r - 2 * (z[i] * z[i]);
      if(k < 0.0)
      {
        k = 0.0;
      }
      x.push_back(sqrt(k));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(thetaa), r * cos(thetab), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      x.push_back(sqrt((r * r - (z[i]*z[i])) / 2));
      y.push_back(x[i]);
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(thetad), r * cos(thetab), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      x.push_back(maxk);
      y.push_back(sqrt(r * r - maxk * maxk - z[i]*z[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    std::vector<double> theta, tempd3, phi;
    theta = SIMPLibMath::linspace(thetac, thetaa, 100);
    for(std::vector<double>::size_type i = 0; i < theta.size(); i++)
    {
      tempd3.push_back(tan(A)*cos(theta[i]));
      double k = 2 * ((tan(A) * tan(A)) - tempd3[i] * tempd3[i]) - (1 - tempd3[i]) * (1 - tempd3[i]);
      if(k < 0.0)
      {
        k = 0.0;
      }
      phi.push_back(acos((((1 - tempd3[i]) + (sqrt(k))) / 2) / (tan(A)*sin(theta[i]))));
      z.push_back(r * cos(theta[i]));
      x.push_back(r * sin(theta[i])*cos(phi[i]));
      y.push_back(sqrt(r * r - x[i]*x[i] - z[i]*z[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));
  }
  else if(A >= deg1 / 2 && A <= maxdeg / 2)
  {
    x.clear();
    y.clear();
    z.clear();
    double theta1 = acos(((1 - maxk) - sqrt(2 * (tan(A) * tan(A) - maxk * maxk) - (1 - maxk) * (1 - maxk))) / (2 * tan(A)));
    double theta2 = acos((1 - sqrt(6 * tan(A) * tan(A) - 2)) / (3 * tan(A)));
    double theta3 = acos((sqrt(tan(A) * tan(A) - 2 * maxk * maxk)) / (tan(A)));

    z = SIMPLibMath::linspace(r * cos(theta2), r * cos(theta3), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      x.push_back(sqrt((r * r - z[i]*z[i]) / 2));
      y.push_back(x[i]);
    }
    ptsa = rodri2pair(x, y, z);
    //std::vector< std::pair<int, int> >pointlist=scalePoints(ptsa, scale);
    //std::vector<QPoint> qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    std::vector<double> theta, tempd3, phi;
    theta = SIMPLibMath::linspace(theta1, theta2, 100);
    for(std::vector<double>::size_type i = 0; i < theta.size(); i++)
    {
      tempd3.push_back(tan(A)*cos(theta[i]));
      double k = 2 * (tan(A) * tan(A) - tempd3[i] * tempd3[i]) - (1 - tempd3[i]) * (1 - tempd3[i]);
      if(k < 0.0)
      {
        k = 0.0;
      }
      phi.push_back(acos((((1 - tempd3[i]) + (sqrt(k))) / 2) / (tan(A)*sin(theta[i]))));
      x.push_back(r * sin(theta[i])*cos(phi[i]));
      z.push_back(r * cos(theta[i]));
      y.push_back(sqrt(r * r - x[i]*x[i] - z[i]*z[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));

    x.clear();
    y.clear();
    z.clear();
    z = SIMPLibMath::linspace(r * cos(theta1), r * cos(theta3), 100);
    for(std::vector<double>::size_type i = 0; i < z.size(); i++)
    {
      x.push_back(maxk);
      y.push_back(sqrt(r * r - maxk * maxk - z[i]*z[i]));
    }
    ptsa = rodri2pair(x, y, z);
    //pointlist=scalePoints(ptsa, scale);
    //qpointlist = pairToQPoint(pointlist);
    //painter.drawPolyline(qpointlist.data(), static_cast<int>(qpointlist.size()));
  }

  return image;
}


std::vector< std::pair<double, double> > CubicOpsMisoColor::rodri2pair(std::vector<double> x, std::vector<double> y, std::vector<double> z)
{
  std::vector< std::pair<double, double> > result;
  double q0, q1, q2, q3, ang, r, x1, y1, z1, rad, xPair, yPair, k;

  for(std::vector<double>::size_type i = 0; i < x.size(); i++)
  {
    //rodri2volpreserv
    q0 = sqrt(1 / (1 + x[i] * x[i] + y[i] * y[i] + z[i] * z[i]));
    q1 = x[i] * q0;
    q2 = y[i] * q0;
    q3 = z[i] * q0;
    ang = acos(q0);
    r = pow(1.5 * (ang - sin(ang) * cos(ang)), (1.0 / 3.0));
    x1 = q1 * r;
    y1 = q2 * r;
    z1 = q3 * r;
    if(sin(ang) != 0)
    {
      x1 = x1 / sin(ang);
      y1 = y1 / sin(ang);
      z1 = z1 / sin(ang);
    }

    //areapreservingx
    rad = sqrt(x1 * x1 + y1 * y1 + z1 * z1);
    if(rad == 0)
    {
      rad++;
    }
    k = 2 * (1 - fabs(x1 / rad));
    if(k < 0)
    {
      k = 0;
    }
    k = rad * sqrt(k);
    xPair = y1 * k;
    yPair = z1 * k;
    k = rad * rad - x1 * x1;
    if(k > 0)
    {
      xPair = xPair / sqrt(k);
      yPair = yPair / sqrt(k);
    }
    result.push_back(std::make_pair(xPair, yPair));
  }
  return result;
}
#endif

// -----------------------------------------------------------------------------
CubicOpsMisoColor::Pointer CubicOpsMisoColor::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}

// -----------------------------------------------------------------------------
QString CubicOpsMisoColor::getNameOfClass() const
{
  return QString("CubicOpsMisoColor");
}

// -----------------------------------------------------------------------------
QString CubicOpsMisoColor::ClassName()
{
  return QString("CubicOpsMisoColor");
}

// -----------------------------------------------------------------------------
CubicOpsMisoColor::Pointer CubicOpsMisoColor::New()
{
  Pointer sharedPtr(new(CubicOpsMisoColor));
  return sharedPtr;
}
