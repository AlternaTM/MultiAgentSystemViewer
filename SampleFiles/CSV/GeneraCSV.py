import csv
import random

# Genera valori per il CSV
def generate_csv(filename):
    shapes = ["cube", "sphere", "cone"]  # Inserire i path dei file obj come segue: r"path/per/il/file/obj"
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        # Scrive l'header del CSV
        writer.writerow(["timestamp", "x", "y", "z", "color", "mesh"])
        
        for i in range(20):
            timestamp = i + 1  # Timestamp progressivo (può partire da 0 e non essere progressivo)
            x = random.randint(-400, 400)  # Valore casuale per x
            y = random.randint(-400, 400)  # Valore casuale per y
            z = random.randint(-400, 400)  # Valore casuale per z
            color = "#{:06x}".format(random.randint(0, 0xFFFFFF))  # Colore esadecimale casuale
            shape = random.choice(shapes)  # Forma casuale
            
            # Scrive la riga nel CSV
            writer.writerow([timestamp, x, y, z, color, shape])

# Esegui la funzione per creare 10 file CSV diversi
for i in range(10):
    filename = f'output_{i+1}.csv'
    generate_csv(filename)