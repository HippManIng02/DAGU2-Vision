# Nom du fichier exécutable à générer
TARGET = vision

# Compilateur
CC = g++
# Options de compilation
CFLAGS = -std=c++11 -Wall

# Dossiers d'inclusion et de bibliothèque OpenCV
INC_DIRS = -I /usr/local/include/opencv4
LIB_DIRS = -L /usr/local/lib
LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio

# Fichiers source
SRCS = vision.cpp
# Fichiers objets générés à partir des sources
OBJS = $(SRCS:.cpp=.o)

# Règle par défaut : exécutable dépend de tous les objets
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB_DIRS) $(LIBS)

# Règle pour générer les fichiers objets
%.o: %.cpp
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
