CC=g++
EXE_FILE=run_wizard

all: $(EXE_FILE)

$(EXE_FILE): catalog.o catalog.h run_wizard.cpp
	$(CC) catalog.o run_wizard.cpp -o $(EXE_FILE)
  
catalog.o: catalog.h catalog.cpp
	$(CC) -c catalog.cpp
  
clean:
	rm -f *.o $(EXE_FILE)