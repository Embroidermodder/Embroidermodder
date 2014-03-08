#include "emb-outline.h"
#include "emb-pattern.h"

#ifdef ARDUINO /* ARDUINO TODO: remove this line when emb-outline is C89 complete. This is a temporary arduino build fix. */
#else          /* ARDUINO TODO: remove this line when emb-outline is C89 complete. This is a temporary arduino build fix. */

struct StitchBlock
{
    IThread Thread { get; set; }
    double Angle { get; set; }
    List<VectorStitch> Stitches { get; set; }
}

double LineLength(EmbPoint a1, EmbPoint a2)
{
    return sqrt(pow(a2.X - a1.X, 2) + pow(a2.Y - a1.Y, 2));
}

double DistancePointPoint(Vector2 p, Vector2 p2)
{
    double dx = p.X - p2.X;
    double dy = p.Y - p2.X;
    return sqrt(dx * dx + dy * dy);
}

float GetRelativeX(EmbPoint a1, EmbPoint a2, Point a3)
{
    return ((a1.X - a2.X) * (a3.X - a2.X) + (a1.Y - a2.Y) * (a3.Y - a2.Y));
}

float GetRelativeY(EmbPoint a1, EmbPoint a2, EmbPoint a3)
{
    return ((a1.X - a2.X) * (a3.Y - a2.Y) - (a1.Y - a2.Y) * (a3.X - a2.X));
}

double GetAngle(VectorStitch vs, VectorStitch vs2)
{
    return atan((vs2.Xy.X - vs.Xy.X)/(vs2.Xy.Y - vs.Xy.Y));
}

StitchBlock* BreakIntoColorBlocks(EmbPattern pattern)
{
    var sa2 = new StitchBlock();
    int oldColor = pattern.StitchList[0].ColorIndex;
    var color = pattern.ColorList[oldColor];
    sa2.Thread = new Thread(color.Red, color.Blue, color.Green);
    foreach (Stitch s in pattern.stitchList)
    {
        if (s.ColorIndex != oldColor)
        {
            yield return sa2;
            sa2 = new StitchBlock();
            color = pattern.ColorList[s.ColorIndex];
            sa2.Thread = new Thread(color.Red, color.Blue, color.Green);
            oldColor = s.ColorIndex;
        }
        var vs = new VectorStitch { Xy = new Point(s.X, s.Y), Color = s.ColorIndex };
        sa2.Stitches.Add(vs);
    }
    yield return sa2;
}



StitchBlock * BreakIntoSeparateObjects(EmbStitchBlock* blocks)
{
    double previousAngle = 0.0;
    foreach (var block in blocks)
    {
        var stitches = new List<VectorStitch>();
        block.Stitches[0].Type = VectorStitchType.Contour;
        block.Stitches[block.Stitches.Count - 1].Type = VectorStitchType.Contour;

        for (int i = 0; i < block.Stitches.Count - 2; i++) // step 0
        {
            double dx = (GetRelativeX(block.Stitches[i].Xy, block.Stitches[i + 1].Xy, block.Stitches[i + 2].Xy));
            block.Stitches[i + 1].Type = dx <= 0 ? VectorStitchType.Run : VectorStitchType.Contour;
            block.Stitches[i].Angle = GetAngle(block.Stitches[i], block.Stitches[i + 1]);
            stitches.Add(block.Stitches[i].Clone());
            if (i > 0)
            {
                if ((block.Stitches[i].Type == VectorStitchType.Contour) && Math.Abs(block.Stitches[i].Angle - previousAngle) > (20/180*Math.PI))
                {
                    yield return
                        new StitchBlock
                            {
                                Stitches = stitches,
                                Angle = stitches.Average(x => x.Angle),
                                Thread = new Thread(block.Thread.Red, block.Thread.Blue, block.Thread.Green)
                            };
                    stitches = new List<VectorStitch>();

                }
            }
        }

        //for (int i = 1; i < sa.Stitches.Count - 3; i++) // step 1
        //{
        //    if (sa.Stitches[i + 1].Type == VectorStitchType.Contour)
        //    {
        //        //float dy = GetRelativeY(sa[i + 1].XY, sa[i + 2].XY, sa[i + 3].XY);
        //        //float dy2 = GetRelativeY(sa[i].XY, sa[i + 1].XY, sa[i + 2].XY);
        //        //float dy3 = GetRelativeY(sa[i + 2].XY, sa[i + 3].XY, sa[i + 4].XY);
        //        //if(dy)
        //        if (sa.Stitches[i - 1].Type == VectorStitchType.Run || sa.Stitches[i + 1].Type == VectorStitchType.Run)
        //        {
        //            sa.Stitches[i].Type = VectorStitchType.Tatami;
        //        }
        //        else
        //        {
        //            sa.Stitches[i].Type = VectorStitchType.Satin;
        //        }
        //    }
        //}
    }
}

StitchObject * FindOutline(EmbStitchBlock* stitchData)
{
    int currColorIndex = 0;
    var pOdd = new List<Point>();
    var pEven = new List<Point>();
    foreach (StitchBlock sa in stitchData)
    {
        if (sa.Stitches.Count > 0)
        {
            sa.Stitches[0].Type = VectorStitchType.Contour;
            sa.Stitches[sa.Stitches.Count - 1].Type = VectorStitchType.Contour;
            for (int i = 0; i < sa.Stitches.Count - 2; i++) // step 0
            {
                float dx = (GetRelativeX(sa.Stitches[i].Xy, sa.Stitches[i + 1].Xy, sa.Stitches[i + 2].Xy));
                sa.Stitches[i + 1].Type = dx <= 0 ? VectorStitchType.Run : VectorStitchType.Contour;
                sa.Stitches[i].Angle = GetAngle(sa.Stitches[i], sa.Stitches[i + 1]);
            }
            //for (int i = 1; i < sa.Stitches.Count - 3; i++) // step 1
            //{
            //    if (sa.Stitches[i + 1].Type == VectorStitchType.Contour)
            //    {
            //        //float dy = GetRelativeY(sa[i + 1].XY, sa[i + 2].XY, sa[i + 3].XY);
            //        //float dy2 = GetRelativeY(sa[i].XY, sa[i + 1].XY, sa[i + 2].XY);
            //        //float dy3 = GetRelativeY(sa[i + 2].XY, sa[i + 3].XY, sa[i + 4].XY);
            //        //if(dy)
            //        if (sa.Stitches[i - 1].Type == VectorStitchType.Run || sa.Stitches[i + 1].Type == VectorStitchType.Run)
            //        {
            //            sa.Stitches[i].Type = VectorStitchType.Tatami;
            //        }
            //        else
            //        {
            //            sa.Stitches[i].Type = VectorStitchType.Satin;
            //        }
            //    }
            //}
        }


        int oddEven = 0;
        foreach (VectorStitch t in sa.Stitches)
        {
            if ((t.Type == VectorStitchType.Contour) && (oddEven % 2) == 0)
            {
                pEven.Add(t.Xy);

                oddEven++;
            }
            else if ((t.Type == VectorStitchType.Contour) && (oddEven % 2) == 1)
            {
                pOdd.Add(t.Xy);
                oddEven++;
            }
        }
        currColorIndex++;
        var so = new StitchObject { SideOne = pEven, SideTwo = pOdd, ColorIndex = currColorIndex };
        yield return so;
        pEven = new List<Point>();
        pOdd = new List<Point>();
        //break;
    }
}

EmbPattern DrawGraphics(EmbPattern p)
{
    var stitchData = BreakIntoColorBlocks(p);


    //var outBlock = new List<StitchBlock>(BreakIntoSeparateObjects(stitchData));
    //foreach(var block in stitchData)
    //{
    //    foreach (var stitch in block.Stitches)
    //    {
    //        if(stitch.Angle != 0)
    //        {
    //            int aaa = 1;
    //        }
    //    }
    //}
    //var xxxxx = outBlock;
    var objectsFound = FindOutline(stitchData);
    var outPattern = new Pattern();
    outPattern.AddColor(new Thread(255, 0, 0, "none", "None"));
    int colorIndex = outPattern.ColorList.Count - 1;
    var r = new Random();
    foreach (StitchObject stitchObject in objectsFound)
    {
        if (stitchObject.SideOne.Count > 1 && stitchObject.SideTwo.Count > 1)
        {
            outPattern.AddColor(new Thread((byte) (r.Next()%256), (byte) (r.Next()%256), (byte) (r.Next()%256),
                                           "none", "None"));
            colorIndex++;
            outPattern.AddStitchRelative(0, 0, StitchTypes.Stop);
            var points = stitchObject.Generate2(75);
            foreach (var point in points)
            {
                outPattern.AddStitchAbsolute(point.X, point.Y, StitchTypes.Normal);
            }
            //break;
            //StitchObject stitchObject = objectsFound[1];))
            //////if (stitchObject.SideOne.Count > 0)
            //////{
            //////    outPattern.StitchList.Add(new Stitch(stitchObject.SideOne[0].X, stitchObject.SideOne[0].Y,
            //////                                         StitchType.Jump, colorIndex));
            //////}
            //////foreach (Point t in stitchObject.SideOne)
            //////{
            //////    outPattern.StitchList.Add(new Stitch(t.X, t.Y,
            //////                                         StitchType.Normal, colorIndex));
            //////}
            //////foreach (Point t in stitchObject.SideTwo)
            //////{
            //////    outPattern.StitchList.Add(new Stitch(t.X, t.Y,
            //////                                         StitchType.Normal, colorIndex));
            //////}
            //break;
        }
    }
    outPattern.AddStitchRelative(0, 0, StitchTypes.End);
    return outPattern;
    //return (SimplifyOutline(outPattern));
}

EmbPattern SimplifyOutline(EmbPattern pattern)
{
    var v = new Vertices();
    v.AddRange(pattern.StitchList.Select(point => new Vector2(point.X, point.Y)));
    var output = SimplifyTools.DouglasPeuckerSimplify(v, 10);
    var patternOut = new Pattern();
    foreach (var color in pattern.ColorList)
    {
        patternOut.AddColor(color);
    }

    foreach (var vertex in output)
    {
        patternOut.AddStitchAbsolute(vertex.X, vertex.Y, StitchTypes.Normal);
    }
    patternOut.AddStitchRelative(0, 0, StitchTypes.End);
    return patternOut;
}

bool[] _usePt;
double _distanceTolerance;

/* Removes all collinear points on the polygon. */
Vertices CollinearSimplify(Vertices vertices, float collinearityTolerance)
{
    /* We can't simplify polygons under 3 vertices */
    if (vertices.Count < 3)
        return vertices;

    var simplified = new Vertices();

    for (int i = 0; i < vertices.Count; i++)
    {
        int prevId = vertices.PreviousIndex(i);
        int nextId = vertices.NextIndex(i);

        Vector2 prev = vertices[prevId];
        Vector2 current = vertices[i];
        Vector2 next = vertices[nextId];

        /* If they collinear, continue */
        if (MathUtils.Collinear(ref prev, ref current, ref next, collinearityTolerance))
            continue;

        simplified.Add(current);
    }

    return simplified;
}

/// <summary>
/// Removes all collinear points on the polygon.
/// Has a default bias of 0
/// </summary>
/// <param name="vertices">The polygon that needs simplification.</param>
/// <returns>A simplified polygon.</returns>
Vertices CollinearSimplify(Vertices vertices)
{
    return CollinearSimplify(vertices, 0);
}

/// Ramer-Douglas-Peucker polygon simplification algorithm. This is the general recursive version that does not use the
/// speed-up technique by using the Melkman convex hull.
/// If you pass in 0, it will remove all collinear points
Vertices DouglasPeuckerSimplify(Vertices vertices, float distanceTolerance)
{
    _distanceTolerance = distanceTolerance;

    _usePt = new bool[vertices.Count];
    for (int i = 0; i < vertices.Count; i++)
    {
        _usePt[i] = true;
    }
    SimplifySection(vertices, 0, vertices.Count - 1);
    var result = new Vertices();
    result.AddRange(vertices.Where((t, i) => _usePt[i]));
    return result;
}

void SimplifySection(Vertices vertices, int i, int j)
{
    if ((i + 1) == j)
        return;

    Vector2 a = vertices[i];
    Vector2 b = vertices[j];
    double maxDistance = -1.0;
    int maxIndex = i;
    for (int k = i + 1; k < j; k++)
    {
        double distance = DistancePointLine(vertices[k], a, b);

        if (distance > maxDistance)
        {
            maxDistance = distance;
            maxIndex = k;
        }
    }
    if (maxDistance <= _distanceTolerance)
    {
        for (int k = i + 1; k < j; k++)
        {
            _usePt[k] = false;
        }
    }
    else
    {
        SimplifySection(vertices, i, maxIndex);
        SimplifySection(vertices, maxIndex, j);
    }
}

double DistancePointLine(EmbPoint p, EmbPoint a, EmbPoint b)
{
    /* if start == end, then use point-to-point distance */
    if (a.X == b.X && a.Y == b.Y)
        return DistancePointPoint(p, a);

    // otherwise use comp.graphics.algorithms Frequently Asked Questions method
    /*(1)     	      AC dot AB
                r =   ---------
                      ||AB||^2

                r has the following meaning:
                r=0 Point = A
                r=1 Point = B
                r<0 Point is on the backward extension of AB
                r>1 Point is on the forward extension of AB
                0<r<1 Point is interior to AB
    */

    double r = ((p.X - a.X) * (b.X - a.X) + (p.Y - a.Y) * (b.Y - a.Y))
               /
               ((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

    if (r <= 0.0) return DistancePointPoint(p, a);
    if (r >= 1.0) return DistancePointPoint(p, b);


    /*(2)
                    (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
                s = -----------------------------
                                Curve^2

                Then the distance from C to Point = |s|*Curve.
    */

    double s = ((a.Y - p.Y) * (b.X - a.X) - (a.X - p.X) * (b.Y - a.Y))
               /
               ((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y));

    return Math.Abs(s) * Math.Sqrt(((b.X - a.X) * (b.X - a.X) + (b.Y - a.Y) * (b.Y - a.Y)));
}

/* From physics2d.net */
public static Vertices ReduceByArea(Vertices vertices, float areaTolerance)
{
    if (vertices.Count <= 3)
        return vertices;

    if (areaTolerance < 0)
    {
        throw new ArgumentOutOfRangeException("areaTolerance", "must be equal to or greater then zero.");
    }

    var result = new Vertices();
    Vector2 v3;
    Vector2 v1 = vertices[vertices.Count - 2];
    Vector2 v2 = vertices[vertices.Count - 1];
    areaTolerance *= 2;
    for (int index = 0; index < vertices.Count; ++index, v2 = v3)
    {
        if (index == vertices.Count - 1)
        {
            if (result.Count == 0)
            {
                throw new ArgumentOutOfRangeException("areaTolerance", "The tolerance is too high!");
            }
            v3 = result[0];
        }
        else
        {
            v3 = vertices[index];
        }
        float old1;
        MathUtils.Cross(ref v1, ref v2, out old1);
        float old2;
        MathUtils.Cross(ref v2, ref v3, out old2);
        float new1;
        MathUtils.Cross(ref v1, ref v3, out new1);
        if (Math.Abs(new1 - (old1 + old2)) > areaTolerance)
        {
            result.Add(v2);
            v1 = v2;
        }
    }
    return result;
}

/* From Eric Jordan's convex decomposition library */
/* Merges all parallel edges in the list of vertices */
public static void MergeParallelEdges(Vertices vertices, float tolerance)
{
    if (vertices.Count <= 3)
        return; /* Can't do anything useful here to a triangle */

    var mergeMe = new bool[vertices.Count];
    int newNVertices = vertices.Count;

    /* Gather points to process */
    for (int i = 0; i < vertices.Count; ++i)
    {
        int lower = (i == 0) ? (vertices.Count - 1) : (i - 1);
        int middle = i;
        int upper = (i == vertices.Count - 1) ? (0) : (i + 1);

        float dx0 = vertices[middle].X - vertices[lower].X;
        float dy0 = vertices[middle].Y - vertices[lower].Y;
        float dx1 = vertices[upper].Y - vertices[middle].X;
        float dy1 = vertices[upper].Y - vertices[middle].Y;
        var norm0 = (float)Math.Sqrt(dx0 * dx0 + dy0 * dy0);
        var norm1 = (float)Math.Sqrt(dx1 * dx1 + dy1 * dy1);

        if (!(norm0 > 0.0f && norm1 > 0.0f) && newNVertices > 3)
        {
            /* Merge identical points */
            mergeMe[i] = true;
            --newNVertices;
        }

        dx0 /= norm0;
        dy0 /= norm0;
        dx1 /= norm1;
        dy1 /= norm1;
        float cross = dx0 * dy1 - dx1 * dy0;
        float dot = dx0 * dx1 + dy0 * dy1;

        if (Math.Abs(cross) < tolerance && dot > 0 && newNVertices > 3)
        {
            mergeMe[i] = true;
            --newNVertices;
        }
        else
            mergeMe[i] = false;
    }

    if (newNVertices == vertices.Count || newNVertices == 0)
        return;

    int currIndex = 0;

    /* Copy the vertices to a new list and clear the old */
    var oldVertices = new Vertices(vertices);
    vertices.Clear();

    for (int i = 0; i < oldVertices.Count; ++i)
    {
        if (mergeMe[i] || newNVertices == 0 || currIndex == newNVertices)
            continue;

        vertices.Add(oldVertices[i]);
        ++currIndex;
    }
}

/* Reduces the polygon by distance. */
Vertices ReduceByDistance(Vertices vertices, float distance)
{
    /* We can't simplify polygons under 3 vertices */
    if (vertices.Count < 3)
        return vertices;

    distance *= distance;

    var simplified = new Vertices();

    for (int i = 0; i < vertices.Count; i++)
    {
        int nextId = vertices.NextIndex(i);

        Vector2 current = vertices[i];
        Vector2 next = vertices[nextId];

        /* If they are closer than the distance, continue */
        if ((next - current).LengthSquared() <= distance)
            continue;

        simplified.Add(current);
    }

    return simplified;
}

/* Reduces the polygon by removing the Nth vertex in the vertices list. */
Vertices ReduceByNth(Vertices vertices, int nth)
{
    /* We can't simplify polygons under 3 vertices */
    if (vertices.Count < 3)
        return vertices;

    if (nth == 0)
        return vertices;

    var result = new Vertices(vertices.Count);
    result.AddRange(vertices.Where((t, i) => i%nth != 0));

    return result;
}

#endif /* ARDUINO TODO: remove this line when emb-outline is C89 complete. This is a temporary arduino build fix. */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
