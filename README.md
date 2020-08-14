# sweptvolumes
Calculating the surface boundary of the swept volume of a rigid body.
## About
This is the implementation for the thesis **Hüllkörperberechnung bewegter Starrkörper**.

This implementation was written in C++ and uses the header only library Eigen. All relevant Eigen files are in the **include** directory.

It can be run as follows:

`> ./swept_volumes [object] [motion] {OPTIONS}`

**[object]**: the object that should be swept (.obj)

**[motion]**: the trajectory along which it is swept (.csv)

**{OPTIONS}**: flags which can be used to set configurable parameters, use `-h` or `--help` to see all flags

The object defined in the .obj file has to be triangulated and the trajecotry (.csv) has to be a list of homogeneous matrices
