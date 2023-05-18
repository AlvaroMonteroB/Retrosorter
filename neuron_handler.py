import image_handler as ih
import numpy as np
import copy
import os

class result:
    def __init__(self,nombre,relacion) -> None:
        self.nombre=nombre
        self.relacion=relacion

class file_weight:
    def __init__(self,vector:list(),name,threshold:int):
        self.vector=vector
        self.name=name
        self.threshold=threshold

class sum_thresh:
    def __init__(self,sum,thresh,file):
        self.sum=sum
        self.thresh=thresh
        inicio = file.rfind("/") + 1
        if inicio==0:
            inicio = file.rfind("\\") + 1
        fin = file.rfind(".")
        parte_deseada = file[inicio:fin]
        self.name=parte_deseada
        
class convolution_filter():
    def __init__(self,height,width,filter:np.array,threshold) -> None:
        self.height=height
        self.width=width
        self.filter=filter
        self.threshold=threshold


class Percept:
    def __init__(self,filenames,thresholds):#Ambos son una lista de los nombres de los archivos
        self.weight_file=list()
        for file in filenames:#Iteramos entre los nombres de  archivos de pesos
            print("Archivo "+file+'\n')
            with open(file, 'r') as f: #leemos cada archivo y dividimos todo
                line = f.read()
                numbers = line.split()
                weight = [int(num) for num in numbers]#Calculamos el arreglo de pesos
            f.close()
            for thresh in thresholds:#Vemos entre los thresholds cual es el que vamos a usar
                aux=os.path.basename(thresh)
                print("Umbral "+aux+"\n")
                fin = thresh.rfind(".")
                parte_deseada = aux[0:fin]
                if parte_deseada in file:#Si el nombre de nuestro archivo de pesos comparte una cadena con el threshold, lo usamos
                    with open(thresh,'r') as s:
                        line= s.readline()
                        line = line.rstrip('\n')
                        thr=int(line)
                        self.weight_file.append(file_weight(weight, os.path.basename(file),thr))#Ahora guaramos el vector de pesos, el nombre del archivo y el threshold
                        s.close()
                        break

            

    def neuron(self,input):#Input tienen que ser los bytes
        resultado=list()
        buffer=np.frombuffer(input, dtype=np.uint8)
        if not len(self.weight_file[0].vector)==len(buffer):
            print("Imagen invalida")
            exit
        for aux1 in self.weight_file:#guarda el vector, su nombre y el threshold
            peso=0
            for i in range(len(aux1.vector)):
                peso+=aux1.vector[i]*buffer[i]
            resultado.append(sum_thresh(peso, aux1.threshold,aux1.name ))
            probable_result=list()
        for cell in resultado:#Lista de los resultantes de la multiplicacion de matrices
            if cell.sum>cell.thresh:#Si el peso supera al umbral, se dispara
                probable_result.append(cell)
            else:
                print("El umbral es "+str(cell.thresh)+" y la suma es "+str(cell.sum))
                continue
        if len(probable_result)==0:
            return None
        elif len(probable_result)>0:#Si hubo por lo menos 1, se armará la lista
            relaciones=list()
            for res in probable_result:
                a:double=(100/res.thresh)*res.sum
                relaciones.append(result(res.name,a))       
            self.output= max(relaciones,key=lambda obj:obj.relacion)
            return self.output
    
    
    def convolution_neuron(files, object:ih.Image,thresh_files):#Path de donde esté la carpeta de caracteristicas y imagen del objeto
        filters=list
        for filter_f in files:#Vamos a iterar en los archivos de filtro convolucional
            
            with open(filter_f,'r') as conv:
                line=conv.readline()#Esta linea lee las caracteristicas basicas, altura y anchura del filtro
                numbers = line.split()
                features = [int(num) for num in numbers]#Guardamos el height y el width
                
                buffer=conv.read()
                buffer1=buffer.split()
                mat=[int(num) for num in buffer1]
                mat_np=np.array(mat)#Leemos el buffer y almacenamos todo como numpy array
                
                for threshold in thresh_files:
                    aux=os.path.basename(threshold)
                    print("Umbral "+aux+"\n")
                    fin = threshold.rfind(".")
                    parte_deseada = aux[0:fin]
                    
                    if parte_deseada in filter_f:
                        
                        with open(threshold,'r') as s:  
                            line= s.readline()
                            line = line.rstrip('\n')
                            thr=int(line)
                            s.close()   
                        filters.append(convolution_filter(features[0],features[1],mat_np.reshape(features[0],features[1]),thr))#Almacenamos sus dimensiones y la matriz
                        break
                conv.close()
        image = object.imagen
        for matrix in filters:#Matrix es la caracteristica a convolucionar, tiene height y width, y la matriz de convolucion en si
            pixels=matrix.filter
            r1=int(matrix.height//2)
            r2=int(matrix.weight//2)

            for y in range(r1,object.height-r1):
                
                for x in range(r2,object.width-r2):#Estos 2 calculan el centro de la matriz de convolucion
                    sum=0
                    for i in range(-r1,r1):
                        
                        for j in range(-r2,r2):#Se mueve en las 2 direcciones del filtro
                           sum+=  image[x+j,y+i]*matrix.filter[j,i]
                if sum>matrix.threshold:
                    break    

                







