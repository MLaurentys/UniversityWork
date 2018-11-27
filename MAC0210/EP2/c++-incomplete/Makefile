CC = g++
.PHONY: clean run

LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lgsl -lgslcblas -lm

APPLICATION = sfml-app

INCLUDE_DIR = -I include/
SOURCE_DIR  = source
OUTPUT_DIR  = bin

SOURCES     = $(notdir $(wildcard $(SOURCE_DIR)/*.cpp))
OBJECTS     = $(addprefix $(OUTPUT_DIR)/, $(patsubst %.cpp, %.o, $(SOURCES)))

run: sfml-app
	./bin/sfml-app

sfml-app: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $(OUTPUT_DIR)/$(APPLICATION)

$(OUTPUT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) -c $< $(INCLUDE_DIR) -o $@

clean:
	rm $(OUTPUT_DIR)/*

print-%  : ; @echo $* = $($*)
