## Install dependencies
```
make install
```

## Build the game
```
make clean && make && rm *.o
```

### Show run options
```
./asteroids --help
```

## Run the game
```
./asteroids
```

### Run the game with custom options
Select the display and resolution from the `graphics-info` list:
```
./asteroids --graphics-info
```

e.g. Running the game with no sounds, showing the frame-per-second rate, for mode 11 of display 1:
```
./asteroids --no-sound --show-fps --display=1 --display-mode=11
```

### Window modes
The game can run either in full screen (0) or as a maximised window (1). The default is full screen:
e.g. Running maximised:
```
./asteroids --display=1 --window-mode=1
```
