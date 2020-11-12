Glossary（专业词汇）
=============

This page contains definitions for terms that are commonly used in the documentation.

### Spline（样条曲线）
A spline is a piecewise function used to smoothly interpolate through or approximate a data set. A spline takes an input value named `t` and returns an interpolated value. The input t values typically correlate with the original data set, IE an input t value equal to n will typically return the n-th input point, and an input t value of n + 0.5 will return a value somewhere in the vicinity of the mid point between the n-th point and the (n+1)-th point.

spline曲线是一种分段函数，用于平滑地插值或近似数据集。 样条曲线采用名为`t`的输入值，并返回插值值。 输入的t值通常与原始数据集相关，即，输入`t`值等于n将返回第n个输入点，输入t值n + 0.5将返回第n个点和第（n + 1）个点中间点附近的某个点。

A core goal of a spline is not just interpolation, but smooth interpolation. This means that, rather than the spline sharply changing direction, it will curve around from one point to the next.

spline曲线的核心目标不仅是插值，还包括平滑插值。 这意味着它会从一个点到另一个点平滑过渡，而不是样条曲线急剧变化。

### Piecewise（分段）
A spline is said to be piecewise because it is not composed of one single mathematical formula. Instead, we say "if the input t is between 0 and 1, use one formula. if the input t is between 1 and 2, use a different formula. if the input t is between 2 and 3, use a still different formula" and so on for every set of input points. The differences in spline types come from how they construct each of these formulas.

样条线是分段的，因为它不是由一个单一的数学公式组成。 比如：如果输入`t`在0到1之间，使用一个公式描述。如果输入`t`在1到2之间，使用另外一个公式，依此类推，即不同段对应不同的公式。 样条曲线有多种类型，它们的差异来自于它们如何构造每个公式。

### Derivative（导数）
The derivative comes from calculus - it describes the rate of change of a function. It answers the question "Given a mathematical formula, how does that formula's output change as the input changes?"

微分来自微积分-它描述函数的变化率。 它回答了一个问题：“给出一个数学公式，该公式的输出如何随着输入的变化而变化？”

In the context of a spline, the first derivative answers the question "in what direction is the spline moving at `t`?". We call this the "tangent".

样条曲线的一阶导数回答了以下问题：“样条曲线在`t`处向哪个方向移动？”。 我们称其为**切线**。

We can also take the derivative of a derivative. This a "second derivative" of the original function. It describes the rate of change of the first derivative. In the context of a spline, this answers the questions "Is the spline speeding up or slowing down at `t`? Is it turning left or turning right?" and is called the "curvature".

我们也可以取导数的导数即“二阶导数”。 它描述了一阶导数的变化率。 在样条曲线中二阶导数表示：“在t处，样条是加速还是减速？它是向左转还是向右转？” 称为**曲率**。

### Continuous（连续性）
In layman's terms, a function is said to be "continuous" if there are no "breaks" or "jumps" in the function. It is always possible to trace a path with our finger from the start of a spline to the end of a spline without lifting our finger.

用外行的话说，如果该函数中没有“中断”或“跳跃”，则该函数称为“连续”。 始终可以用手指从样条线的起点到样条线的终点跟踪路径，而无需抬起手指。

Derivatives of a function can be continuous or non-continous as well. We can visually check if a spline's derivative is continuous in the demo. If a spline's derivative wasn't continuous, the spline would appear to sharply change direction, rather than forming a smooth curve.

函数的导数也可以是连续的或不连续的。 我们可以在演示中直观地检查样条的导数是否连续。 如果样条曲线的导数不连续，则样条曲线似乎会急剧改变方向，而不是形成平滑曲线。

As said before, a core goal of a spline is not just interpolation, but smooth interpolation. In order to meat this goal, a spline must be continuous, and it must have a continuous first derivative.

如前所述，样条曲线的核心目标不仅在于插值，还在于平滑插值。 为了达到这个目标，样条必须是连续的，并且必须具有连续的一阶导数。

### Continuous Curvature（连续曲率）
Second derivatives can also be continuous or non-continous, but this is not a priority for many spline types. It is much harder to visually verify that a spline's curvature is continuous, but it can be a useful property for some applications, so the [Spline Types](SplineTypes.md) page lists whether or not the curvature is continuous for each type.

二阶导数也可以是连续的或不连续的，但这并不是许多样条曲线的优先级。 肉眼很难观察样条曲线的曲率是否连续，但是对于某些应用来说这可能是有用的属性，因此[Spline Types](SplineTypes.md)中列出了每种类型的曲率是否连续。

### Local Control（局部控制）
Local control is an optional property of splines that is very desirable for many applications. For a spline with local control, moving a data point will only affect the spline segments close to that point, leaving all others unaffected. This can be seen in the demo when looking at a `CRSpline`, which has local control: If we move one data point, two segments to the left and two segments to the right will move, but all other parts of the spline will remain untouched.

局部控制是样条的可选属性，对于许多应用来说都有这个需求。 对于具有局部控制的样条线，移动数据点只会影响靠近该点的样条线段，而其他所有点均不受影响。 这可以在演示`CRSpline`中看到：如果我们移动一个数据点，则左移两段，右移两段，但样条线的所有其他部分不会发生任何变化。

Conversely, the `NaturalSpline` is an example of a spline that does not have local control. If we move a data point in the demo, every single spline segment is affected, rather than just the ones near the data point.

相反，`NaturalSpline`样条则不能进行局部控制，即移动某个数据点，则每个样条线段都会受到影响，而不仅仅是数据点附近的那个样条线段会受到影响。

It is interesting to point out here that the Natural Spline has a continuous curvature, while the Catmull-Rom Spline does not, this highlighting the fact that choosing a spline type is about balancing tradeoffs between different features of each type. 

有趣的是，Natural Spline 样条曲线具有连续的曲率，而Catmull-Rom样条曲线则没有，这表明了选择样条曲线类型是要在每种类型的不同特征之间权衡取舍。