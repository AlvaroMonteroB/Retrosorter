import numpy as np


file="D:\Repositorios\Retrosorter\weights\weight_data_ps1.txt"
with open(file,'r') as raw:
     text=raw.read()
     numbers = text.split()
     weight = [int(num) for num in numbers]#Calculamos el arreglo de pesos
     upper=max(weight)
     print(str(upper)+"\n")
     for i in weight:
          i=int((i/upper)*10)
     raw.close
with open(file,'w') as stream:
     for i in weight:
          stream.write(str(i)+" ")
     stream.close()
     
print("Programa terminado")
