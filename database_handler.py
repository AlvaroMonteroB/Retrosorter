import easyocr as esr
import csv


class database:
    def __init__(self,path):
        with open(path,'r') as f:
            self.data=list(csv.reader(f))
        self.campos=len((self.archivo[0]))#Numero de columnas
        self.pattern=len(list(self.archivo))#numero de filas