from PIL import Image
import warnings
import math
import sys
import itertools
import time
from funciones import *
from UnitTests import *
from Utils import *

# deactivate huge images warning

warnings.simplefilter('ignore', Image.DecompressionBombWarning)

# read file

filename = "hexaedro_small.png"
print("reading the file...", filename)
hexaedro = Image.open(filename)
print("file read correctly")



# compute bounding box

diagonal= hexaedro.getbbox()

esquina_arriba_izquierda = diagonal[0]
esquina_arriba_derecha   = diagonal[1]
esquina_abajo_izquierda  = diagonal[2]
esquina_abajo_derecha    = diagonal[3]

lado1 = esquina_abajo_derecha / 3
lado2 = esquina_abajo_izquierda / 4

width = esquina_abajo_izquierda
heigth= esquina_abajo_derecha

assert math.fabs(lado2-lado1)<1,"Image loaded doesnot have a 4:3 ratio"

lado = max(lado1,lado2)
lado = math.ceil(ladois

lado =int(lado)

print("file",filename," is", width," x ", heigth," pixels","total:",heigth*width/10E6,"Millions")         
print("Side of the cubemap is", lado," pixels")

# ----------------------------------------------------------------------------------------
# Instead of looking for each pixel in the cubemap and then find its intersection in the sphere,
# to avoid eventual interpolation, instead we are going to iterate through the sphere and then find
# the intersection with the cube map
#-----------------------------------------------------------------------------------------

# User choose the size of the cubemap
# Equirectangular ratio must be 2 : 1

factor_de_compresion = 1.0 # 1 para misma resolucion que el hexaedro

heigth_equirectangular =   math.floor(math.sqrt( heigth * width/2) * factor_de_compresion)
width_equirectangular  =   heigth_equirectangular * 2

# create the equirectangular image

bgcolor ="white"
equirectangular = Image.new(hexaedro.mode,(width_equirectangular,heigth_equirectangular),bgcolor)

# pixel mapping
# each pixel is a color (R,G,B)

hexaedero_pixel_map       =  hexaedro.load()
equirectangular_pixel_map =  equirectangular.load()

# we iterate through theequirectangular from left to right aund up to down


# Axis references in the cubemap:
#
#   i -> front side
#  -i -> back side
#   j -> left side
#  -j -> right side
#   k -> top side
#  -k -> down  side
# 
#  theta = 0   fi =  0 -> axis  i (front side )
#  theta = 90  fi =  0 -> axis  j (left side)
#  theta = 180 fi =  0 -> axis -i (back side)
#  theta = 270 fi =  0 -> axis -j (right side)
#  
#  theta = 0   fi = 90  -> axis K (side up)
#  theta = 0   fi =-90  -> axis -k (side down)
#   
#   180                  270          360-0        90             180
#   ----------------------------------------------------------------  -90
#   (u,v)                            (0,width/2)                  (0,width)               
#   |                                                              |
#   |                                                              |
#   back                 right        front        left         back
#   |                                                              |
#   |                                                              |
#   |                                                              |
#   |---------------------------------------------------------------  +90
#
#   (u,v) coordinates refereence of the equirectangular pixels
#   heigth ->180

# phi theta liss:


equi_theta_list_left  = [ 180*2*pixel_u/width_equirectangular  + 180 for pixel_u in range(0,int(width_equirectangular/2))]  

equi_theta_list_right = [ 360*(pixel_u-width_equirectangular/2)/width_equirectangular for pixel_u in range(int(width_equirectangular/2),int(width_equirectangular))] 

# merge in a unique list

equi_theta_list = equi_theta_list_left + equi_theta_list_right

# reverse list to ensure correct behaviour of trigonometrics functions
equi_theta_list.reverse() 

# latitudes mapping, from -90 to 90

equi_fi_list    = [ 90*(heigth_equirectangular-2*pixel_v)/heigth_equirectangular     for pixel_v in range(0,heigth_equirectangular)] 

total_pixels = len(equi_theta_list) * len(equi_fi_list)

medio_lado = lado/2

# unit tests

#Unit_test_transform_espherical_to_cartesians() # test unitario spherical to cartesians coordinates transform
#Unit_test_interseccion(lado)                   # test unitario ray trace intersection
#Unit_test_find_pixel(lado,medio_lado)          # test unitario (u,v) transform of cartesian intersection point

# cubemap to equirectangular

print("lanzado transformacion hexaedro equirectangular")
print("pixels a procesar...", total_pixels/10e6," Millones")


# couners initialization

i=j=0
cnt=0
cntpr=0
caras_cnt=[0,0,0,0,0,0]
caras_out_of_range=[0,0,0,0,0,0]
max_error_heigth=0
max_error_width=0

# process

inicio=time.time()

for theta in equi_theta_list:

    for fi in equi_fi_list:
        
        vector=transform_espherical_to_cartesians((theta,fi))                   # For every pixel from the equirectangular, I build its unitary vector and I transform it in cartesian coordinates of the cubemap
        punto_interseccion=interseccion(vector,medio_lado)                      # Finding the intersection of the mentionned unitary vector with the cubemap
        pixel_hexaedro=find_pixel(punto_interseccion,lado,heigth,width  )       # final coordinates transform to find the correct pixel in the cubemap
       
        caras_cnt[punto_interseccion[3]]+=1
        
        try:
            pixel_color=hexaedero_pixel_map[pixel_hexaedro[0],pixel_hexaedro[1]]
            equirectangular_pixel_map[i,j]=pixel_color
           
        
        except Exception as e:

            error_width=pixel_hexaedro[2]
            error_heigth=pixel_hexaedro[3]

            if error_heigth> max_error_heigth:
                max_error_heigth = error_heigth
            if error_width> max_error_width:
                max_error_width = error_width

            cnt+=1
            caras_out_of_range[punto_interseccion[3]]+=1

        cntpr+=1  
        j+=1 
    i+=1
    j=0
    progbar(cntpr, total_pixels, 80)
       
# metrics of the process

fin = time.time()
pixels_translated=heigth_equirectangular*width_equirectangular-cnt
ratio= pixels_translated/(heigth*width)
caras_cnt_pct=[x*100/pixels_translated for x in caras_cnt]

print("\n------------------------------------------------------------------------------------------")
print("\npixels processed:",pixels_translated/10e6," Millions")
print("ratio over initual cubemap:",int(100*ratio),"%")
print("recommende equirectangular resolutionfor a 1:1 ratio:",int(heigth_equirectangular/ratio),"x",int(width_equirectangular/ratio))
print("pixels processed by sides (%):")
print("right - left - front - back - up - down")
print(caras_cnt_pct)
print("\n------------------------------------------------------------------------------------------")
print("errors summary: ")
print("pixels out of range:",cnt)
print("pixels ouf of range by side:",caras_out_of_range)
print("Maximum error in pixel horizontal (pixels)",max_error_width)
print("Maximum error in pixel vertical (pixels)",max_error_heigth)
print("\n------------------------------------------------------------------------------------------")
# limpiamos...

hexaedro.close()
equirectangular.show()       
equirectangular.save("equirectangular.png") 
equirectangular.close()  

tiempo_transcurrido = fin - inicio

print("Execution time: %sh:%sm:%ss" % hh_mm_ss(tiempo_transcurrido))


