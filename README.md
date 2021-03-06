# raycast-engine

Partial implementation of the "Wolfenstein 3D engine".
---------
## MacOS - ![Build](https://github.com/dolovnyak/raycast-engine/actions/workflows/c-cpp.yml/badge.svg?branch=master)

### [Download demo for windows](https://github.com/GodFlight/Wolf3D/raw/master/raycast_demo.zip)

Build and run on `MacOS`:
------------
```
make
./wolf3d

# You can change ./json/config.json to setup game with another resolution/map/textures
```

Controls
---------
`ESC` - Close the program.  
`W`/`A`/`S`/`D` - Movement in space.    
`RIGHT` - Camera rotation to the right.  
`LEFT` - Camera rotation to the left.  
`SHIFT` - Acceleration.   
`SPACE` - Turn on/off music.  

Map file format
-----------
```
1   1   1   1  
1   910 9   1  
1   0   0   1  
1   999 0   1  
1   1   1   1  
```

`0`         - **void, wall/flr cast ignore it, physic ignore it**.  
`1`-`99`    - **default walls without states and interactions, can be place several identical id**.  
`900`-`998` - **objects**.  
`999`       - **player position**.

Screenshots:
-----------
![Image alt](https://github.com/GodFlight/Wolf3D/raw/master/image/1.png)
![Image alt](https://github.com/GodFlight/Wolf3D/raw/master/image/2.png)
![Image alt](https://github.com/GodFlight/Wolf3D/raw/master/image/3.png)
