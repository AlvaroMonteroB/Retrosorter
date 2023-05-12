import image_handler as ih
import numpy as np
import copy

class file_weight:
    def __init__(self,vector,name,threshold):
        self.vector=vector
        self.name=name
        self.threshold

class sum_thresh:
    def __init__(self,sum,thresh,file):
        self.sum=sum
        self.thresh=thresh
        inicio = file.rfind("/") + 1
        fin = file.rfind(".")
        parte_deseada = direccion[inicio:fin]
        self.name=parte_deseada


class Percept:
    def __init__(self,filenames,thresholds):#Ambos tienen que ser carpetas raiz
        self.weight_file=list()
        for file in filenames:#Iteramos entre los archivos de pesos
            with open(file, 'r') as f:
                line = f.readline()
                numbers = line.split()
                weight = [int(num) for num in numbers]#Calculamos el arreglo de pesos
                inicio = file.rfind("/") + 1
                fin = file.rfind(".")
                parte_deseada = direccion[inicio:fin]
                for thresh in thresholds:#Vemos entre los thresholds cual es el que vamos a usar
                 if parte_deseada in thresh:#Si el nombre de nuestro archivo de pesos comparte una cadena con el threshold, lo usamos
                    with open(thresh,'r') as s:
                        line= s.readline()
                        line = line.rstrip('\n')
                        thr=int(line)
                        weight_file.append(file_weight(weight, file,line))
                        break

            f.close()

    def neuron(self,input:list()):#Input tienen que ser los bytes
        result=list()
        for aux1 in self.weight_file:#guarda el vector, su nombre y el threshold
            peso=0
            for i in range(len(aux1.vector)):
                peso+=aux1.vector[i]*input[i]
            result.append(sum_thresh(peso, aux1.thresh,aux1.name ))
        return result

        
