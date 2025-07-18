#ifndef CONCAVE_HULL
#define CONCAVE_HULL
#include <vector>
#include <algorithm>
#define INF 1000000000
class Point {
public:
    double x,y;
    Point () {}
    Point (double x, double y) : x(x), y(y) {}
    Point (const cv::Point& p): x(p.x), y(p.y) {}

    bool operator == (const Point& p) const {
        return x == p.x && y == p.y;
    }

    bool operator < (const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

double determinant (double a1, double b1, double a2, double b2) {
    return a1*b2 - a2*b1;
}

class Segment {
public:
    //double x1,y1,x2,y2;
    Point p1,p2;
    Segment () {}
    Segment (const Point& p1, const Point& p2) : p1(p1), p2(p2) {}

    bool intersect (const Segment& s) const {
        if (p1 == s.p1 || p1 == s.p2 || p2 == s.p1 || p2 == s.p2) return false;
        double a1 = p2.y-p1.y;
        double b1 = p2.x-p1.x;
        double c1 = p1.x*a1-p1.y*b1;
        b1 = -b1;
        double a2 = s.p2.y-s.p1.y;
        double b2 = s.p2.x-s.p1.x;
        double c2 = s.p1.x*a2-s.p1.y*b2;
        b2 = -b2;
        double d = determinant (a1, b1, a2, b2);
        if (d == 0) return false;
        double dx,dy;
        dx = determinant (c1,b1,c2,b2) / d;
        dy = determinant (a1,c1,a2,c2) / d;
        if (!contains (Point (dx,dy)) || !s.contains (Point (dx,dy))) {
            return false;
        }
        return true;
    }

    bool contains (const Point& p) const {
        return p.x >= std::min (p1.x,p2.x) && p.x <= std::max (p1.x,p2.x) &&
               p.y >= std::min (p1.y,p2.y) && p.y <= std::max (p1.y,p2.y);// &&
               //(p.x - p1.x) * (p2.y - p1.y) == (p.y - p1.y) * (p2.x - p1.x);
    }
};

template <class T>
using Vector = std::vector <T>;
typedef Vector <Point> Polygon;

bool intersect (const Polygon& p, const Segment& s) {
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        if (s.intersect(Segment (p[i], p[(i+1)%n]))) {
            return true;
        }
    }
    return false;
}

double cross (const Point& p1, const Point& p2) {
    return p1.x*p2.y - p2.x*p1.y;
}

double area (const Polygon& p) {
    double res = 0;
    int n = p.size();
    for (int i = 0; i < n; ++i) {
        res += cross (p[i], p[(i+1)%n]);
    }
    return abs(res)/2;
}

double angle (const Point& p1, const Point& p2, const Point& p3) {
    return -1;
}

Polygon merge (const Polygon& p1, const Polygon& p2, int x1, int y1, int x2, int y2, const double alpha) {
    if ( (Segment (p1[x1], p2[x2])).intersect (Segment (p1[y1], p2[y2]))) {
        std::swap (x2, y2);
    }
    if ( (Segment (p1[x1], p2[x2])).intersect (Segment (p1[y1], p2[y2]))) {
        return Polygon();
    }
    if (intersect(p1, Segment (p1[x1], p2[x2]))  ||
        intersect(p1, Segment (p1[y1], p2[y2]))  ||
        intersect(p2, Segment (p1[x1], p2[x2]))  ||
        intersect(p2, Segment (p1[y1], p2[y2]))
            ) {
        return Polygon();
    }
    int n1=p1.size(), n2=p2.size();
    Polygon r1, r2, r3, r4, res;
    double a1, a2, a3, a4, max;

    for (int i = y1; i != x1; i=(i+1)%n1) {
        r1.push_back (p1[i]);
    }
    r1.push_back(p1[x1]);
    for (int i = x2; i != y2; i=(i+1)%n2) {
        r1.push_back(p2[i]);
    }
    r1.push_back(p2[y2]);





    for (int i = y1; i != x1; i=(i+1)%n1) {
        r2.push_back (p1[i]);
    }
    r2.push_back(p1[x1]);
    for (int i = x2; i != y2; i=(i-1+n2)%n2) {
        r2.push_back(p2[i]);
    }
    r2.push_back(p2[y2]);





    for (int i = y1; i != x1; i=(i-1+n1)%n1) {
        r3.push_back (p1[i]);
    }
    r3.push_back(p1[x1]);
    for (int i = x2; i != y2; i=(i+1)%n2) {
        r3.push_back(p2[i]);
    }
    r3.push_back(p2[y2]);




    for (int i = y1; i != x1; i=(i-1+n1)%n1) {
        r4.push_back (p1[i]);
    }
    r4.push_back(p1[x1]);
    for (int i = x2; i != y2; i=(i-1+n2)%n2) {
        r4.push_back(p2[i]);
    }
    r4.push_back(p2[y2]);

    a1 = area(r1);
    a2 = area(r2);
    a3 = area(r3);
    a4 = area(r4);
    max = a1;
    res = r1;
    if (a2 > max) {
        max = a2;
        res = r2;
    }
    if (a3 > max) {
        max = a3;
        res = r3;
    }
    if (a4 > max) {
        max = a4;
        res = r4;
    }

    /*
    for (int i = y1; i != x1; i=(i+1)%n1) {
        res.push_back (p1[i]);
    }
    res.push_back(p1[x1]);
    for (int i = x2; i != y2; i=(i+1)%n2) {
        res.push_back(p2[i]);
    }
    res.push_back(p2[y2]);
    */
    return res;
}

void _print (const Polygon& p) {
    std::cout << "current polygon" << std::endl;
    for (int i = 0; i < p.size(); ++i) {
        std::cout << p[i].x << " " << p[i].y << std::endl;
    }
}

Polygon _concaveHull (const Vector <Point>& input, int b, int e, const double alpha) {
    Polygon res;
    if (e-b <= 3) {
        for (int i = b; i < e; ++i) {
            res.push_back (input[i]);
        }
        return res;
    }
    int m = (b+e)/2;
    Polygon p1 = _concaveHull (input, b, m, alpha);
    Polygon p2 = _concaveHull (input, m, e, alpha);
    double minArea = INF;
    //int n1 = p1.size();
    //int n2 = p2.size();
    for (int i1 = 0; i1 < p1.size(); ++i1) {
        for (int j1 = i1+1; j1 < p1.size(); ++j1) {
            for (int i2 = 0; i2 < p2.size(); ++i2) {
                for (int j2 = i2+1; j2 < p2.size(); ++j2) {
                    Polygon tmpPolygon = merge (p1, p2, i1, j1, i2, j2, alpha);
                    if (tmpPolygon.size() <= 0) {
                        continue;
                    }
                    double tmpArea = area (tmpPolygon);
                    if (tmpArea < minArea) {
                        minArea = tmpArea;
                        res = tmpPolygon;
                    }
                }
            }
        }
    }
    //_print (res);
    return res;
}

bool comp (const Point& p1, const Point& p2) {
    return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
}

Polygon concaveHull (const Vector <Point>& inp, const double alpha){
    Vector <Point> input = inp;
    std::sort (input.begin(), input.end());
    return _concaveHull (input, 0, input.size(), alpha+180);
}

#endif // CONCAVE_HULL

