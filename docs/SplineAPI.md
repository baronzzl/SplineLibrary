Spline API
=============

The `Spline` class is an abstract class that serves as the base class of all spline types. This file documents its public methods.

The Spline class (and all subclasses) is a template class. It takes two template arguments: The first is the type of data to be interpolated (IE, a 2d vector, or a 3d vector). It expects the vector to have an API matching Qt's QVector2D class, but will support data with any dimension. Custom classes may be used as long as they provide the same public methods as QVector2D. Custom classes can ven use double instead of float as the internal data type if more precision is needed.

The second template parameter is the floating point type to use for internal calculations (IE, float, double, some BigDecimal class). This defaults to float and it's safe to leave this a float for most applications, but if you use an InterpolationType custom class that stores its data as doubles, you'll get more precision by telling the Spline to use doubles as well.

#### getPosition(t)
This method computes the interpolated position at T.

The interpolated position is always continuous, IE there are never any "gaps" in the spline.

For looping splines, if the input T is outside of the allowable range (Ie less than zero or larger than the maximum T), the method will perform circular modular arithmetic to bring the T value into range. For example, if the input T value is -1, it will be adjusted to maxT - 1 before interpolation. This underlines the cyclic nature of looping splines.

For non-looping splines, if the input T is outside of the allowable range, the results are undefined.

#### getTangent(t)
This method interpolates the position and derivative of the position (aka the tangent) at T, and returns a struct containing both. While it's possible to compute only the tangent, the API doesn't support it, in the interest of simplifying the API.

The interpolated tangent is always continuous.

The derivative is determined analytically at compile-time, as opposed to numerically at run-time, so it is fast and numerically stable to compute.

The behavior when the T value is out of range is the same as for the getPosition method.

#### getCurvature(t)
This method interpolates the position, the first derivative of position (AKA the tangent), the second derivative of position (AKA the curvature), and returns a struct containing all three. While it's possible to compute only the curvature, the API doesn't support it, in the interest of simplifying the API.

The interpolated curvature is sometimes continuous - see the [Spline Types](SplineTypes.md) entry for your chosen spline type to find out whether or not its curvature is continuous.

The derivatives are determined analytically at compile-time, as opposed to numerically at run-time, so they are fast and numerically stable to compute.

The behavior when the T value is out of range is the same as for the getPosition method.

#### getWiggle(t) const
This method interpolates the position, the first derivative of position (AKA the tangent), the second derivative of position (AKA the curvature), the third derivative of position (AKA the wiggle), and returns a struct containing all four. While it's possible to compute only the wiggle, the API doesn't support it, in the interest of simplifying the API.

此方法对位置，位置的一阶导数（正切线），位置的二阶导数（曲率），位置的**三阶导数（摆动）**进行插值，并返回包含所有四个的结构。 尽管可以只计算摆动，但是为了简化API，API不支持只返回摆动。

For all current spline types, the wiggle is never continuous from segment to segment. For cubic splines, it is always a constant within each segment, although it may change from segment to segment.

对于所有当前的样条曲线类型，摆动在每个段之间都不是连续的。 对于三次样条，尽管每个段之间可能会有所变化，但在每个段内它始终是常数。

The derivatives are determined analytically at compile-time, as opposed to numerically at run-time, so they are fast and numerically stable to compute.

导数是在编译时通过解析确定的，而不是在运行时通过数值确定的，因此它们可以快速且数值稳定地进行计算。

The behavior when the T value is out of range is the same as for the getPosition method.

T值超出范围时的行为与getPosition方法相同。

#### arcLength(a, b) const
This method computes the arc length between a and b. IE, if you traceda path with your finger along the spline from a to b, how much distance would it cover?

此方法计算a和b之间的弧长。 即如果你用手指沿样条从a到b画过的路径的长度？

This is found by numerically computing the integral of the magnitude of the tangent. In real world terms, it computes the tangent at several points between a and b and then combines the results.

通过数值计算切线大小的积分可以发现这一点。 用现实的术语来说，它计算a和b之间几个点的切线，然后将结果合并。

For looping splines, it will use modular arithmetic to ensure that a and b are less than one "circuit" away from each other. Notably, this means that `arcLength(0, maxT)` will return 0 for looping splines, because it detects that 0 to maxT is a complete circuit and removes it. If you want to compute the length of the whole spline, use `totalLength()` instead.

对于环形样条，它将使用模块化算法来确保a和b彼此之间的距离小于一个环。 值得注意的是，这意味着`arcLength（0，maxT）`对于环形样条将返回0，因为它检测到maxT的0是完整环并将其删除。 如果要计算整个样条线的长度，请改用`totalLength（）`。

#### totalLength() const
This method computes the arc length of the entire spline, from beginning to end. IE, if you traceda path with your finger along the spline from start to end, how much distance would it cover?

This is found by numerically computing the integral of the magnitude of the tangent. In real world terms, it computes the tangent at several points between the start and end and then combines the results.

For computing the total length of non-looping splines, calling `totalLength()` is preferred over calling `arcLength(0, maxT)` because it's slightly faster.

#### getMaxT() const
This method returns the largest in-range T value.

All spline types normalize T values so that the maximum T value is equal to the index of the corresponding point.
* For non-looping splines, the maximum T depends on the spline type, but is generally equal to the number of points that were actually used, minus one. For example, a non-looping Catmull-Rom spline uses `size - 2` points internally - the other two are used only to calculate tangents. In this case, the maximum T value will be `size - 3`.
* For looping splines, the maximum T is always equal to the size of the input vector of points.

The minimum T value is always 0.

#### isLooping() const
Returns true if this spline is a looping spline, and false if this is a non-looping spline.

#### segmentCount() const
Returns the number of segments in the spline. As indicated in the [glossary](Glossary.md), most splines are piecewise functions. Internally, this library refers to these pieces as "segments".

#### segmentT(size_t index) const
Return the T value for the beginning of the specified segment index. Index should be less than segmentCount()
