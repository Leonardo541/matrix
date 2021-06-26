/*
 * MIT License
 *
 * Copyright (c) 2021 Leonardo541
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

// https://github.com/Zeex/amx_assembly
// phys_memory.inc
// os.inc

// https://github.com/katursis/Pawn.RakNet
// Pawn.RakNet.inc

#define FILTERSCRIPT

#include <a_samp>
#include <matrix>
#include <phys_memory>
#include <os>
#include <Pawn.RakNet>

#include "matrix_example.inc"

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	new text[256];
	
	format(text, sizeof(text), "entring vehicleid: %d", vehicleid);
	SendClientMessage(playerid, 0xFFFFFFFF, text);
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	new text[256];
	new cmd[256], tmp[256];
	new idx;
	
	cmd = strtok(cmdtext, idx);
	
	if(strcmp(cmd, "/getvrot", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /getvrot [vehicleid or @]");
			return 1;
		}
		
		new vehicleid = strcmp(tmp, "@") == 0 ? GetPlayerVehicleID(playerid) : strval(tmp);
		
		new rot[VECTOR];
		
		rot = GetVehicleRot(vehicleid);
		
		new Float:angle;
		GetVehicleZAngle(vehicleid, angle);
		
		format(text, sizeof(text), "rot: %f %f %f (angle: %f)", rot[VEC_X], rot[VEC_Y], rot[VEC_Z], angle);
		SendClientMessage(playerid, 0xFFFFFFFF, text);
		
		return 1;
	}
	
	if(strcmp(cmd, "/setvrot", true) == 0)
	{
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /setvrot [vehicleid or @] [rot_x] [rot_y] [rot_z]");
			return 1;
		}
		
		new vehicleid = strcmp(tmp, "@") == 0 ? GetPlayerVehicleID(playerid) : strval(tmp);
		
		new rot[VECTOR];
		
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /setvrot [vehicleid or @] [rot_x] [rot_y] [rot_z]");
			return 1;
		}
		
		rot[VEC_X] = floatstr(tmp);
		
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /setvrot [vehicleid or @] [rot_x] [rot_y] [rot_z]");
			return 1;
		}
		
		rot[VEC_Y] = floatstr(tmp);
		
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /setvrot [vehicleid or @] [rot_x] [rot_y] [rot_z]");
			return 1;
		}
		
		rot[VEC_Z] = floatstr(tmp);
		
		new velocity[VECTOR];
		
		velocity = GetVehicleVelocityFrontVector(vehicleid);
		
		SetVehicleRot(vehicleid, rot);
		
		SetVehicleVelocityFrontVector(vehicleid, velocity);
		return 1;
	}
	
	if(strcmp(cmd, "/boost", true) == 0) // same result as /turbo
	{
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /boost [vehicleid or @]");
			return 1;
		}
		
		new vehicleid = strcmp(tmp, "@") == 0 ? GetPlayerVehicleID(playerid) : strval(tmp);
		
		new vec[VECTOR];
		
		vec = GetVehicleVelocityFrontVector(vehicleid);
		
		vec[VEC_Y] += 1.0;
		
		SetVehicleVelocityFrontVector(vehicleid, vec);
		return 1;
	}
	
	if(strcmp(cmd, "/turbo", true) == 0) // same result as /boost
	{
		tmp = strtok(cmdtext, idx);
		
		if(strlen(tmp) == 0)
		{
			SendClientMessage(playerid, 0xFFFFFFFF, "use: /turbo [vehicleid or @]");
			return 1;
		}
		
		new vehicleid = strcmp(tmp, "@") == 0 ? GetPlayerVehicleID(playerid) : strval(tmp);
		
		new mat[MATRIX];
		new vec[VECTOR];
		
		mat = GetVehicleMatrix(vehicleid);
		vec = Vector(0.0, 1.0, 0.0);
		vec = Matrix_Multiply3x3(mat, vec);
		
		new velocity[VECTOR];
		
		GetVehicleVelocity(vehicleid, velocity[VEC_X], velocity[VEC_Y], velocity[VEC_Z]);
		
		velocity[VEC_X] += vec[VEC_X];
		velocity[VEC_Y] += vec[VEC_Y];
		velocity[VEC_Z] += vec[VEC_Z];
		
		SetVehicleVelocity(vehicleid, velocity[VEC_X], velocity[VEC_Y], velocity[VEC_Z]);
		return 1;
	}
	
	return 0;
}

stock strtok(const string[], &index)
{
	new length = strlen(string);
	while ((index < length) && (string[index] <= ' '))
	{
		index++;
	}
	
	new offset = index;
	new result[20];
	while ((index < length) && (string[index] > ' ') && ((index - offset) < (sizeof(result) - 1)))
	{
		result[index - offset] = string[index];
		index++;
	}
	result[index - offset] = EOS;
	return result;
}
