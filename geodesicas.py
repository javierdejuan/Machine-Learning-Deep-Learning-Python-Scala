#-------------------------------------------------------------------------------------------------------------
#   geodesics:
#
#   This scripts computes geodesics in a 2-sphere given initial starting point and initial velocity
#   For each iteration, Christoffel symbols are evaluated and the geodesic's differential equation is computed.
#
#   spherical coordinates: 
#
#    x = np.cos(u)*np.cos(v)
#    y = np.sin(u)*np.cos(v)
#    z = np.sin(v)
#
#    theta -> v (90 is the north pole)
#    phi    ->u
#
#
#  copyright Javier de Juan
#
#--------------------------------------------------------------------------------------------------------------

import sympy as sym
import matplotlib
import math as m
import numpy as np
import scipy.integrate as sc
import matplotlib.pyplot as plt
from sympy.abc import u,v
from sympy import tan, cos ,sin,cot



def Christoffel2nd_sphere():
   from sympy.abc import u,v
   
    # found using sympy.diffgeom
   return sym.Matrix([[(0, -tan(v)),  (0, 0)],[(sin(v)*cos(v), 0), (0, 0)]])
         

def f(y,s,C,u,v): # converts 2 degree geodesic's diff equation into 4 one degree diff equation
    y0 = y[0]  #u
    y1 = y[1]  #u'
    y2 = y[2]  #v
    y3 = y[3]  #v'
    dy = np.zeros_like(y)
    dy[0] = y1
    dy[2] = y3
 
    C = C.subs({u:y0,v:y2})
 
    dy[1] = -C[0,0][0]*dy[0]**2 -\
           2*C[0,0][1]*dy[0]*dy[2] -\
             C[0,1][1]*dy[2]**2
    dy[3] = -C[1,0][0]*dy[0]**2 -\
           2*C[1,0][1]*dy[0]*dy[2] -\
             C[1,1][1]*dy[2]**2

    
    return dy
 

fig = plt.figure()

C = Christoffel2nd_sphere() #compute Christoffel symbols for the 2-sphere

for alfa in range(0,180,15): # alpha is the angle of the tangent vector to the sphere
                             # fixing a starting point at (theta=0,phi=0) this loop cycle through
                             # different angle direction (alpha)

    alfa_rad=m.radians(alfa)    # converting alpha to radians
    
    phi=m.radians(0)            # setting initial conditions, phi and theta are the initial coordinates of our point
    theta=m.radians(0)
    
    phi1=m.cos(alfa_rad)        # setting initial velocity, phi1 and theta1 are the initial velocity of our point
    theta1=m.sin(alfa_rad)
    
    u0=[phi,phi1,theta,theta1]  #state vector

    
    
    s0 = 0
    s1 = 25*np.pi
    ds = 0.0015
    from sympy.abc import u,v
    s = np.arange(s0,s1+ds,ds)
    
    # solve ODE
    print("solving geodesic diff. equations for phi=%4.2f,theta=%4.2f, angle alfa=%d: " %(u0[0] * 180 / np.pi,u0[2] * 180 / np.pi,alfa) )
    
    X= sc.odeint(f,u0,s,args=(C,u,v))
    N = X[:,0].shape[0]
   
    u=np.degrees(X[:,0])
    v=np.degrees(X[:,2])
   
    #plot results
  
    plt.plot(u ,v,label=str(alfa))


plt.ylim(-90, 90)
plt.xlim(0, 360)

plt.ylabel('theta')
plt.xlabel('phi')


ax = fig.gca()
ax.set_xticks(np.arange(0, 360, 45))
ax.set_yticks(np.arange(-90, 90, 45))

plt.grid()

plt.show()