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

// https://github.com/IllidanS4/i_quat
// i_quat.inc

#define FILTERSCRIPT

#include <a_samp>
#include <matrix>
#include <phys_memory>
#include <os>
#include <i_quat>

#include "matrix_example.inc"

new objectid_matrix = INVALID_OBJECT_ID;
new objectid_attach = INVALID_OBJECT_ID;
new objectid_quat = INVALID_OBJECT_ID;

new Text3D:label_matrix = Text3D:INVALID_3DTEXT_ID;
new Text3D:label_attach = Text3D:INVALID_3DTEXT_ID;
new Text3D:label_quat = Text3D:INVALID_3DTEXT_ID;

new attached_to_vehicleid = INVALID_VEHICLE_ID;

new attach_offset_pos[VECTOR];
new attach_offset_rot[VECTOR];

new timerid = -1;

public OnFilterScriptInit()
{
	objectid_matrix = CreateObject(19817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	objectid_attach = CreateObject(19817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	objectid_quat = CreateObject(19817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	
	label_matrix = Create3DTextLabel("matrix", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
	label_attach = Create3DTextLabel("attach", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
	label_quat = Create3DTextLabel("quat", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
}

public OnFilterScriptExit()
{
	DestroyObject(objectid_matrix);
	DestroyObject(objectid_attach);
	DestroyObject(objectid_quat);
	
	Delete3DTextLabel(label_matrix);
	Delete3DTextLabel(label_attach);
	Delete3DTextLabel(label_quat);
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(strcmp(cmdtext, "/matrix", true) == 0)
	{
		if(timerid == -1)
		{
			SendClientMessage(playerid, -1, "TEST MATRIX START");
			
			timerid = SetTimer("timer", 1, true);
		}
		else
		{
			SendClientMessage(playerid, -1, "TEST MATRIX STOP");
			
			KillTimer(timerid);
			timerid = -1;
			
			if(attached_to_vehicleid != INVALID_VEHICLE_ID)
			{
				DestroyObject(objectid_attach);
				
				Delete3DTextLabel(label_matrix);
				Delete3DTextLabel(label_attach);
				Delete3DTextLabel(label_quat);
				
				new mat[MATRIX];
				new pos[VECTOR];
				new rot[VECTOR];
				
				mat = GetVehicleMatrix(attached_to_vehicleid);
				pos = Matrix_Multiply3x3(mat, Vector(0.0, 10.0, -0.75));
				rot = Matrix_GetRotation(mat);
				
				pos[VEC_X] += mat[MAT_POS][VEC_X];
				pos[VEC_Y] += mat[MAT_POS][VEC_Y];
				pos[VEC_Z] += mat[MAT_POS][VEC_Z];
				
				objectid_attach = CreateObject(19817, pos[VEC_X], pos[VEC_Y], pos[VEC_Z], rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
				
				label_matrix = Create3DTextLabel("matrix", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
				label_attach = Create3DTextLabel("attach", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
				label_quat = Create3DTextLabel("quat", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
				
				attached_to_vehicleid = INVALID_VEHICLE_ID;
			}
		}
		
		return 1;
	}
	
	if(strcmp(cmdtext, "/attach", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, -1, "TEST ATTACH");
			
			new vehicleid = GetPlayerVehicleID(playerid);
			
			attach_offset_pos = Vector(0.0, 10.0, -0.75);
			attach_offset_rot = Vector(0.0, 0.0, 0.0);
			
			AttachObjectToVehicle(objectid_attach, vehicleid, 0.0, 10.0, -0.75, 0.0, 0.0, 0.0);
		}
		
		return 1;
	}
	
	if(strcmp(cmdtext, "/attach_calc_offset", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, -1, "TEST ATTACH");

			new vehicleid = GetPlayerVehicleID(playerid);

			new mat[MATRIX];
			new pos[VECTOR];
			new rot[VECTOR];
			
			new Float:x, Float:y, Float:z;
			GetObjectPos(objectid_attach, x, y, z);
			
			new Float:rx, Float:ry, Float:rz;
			GetObjectRot(objectid_attach, rx, ry, rz);
			
			mat = GetVehicleMatrix(vehicleid);
			pos = Matrix_GetOffsetPosition(mat, Vector(x, y, z));
			rot = Matrix_GetOffsetRotation(mat, Vector(rx, ry, rz));
			
			attach_offset_pos = pos;
			attach_offset_rot = rot;
			
			AttachObjectToVehicle(objectid_attach, vehicleid, pos[VEC_X], pos[VEC_Y], pos[VEC_Z], rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
		}
		
		return 1;
	}
	
	if(strcmp(cmdtext, "/detach", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, -1, "TEST DETACH");
			
			new vehicleid = GetPlayerVehicleID(playerid);
			
			DestroyObject(objectid_attach);
			
			new mat[MATRIX];
			new pos[VECTOR];
			new rot[VECTOR];
			
			mat = GetVehicleMatrix(vehicleid);
			pos = Matrix_Multiply3x3(mat, attach_offset_pos);
			rot = Matrix_GetOffsetRotation(mat, attach_offset_rot, false);
			
			pos[VEC_X] += mat[MAT_POS][VEC_X];
			pos[VEC_Y] += mat[MAT_POS][VEC_Y];
			pos[VEC_Z] += mat[MAT_POS][VEC_Z];
			
			objectid_attach = CreateObject(19817, pos[VEC_X], pos[VEC_Y], pos[VEC_Z], rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
			return 1;
		}
	}
	
	return 0;
}

public OnPlayerUpdate(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new vehicleid = GetPlayerVehicleID(playerid);
		
		if(vehicleid != attached_to_vehicleid && timerid != -1)
		{
			attached_to_vehicleid = vehicleid;
			
			AttachObjectToVehicle(objectid_attach, vehicleid, 0.0, 10.0, -0.75, 0.0, 0.0, 0.0);
			
			Attach3DTextLabelToVehicle(label_matrix, vehicleid, -3.0, 10.0, 2.0);
			Attach3DTextLabelToVehicle(label_attach, vehicleid, 0.0, 10.0, 2.0);
			Attach3DTextLabelToVehicle(label_quat, vehicleid, 3.0, 10.0, 2.0);
			
			attach_offset_pos = Vector(0.0, 10.0, -0.75);
			attach_offset_rot = Vector(0.0, 0.0, 0.0);
		}
	}
	
	return 1;
}

forward timer();
public timer()
{
	if(attached_to_vehicleid != INVALID_VEHICLE_ID)
	{
		new mat[MATRIX];
		new pos[VECTOR];
		new rot[VECTOR];
		
		mat = GetVehicleMatrix(attached_to_vehicleid);
		pos = Matrix_Multiply3x3(mat, Vector(-3.0, 10.0, -0.75));
		rot = Matrix_GetRotation(mat);
		
		pos[VEC_X] += mat[MAT_POS][VEC_X];
		pos[VEC_Y] += mat[MAT_POS][VEC_Y];
		pos[VEC_Z] += mat[MAT_POS][VEC_Z];
		
		SetObjectPos(objectid_matrix, pos[VEC_X], pos[VEC_Y], pos[VEC_Z]);
		SetObjectRot(objectid_matrix, rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
		
		pos = Matrix_Multiply3x3(mat, Vector(3.0, 10.0, -0.75));
		
		pos[VEC_X] += mat[MAT_POS][VEC_X];
		pos[VEC_Y] += mat[MAT_POS][VEC_Y];
		pos[VEC_Z] += mat[MAT_POS][VEC_Z];
		
		new Float:qw, Float:qx, Float:qy, Float:qz, Float:rx, Float:ry, Float:rz;
		
		GetVehicleRotationQuat(attached_to_vehicleid, qw, qx, qy, qz);
		GetQuaternionAngles(qw, qx, qy, qz, rx, ry, rz);
		
		SetObjectPos(objectid_quat, pos[VEC_X], pos[VEC_Y], pos[VEC_Z]);
		SetObjectRot(objectid_quat, rx, ry, rz);
	}
}

