## How to build an Equirectangular Grid

The equirectangular grid is used to build panoramic images as theses ones:
<p align="center">
  <img width="400" height="200" src="https://github.com/javierdejuan/stuff/blob/master/differential_geometry/cocina.jpg">
</p>

<p align="center">
  <img width="400" height="200" src="https://github.com/javierdejuan/stuff/blob/master/differential_geometry/mapamundi.jpg">
</p>



WIth the correct viewer, this images can then be shown in a [360](https://veer.tv/photos/improved-street-jazz-concert-180510) degrees environnement:

<p align="center">
  <img width="400" height="200" src="https://github.com/javierdejuan/stuff/blob/master/differential_geometry/grid%20web.png">
</p>

A detailed explanation of the foundatinos of the Equirectangular grid can be fond [here](https://artbellinsky.com/curved-drawing-lessons-2-the-spherical-geometry/). In a glimpse, the equirectangular grid is nothing but the register of the lattitude and longitude angles in a cartesian plane where the vertical axis is the latitude and the horizontal axis is the longitude.

The purpose of this exercise is to find a mathematical definition of the equirectangular grid. There are many ways to solve this problems, here we have chosen to introduce some basic ideas in Riemanm's Geometry because of his understanding of curved spaces. 



## Geodesics in the 2-sphere

This code computes geodesic's differential equation in a 2-sphere. For each iteration of the loop, the initial vector is updgrade.
Parallel transport is then confirmed since the ending vector directions equals the starting direction.
<p align="center">
  <img width="460" height="300" src="https://github.com/javierdejuan/stuff/blob/master/differential_geometry/Equirectangular.png">
</p>
