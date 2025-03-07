## Install dependencies
```
make install
```

## Build the game
```
make
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

e.g. Running the game with no sounds, for mode 11 of display 1, and in full screen:
```
./asteroids --no-sound --show-fps --display=1 --display-mode=11 --window-mode=1
```

### Window modes
The game can run either as a maximised window (0, default) or in full screen (1). 
e.g. Running maximised:
```
./asteroids --window-mode=0
```
