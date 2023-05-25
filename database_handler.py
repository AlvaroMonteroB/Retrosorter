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
    def get_cell(self, iterador, posicion:int):#Hacer un select directo a una celda, iterador=y, posicion=x
        if iterador<self.pattern and iterador>=0 and posicion<self.campos and posicion>=0 and self.type_list!=None:
            return switch_type[self.type_list[posicion]](self.data[iterador][posicion])
        else:
            return None

    def get_row(self,iterador):#Devuelve el renglon pero con los cast adecuados
        if iterador<self.pattern and iterador>=0 and self.type_list!=None:
            aux= self.data[iterador]
            row=list()
            for i in range(self.pattern):#switchtype== tipo de dato ,self type==int, aux==dato a castear
                row.append(switch_type[self.type_list[i]](aux[i]))#ejem switchtpe[self.typelist[i]]=1 ||int(aux[i])
        else:
            return None
    
    def get_row_by_name(self,name, x): #El iterador es para el eje x ahora
        if iterador<self.campos:
            for row,i in self.data,range(len(self.data)):
                if name==self.get_cell(i, x):#Si el nombre que buscamos coincide en la columna crrecta
                    output=list()
                    for j in range(self.pattern):
                        output.appnd(switch_type[self.type_list[j]](row[j]))
                        return output
                else:
                    continue

        if len(output)==0:
            return None
                




switch_type={
    1:int,
    2:float,
    3:str,
    4:bool
}