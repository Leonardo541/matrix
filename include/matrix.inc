
#if defined(MATRIX_INC)
	#endinput
#endif
#define MATRIX_INC

/*

native Vector(Float:x, Float:y, Float:z);
native Matrix(const right[VECTOR], const front[VECTOR], const up[VECTOR], const pos[VECTOR]);
native Matrix_Invert(const matrix[MATRIX]);
native Matrix_BuildUp(matrix[MATRIX]);
native Matrix_SetRotation(matrix[MATRIX], Float:rx, Float:ry, Float:rz);
native Float:Matrix_GetRotationX(const matrix[MATRIX]);
native Float:Matrix_GetRotationY(const matrix[MATRIX]);
native Float:Matrix_GetRotationZ(const matrix[MATRIX]);
native Matrix_GetRotation(const matrix[MATRIX]);
native Matrix_Multiply3x3(const matrix[MATRIX], const vector[VECTOR]);
native Matrix_Multiply4x4(const matrix1[MATRIX], const matrix2[MATRIX]);
native Matrix_GetOffsetPosition(const matrix[MATRIX], const vector[VECTOR]);
native Matrix_GetOffsetRotation(const matrix[MATRIX], const vector[VECTOR], bool:invert = true);

*/

enum VECTOR
{
	Float:VEC_X,
	Float:VEC_Y,
	Float:VEC_Z
};

enum MATRIX
{
	MAT_RIGHT[VECTOR],
	MAT_FRONT[VECTOR],
	MAT_UP[VECTOR],
	MAT_POS[VECTOR]
};

stock Vector(Float:x, Float:y, Float:z)
{
	new out[VECTOR];
	
	out[VEC_X] = x;
	out[VEC_Y] = y;
	out[VEC_Z] = z;
	
	return out;
}

stock Matrix(const right[VECTOR], const front[VECTOR], const up[VECTOR], const pos[VECTOR])
{
	new out[MATRIX];
	
	out[MAT_RIGHT] = right;
	out[MAT_FRONT] = front;
	out[MAT_UP] = up;
	out[MAT_POS] = pos;
	
	return out;
}

stock Matrix_Invert(const matrix[MATRIX])
{
	new out[MATRIX];
	
	out[MAT_RIGHT][VEC_X] = matrix[MAT_UP][VEC_Z] * matrix[MAT_FRONT][VEC_Y] - matrix[MAT_UP][VEC_Y] * matrix[MAT_FRONT][VEC_Z];
	out[MAT_RIGHT][VEC_Y] = -(matrix[MAT_UP][VEC_Z] * matrix[MAT_RIGHT][VEC_Y] - matrix[MAT_UP][VEC_Y] * matrix[MAT_RIGHT][VEC_Z]);
	out[MAT_RIGHT][VEC_Z] = matrix[MAT_FRONT][VEC_Z] * matrix[MAT_RIGHT][VEC_Y] - matrix[MAT_FRONT][VEC_Y] * matrix[MAT_RIGHT][VEC_Z];
	
	new Float:d = matrix[MAT_FRONT][VEC_X] * out[MAT_RIGHT][VEC_Y] + matrix[MAT_UP][VEC_X] * out[MAT_RIGHT][VEC_Z] + out[MAT_RIGHT][VEC_X] * matrix[MAT_RIGHT][VEC_X];
	new Float:x = 1.0;
	
	if(d != 0.0)
		x = 1.0 / d;
	
	out[MAT_RIGHT][VEC_X] = x * out[MAT_RIGHT][VEC_X];
	out[MAT_RIGHT][VEC_Y] = x * out[MAT_RIGHT][VEC_Y];
	out[MAT_RIGHT][VEC_Z] = x * out[MAT_RIGHT][VEC_Z];
	out[MAT_FRONT][VEC_X] = -((matrix[MAT_UP][VEC_Z] * matrix[MAT_FRONT][VEC_X] - matrix[MAT_UP][VEC_X] * matrix[MAT_FRONT][VEC_Z]) * x);
	out[MAT_FRONT][VEC_Y] = (matrix[MAT_UP][VEC_Z] * matrix[MAT_RIGHT][VEC_X] - matrix[MAT_UP][VEC_X] * matrix[MAT_RIGHT][VEC_Z]) * x;
	out[MAT_FRONT][VEC_Z] = -((matrix[MAT_FRONT][VEC_Z] * matrix[MAT_RIGHT][VEC_X] - matrix[MAT_FRONT][VEC_X] * matrix[MAT_RIGHT][VEC_Z]) * x);
	out[MAT_UP][VEC_X] = (matrix[MAT_UP][VEC_Y] * matrix[MAT_FRONT][VEC_X] - matrix[MAT_UP][VEC_X] * matrix[MAT_FRONT][VEC_Y]) * x;
	out[MAT_UP][VEC_Y] = -((matrix[MAT_UP][VEC_Y] * matrix[MAT_RIGHT][VEC_X] - matrix[MAT_UP][VEC_X] * matrix[MAT_RIGHT][VEC_Y]) * x);
	out[MAT_UP][VEC_Z] = (matrix[MAT_FRONT][VEC_Y] * matrix[MAT_RIGHT][VEC_X] - matrix[MAT_FRONT][VEC_X] * matrix[MAT_RIGHT][VEC_Y]) * x;
	out[MAT_POS][VEC_X] = -(out[MAT_UP][VEC_X] * matrix[MAT_POS][VEC_Z] + matrix[MAT_POS][VEC_Y] * out[MAT_FRONT][VEC_X] + matrix[MAT_POS][VEC_X] * out[MAT_RIGHT][VEC_X]);
	out[MAT_POS][VEC_Y] = -(out[MAT_UP][VEC_Y] * matrix[MAT_POS][VEC_Z] + matrix[MAT_POS][VEC_Y] * out[MAT_FRONT][VEC_Y] + matrix[MAT_POS][VEC_X] * out[MAT_RIGHT][VEC_Y]);
	out[MAT_POS][VEC_Z] = -(out[MAT_UP][VEC_Z] * matrix[MAT_POS][VEC_Z] + matrix[MAT_POS][VEC_Y] * out[MAT_FRONT][VEC_Z] + matrix[MAT_POS][VEC_X] * out[MAT_RIGHT][VEC_Z]);
	
	return out;
}

stock Matrix_BuildUp(matrix[MATRIX])
{
	matrix[MAT_UP][VEC_X] = matrix[MAT_RIGHT][VEC_Y] * matrix[MAT_FRONT][VEC_Z] - matrix[MAT_RIGHT][VEC_Z] * matrix[MAT_FRONT][VEC_Y];
	matrix[MAT_UP][VEC_Y] = matrix[MAT_RIGHT][VEC_Z] * matrix[MAT_FRONT][VEC_X] - matrix[MAT_RIGHT][VEC_X] * matrix[MAT_FRONT][VEC_Z];
	matrix[MAT_UP][VEC_Z] = matrix[MAT_RIGHT][VEC_X] * matrix[MAT_FRONT][VEC_Y] - matrix[MAT_RIGHT][VEC_Y] * matrix[MAT_FRONT][VEC_X];
}

stock Matrix_SetRotation(matrix[MATRIX], Float:rx, Float:ry, Float:rz)
{
	new Float:rx_cos = floatcos(rx, degrees);
	new Float:rx_sin = floatsin(rx, degrees);
	new Float:ry_cos = floatcos(ry, degrees);
	new Float:ry_sin = floatsin(ry, degrees);
	new Float:rz_cos = floatcos(rz, degrees);
	new Float:rz_sin = floatsin(rz, degrees);
	
	new Float:dx = rz_sin * rx_sin;
	new Float:dy = rz_cos * rx_sin;
	
	matrix[MAT_RIGHT][VEC_X] = rz_cos * ry_cos - dx * ry_sin;
	matrix[MAT_RIGHT][VEC_Y] = dy * ry_sin + rz_sin * ry_cos;
	matrix[MAT_RIGHT][VEC_Z] = -(ry_sin * rx_cos);
	
	matrix[MAT_FRONT][VEC_X] = -(rz_sin * rx_cos);
	matrix[MAT_FRONT][VEC_Y] = rz_cos * rx_cos;
	matrix[MAT_FRONT][VEC_Z] = rx_sin;
	
	matrix[MAT_UP][VEC_X] = rz_cos * ry_sin + dx * ry_cos;
	matrix[MAT_UP][VEC_Y] = rz_sin * ry_sin - dy * ry_cos;
	matrix[MAT_UP][VEC_Z] = ry_cos * rx_cos;
}

stock Float:Matrix_GetRotationX(const matrix[MATRIX])
{
	new Float:x = floatsqroot(matrix[MAT_FRONT][VEC_X] * matrix[MAT_FRONT][VEC_X] + matrix[MAT_FRONT][VEC_Y] * matrix[MAT_FRONT][VEC_Y]);
	
	if(matrix[MAT_UP][VEC_Z] < 0.0)
		x = x * -1.0;
	
	x = atan2(x, matrix[MAT_FRONT][VEC_Z]) - 270.0;
	
	while(x < 0.0) x += 360.0;
	while(x >= 360.0) x -= 360.0;
	
	return x;
}

stock Float:Matrix_GetRotationY(const matrix[MATRIX])
{
	new Float:y = floatsqroot(matrix[MAT_RIGHT][VEC_X] * matrix[MAT_RIGHT][VEC_X] + matrix[MAT_RIGHT][VEC_Y] * matrix[MAT_RIGHT][VEC_Y]);
	
	if(matrix[MAT_UP][VEC_Z] < 0.0)
		y = y * -1.0;
	
	y = atan2(y, matrix[MAT_RIGHT][VEC_Z]) - 90.0;
	
	while(y < 0.0) y += 360.0;
	while(y >= 360.0) y -= 360.0;
	
	return y;
}

stock Float:Matrix_GetRotationZ(const matrix[MATRIX])
{
	new Float:z = 270.0 - atan2(matrix[MAT_FRONT][VEC_Y], -matrix[MAT_FRONT][VEC_X]);

	if(matrix[MAT_UP][VEC_Z] < 0.0)
		z = z + 180.0;
	
	while(z < 0.0) z += 360.0;
	while(z >= 360.0) z -= 360.0;
	
	return z;
}

stock Matrix_GetRotation(const matrix[MATRIX])
{
	new out[VECTOR];
	
	new matrix1[MATRIX];
	new matrix2[MATRIX];
	
	matrix1 = matrix;
	
	out[VEC_Z] = Matrix_GetRotationZ(matrix1);
	
	Matrix_SetRotation(matrix2, 0.0, 0.0, -out[VEC_Z]);
	matrix1 = Matrix_Multiply4x4(matrix1, matrix2);
	
	out[VEC_X] = Matrix_GetRotationX(matrix1);
	
	Matrix_SetRotation(matrix2, -out[VEC_X], 0.0, 0.0);
	matrix1 = Matrix_Multiply4x4(matrix1, matrix2);
	
	out[VEC_Y] = Matrix_GetRotationY(matrix1);
	
	return out;
}

stock Matrix_Multiply3x3(const matrix[MATRIX], const vector[VECTOR])
{
	new out[VECTOR];
	
	out[VEC_X] = matrix[MAT_RIGHT][VEC_X] * vector[VEC_X] + matrix[MAT_FRONT][VEC_X] * vector[VEC_Y] + matrix[MAT_UP][VEC_X] * vector[VEC_Z];
	out[VEC_Y] = matrix[MAT_RIGHT][VEC_Y] * vector[VEC_X] + matrix[MAT_FRONT][VEC_Y] * vector[VEC_Y] + matrix[MAT_UP][VEC_Y] * vector[VEC_Z];
	out[VEC_Z] = matrix[MAT_RIGHT][VEC_Z] * vector[VEC_X] + matrix[MAT_FRONT][VEC_Z] * vector[VEC_Y] + matrix[MAT_UP][VEC_Z] * vector[VEC_Z];
	
	return out;
}

stock Matrix_Multiply4x4(const matrix1[MATRIX], const matrix2[MATRIX])
{
	new out[MATRIX];
	
	out[MAT_RIGHT][VEC_X] = matrix1[MAT_RIGHT][VEC_X] * matrix2[MAT_RIGHT][VEC_X] + matrix1[MAT_RIGHT][VEC_Y] * matrix2[MAT_FRONT][VEC_X] + matrix1[MAT_RIGHT][VEC_Z] * matrix2[MAT_UP][VEC_X];
	out[MAT_RIGHT][VEC_Y] = matrix1[MAT_RIGHT][VEC_X] * matrix2[MAT_RIGHT][VEC_Y] + matrix1[MAT_RIGHT][VEC_Y] * matrix2[MAT_FRONT][VEC_Y] + matrix1[MAT_RIGHT][VEC_Z] * matrix2[MAT_UP][VEC_Y];
	out[MAT_RIGHT][VEC_Z] = matrix1[MAT_RIGHT][VEC_X] * matrix2[MAT_RIGHT][VEC_Z] + matrix1[MAT_RIGHT][VEC_Y] * matrix2[MAT_FRONT][VEC_Z] + matrix1[MAT_RIGHT][VEC_Z] * matrix2[MAT_UP][VEC_Z];
	
	out[MAT_FRONT][VEC_X] = matrix1[MAT_FRONT][VEC_X] * matrix2[MAT_RIGHT][VEC_X] + matrix1[MAT_FRONT][VEC_Y] * matrix2[MAT_FRONT][VEC_X] + matrix1[MAT_FRONT][VEC_Z] * matrix2[MAT_UP][VEC_X];
	out[MAT_FRONT][VEC_Y] = matrix1[MAT_FRONT][VEC_X] * matrix2[MAT_RIGHT][VEC_Y] + matrix1[MAT_FRONT][VEC_Y] * matrix2[MAT_FRONT][VEC_Y] + matrix1[MAT_FRONT][VEC_Z] * matrix2[MAT_UP][VEC_Y];
	out[MAT_FRONT][VEC_Z] = matrix1[MAT_FRONT][VEC_X] * matrix2[MAT_RIGHT][VEC_Z] + matrix1[MAT_FRONT][VEC_Y] * matrix2[MAT_FRONT][VEC_Z] + matrix1[MAT_FRONT][VEC_Z] * matrix2[MAT_UP][VEC_Z];
	
	out[MAT_UP][VEC_X] = matrix1[MAT_UP][VEC_X] * matrix2[MAT_RIGHT][VEC_X] + matrix1[MAT_UP][VEC_Y] * matrix2[MAT_FRONT][VEC_X] + matrix1[MAT_UP][VEC_Z] * matrix2[MAT_UP][VEC_X];
	out[MAT_UP][VEC_Y] = matrix1[MAT_UP][VEC_X] * matrix2[MAT_RIGHT][VEC_Y] + matrix1[MAT_UP][VEC_Y] * matrix2[MAT_FRONT][VEC_Y] + matrix1[MAT_UP][VEC_Z] * matrix2[MAT_UP][VEC_Y];
	out[MAT_UP][VEC_Z] = matrix1[MAT_UP][VEC_X] * matrix2[MAT_RIGHT][VEC_Z] + matrix1[MAT_UP][VEC_Y] * matrix2[MAT_FRONT][VEC_Z] + matrix1[MAT_UP][VEC_Z] * matrix2[MAT_UP][VEC_Z];
	
	out[MAT_POS][VEC_X] = matrix1[MAT_POS][VEC_X] * matrix2[MAT_RIGHT][VEC_X] + matrix1[MAT_POS][VEC_Y] * matrix2[MAT_FRONT][VEC_X] + matrix1[MAT_POS][VEC_Z] * matrix2[MAT_UP][VEC_X] + matrix2[MAT_POS][VEC_X];
	out[MAT_POS][VEC_Y] = matrix1[MAT_POS][VEC_X] * matrix2[MAT_RIGHT][VEC_Y] + matrix1[MAT_POS][VEC_Y] * matrix2[MAT_FRONT][VEC_Y] + matrix1[MAT_POS][VEC_Z] * matrix2[MAT_UP][VEC_Y] + matrix2[MAT_POS][VEC_Y];
	out[MAT_POS][VEC_Z] = matrix1[MAT_POS][VEC_X] * matrix2[MAT_RIGHT][VEC_Z] + matrix1[MAT_POS][VEC_Y] * matrix2[MAT_FRONT][VEC_Z] + matrix1[MAT_POS][VEC_Z] * matrix2[MAT_UP][VEC_Z] + matrix2[MAT_POS][VEC_Z];
	
	return out;
}

stock Matrix_GetOffsetPosition(const matrix[MATRIX], const vector[VECTOR])
{
	new out[VECTOR];
	
	new Float:dx = vector[VEC_X] - matrix[MAT_POS][VEC_X];
	new Float:dy = vector[VEC_Y] - matrix[MAT_POS][VEC_Y];
	new Float:dz = vector[VEC_Z] - matrix[MAT_POS][VEC_Z];
	
	out[VEC_X] = matrix[MAT_RIGHT][VEC_X] * dx + matrix[MAT_RIGHT][VEC_Y] * dy + matrix[MAT_RIGHT][VEC_Z] * dz;
	out[VEC_Y] = matrix[MAT_FRONT][VEC_X] * dx + matrix[MAT_FRONT][VEC_Y] * dy + matrix[MAT_FRONT][VEC_Z] * dz;
	out[VEC_Z] = matrix[MAT_UP][VEC_X] * dx + matrix[MAT_UP][VEC_Y] * dy + matrix[MAT_UP][VEC_Z] * dz;
	
	return out;
}

stock Matrix_GetOffsetRotation(const matrix[MATRIX], const vector[VECTOR], bool:invert = true)
{
	new out[VECTOR];

	new matrix1[MATRIX];
	new matrix2[MATRIX];
	
	if(invert)
	{
		matrix1 = Matrix_Invert(matrix);
	}
	else
	{
		matrix1 = matrix;
	}
	
	Matrix_SetRotation(matrix2, vector[VEC_X], vector[VEC_Y], vector[VEC_Z]);
	matrix2 = Matrix_Multiply4x4(matrix2, matrix1);
	
	out = Matrix_GetRotation(matrix2);
	
	return out;
}

