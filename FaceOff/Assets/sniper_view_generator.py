# installing pil
import numpy 

from scipy import misc
f = misc.face()
misc.imsave('face.png', f) # uses the Image module (PIL)



w = 400
h = 300

filename = "sniper_view.png"


# http://stackoverflow.com/questions/1696135/how-to-create-3-dimensions-matrix-in-numpy-like-matlab-a
sniper_view_array = numpy.empty((h,w,4))

'''
for y in range(0, h):
	for x in range(0, w):
		sniper_view_array[y:x] = [255, 0, 0, 255]
'''

sniper_view_array[100:120] = [255, 0, 0, 255]

sniper_view_array[:,:] = [0, 0, 0, 255]


# the circle
lx = sniper_view_array.shape[0]
ly = sniper_view_array.shape[1]

X, Y = numpy.ogrid[0:lx, 0:ly]
mask = (X - lx / 2) ** 2 + (Y - ly / 2) ** 2 < lx * ly / 10

# Masks
sniper_view_array[mask] = [255, 255, 255, 255]
# Fancy indexing
#face[range(400), range(400)] = 255


# the cross 
sniper_view_array[lx/2] = [0, 0, 0, 255]
sniper_view_array[lx/2+1] = [0, 0, 0, 255]

sniper_view_array[ly/2] = [0, 0, 0, 255]
sniper_view_array[ly/2+1] = [0, 0, 0, 255]


sniper_view_array[mask] = [255, 255, 255, 255]

sniper_view_array[h/2 : h/2+1, :] = [0, 0, 0, 255]
sniper_view_array[:, w/2 : w/2+1] = [0, 0, 0, 255]



misc.imsave(filename, sniper_view_array) # uses the Image module (PIL)



