from tkinter import *
import image_handler as ih
import neuron_handler as nh
import os
import database_handler as dh




def ventana_principal(counter):#La ventana principal tendra los botones para procesar una carpeta y escribir el txt de entrenamiento
    AI=None
    def procesar():
        path_img=casilla1.get()#imagen a comparar
        path_weight="D:\Repositorios\Inv_proj1\weights"#direccion de los pesos     //D:\Repositorios\Inv_proj1\weights  //C:/Users/diavl/OneDrive/Escritorio/Repositorios/Inv_proj1/weights
        path_threshold="D:/Repositorios/Inv_proj1/thresholds" #direccion de umbrales  D:/Repositorios/Inv_proj1/thresholds  //C:/Users/diavl/OneDrive/Escritorio/Repositorios/Inv_proj1/thresholds
        weight=get_files(path_weight)#Guardamos las direcciones completas de todos los archivos de pesos
        ventana_emergente(str(len(weight))+" pesos guardados")
        thresholds=get_files(path_threshold)#Direcciones completas de los archivos de umbral
        AI=nh.Percept(weight,thresholds)
        print(str(len(AI.weight_file))) 
        bandera=0
        image=ih.Image()
        result=backend_process(AI,path_img,bandera,image)
        if bandera>0 or result==None:
            mensaje.config(text="No se encontro al objeto dentro de la clasificacion")
            image.show_image()
            return
        else:
            inicio=result.nombre.rfind("_")
            aux=result.nombre[inicio+1:]
            mensaje.config(text="Tu objeto es "+aux)
            image.show_image()
        database_process(aux)

            
        


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
    
    
    
def backend_process(AI:nh.Percept,input,bandera,img:ih.Image()):
    img.read_img(input,0,504,378)
    aux=img.canny(200,600)
    result=AI.neuron(aux.byte)#result list->SUM_THRESH// sum,thresh, name
    return result
      

def convolution_process(resultado:nh.result,path):#Va a ser una matriz de convolucion que recorra la imagen para encontrar caracteristicas
    folders=get_folder(path)
    for folder in folders:#Buscamos en el folder del objeto que se reconocio para obtener los pesos de las marcas
        files=get_files(folder)
        
def database_process(name):
    path=""
    #files=get_files(path)
    #for archivo in files:
    #    if name in archivo:
    #        dh.database(archivo)
    #        break
    dh.database("C:/Users/diavl/OneDrive/Escritorio/Repositorios/Inv_proj1/database/Base1.txt") #C:/Users/diavl/OneDrive/Escritorio/Repositorios/Inv_proj1/database/Base1.txt
    
    
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



switch_carpetas={#Es para la convolucion

}