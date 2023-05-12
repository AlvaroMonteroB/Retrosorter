from tkinter import *
import image_handler as ih
def ventana_principal():#La ventana principal tendra los botones para procesar una carpeta y escribir el txt de entrenamiento
    
    def procesar():
        path_img=casilla1.get()
        path_weight=casilla2.get()
        backend_funct(path_img, path_weight)
        mensaje.config(text="Entrenamiento concluido")
    
    def most_img():
        path_img=casilla1.get()
        #list_name=get_files(path_img)
        img=ih.Image()
        img.read_img("C:/Users/diavl/OneDrive/Escritorio/Repositorios/Inv_proj1/Camera_data/train_data/camara1.bmp", 0)
        mensaje.config(text="Imagen leida")
        img.show_image()

    
    def borrar_texto(event):
        casilla1.delete(0, "end")
    def borrar_texto2(event):
        casilla2.delete(0,"end")

    ventana = Tk()
    ventana.title("Interfaz de entrenamiento")
    ventana.geometry("600x400")
    ventana.configure(bg="#FDF6FF")


    casilla1 = Entry(ventana)
    casilla1.insert(0, "Training data(Folder)")
    casilla1.bind("<FocusIn>",borrar_texto)
    casilla1.pack()

    casilla2 = Entry(ventana)
    casilla2.insert(0, "Weight_file(txt)")
    casilla2.bind("<FocusIn>",borrar_texto2)

    casilla2.pack()

    # Crear botones
    boton_procesar = Button(ventana, text="Procesar", command=procesar)
    boton_procesar.pack()

    boton_imagen = Button(ventana, text="Mostrar imagen", command=most_img)
    boton_imagen.pack()

    mensaje=Label(ventana,text="")
    mensaje.pack()

    ventana.mainloop()



def backend_funct(root_path,train_name):#Recibimos la lista de los nombres de las imagenes para procesarlos y escribir el resultante
    Data=ih.Image()
    
    list_name=get_files(root_path)
    Data.read_img(list_name, 0, byte)
    weight=[0]*(Data.height*Data.width*Data.mode)
    for name in list_name:#Escribiendo el vector de pesos
        Data.read_img(name, 0, byte)
        bytess=Data.byte
        for i,byte in enumerate(bytess):
            if byte==255:
                weight[i]+=1


  
    with open(train_name,"w") as f:#Guardando el vector de pesos en un archivo
        for vals in weight:
            f.write(str(vals)+" ")
    return 0

def get_files(root_path):
    archivos=ih.os.listdir(root_path)
    return archivos