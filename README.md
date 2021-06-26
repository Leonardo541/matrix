# Matrix Functions for SA-MP

https://youtu.be/vSZyMilgcuU

### Functions ([matrix.inc])

| return | function |
| ------ | ------ |
| VECTOR | Vector(Float:x, Float:y, Float:z); |
| MATRIX | Matrix(const right[VECTOR], const front[VECTOR], const up[VECTOR], const pos[VECTOR]); |
| MATRIX | Matrix_Invert(const matrix[MATRIX]); |
| | Matrix_BuildUp(matrix[MATRIX]); |
| | Matrix_SetRotation(matrix[MATRIX], Float:rx, Float:ry, Float:rz); |
| float | Float:Matrix_GetRotationX(const matrix[MATRIX]); |
| float | Float:Matrix_GetRotationY(const matrix[MATRIX]); |
| float | Float:Matrix_GetRotationZ(const matrix[MATRIX]); |
| VECTOR | Matrix_GetRotation(const matrix[MATRIX]); |
| VECTOR | Matrix_Multiply3x3(const matrix[MATRIX], const vector[VECTOR]); |
| MATRIX | Matrix_Multiply4x4(const matrix1[MATRIX], const matrix2[MATRIX]); |
| VECTOR | Matrix_GetOffsetPosition(const matrix[MATRIX], const vector[VECTOR]); |
| VECTOR | Matrix_GetOffsetRotation(const matrix[MATRIX], const vector[VECTOR], bool:invert = true); |

### Functions ([matrix_example.inc])

| return | function |
| ------ | ------ |
| MATRIX | GetVehicleMatrix(vehicleid); |
| | SetVehicleMatrix(vehicleid, mat[MATRIX]); |
| VECTOR | GetVehicleRot(vehicleid); |
| | SetVehicleRot(vehicleid, rot[VECTOR]); |
| MATRIX | GetVehicleVelocityFrontVector(vehicleid) |
| | SetVehicleVelocityFrontVector(vehicleid, vec[VECTOR]); |

### Commands ([matrix_example1.pwn])

| command | description  |
| ------ | ------ |
| /matrix | Start/Stop comparison between matrix, attach and i_quat |
| /attach | Attach object to vehicle, with predefined offset |
| /attach_calc_offset | Attach object to vehicle, automatically calc offset |
| /detach | detach object from vehicle |

### Commands ([matrix_example2.pwn])

| command | description  |
| ------ | ------ |
| /getvrot | Example of GetVehicleRot |
| /setvrot | Example of SetVehicleRot |
| /boost | Example of SetVehicleVelocityFrontVector  |
| /turbo | Same result as /boost |

### Thank to

IllidanS4 for [i_quat], It allowed me to create the Matrix_BuildUp function based on GetVehicleRotationQuatFixed.<br />
Zeex for [amx_assembly], it allowed me to create the GetVehicleMatrix and SetVehicleMarix function. Thanks for that.<br />
katursis for [Pawn.RakNet], it allowed me to create the SetVehicleMarix function. Thanks for that.<br />

[matrix.inc]: <https://github.com/Leonardo541/matrix/blob/main/include/matrix.inc>
[matrix_example.inc]: <https://github.com/Leonardo541/matrix/blob/main/filterscripts/matrix_example.inc>
[matrix_example1.pwn]: <https://github.com/Leonardo541/matrix/blob/main/filterscripts/matrix_example1.pwn>
[matrix_example2.pwn]: <https://github.com/Leonardo541/matrix/blob/main/filterscripts/matrix_example2.pwn>
[i_quat]: <https://github.com/IllidanS4/i_quat>
[amx_assembly]: <https://github.com/Zeex/amx_assembly>
[Pawn.RakNet]: <https://github.com/katursis/Pawn.RakNet>
