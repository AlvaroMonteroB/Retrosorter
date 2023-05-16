from tkinter import *
import image_handler as ih
import neuron_handler as nh
import os

class result:
    def __init__(self,nombre,relacion) -> None:
        self.nombre=nombre
        self.relacion=relacion


def ventana_principal(counter):#La ventana principal tendra los botones para procesar una carpeta y escribir el txt de entrenamiento
    AI=None
    def procesar():
        path_img=casilla1.get()#imagen a comparar
        path_weight="D:\Repositorios\Inv_proj1\weights"#direccion de los pesos     //D:\Repositorios\Inv_proj1\weights  //C:/Users/diavl/OneDrive/Escritorio/Repositorios/Inv_proj1/weights
        path_threshold="D:/Repositorios/Inv_proj1/thresholds" #direccion de umbrales
        weight=get_files(path_weight)#Guardamos las direcciones completas de todos los archivos de pesos
        ventana_emergente(str(len(weight))+" pesos guardados")
        thresholds=get_files(path_threshold)#Direcciones completas de los archivos de umbral
        AI=nh.Percept(weight,thresholds)
        print(str(len(AI.weight_file))) 
        bandera=0
        result=backend_process(AI,path_img,bandera)
        if bandera>0 or result==None:
            mensaje.config(text="No se encontro al objeto dentro de la clasificacion")
            return
        else:
            mensaje.config(text="Tu objeto es "+result.nombre)
        


    def ventana_emergente(string):
        ventana_e=Toplevel()
        ventana_e.title("Aviso")
        etiqueta=Label(ventana_e,text=string)
        etiqueta.pack()

    
    def borrar_texto(event):
        casilla1.delete(0, "end")


    ventana = Tk()
    ventana.title("Interfaz de Operaciones")
    ventana.geometry("320x100")
    ventana.configure(bg="#FDF6FF")


    casilla1 = Entry(ventana)
    casilla1.insert(0, "Imagen")
    casilla1.bind("<FocusIn>",borrar_texto)
    casilla1.pack()

    

    # Crear botones
    boton_procesar = Button(ventana, text="Procesar", command=procesar)
    boton_procesar.pack()


    mensaje=Label(ventana,text="")
    mensaje.pack()

    ventana.mainloop()
    
    
    
def backend_process(AI:nh.Percept,input,bandera):
    img=ih.Image()
    img.read_img(input,0,504,378)
    result=AI.neuron(img.byte)#result list->SUM_THRESH// sum,thresh, name
    return result
      

def convolution_process(resultado:result,path):#Va a ser una matriz de convolucion que recorra la imagen para encontrar caracteristicas
    folders=get_folder(path)
    for folder in folders:#Buscamos en el folder del objeto que se reconocio para obtener los pesos de las marcas
        files=get_files(folder)
        
                
    
    
def get_files(root_path):
    archivos = os.listdir(root_path)
    rutas_completas = []

    for archivo in archivos:
        ruta_completa = os.path.join(root_path, archivo)
        if os.path.isfile(ruta_completa):
            rutas_completas.append(ruta_completa)

    return rutas_completas

def get_folder(root_path):
    archivos = os.listdir(root_path)
    carpetas = []

    for archivo in archivos:
        ruta_completa = os.path.join(root_path, archivo)
        if os.path.isdir(ruta_completa):
            carpetas.append(archivo)

    return carpetas