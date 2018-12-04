import matplotlib.pyplot as plt 
import numpy as np
import scipy.integrate as integrate
import random

SAMPLE_SIZE = 1000
SAMPLE_STARTING_POINT = (0, 30)
STEP = 0.5
SMOOTH = 0.4
SPLINES_AMT = 106
POINTS_RANGE = SPLINES_AMT - 6
B_SPLINE_PRECISION = 0.1
SMOOTHNESS_WEIGHT = 0

class Spline:
    def beta(self, t):
        #results as in PDF
        t = abs(t)
        if t < 1:
            retVal = 1.5*(0.5*t - 1)*(t*t) + 1
        elif t > 2:
            retVal = 0
        else:
            retVal = (2-t)**3 * 0.25

        return retVal

    def beta_2_dirivative(self, t):
        t = abs(t)
        if t < 1:
            retVal = 4.5*t - 3
        elif t > 2:
            retVal = 0
        else:
            retVal = 3 - 1.5*t
        return retVal

    def integ_expr(self, t, k, i):
        return self.beta_2_dirivative(t-k) * self.beta_2_dirivative(t - i) * 2

class B_Spline:
    def getSplineValue(self, t):
        ind = int(t)
        d = t - ind
        if(d < 0 or d > 1 or d > 99):
            print("erro no t")
        retVal = 0
        if ind >= 0:
            for i in range (4):
                if(ind + i < SPLINES_AMT):
                    retVal += self.coef[ind + i] * self.s.beta(d - 2 + i)
                else:
                    break
        return  retVal

    # def getSplineValue(self, t):
    #     ind = int(t)
    #     d = ind - t
    #     retVal = 0
    #     if ind >= 0:
    #         for i in range (4):
    #             if(ind + i < SPLINES_AMT):
    #                 retVal += self.coef[ind + i] * self.s.beta(d - 1 + i)
    #             else:
    #                 break
    #    return  retVal

    def calculateBSpline(self):
        curve = []
        i = SAMPLE_STARTING_POINT[0]
        while(i <= POINTS_RANGE):
            curve.append((i, self.getSplineValue(i)))
            i += B_SPLINE_PRECISION        
        return curve

    def calculateCoeficients(self, points):
        self.b_matrix = np.zeros((SAMPLE_SIZE, SPLINES_AMT))
        for i in range(len(self.b_matrix)):
            for j in range(len(self.b_matrix[0])):
                self.b_matrix[i][j] = self.s.beta((points[i][0]) - j)
            print("i = " + str(i) + str(self.b_matrix[i]))

        self.mlk_matrix = np.zeros((SPLINES_AMT, SPLINES_AMT))
        for i in range (len(self.mlk_matrix)):
            for j in range (len(self.mlk_matrix)):
                self.mlk_matrix[i][j] = integrate.quad(self.s.integ_expr, 0, POINTS_RANGE, args=(j, i ), limit=SAMPLE_SIZE)[0]
        y_spline = [0]*SAMPLE_SIZE
        for i in range (SAMPLE_SIZE):
            y_spline[i] = points[i][1]
        b_transp = np.transpose(self.b_matrix)
        result = np.matmul(b_transp, y_spline)
        multiplier = (np.matmul(b_transp, self.b_matrix) + SMOOTHNESS_WEIGHT*self.mlk_matrix)
        # print("B = " + str(self.b_matrix))
        # print("B^T = " + str(b_transp))
        # print("B^T * y = " + str(result))
        # for i in range (SPLINES_AMT):
        #     print("i = " + str(i) + "    " + str(self.mlk_matrix[i]))
        coefs = np.linalg.solve(multiplier, result)

        return coefs

    def __init__(self, points):
        self.s = Spline()
        self.coef = self.calculateCoeficients(points)
        self.b_s = self.calculateBSpline()




def generateSample():
    points = [0]*SAMPLE_SIZE
    x = SAMPLE_STARTING_POINT[0]
    y = SAMPLE_STARTING_POINT[1]
    
    xVals = [0]*SAMPLE_SIZE
    yVals = [0]*SAMPLE_SIZE
    for i in range (SAMPLE_SIZE):
        xVals[i] = x
        yVals[i] = y
        x += random.uniform(0.3*STEP, 2*STEP)
        y += random.uniform(-SMOOTH,SMOOTH)

    max_x = xVals[SAMPLE_SIZE-1]
    factor = max_x / (POINTS_RANGE)
    for i in range (SAMPLE_SIZE):
        points[i] = (xVals[i]/factor, yVals[i])

    return points

def createWindow():
    points = generateSample()

    for pt in points:
        plt.scatter(pt[0], pt[1], marker='.', s=2)

    bs = B_Spline(points)
    x = []
    y = []
    for pt in bs.b_s:
        x.append(pt[0])
        y.append(pt[1])
    plt.plot(x, y)

def main():
    random.seed()
    createWindow()
    plt.show()



if __name__ == '__main__':
    main()