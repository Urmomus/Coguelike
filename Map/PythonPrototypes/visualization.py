import tkinter as tk
from tkinter import Tk, Canvas

# здесь импортируем библиотеки, с помощью которых будем создавать карту
import prototype

# константы
cells_x = 8    # размер ячейки в ширину
cells_y = 8    # размер ячейки в длину

# объекты pygame
size_y = 100
size_x = 100
window = window = tk.Tk()    # создаём окно ТKinter
canvas = Canvas(window, width = size_x * cells_x, height = size_y * cells_y)

# принимает на вход двумерный массив нулей и единиц -- Map
# Где 0 -- рисует пространство
# Где 1 -- рисует стенку
def print_map(Map):
    
    # меняем размер окна TKinter
    window.geometry('{}x{}+200+100'.format(size_x * cells_x, size_y * cells_y))
    
    # отрисовываем карту
    for i in range(size_y):
        for j in range(size_x):
            color = 'gold2'
            if Map[i][j]:   # если ячейка является стеной -- меняем цвет на чёрный
                color = 'dark green'
                
            # рисуем ячейку
            canvas.create_rectangle(j*cells_x, i*cells_y, (j+1)*cells_x, (i+1)*cells_y,
            fill = color,
            outline = color
            )




    # запускаем всё вышеописанное мероприятие
    canvas.pack()

    
# считывает карту, которую надо отрисовать, из файла filename,
# и возвращает её же
def read_map_from_file(filename):
    
    # открываем входной файл
    f_in = open(filename, 'r')
    
    size_x, size_y = 0, 0 # размеры карты по (x, y)
    
    # считали размеры карты
    size_x = int(f_in.readline())
    size_y = int(f_in.readline())
    
    new_Map = [[0] * size_y for i in range(size_x)]
    for i in range(size_y):
        for j in range(size_x):
            type_of_cell = int(f_in.readline())
            new_Map[i][j] = type_of_cell
    
    # закрываем входной файл
    f_in.close()
    
    return new_Map

def read_objects_from_file(filename):
    # открываем входной файл
    f_in = open(filename, 'r')

    objects = []        # массив объектов, что лежат в файле
    num_of_chars = 3    # кол-во свойств на один предмет

    for ind, val in enumerate(f_in):
        if ind % num_of_chars == 0:     # это координата по х
            objects.append([])
            objects[ind//num_of_chars].append(int(val))

        if ind % num_of_chars == 1:     # это координата по y
            objects[ind//num_of_chars].append(int(val))
        
        if ind % num_of_chars == 2:     # это координата по y
            objects[ind//num_of_chars].append(val.replace('\n', ''))

    # закрываем входной файл
    f_in.close()

    return objects

# принимает на вход массив объектов;
# объект -- массив из трёх элементов (свойств):
#   0. координата по х
#   1. координата по y
#   2. тип (монстр али предмет)
# и отрисовывает их (карта должна быть уже отрисована)
def print_objects(objects):
    
    # отрисовываем объекты
    for x, y, type in objects:
            print(x,y,type)
            color = "cadetblue1"

            if type == "unit":
                color = 'brown1'
            
            if type == "item":
                color = "blue2"
            
            canvas.create_rectangle(x*cells_x, y*cells_y, (x+1)*cells_x, (y+1)*cells_y,
                fill = color,
                outline = color
                )

    # обновляем происходящее на экране
    canvas.pack() 

if __name__ == "__main__":
    #test_map = prototype.generate_map(130, 0.36, 3, 4, 5)
    test_map = read_map_from_file('test_map.txt')
    maps_objects = read_objects_from_file('objects.txt')
    print_map(test_map)

    input() # делаем паузу, чтобы можно было посмотреть сначала на голый ландшафт
    print_objects(maps_objects)
    
    window.mainloop()   # отображаем всё вышеотрисованное
