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
        path_weight=casilla2.get()#direccion de los pesos
        path_threshold=casilla3.get()#direccion de umbrales
        weight=get_files(path_weight)
        thresholds=get_files(path_threshold)
        AI=nh.Percept(weight,thresholds) 
        mensaje.config(text="Ya no necesitas los pesos y los umbrales")
    
             
        result=backend_process(AI,path_img)
        mensaje.config(text="Tu objeto es "+result.nombre)


    
    def borrar_texto(event):
        casilla1.delete(0, "end")
    def borrar_texto2(event):
        casilla2.delete(0,"end")
    def borrar_texto3(event):
        casilla3.delete(0,"end")

    ventana = Tk()
    ventana.title("Interfaz de entrenamiento")
    ventana.geometry("320x100")
    ventana.configure(bg="#FDF6FF")


    casilla1 = Entry(ventana)
    casilla1.insert(0, "Imagen")
    casilla1.bind("<FocusIn>",borrar_texto)
    casilla1.pack()

    casilla2 = Entry(ventana)
    casilla2.insert(0, "Pesos")
    casilla2.bind("<FocusIn>",borrar_texto2)
    casilla2.pack()
    
    casilla3=Entry(ventana)
    casilla3.insert(0,"Umbrales")
    casilla3.bind("<FocusIn>",borrar_texto3)
    casilla3.pack()
    

    # Crear botones
    boton_procesar = Button(ventana, text="Procesar", command=procesar)
    boton_procesar.pack()


    mensaje=Label(ventana,text="")
    mensaje.pack()

    ventana.mainloop()
    
    
    
def backend_process(AI:nh.Percept,input):
    img=ih.Image()
    img.read_img(input,0,504,378)
    result=AI.neuron(img.byte)
    probable_result=list()
    for row in result:#Lista de los resultantes de la multiplicacion de matrices
        if row.peso>row.threshold:
            probable_result.append(row)
    relaciones=list()
    for res in probable_result:
      a=(100/res.threshold)*res.peso
      relaciones.append(result(res.name,a))
    return max(relaciones,key=lambda obj:result.relacion)
      

            
    
    
def get_files(root_path):
    archivos = os.listdir(root_path)
    rutas_completas = []

    for archivo in archivos:
        ruta_completa = os.path.join(root_path, archivo)
        rutas_completas.append(ruta_completa)

    return rutas_completas