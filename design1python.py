import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 
import matplotlib.patches as mpatches



# # Collector curve
data = pd.DataFrame()
data = pd.read_excel('design1results.xlsx', index_col=0)  
x = data['VCE'].to_numpy()
y = data['Ic'].to_numpy()
Vcc = 20

# # Load Line
data["lineC"]=data['VCE'].apply(lambda x:(Vcc - x))
yt = data["lineC"].to_numpy()
fig, ax = plt.subplots()
fig, ax = plt.subplots()

# # Using set_dashes() to modify dashing of an existing line
line1,=ax.plot(x,y,label="Curva de colector")
line2,=ax.plot(x,yt,label="Recta de colector")
plt.legend()

# # ax.legend()
plt.title('Ic vs VCE')
plt.xlabel('VCE (V)')
plt.ylabel('Ic (mA)')
plt.show()

ym = yt - y
print (ym)