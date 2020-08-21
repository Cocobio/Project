import numpy as np
import matplotlib.pyplot as plt

histogram_file = "2d histogram data.txt"

dimention = (360*20,180*20)

with open(histogram_file, "r") as f:
	str_data = f.readline().split(";")

	mat = np.array( [int(x) for x in str_data[:-1]], dtype=np.uint8).reshape(dimention).transpose()

print(mat.shape)

plt.imshow(mat, interpolation='none', extent=[-180,180,90,-90])

plt.colorbar()

plt.title('Grafica de profundidad de nodos del QuadTree', fontsize=14)
plt.xlabel('Longitud', fontsize=12)
plt.ylabel('Latitud', fontsize=12)

x_label_list = np.linspace(-180,180,9)
y_label_list = np.linspace(-90,90,5)
plt.xticks(x_label_list)
plt.yticks(y_label_list, y_label_list[::-1])

plt.show()