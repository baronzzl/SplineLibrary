#ifndef GENERIC_B_SPLINE
#define GENERIC_B_SPLINE


#include "spline_library/spline.h"
#include "spline_library/utils/spline_setup.h"

#include <unordered_map>

template<class InterpolationType, typename floating_t=float>
class GenericBSpline final : public Spline<InterpolationType, floating_t>
{
//constructors
public:
    GenericBSpline(const std::vector<InterpolationType> &points, int degree);

//methods
public:
    InterpolationType getPosition(floating_t x) const override;
    typename Spline<InterpolationType,floating_t>::InterpolatedPT getTangent(floating_t x) const override;
    typename Spline<InterpolationType,floating_t>::InterpolatedPTC getCurvature(floating_t x) const override;
    typename Spline<InterpolationType,floating_t>::InterpolatedPTCW getWiggle(floating_t x) const override;

    floating_t getT(int index) const override;
    floating_t getMaxT(void) const override;

    bool isLooping(void) const override { return false; }

//methods
private:
    InterpolationType computeDeboor(size_t knotIndex, int degree, float globalT) const;
    InterpolationType computeDeboorDerivative(size_t knotIndex, int degree, float globalT, int derivativeLevel) const;

//data
private:
    std::vector<InterpolationType> positions;
    std::vector<floating_t> knots;

    floating_t maxT;

    int splineDegree;

    //map from index to t value. it's a map and not an array so we can store negative indexes
    std::unordered_map<int,floating_t> indexToT;
};

template<class InterpolationType, typename floating_t>
GenericBSpline<InterpolationType,floating_t>::GenericBSpline(const std::vector<InterpolationType> &points, int degree)
    :Spline<InterpolationType,floating_t>(points), splineDegree(degree)
{
    assert(points.size() > splineDegree);

    int size = points.size();
    int padding = degree - 1;

    //compute the T values for each point
    indexToT = SplineSetup::computeBSplineKnots(points, 0.0f, padding);
    maxT = indexToT[size - degree];

    //we need enough space to repeat the first and last elements (degree - 1) times
    positions = points;
    knots = std::vector<floating_t>(indexToT.size());

    for(int i = -padding; i < size + padding; i++)
    {
        knots[i + padding] = indexToT[i];
    }
}

template<class InterpolationType, typename floating_t>
InterpolationType GenericBSpline<InterpolationType,floating_t>::getPosition(floating_t globalT) const
{
    size_t startIndex = SplineSetup::getIndexForT(knots, globalT);
    return computeDeboor(startIndex + 1, splineDegree, globalT);
}



template<class InterpolationType, typename floating_t>
typename Spline<InterpolationType,floating_t>::InterpolatedPT
    GenericBSpline<InterpolationType,floating_t>::getTangent(floating_t globalT) const
{
    size_t startIndex = SplineSetup::getIndexForT(knots, globalT);

    return typename Spline<InterpolationType,floating_t>::InterpolatedPT(
                computeDeboor(startIndex + 1, splineDegree, globalT),
                computeDeboorDerivative(startIndex + 1, splineDegree, globalT, 1)
                );
}

template<class InterpolationType, typename floating_t>
typename Spline<InterpolationType,floating_t>::InterpolatedPTC
    GenericBSpline<InterpolationType,floating_t>::getCurvature(floating_t globalT) const
{
    size_t startIndex = SplineSetup::getIndexForT(knots, globalT);

    return typename Spline<InterpolationType,floating_t>::InterpolatedPTC(
                computeDeboor(startIndex + 1, splineDegree, globalT),
                computeDeboorDerivative(startIndex + 1, splineDegree, globalT, 1),
                computeDeboorDerivative(startIndex + 1, splineDegree, globalT, 2)
                );
}

template<class InterpolationType, typename floating_t>
typename Spline<InterpolationType,floating_t>::InterpolatedPTCW
    GenericBSpline<InterpolationType,floating_t>::getWiggle(floating_t globalT) const
{
    size_t startIndex = SplineSetup::getIndexForT(knots, globalT);

    return typename Spline<InterpolationType,floating_t>::InterpolatedPTCW(
                computeDeboor(startIndex + 1, splineDegree, globalT),
                computeDeboorDerivative(startIndex + 1, splineDegree, globalT, 1),
                computeDeboorDerivative(startIndex + 1, splineDegree, globalT, 2),
                computeDeboorDerivative(startIndex + 1, splineDegree, globalT, 3)
                );
}

template<class InterpolationType, typename floating_t>
InterpolationType GenericBSpline<InterpolationType,floating_t>::computeDeboor(size_t knotIndex, int degree, float globalT) const
{
    if(degree == 0)
    {
        return positions[knotIndex];
    }
    else
    {
        floating_t alpha = (globalT - knots[knotIndex - 1]) / (knots[knotIndex + splineDegree - degree] - knots[knotIndex - 1]);

        InterpolationType leftRecursive = computeDeboor(knotIndex - 1, degree - 1, globalT);
        InterpolationType rightRecursive = computeDeboor(knotIndex, degree - 1, globalT);

        InterpolationType blended = leftRecursive * (1 - alpha) + rightRecursive * alpha;

        return blended;
    }
}

template<class InterpolationType, typename floating_t>
InterpolationType GenericBSpline<InterpolationType,floating_t>::computeDeboorDerivative(size_t knotIndex, int degree, float globalT, int derivativeLevel) const
{
    if(degree == 0)
    {
        return InterpolationType();
    }
    else
    {
        float multiplier = degree / (knots[knotIndex + splineDegree - degree] - knots[knotIndex - 1]);

        if(derivativeLevel <= 1)
        {
            //once we reach this point we don't want to compute the derivative anymore
            return multiplier *
                    (computeDeboor(knotIndex, degree - 1, globalT)
                   - computeDeboor(knotIndex - 1, degree - 1, globalT)
                     );
        }
        else
        {
            //recursively call the derivative function to compute a higher derivative
            return multiplier *
                    (computeDeboorDerivative(knotIndex, degree - 1, globalT, derivativeLevel - 1)
                   - computeDeboorDerivative(knotIndex - 1, degree - 1, globalT, derivativeLevel - 1)
                     );
        }
    }
}

template<class InterpolationType, typename floating_t>
floating_t GenericBSpline<InterpolationType,floating_t>::getT(int index) const
{
    return indexToT.at(index);
}

template<class InterpolationType, typename floating_t>
floating_t GenericBSpline<InterpolationType,floating_t>::getMaxT(void) const
{
    return maxT;
}

#endif // GENERIC_B_SPLINE

