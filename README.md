# Matrix Functions for SA-MP

https://youtu.be/vSZyMilgcuU

### Functions

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


### Thank to

IllidanS4 for [i_quat].
Zeex for [amx_assembly].

I had created Matrix_BuildUp in this way: (code in C ++, before converting to Pawn code)

```C++
void matrix_t::BuildUp()
{
	matrix_t matrix1;
	matrix_t matrix2;
	
	matrix1 = *this;
	matrix1.up.Z = 1.0;
	
	vector_t rotation = matrix1.GetRotation();
	
	matrix2.SetRotation(rotation.X, rotation.Y, rotation.Z);
	
	if(right != matrix2.right || front != matrix2.front)
	{
		matrix1.up.Z = -1.0;
		rotation = matrix1.GetRotation();
	}
	
	SetRotation(rotation.X, rotation.Y, rotation.Z);
}
```
But then I found out the way i_quat did it, and I preferred to use that solution. Thanks for that.

Also by amx_assembly, it allowed me to create the GetVehicleMatrix function. This function is inside the filterscript, and is not part of the include. It is only available as a test and example to use the include functions. Thanks for that.

[i_quat]: <https://github.com/IllidanS4/i_quat>
[amx_assembly]: <https://github.com/Zeex/amx_assembly>
