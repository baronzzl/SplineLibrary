Spline Types（样条曲线类型）
=============
This page gives a breakdown of each spline type, how to use each one, and the advantages/disadvantages of each type. These examples use QVector2D as the type to interpolate, but splines are template classes, so other types are allowed.

该篇文章提供了每种样条线的细分，每种样条线的使用方法以及每种类型的优点/缺点。 这些示例使用`QVector2D`作为内插类型，但是样条线是模板类，因此允许其他类型。

Tl;dr
-------------
If you need the spline to pass through the input points, start with a [Uniform Catmull-Rom Spline](#catmull-rom-spline). If you don't, start with a [Uniform Cubic B-Spline](#uniform-cubic-b-spline).

如果你需要样条曲线通过输入点，可以从 [Uniform Catmull-Rom Spline](#catmull-rom-spline)开始看。 如果不是，则可以从[Uniform Cubic B-Spline](#uniform-cubic-b-spline)开始看。

Simple Types（简单类型）
------------- 
If you're not sure which one to use, start with these three.

### Catmull-Rom Spline
A Catmull-Rom Spline computes the tangent for each point from the positions of the two closest points, then interpolates based on both the position and the tangent.

Catmull-Rom样条曲线从两个最近点的位置计算每个点的切线，然后根据位置和切线进行插值。

To use, import the appropriate header:
`#include "spline_library/splines/uniform_cr_spline.h"`

Create a catmull-rom spline by passing a `std::vector<T>` to the constructor, containing a list of points to interpolate through:
```c++
std::vector<Qvector2D> splinePoints = ...;
UniformCRSpline<QVector2D> mySpline(splinePoints);
QVector2D interpolatedPosition = mySpline.getPosition(0.5f);
```

##### Advantages
* Local control [(?)](Glossary.md#local-control)

##### Disadvantages
* Curvature isn't continuous [(?)](Glossary.md#continuous-curvature). Not a problem unless you know you need it.
* There must be a nonzero distance between each adjacent pair of points
* Non-looping version requires an "extra" point on either end of the data set which will not be interpolated
* 曲率不是连续的[(?)](Glossary.md#continuous-curvature)；
* 每对相邻点之间的距离必须为非零；
* 非环的数据集两端需要一个“额外”点，该点不会被内插。

### Uniform Cubic B-Spline
The B-Spline (Basis Spline) is very similar in concept to the Bezier Curve, and the Uniform Cubic B-Spline is a specific type of B-Spline.

B-Spline样条在概念上和Bezier曲线的非常相似，均匀三次B样条曲线是B样条曲线的一种特殊类型。

It is possible to create B-Splines with arbitrary powers (as opposed to only cubic) but enforcing cubic allows for much simpler formulas and better performance. See [Generic B-Spline](#gener-c-b-spline) below.

理论上可以创建具有任意次的B样条曲线（而不是三次方），但是三次方可以简化公式并提高性能。 请参见下面的 [Generic B-Spline](#gener-c-b-spline) 。

To use, import the appropriate header:
`#include "spline_library/splines/uniform_cubic_bspline.h"`

Create a Uniform Cubic B-Spline by passing a std::vector<T> to the constructor, containing a list of control points:
```c++
std::vector<QVector2D> splinePoints = ...;
UniformCubicBSpline<QVector2D> mySpline(splinePoints);
QVector2D interpolatedPosition = mySpline.getPosition(0.5f);
```

##### Advantages
* Local control [(?)](Glossary.md#local-control)
* Curvature is continuous [(?)](Glossary.md#continuous-curvature)

* 可以进行局部控制
* 曲率连续

##### Disadvantages
* The interpolated line does not necessarily pass through the specified points
* Non-looping variation requires an "extra" point on either end of the data set which will not be interpolated
* 曲线不需要经过输入点
* 非环变化要求在数据集的任一端都需要一个“额外”点，该点不会被内插

### Natural Spline
The Natural Spline computes the curvature for each point, based on the position of every point, then interpolates the spline based on the list of points and the corresponding list of curvatures.

Natural Spline样条线根据每个点的位置计算每个点的曲率，然后根据点列表和相应的曲率列表对样条曲线进行插值。

To use, import the appropriate header:
`#include "spline_library/splines/natural_spline.h"`

Create a Natural Spline by passing a std::vector<T> to the constructor, containing a list of points to interpolate through:
```c++
std::vector<QVector2D> splinePoints = ...;
NaturalSpline<QVector2D> mySpline(splinePoints);
QVector2D interpolatedPosition = mySpline.getPosition(0.5f);
```

##### Advantages
* Curvature is continuous [(?)](Glossary.md#continuous-curvature)

##### Disadvantages
* No local control [(?)](Glossary.md#local-control)

Advanced Types（高级类型）
-------------
If one of the simple types above doesn't meet your needs, the following types are also available. These spline types are more powerful, but also more difficult to use correctly, and/or carry important caveats that may not be immediately obvious.

如果上述简单类型之一不能满足你的需求，则还可以使用以下类型。 这些样条曲线类型功能更强大，但也更难于正确使用，并且/或者带有可能不立即显而易见的重要警告。

### Centripetal Catmull-Rom Spline
The Centripetal CR Spline is a variation of the Catmull-Rom Spline formula. Instead of spacing each point exactly one T apart, the distance in T between any two points will be proportional to the square root of distance between the two points. Thus, points that are very far apart will be further apart in T than points that are close together.

Centripetal CR Spline样条线是Catmull-Rom样条线公式的变体。 而不是将每个点精确地隔开一个T，而是将任意两个点之间的T距离与两个点之间的距离的平方根成比例。 因此，相距很远的点在T上的距离比相距很近的点还要远。

To use, import the appropriate header:
`#include "spline_library/splines/cubic_hermite_spline.h"`

Then provide a value for the optional `alpha` parameter in the `CubicHermiteSpline` constructor. A value of 0.5 will produce a centripetal Catmull-Rom Spline.
```c++
std::vector<QVector2D> splinePoints = ...;
CubicHermiteSpline<QVector2D> mySpline(splinePoints, **0.5f**);
```

Other values for `alpha` are allowed too - a value of 1.0 will result in a "chordal" variation, and the formula will work with any number, negative or positive. Values other than 0.0 or 0.5 should be very rare, however. (Side note: 'alpha' is a parameter on nearly every spline type. It has the same effect on other spline types as it does on CubicHermiteSpline)

Providing a value of 0.0 will create a standard Catmull-Rom spline, identical to that created by `UniformCRSpline` -- CubicHermiteSpline is more powerful and flexible, but that comes at a performance and memory cost.

It has been proven mathematically that on Catmull-Rom Splines, the centripetal variation avoids certain types of self-intersections, cusps, and overshoots, producing a more aesthetically pleasing spline.

##### Advantages (compared to UniformCRSpline)
* Proven to avoid self-intersections and overshoots when there are large variations in distance between adjacent points.

##### Disadvantages (compared to UniformCRSpline)
* Modifies T values of points - points that are close together will have a smaller T distance and vice versa. This may be a problem if the points are keyframes for an animation, for example, or any other data series where the T values have some external meaning
* Slower than UniformCRSpline, due to more to calculate and more data competing for cache space.

### Generic B-Spline
The B-Spline (Basis Spline) is very similar in concept to the Bezier Curve, but you can control the spline degree easily rather than it being determined by the number of input points.

Creating a GenericBSpline with degree 3 is intended to be 100% interchangeable with CubicBSpline, although the CubicBSpline will be much faster.

To use, import the appropriate header:
`#include "spline_library/splines/generic_b_spline.h"`

Create a Generic B-Spline by passing a std::vector<T> to the constructor, containing a list of control points, as well as the desired degree:
```c++
std::vector<QVector2D> splinePoints = ...;
GenericBSpline<QVector2D> mySpline(splinePoints, 4);
```

The degree must be less than the number of input points, and must be at least 1.

##### Advantages
* Local control [(?)](Glossary.md#local-control)
* Curvature is continuous if degree is >= 3 [(?)](Glossary.md#continuous-curvature)
* Wiggle is continuous if degree is >= 4 [(?)](Glossary.md#continuous-curvature)

##### Disadvantages
* The interpolated line does not necessarily pass through the specified points
* Non-looping variation requires an "extra" point on either end of the data set which will not be interpolated
* Much slower than CubicBSpline 

### Cubic Hermite Spline
The Cubic Hermite Spline takes a list of points, and a corresponding list of tangents for each point. The Catmull-Rom Spline is a special type of the Cubic Hermite Spline which automatically computes the tangents, whereas this type expects the user to supply them.

An example use case for this spline type is for physical simulation time series data, where spline->getPosition(t) returns the object's position at time T. If you know the object's velocity in addition to its position, you can make the interpolation more accurate by providing that velocity as the tangent.

To use, import the appropriate header:
`#include "spline_library/splines/cubic_hermite_spline.h"`

Create a Cubic Hermite Spline by passing two equal-length std::vector<T> to the constructor, one containing a list of points to interpolate through, and the other containing the corresponding tangent for each point:
```c++
std::vector<QVector2D> splinePoints = ...;
std::vector<QVector2D> splineTangents = ...;
CubicHermiteSpline<QVector2D> mySpline(splinePoints, splineTangents);
```

##### Advantages
* Local control [(?)](Glossary.md#local-control)
* Easily control the tangent at each point

##### Disadvantages
* Curvature isn't continuous [(?)](Glossary.md#continuous-curvature). Not a problem unless you know you need it.
* Cannot be used if you don't know the desired tangent for each point

### Quintic Hermite Spline
The Quintic Hermite Spline takes a list of points, a corresponding list of tangents for each point, and a corresponding list of curvatures for each point.

An example use case for this spline type is for physical simulation time series data, where spline->getPosition(t) returns the object's position at time T. If you know the object's velocity and acceleration in addition and position, you can make the interpolation more accurate by providing the velocity as the tangent, and acceleration as the curvature.

To use, import the appropriate header:
`#include "spline_library/splines/quintic_hermite_spline.h"`

Create a Quintic Hermite Spline by passing three equal-length std::vector<T> to the constructor, one containing a list of points to interpolate through, another containing the corresponding tangent for each point, and a third containing the corresponding curvature for each point:
```c++
std::vector<QVector2D> splinePoints = ...;
std::vector<QVector2D> splineTangents = ...;
std::vector<QVector2D> splineCurvatures = ...;
CubicHermiteSpline<QVector2D> mySpline(splinePoints, splineTangents, splineCurvatures);
```

##### Advantages
* Local control [(?)](Glossary.md#local-control)
* Easily control the tangent and curvature at each point
* Curvature is continuous [(?)](Glossary.md#continuous-curvature)

##### Disadvantages
* Cannot be used if you don't know the desired tangent and curvature for each point
* More computationally intensive than the cubic version
* More "wiggly" than the cubic version. This sounds vague, but it's actually quantifiable: For the cubic version, the derivative of curvature is constant, but for the quintic version, the derivative of curvature is a quadractic function.
