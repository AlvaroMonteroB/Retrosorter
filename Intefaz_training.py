import tkinter
import image_handler as ih
def ventana_principal():#La ventana principal tendra los botones para procesar una carpeta y escribir el txt de entrenamiento
    def procesar():
        global names
    ventana = Tk()
    ventana.title("Interfaz de entrenamiento")
    ventana.geometry("600x400")
    ventana.configure(bg="#FDF6FF")


def backend_funct(list_name:list(),train_name):#Recibimos la lista de los nombres de las imagenes para procesarlos y escribir el resultante
    Data=ih.Image()
    for name in list_name:
        Data.read_img(name, 0, byte)




    weight=list()
    with open(train_name,"w") as f:
        for vals in weight:
            f.write(str(vals)+" ")