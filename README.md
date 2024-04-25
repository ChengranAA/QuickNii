# QuickNii - Quick and easy NIfTI file viewer

QuickNii is a simple NIfTI file viewer that allows you to quickly view NIfTI files in a simple and easy to use interface. It is designed to be a lightweight (only about 3MB!) and fast viewer that can be used to quickly view NIfTI files without the need for a full-featured medical imaging viewer.

<img src="./README.assets/demo.png" alt="demo" style="zoom:25%;" />


## Features

- Simple and easy to use interface
- Fast and lightweight
- Supports viewing NIfTI files in both 3D and 4D (Work in progress, 3D support at the moment, can view the first volume of 4D data)
- Supports viewing BrainVoyager files (vmr, fmr) (Work in progress, vmr support at the moment)
- Static Linked (No need to install any dependencies)

## Usage
```
quicknii <NIfTI file/ VMR file> // that's it
```

## Development
Clone the repository and run the following commands:
```
chmod +x init.sh && ./init.sh
make
```
The init.sh script will download the necessary dependencies. The make command will build the project and create the QuickNii executable in the build directory.

To install the executable, run the following command:
```
make install
```
It require sudo user privileges to install the executable.

## License
MIT License
