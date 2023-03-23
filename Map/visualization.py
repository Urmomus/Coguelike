import tkinter as tk
from tkinter import Tk, Canvas

# здесь импортируем библиотеки, с помощью которых будем создавать карту
import prototype

# константы
cells_x = 25    # размер ячейки в ширину
cells_y = 25    # размер ячейки в длину

# принимает на вход двумерный массив нулей и единиц -- Map
# Где 0 -- рисует пространство
# Где 1 -- рисует стенку
def print_map(Map):
    window = tk.Tk()    # создаём окно ТKinter
    
    # определяем размеры переданного массива
    size_y = len(Map)
    size_x = len(Map[0])
    
    # меняем размер окна TKinter
    window.geometry('{}x{}+200+100'.format(size_x * cells_x, size_y * cells_y))
    
    # создаём Canvas, где будем отрисовывать карту
    canvas = Canvas(window, width = size_x * cells_x, height = size_y * cells_y)
    
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
    window.mainloop()

    
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

if __name__ == "__main__":
    #test_map = prototype.generate_map(130, 0.36, 3, 4, 5)
    test_map = read_map_from_file('test_map.txt')
    print_map(test_map)
