import image_handler as ih
import numpy as np
import copy

class file_weight:
    def __init__(self,vector,name,threshold:int):
        self.vector=vector
        self.name=name
        self.threshold

class sum_thresh:
    def __init__(self,sum,thresh,file):
        self.sum=sum
        self.thresh=thresh
        inicio = file.rfind("/") + 1
        fin = file.rfind(".")
        parte_deseada = file[inicio:fin]
        self.name=parte_deseada


class Percept:
    def __init__(self,filenames,thresholds):#Ambos son una lista de los nombres de los archivos
        self.weight_file=list()
        for file in filenames:#Iteramos entre los nombres de  archivos de pesos
            with open(file, 'r') as f: #leemos cada archivo y dividimos todo
                line = f.read()
                numbers = line.split()
                weight = [int(num) for num in numbers]#Calculamos el arreglo de pesos
            f.close()
            for thresh in thresholds:#Vemos entre los thresholds cual es el que vamos a usar
                inicio = thresh.rfind("/") + 1
                fin = thresh.rfind(".")
                parte_deseada = thresh[inicio:fin]
                if parte_deseada in file:#Si el nombre de nuestro archivo de pesos comparte una cadena con el threshold, lo usamos
                    with open(thresh,'r') as s:
                        line= s.readline()
                        line = line.rstrip('\n')
                        thr=int(line)
                        self.weight_file.append(file_weight(weight, file,thr))#Ahora guaramos el vector de pesos, el nombre del archivo y el threshold
                        s.close()
                        break

            

    def neuron(self,input):#Input tienen que ser los bytes
        result=list()
        input=np.frombuffer(input, dtype=np.uint8)
        if len( not self.weight_file[0])==len(input):
            print("Imagen invalida")
            exit
        for aux1 in self.weight_file:#guarda el vector, su nombre y el threshold
            peso=0
            for i in range(len(aux1.vector)):
                peso+=aux1.vector[i]*input[i]
            result.append(sum_thresh(peso, aux1.thresh,aux1.name ))
        return result

        
