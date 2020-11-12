This file outlines some utility classes for interacting with splines.

Spline Inverter
=============
The Spline Inverter object is used to answer the question "Given a data point (called the Query Point), what t value brings the spline closest to that point?" This can be used for query points which are on the spline itself, but it can also be used for points that are near the spline, or even for query points that are far away.

Spline Inverter对象用于回答以下问题：“给定一个数据点（称为查询点），哪个t值使样条曲线最接近该点？” 它可用于样条曲线本身上的查询点，但也可用于样条曲线附近的点，甚至用于较远的查询点。

An example use case is an AI system for a race track. The spline could present the optimal line through a curve in the track, and the AI system could use its current position as a query point to the Spline Inverter to get the closest point on the spline. This will tell the AI where it should be via the interpolated position, what direction it should be going via the interpolated tangent tangent, if it should be turning via the interpolated curvature, etc.

一个示例用例是用于赛道的AI系统。 样条曲线可以通过轨迹中的曲线呈现最佳线，并且AI系统可以使用其当前位置作为样条Inverter的查询点，以获取样条曲线上的最近点。 这将告诉AI应该通过插值位置在哪里，应该通过插值切线切线走向什么方向，是否应该通过插值曲率来旋转等。

This must be computed numerically - there is no reasonable analytic solution like there is for derivatives.

这只能通过数值方式计算-没有像导数一样的解析解。

To create a Spline Inverter, import `spline_library/utils/splineinverter.h`, and create a new `SplineInverter` object by passing a reference to a Spline to the constructor.
```c++
std::vector<QVector2D> splinePoints = ...;
UniformCRSpline<QVector2D> mySpline(splinePoints);
SplineInverter<QVector2D> inverter(mySpline);
```

Alternatively, if you have a shared_ptr to a Spline, you can do the following:
```c++
std::shared_ptr<Spline<QVector2D>> mySplinePtr = ...;
SplineInverter<QVector2D> inverter(*mySplinePtr);
```

The SplineInverter stores a reference to the spline, so it should not live longer than the spline it refers to.

In the SplineInverter constructor, it takes "samples" of the spline at regular intervals. By default it takes 10 samples per T, but this can be changed via a constructor parameter. When given a query point, it first finds the closest sample to the query point, then uses that sample location as the starting point for a refining algorithm.

SplineInverter储存了对样条线的引用，因此它的生存期不应长于所引用的样条线。

在SplineInverter构造函数中，它以固定间隔获取样条的“样本”。 默认情况下，每个T采样10个样本，但这可以通过构造函数参数进行更改。 给定查询点时，它首先找到与查询点最接近的样本，然后使用该样本位置作为优化算法的起点。

### findClosestT(queryPoint) const
This method finds the closest sample to the query point, and uses that closest sample as a starting point for [Brent's Method](http://en.wikipedia.org/wiki/Brent%27s_method).

Example:
```c++
SplineInverter<QVector2D> inverter = ...;
float t = inverter.findClosestT(QVector2D(5, 1));
```


Arc Length Solver
=============
The arc length solver methods, found in `spline_library/utils/arclength.h` all deal with a similar question: Given a starting t value on the spline and a desired arc length, what secondary T value will yield my desired arc length? All methods listed here will accept any spline type. They will accept references to the parent Spline class, but they're all template functions on spline type, so it's possible to avoid virtual function calls by passing in a reference to a concrete spline type.

在`spline_library / utils / arclength.h`中找到的弧长求解器方法都处理类似的问题：给定样条曲线上的起始t值和所需的弧长，计算其终点t。 该文件中列出的所有方法将接受任何样条线类型。 它们将接受对父样条线类的引用，但是它们都是样条线类型上的所有模板函数，因此可以通过传递对具体样条线类型的引用来避免虚拟函数调用。

### ArcLength::solveLength(const spline&, a, desiredLength)
Given a spline, a starting T value on the spline `a`, and a desired arc length `desiredLength`, compute and return `b` that satisfies the condition `spline.arcLength(a, b) ~= desiredLength`

If the desired length goes past the end of the spline (IE if b would be greater than maxT), maxT is returned.

Example:
```c++
std::vector<QVector2D> splinePoints = ...;
UniformCRSpline<QVector2D> mySpline(splinePoints);

float a = 1.2f;
float desiredArcLength = 1.5f;

float b = ArcLength::solveLength(mySpline, a, desiredArcLength);
```


### ArcLength::partition(const spline&, desiredLength)
Given a spline and a desired arc length `desiredLength`, partition the spline into as many pieces as possible, such that each piece has arc length `desiredLength`. Returns a `std::vector` of T values marking the beginning/end of each partitioned piece.

IE, if it returns `std::vector<float>{0.0, 1.2, 2.6}` this signifies that the spline was partitioned into two pieces, with the first starting at T = 0 and ending at T = 1.2, and the second beginning at T = 1.2 and ending at T = 2.6.

If `desiredLength` does not cleanly divide the spline's total arc length (it almost certainly won't due to floating point inaccuracy), the "remainder" is not included in the list, so the final value will be the T value at the end of the final full-length piece. If you want a guarantee that the entire spline is partitioned and there is no remainder, see `ArcLength::partitionN` below.

Example:
```c++
std::vector<QVector2D> splinePoints = ...;
UniformCRSpline<QVector2D> mySpline(splinePoints);

float desiredArcLength = 1.5f;

std::vector<float> partitionBoundaries = ArcLength::partition(mySpline, desiredArcLength);
```


### ArcLength::partitionN(const spline&, n)
Given a spline and a number pf pieces `n`, divide the spline into `n` pieces such that each piece has the same arc length. Returns a `std::vector` containing n+1 T values marking the beginning/end of each partitoned piece.

给定一个样条曲线和pf个“ n”个片段，将样条曲线分成“ n”个片段，以使每个片段具有相同的弧长。 返回一个`std :: vector`，其中包含n + 1个T值，这些值标记了每个分块的开始/结束。

The first element in the returned vector is always 0, and the last element is always maxT.

Example:
```c++
std::vector<QVector2D> splinePoints = ...;
UniformCRSpline<QVector2D> mySpline(splinePoints);

size_t n = 6;

std::vector<float> partitionBoundaries = ArcLength::partitionN(mySpline, n);
```
