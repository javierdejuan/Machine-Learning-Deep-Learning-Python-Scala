## Cubemap, an alternative to equirectangular grid

<p align="center">
  <img width="400" height="300" src="https://github.com/javierdejuan/stuff/blob/master/VirtualReallityFoundamentals/code/hexaedro%20equiangular.jpg">
</p>
<p align="center">
Cubemap grid
</p>

An easy way to build a panoramic illustration is through a cubemap.

This simplification of the sphere in 6 sides, can give very good results in some cases, where the objects represented are far enough away 
from the center of the cubemap.
It works very well in urban skecthes, for example.

The advantage of this method is that we avoid the distortion of the equirectangular grid when we approach the pole, making very tricky freehand drawings.
In contrast, using the cube map, we only need to make 6 drawings, each one in a very easy central perspective, each drawing covering a 45 Field Of View angle.
Each of this drawings represents the intersection of the world with each side of the cubemap.
In practice, a draughtman needs to rotate 90 degrees in latitude or longitude from his initial point of view, and then draw what he sees in front of him.


<p align="center">
  <img width="400" height="300" src="https://github.com/javierdejuan/stuff/blob/master/VirtualReallityFoundamentals/code/marche%20des%20halles%20angouleme%20Hexaedro.png">
</p>
<p align="center">
Cubemap Hand Illustration
</p>


Una vez tenemos el cubemap, debemos convertirlo a un formato equirectangular, que es el formato que usan los viewer de 360 mas habituales.

## conversion from cubemap to equirectangular

<p align="center">
  <img width="400" height="200" src="https://github.com/javierdejuan/stuff/blob/master/VirtualReallityFoundamentals/code/marche%20des%20halles%20angouleme.jpg">
</p>
<p align="center">
Cubemap Hand Illustration
</p>

The following scripts converts a cubemap to an equirectangular. 
You can find the full script here.

In order to avoid interpolation in the final equirectgangular, we have choosen a reverse method to transform the cubemap.
The algorithm iterates through the equirectangular file, then, builds a unitary vector of the given point with its latitude and longitude and 
then finds the intersection of this ray within the cubemap.
Finaly, the scripts copies the pixel found to the its location in the equirectangular file.

