# psp2spvc - An experimental shader compiler for the Playstation Vita.

psp2spvc is a program that translates between the SPIRV and GXP formats.
The goal of the project is to provide an easy, open-source way to generate GXP shaders for the Playstation Vita.

gxmfun shader compatibility:
 - [x] clear_f
 - [x] clear_v
 - [ ] color_f
 - [ ] color_v
 - [ ] cube_f
 - [ ] cube_v

The project is still early into its development, shaders can be unstable and many features are not yet implemented.
If you found a problem or would like to request a feature, feel free to make an [issue](https://github.com/1whatleytay/psp2spvc/issues).

### GLSL
[glslangValidator](https://github.com/KhronosGroup/glslang) is the only GLSL compiler verified to work.
Other GLSL compilers may be supported in the future.

Compile your GLSL as follows:
```shell script
glslangValidator path/to/input.glsl -V -o path/to/output.spv
```
A Vulkan target is recommended.

#### Uniforms
As a workaround to the Vulkan target, uniforms in the default buffer should be defined as follows:
```glsl
uniform export_name { type self; } reference_name;
```
`export_name` is what will be visible in the binary. What will be used for `sceGxmProgramFindParameterByName`.

`reference_name` will be the name used in the rest of the shader.

`type` is the type of the uniform. It can be a struct.

You can refer to the uniform as follows: `reference_name.self`.
OpenGL target/uniform syntax will be provided in the future (minor tweaking is needed).

## Usage
psp2spvc must be provided with one input SPIRV file and one output GXP path.
The path to the input file should be provided as an argument.
The path to the output file must also be provided as argument but must be preceded by `-o`.
```shell script
psp2spvc path/to/input.spv -o path/to/output.gxp
```

### Options
Options can be provided as arguments to modify build or print debug information.

Option | Description
--- | ---
-S | Print shader assembly.
-A | Print allocation messages.
-L | Print other debug messages.
-Oreg-space | Enable register space optimization. Required for larger shaders.

## Building
You need [CMake](https://cmake.org/) to build the project.
The following commands will prepare and build the project on UNIX based systems.
```shell script
git submodule update --init --recursive
mkdir build
cmake ..
make
```

No prebuilt binaries are provided.
