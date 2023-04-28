import random

# некоторые константы
FREE = 0    # пустая клетка
WALL = 1    # стена

def cnt_neighbours(x, y, Map):
# считает кол-во соседей у клетки (x, y) на карте Map
    cnt = 0             # кол-во соседей у клетки
    size = len(Map)     # размер карты
    
    for i in range(-1, 2):
        for j in range(-1, 2):
            neigbour_x = x + j
            neigbour_y = y + i
            
            if (neigbour_x, neigbour_y) == (x, y):
                # саму клетку не считаем!
                continue
            
            if (neigbour_x < 0) or (neigbour_y < 0) or (neigbour_x >= size) or (neigbour_y >= size):
                # если сосед -- край карты, то засчитываем
                cnt += 1
                continue
            
            if Map[neigbour_x][neigbour_y]:
                # если сосед -- стенка, то засчитываем
                cnt += 1
    
    return cnt


def next_step(Map, dl, bl):
# переводит ячейки карты в следующее состояние в соответствии с базовыми правилами
    size = len(Map)
    new_Map = [[FREE] * size for i in range(size)]
    
    for x in range(size):
        for y in range(size):
            num_of_neighbours = cnt_neighbours(x, y, Map)
            if Map[x][y]:
                if num_of_neighbours < dl:
                    new_Map[x][y] = FREE
                else:
                    new_Map[x][y] = WALL
            else:
                if num_of_neighbours > bl:
                    new_Map[x][y] = WALL
                else:
                    new_Map[x][y] = FREE
    return new_Map[::]




def generate_map(size, chance, dl, bl, steps):
# возвращает двумерный массив формата: 0 -- свободно, 1 -- стена
# массив является картой, созданной на основе небезызвестной игры "жизнь"
# size      -- размеры size x size массива
# chance    -- вероятность, что при исходной генерации клетка будет являться единицей (стеной)
# dl        -- death_limit
# bl        -- birth_limit
# steps     -- кол-во обновлений карты

    random.seed()
    
    # создаём массив из нулей (стен пока нет)
    result = [[FREE] * size for i in range(size)]
    
    # расставим стены на рандоме
    for i in range(size):
        for j in range(size):
            if random.random() < chance:
                result[i][j] = WALL
    
    for i in range(steps):
        result = next_step(result, dl, bl)
        
    
    # возвращаем сгенерированную карту
    return result