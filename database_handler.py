import csv


class database:
    def __init__(self,path):
        with open(path,'r') as f:
            self.data=list(csv.reader(f))
        self.campos=len((self.archivo[0]))#Numero de columnas
        self.pattern=len(list(self.archivo))#numero de filas
    
    
    def auto_set(self):
        types=list()
        aux=self.archivo[0]
        for column,i in aux,range(len(aux)):
            if '.' in column:
                types.append(2)
            elif column.isdigit():
                count=0
                for col in self.archivo:#Vamos a determinar si es bool
                    if col[i]!=0 and  col[i]!=1:
                        count=+1
                        if count>0:
                            break
                if count==0:
                    types.append(4)
                else:
                    types.append(1)
            else:
                types.append(3)
        self.campos=types


    def get_row(self,iterador):#Devuelve el renglon pero con los cast adecuados
        if iterador<self.pattern and iterador>=0 and self.type_list!=None:
            aux= self.archivo[iterador]
            row=list()
            for i in range(self.pattern):
                row.append(switch_type[self.type_list[posicion]](aux[i]))                
        else:
            return None




switch_type={
    1:int,
    2:float,
    3:str,
    4:bool
}