# QuickNii
A NIFTI viewer written in OpenGL (only MacOS for now). 

<img width="1047" alt="Screenshot 2023-09-19 at 5 25 25 AM" src="https://github.com/ChengranAA/QuickNii/assets/56989661/56f0fa88-9499-41e3-94d9-2ecd3e038c79">

# Keyboard Function Usage

| Key           | Action                                               |
| ------------- | ---------------------------------------------------- |
| 'q', 'Q', \<Esc\>  | Exit the application |
| 'a'           | Decrease slice index by 1       |
| 'A'           | Decrease slice index by 10 (speed mode) |
| 'd'           | Increase slice index by 1      |
| 'D'           | Increase slice index by 10 (speed mode) |
| 'x', 'X'      | Toggle display of slice index on/off                |
| 'h', 'H'      | Toggle visibility of a header window                |
| 'r'           | Rotate displayed matrix 90 degrees counterclockwise |
| 'R'           | Rotate displayed matrix 90 degrees clockwise       |

## Build from source
```
make all # to build
make cleanall # rm everything
```
