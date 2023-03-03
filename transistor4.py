import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 

# # Collector curve
data = pd.DataFrame()
data = pd.read_excel('design4results.xlsx', index_col=0)  
x = data['VCE'].to_numpy()
y = data['Icm'].to_numpy()
Vcc = 12

# # Load Line
data["lineC"]=data['VCE'].apply(lambda x:(Vcc - x)/3)
yt = data["lineC"].to_numpy()
fig, ax = plt.subplots()
fig, ax = plt.subplots()

# # Using set_dashes() to modify dashing of an existing line
line1,=ax.plot(x,y,label="Curva de colector")
line2,=ax.plot(x,yt,label="Recta de carga")
plt.legend()

ym = yt - y
if np.any(ym == -0.02277151):
    print("go")
    plt.plot(x,y,'ro')
print(x)
print(ym)
# # ax.legend()
plt.title('Ic vs VCE')
plt.xlabel('VCE (V)')
plt.ylabel('Ic (mA)')
# #plt.plot(8.5408,1.63213,'ro') 
plt.show()