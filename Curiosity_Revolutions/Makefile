CC=clang 

PROGRAMMES=test_terrain test_robot robot_terrain curiosity curiosity-test test_generation_terrains curiosity-perf curiosity-obs

all: $(PROGRAMMES)

test_terrain: test_terrain.o terrain.o

test_robot: test_robot.o robot.o
    
robot_terrain: robot_terrain.o terrain.o robot.o
    
curiosity: curiosity.o environnement.o programme.o interprete.o \
    robot.o terrain.o type_pile.o observateur.o
    
curiosity-test: curiosity-test.o environnement.o programme.o interprete.o \
    robot.o terrain.o type_pile.o observateur.o
    
curiosity-test%: curiosity-test.o environnement.o programme.o interprete%.o \
    robot.o terrain.o type_pile.o observateur.o

test_generation_terrains: test_generation_terrains.o generation_terrains.o terrain.o

curiosity-perf: curiosity-perf.o generation_terrains.o environnement.o programme.o interprete.o \
    robot.o terrain.o type_pile.o observateur.o

curiosity-obs: curiosity-obs.o environnement.o programme.o interprete.o \
    robot.o terrain.o type_pile.o observateur_exo5.o

clean:
	rm -f $(PROGRAMMES) *.o