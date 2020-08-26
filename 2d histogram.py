import numpy as np
import matplotlib.pyplot as plt
import math as math

import matplotlib.animation as animation
from mpl_toolkits.axes_grid1 import make_axes_locatable

#########################################################################################################
# histogram_file = "Data/2dHistogram.txt"

# dimention = (360*30,180*30)

# with open(histogram_file, "r") as f:
# 	str_data = f.readline().split(";")

# 	mat = np.array( [int(x) for x in str_data[:-1]], dtype=np.uint8).reshape(dimention).transpose()

# plt.figure()
# ax = plt.gca()
# im = ax.imshow(mat, interpolation='none', extent=[-180,180,90,-90])
# divider = make_axes_locatable(ax)
# cax = divider.append_axes("right", size="5%", pad=0.05)
# plt.colorbar(im, cax=cax)

# ax.set_title('Grafica de profundidad de nodos del QuadTree', fontsize=14)
# ax.set_xlabel('Longitud', fontsize=12)
# ax.set_ylabel('Latitud', fontsize=12)

# x_label_list = np.linspace(-180,180,9)
# y_label_list = np.linspace(-90,90,5)
# ax.set_xticks(x_label_list)
# ax.set_yticks(y_label_list)
# ax.set_yticklabels([str(i-90) for i in y_label_list])

# plt.savefig("Imagenes/Histograma de profundidad 2D")
# plt.show()


#########################################################################################################
# filename = "Data/Movie1.txt"
# step = 241;
# dimention = (360,180)

# with open(filename, "r") as f:
# 	content = f.readlines()

# 	str_data = content[0].split(";")
# 	mat = np.array( [int(x) for x in str_data[:-1]], dtype=np.uint8).reshape(dimention).transpose()

# 	plt.figure()
# 	ax = plt.gca()

# 	im = ax.imshow(mat)

# 	divider = make_axes_locatable(ax)
# 	cax = divider.append_axes("right", size="5%", pad=0.05)

# 	im.set_clim(0,mat.max())
# 	plt.colorbar(im, cax=cax)

# 	ax.set_title('Video de profundidad de nodos del QuadTree', fontsize=14)
# 	ax.set_xlabel('Longitud', fontsize=12)
# 	ax.set_ylabel('Latitud', fontsize=12)

# 	x_label_list = np.linspace(0,360,9)
# 	y_label_list = np.linspace(0,180,5)
# 	ax.set_xticks(x_label_list)
# 	ax.set_xticklabels([str(i-180) for i in x_label_list])
# 	ax.set_yticks(y_label_list)
# 	ax.set_yticklabels([str(i-90) for i in y_label_list])

# 	for i in content[1:-1]:
# 		str_data = i.split(";")
# 		mat = np.array( [int(x) for x in str_data[:-1]], dtype=np.uint8).reshape(dimention).transpose()

# 		plt.pause(1)
# 		im.set_data(mat)
# 		im.set_clim(0,mat.max())
# plt.pause(0.1)
# plt.show()


#########################################################################################################
# filename = "Data/Depth Histogram.txt"
# # filename = "Data/Depth Histogram GREY.txt"

# with open(filename, "r") as f:
# 	content = f.readlines()

# m = int(content[-1].split(':')[0])+1

# h = np.zeros(m, np.uint32)

# for i,j in [k.split(':') for k in  content]:
# 	h[int(i)] = int(j)

# profundidades = 0
# for i in range(len(h)):
# 	profundidades += i*h[i]
# print(profundidades/h.sum())


# plt.rcParams["axes.prop_cycle"] = plt.cycler("color", plt.cm.viridis(np.linspace(0,1,m+1)))

# for x,y in zip(range(m),h):
# 	plt.bar(x,y)
# plt.yscale("log")
# plt.title("Histograma de profundidad de nodos hojas", fontsize=14)
# plt.xlabel("Profundidad", fontsize=14)
# plt.ylabel("Frecuencia", fontsize=14)
# plt.xticks(range(0,m+1,2))
# # plt.savefig("Imagenes/Histograma de profundidad")
# plt.show()



#########################################################################################################
# filename = "Data/times.txt"
# filename2 = "Data/avr_depth.txt"

# with open(filename, 'r') as f:
# 	str_data = f.readline().split(';')

# with open(filename2, 'r') as f:
# 	str_data2 = f.readline().split(';')

# fig, ax1 = plt.subplots()

# # x = range(len(str_data)-1)
# x = np.linspace(0,len(str_data)-1, len(str_data)-2)

# color1='#684cac'
# color2='#55b439'
# color3='#f67752'

# ax1.bar(x, [float(i) for i in str_data[1:-1]], color=color1)

# ax1.set_title('Tiempo de inserción vs Profundidad promedio de inserción', fontsize=14)

# ax1.set_ylabel('Tiempo de inserción [s]', color=color1, fontsize=14)
# ax1.set_xlabel('Bins de 30 mil ciudades que se insertan.', fontsize=14)
# ax1.tick_params(axis='y', labelcolor=color1)

# ax2 = ax1.twinx()
# plt.plot(x, [float(i) for i in str_data2[1:-1]], color=color2, label='Profundidad real')
# plt.plot(x, [math.log(float(i+1)*22434.7,4) for i in x], color=color3, label='Profundidad ideal')
# ax2.set_ylabel('Profundidad promedio', color=color2, fontsize=14)
# ax2.set_yticks(range(15))
# ax2.tick_params(axis='y', labelcolor=color2)

# ax2.legend(loc='upper left')
# # plt.savefig("Imagenes/t y depth avr")
# plt.show()



#########################################################################################################
# filename = "Data/black grey white nodes.txt"

# with open(filename, 'r') as f:
# 	content = f.readlines()

# 	black = [int(i.split(';')[0]) for i in content[:-1]]
# 	grey = [int(i.split(';')[1]) for i in content[:-1]]
# 	white = [int(i.split(';')[2]) for i in content[:-1]]

# x = range(len(black))

# plt.plot(x, black, label='Nodos negros', color='b')
# plt.plot(x, grey, label='Nodos grices', color='g')
# plt.plot(x, white, label='Nodos blancos', color='r')

# plt.xticks(np.linspace(0,len(black),6), np.linspace(0,100,6))
# plt.yticks(range(0,3000001,500000), [0 if int(i)==0 else str(int(i)/1000000)+'M' for i in range(0,3000001,500000)] )

# plt.xlabel('Porcentaje de ciudades insertadas', fontsize=14)
# plt.ylabel('Numero de nodos', fontsize=14)

# plt.title('Cantidad de nodos por tipo vs inserciones', fontsize=14)

# plt.legend()

# # plt.savefig("Imagenes/n de nodos al insertar")

# plt.show()



#########################################################################################################
# filename = 'Data/remove t y depth avr.txt'

# with open(filename, 'r') as f:
# 	content = f.readlines()[:]

# 	t = [float(i.split(';')[0]) for i in content[:]]
# 	d = [float(i.split(';')[1]) for i in content[:]]

# x = range(1,1+len(t))
# print(len(t))

# color1='#684cac'
# color2='#55b439'
# color3='#f67752'

# fig, ax1 = plt.subplots()

# ax1.set_title('Tiempo de delete vs Profundidad promedio de delete', fontsize=14)
# ax1.set_ylabel('Tiempo de delete [s]', color=color1, fontsize=14)
# ax1.set_xlabel('Porcentaje de ciudades eliminados.', fontsize=14)
# ax1.tick_params(axis='y', labelcolor=color1)

# ax1.bar(x,t, color=color1)
# ax2 = ax1.twinx()
# plt.plot(x, d, color=color2, label='Profundidad real')
# plt.plot(x, [math.log(float(i+1)*22434.7,4) for i in x][::-1], color=color3, label='Profundidad ideal')

# ax2.set_ylabel('Profundidad promedio de nodos eliminados', color=color2, fontsize=14)
# ax2.tick_params(axis='y', labelcolor=color2)

# ax2.set_yticks(range(15))

# ax2.legend(loc='lower left')

# # plt.savefig("Imagenes/remove and depth")

# plt.show()



#########################################################################################################
# filename = 'data/contained point search.txt'
# filename2 = 'data/random point search.txt'

# with open(filename,'r') as f:
# 	content = f.readlines()

# 	t = [float(i.split(';')[0]) for i in content[:]]
# 	d = [float(i.split(';')[1]) for i in content[:]]

# with open(filename2, 'r') as f:
# 	content = f.readlines()

# 	t2 = [float(i.split(';')[0]) for i in content[:]]
# 	d2 = [float(i.split(';')[1]) for i in content[:]]

# x = range(1,1+len(t))

# color1='#5b09ad'
# color2='#008002'
# color3='#f74d80'
# color4='#339ea3'

# fig, ax1 = plt.subplots()

# ax1.set_title('Tiempo y profundidades promedios de busqueda para puntos', fontsize=14)
# ax1.set_ylabel('Tiempo de busqueda [s]', color=color1, fontsize=14)
# ax1.set_xlabel('2M de datos buscados por bin.', fontsize=14)
# ax1.tick_params(axis='y', labelcolor=color1)

# ax1.bar(x,t, color=color1, label='Puntos Contenidos')
# ax1.bar(x,t2, color=color4, label='Cordenadas Aleatorias')
# ax2 = ax1.twinx()
# plt.plot(x, d, color=color2, label='Datos Aleatorios Contenidos')
# plt.plot(x, d2, color=color3, label='Datos Aleatorios', linewidth=3)

# ax2.set_ylabel('Profundidad promedio', color=color2, fontsize=14)
# ax2.tick_params(axis='y', labelcolor=color2)
# ax2.set_yticks(range(15))

# ax1.legend(loc='lower right')
# ax2.legend(loc='lower left')

# # plt.savefig("Imagenes/point search")

# plt.show()



#########################################################################################################
# filename = "data/random region search.txt"

# with open(filename, 'r') as f:
# 	content = f.readlines()

# 	t = [float(i.split(';')[0])/20 for i in content]
# 	d = [float(i.split(';')[1]) for i in content]


# x = range(10, 10+len(content))

# color1='#3383e9'
# color2='#dc41f5'

# fig, ax1 = plt.subplots()

# ax1.set_title('Tiempos y numero de ciudades promedios de busqueda por region', fontsize=14)
# ax1.set_ylabel('Tiempo promedio de busqueda [s]', color=color1, fontsize=14)
# ax1.set_xlabel('Tamaño de arista de la region', fontsize=14)
# ax1.tick_params(axis='y', labelcolor=color1)

# ax1.bar(x,t, color=color1, label='Puntos Contenidos')
# ax2 = ax1.twinx()
# plt.plot(x, d, color=color2, label='Datos Aleatorios Contenidos')

# ax2.set_ylabel('Numero de ciudades promedio', color=color2, fontsize=14)
# ax2.tick_params(axis='y', labelcolor=color2)


# # plt.savefig("Imagenes/point search")

# plt.show()