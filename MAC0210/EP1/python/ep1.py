import matplotlib.pyplot as plt 
import numpy as np
import random
import sympy as sym

CURVE_AMT = 7
DRAW_PRECISION = 0.04
DRAW_SPACE = np.linspace(0.0, 1.0, num=1.0/DRAW_PRECISION)

class Point:
    def __init__(self, x, y):
        self.point = (x, y)
    def x(self):
        return self.point[0]
    def y(self):
        return self.point[1]
    def __str__(self):
        return "(" + str(self.x())+","+str(self.y()) + ")"

def addPt(a, b):
    return Point(a.x() + b.x(), a.y() + b.y())
def subPt(a, b):
    return Point(a.x() - b.x(), a.y() - b.y())
def multScPt(c, a):
    return Point(c*a.x(), c*a.y())
def distPt(a, b):
    return (a.x() - b.x())**2 + (a.y() - b.y())**2 

class Curve:
    def CalculateCurve(self):
        self.curveX = []
        self.curveY = []
        for t in DRAW_SPACE:
            p0 = addPt(self.coefs[0], multScPt(t, subPt(self.coefs[1], self.coefs[0])))
            p1 = addPt(self.coefs[1], multScPt(t, subPt(self.coefs[2], self.coefs[1])))
            p2 = addPt(self.coefs[2], multScPt(t, subPt(self.coefs[3], self.coefs[2])))
            p3 = addPt(p0, multScPt(t, subPt(p1, p0)))
            p4 = addPt(p1, multScPt(t, subPt(p2, p1)))
            p5 = addPt(p3, multScPt(t, subPt(p4, p3)))
            self.curveX.append(p5.x())
            self.curveY.append(p5.y())

    def __init__(self):
        self.coefs = [Point(random.random(), random.random()),
                      Point(random.random(), random.random()),
                      Point(random.random(), random.random()),
                      Point(random.random(), random.random())]
        self.CalculateCurve()

class Scene:
    def draw(self):
        plt.xlim(0, 1)
        plt.ylim(0, 1)
        for i in range(CURVE_AMT):
            color = 'r' if i != self.selected else 'g'
            plt.plot(self.curves[i].curveX, self.curves[i].curveY, color)

    def rootDist(self, mouse, curveIndex):
        t = sym.Symbol('t')
        dist = sym.expand(( mouse.x() - self.curves[curveIndex].coefs[0].x() *((1 - t)**3)         \
                           - 3 * self.curves[curveIndex].coefs[1].x() * (1 - t)**2 * t  \
                           - 3 * self.curves[curveIndex].coefs[2].x() *(1 - t)* (t**2)  \
                           - self.curves[curveIndex].coefs[3].x() * (t**3)     )**2     \
              +( mouse.y() - self.curves[curveIndex].coefs[0].y() *((1 - t)**3)         \
                           - 3 * self.curves[curveIndex].coefs[1].y() * (1 - t)**2 * t  \
                           - 3 * self.curves[curveIndex].coefs[2].y() *(1 - t)* (t**2)  \
                           - self.curves[curveIndex].coefs[3].y() * (t**3)     )**2     )
                           

        curveValX = sym.expand(
            self.curves[curveIndex].coefs[0].x() *((1 - t)**3) \
          + 3 * self.curves[curveIndex].coefs[1].x() * (1 - t)**2 * t\
          + 3 * self.curves[curveIndex].coefs[2].x() *(1 - t)* (t**2)\
          + self.curves[curveIndex].coefs[3].x() * (t**3)
        )
        curveValY = sym.expand(
            self.curves[curveIndex].coefs[0].y() *((1 - t)**3) \
          + 3 * self.curves[curveIndex].coefs[1].y() * (1 - t)**2 * t\
          + 3 * self.curves[curveIndex].coefs[2].y() *(1 - t)* (t**2)\
          + self.curves[curveIndex].coefs[3].y() * (t**3)
        )

        dist = sym.simplify(dist)

        deriv = sym.diff(dist, t)
        roots = list(sym.solveset(deriv, t))
        minDist = float('inf')
        minIndex = -1
        minT = -1  
        for i in range(len(roots)):
            rt = complex(roots[i].evalf())
            if rt.imag == 0:
                temp = rt.real
                if temp < 1 and temp > 0:
                    d = dist.evalf(subs={t:temp})
                    if(d < minDist):
                        minT = rt.real
                        minDist = d
                        minIndex = i

        if minT != -1:
            curveX = curveValX.evalf(subs={t:minT})
            curveY = curveValY.evalf(subs={t:minT})
            print(curveX)
            print(curveY)
            print()
            x = [mouse.x(), curveX]
            y = [mouse.y(), curveY]
        else:
            x = [mouse.x(), 0]
            y = [mouse.y(), 0]
        # print (dist)
        # print(deriv)
        # print(roots)
        # print(minDist)
        # print(minIndex)
        # print()
        return minDist, x, y

    def GetClosest(self, point):
        retVal = -1
        dist = float('inf')
        toDrawLinesX = []
        toDrawLinesY = []
        for i in range (CURVE_AMT):
            aDist = distPt(point, self.curves[i].coefs[0])
            dDist = distPt(point, self.curves[i].coefs[3])
            rDist, x, y = self.rootDist(point, i)
            toDrawLinesX.append(x)
            toDrawLinesY.append(y)
            print("a dist = " + str(aDist) + "\n d dist = " + str(dDist) + "\n r Dist = " + str(rDist))
            d = min(aDist, dDist, rDist)
            if(d < dist):
                dist = d
                retVal = i
            # if d == aDist or d == dDist:
            #     if aDist < dDist:

            # else:
        return retVal, toDrawLinesX, toDrawLinesY

    def Select(self, point):


        closest, drawX, drawY = self.GetClosest(point)
        print(drawX)
        self.selected = closest

        plt.gcf().clear()
        for i in range (0, CURVE_AMT):
            x = [drawX[i][0], drawX[i][1]]
            y = [drawY[i][0], drawY[i][1]]
            #plt.plot(x, y, 'bo-')
        self.draw()
        plt.show()

    def Click(self, event):
        x, y = event.x, event.y
        inv = self.ax.transAxes.inverted()
        pt = inv.transform((x, y))
        point = Point(pt[0], pt[1])
        print("antes em %d, %d"%(x, y))
        print("clicou em (" + str(point.x()) + ", " + str(point.y()) + ")")
        self.Select(point)

    def __init__(self):
        self.selected = -1
        self.ax = plt.subplot(111)
        self.curves = []
        for i in range(CURVE_AMT):
            self.curves.append(Curve())
        plt.connect('button_release_event', self.Click)

def main():
    random.seed()
    scene = Scene()
    scene.draw()
    plt.show()

if __name__ == '__main__':
    main()