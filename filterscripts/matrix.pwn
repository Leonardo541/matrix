
// phys_memory.inc & os.inc: https://github.com/Zeex/amx_assembly
// i_quat.inc: https://github.com/IllidanS4/i_quat

#include <a_samp>
#include <matrix>
#include <phys_memory>
#include <os>
#include <i_quat>

new test_objectid_matrix = INVALID_OBJECT_ID;
new test_objectid_attach = INVALID_OBJECT_ID;
new test_objectid_quat = INVALID_OBJECT_ID;

new Text3D:test_label_matrix = Text3D:INVALID_3DTEXT_ID;
new Text3D:test_label_attach = Text3D:INVALID_3DTEXT_ID;
new Text3D:test_label_quat = Text3D:INVALID_3DTEXT_ID;

new test_vehicleid = INVALID_VEHICLE_ID;
new test_unoccupied[MAX_VEHICLES] = { 1, ... };

new test_attach_offset_pos[VECTOR];
new test_attach_offset_rot[VECTOR];

new test_timerid = -1;

public OnFilterScriptInit()
{
	test_objectid_matrix = CreateObject(19817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	test_objectid_attach = CreateObject(19817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	test_objectid_quat = CreateObject(19817, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	
	test_label_matrix = Create3DTextLabel("matrix", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
	test_label_attach = Create3DTextLabel("attach", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
	test_label_quat = Create3DTextLabel("quat", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
}

public OnFilterScriptExit()
{
	DestroyObject(test_objectid_matrix);
	DestroyObject(test_objectid_attach);
	DestroyObject(test_objectid_quat);
	
	Delete3DTextLabel(test_label_matrix);
	Delete3DTextLabel(test_label_attach);
	Delete3DTextLabel(test_label_quat);
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(strcmp(cmdtext, "/test_matrix", true) == 0)
	{
		if(test_timerid == -1)
		{
			SendClientMessage(playerid, -1, "TEST MATRIX START");
			
			test_timerid = SetTimer("test_timer", 1, true);
		}
		else
		{
			SendClientMessage(playerid, -1, "TEST MATRIX STOP");
			
			KillTimer(test_timerid);
			test_timerid = -1;
			
			if(test_vehicleid != INVALID_VEHICLE_ID)
			{
				DestroyObject(test_objectid_attach);
				
				Delete3DTextLabel(test_label_matrix);
				Delete3DTextLabel(test_label_attach);
				Delete3DTextLabel(test_label_quat);
				
				new mat[MATRIX];
				new pos[VECTOR];
				new rot[VECTOR];
				
				mat = GetVehicleMatrix(test_vehicleid);
				pos = Matrix_Multiply3x3(mat, Vector(0.0, 10.0, -0.75));
				rot = Matrix_GetRotation(mat);
				
				pos[VEC_X] += mat[MAT_POS][VEC_X];
				pos[VEC_Y] += mat[MAT_POS][VEC_Y];
				pos[VEC_Z] += mat[MAT_POS][VEC_Z];
				
				test_objectid_attach = CreateObject(19817, pos[VEC_X], pos[VEC_Y], pos[VEC_Z], rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
				
				test_label_matrix = Create3DTextLabel("matrix", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
				test_label_attach = Create3DTextLabel("attach", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
				test_label_quat = Create3DTextLabel("quat", 0xFFFFFFFF, 0.0, 0.0, 0.0, 50.0, 0, 1);
				
				test_vehicleid = INVALID_VEHICLE_ID;
			}
		}
		
		return 1;
	}
	
	if(strcmp(cmdtext, "/test_attach", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, -1, "TEST ATTACH");
			
			new vehicleid = GetPlayerVehicleID(playerid);
			
			test_attach_offset_pos = Vector(0.0, 10.0, -0.75);
			test_attach_offset_rot = Vector(0.0, 0.0, 0.0);
			
			AttachObjectToVehicle(test_objectid_attach, vehicleid, 0.0, 10.0, -0.75, 0.0, 0.0, 0.0);
		}
		
		return 1;
	}
	
	if(strcmp(cmdtext, "/test_attach_calc_offset", true) == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, -1, "TEST ATTACH");
			
			new vehicleid = GetPlayerVehicleID(playerid);
			
			new mat[MATRIX];
			new pos[VECTOR];
			new rot[VECTOR];
			
			new Float:x, Float:y, Float:z;
			GetObjectPos(test_objectid_attach, x, y, z);
			
			new Float:rx, Float:ry, Float:rz;
			GetObjectRot(test_objectid_attach, rx, ry, rz);
			
			mat = GetVehicleMatrix(vehicleid);
			pos = Matrix_GetOffsetPosition(mat, Vector(x, y, z));
			rot = Matrix_GetOffsetRotation(mat, Vector(rx, ry, rz));
			
			test_attach_offset_pos = pos;
			test_attach_offset_rot = rot;
			
			AttachObjectToVehicle(test_objectid_attach, vehicleid, pos[VEC_X], pos[VEC_Y], pos[VEC_Z], rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
		}
		
		return 1;
	}
	
	if(strcmp(cmdtext, "/test_detach", true) == 0)
	{
		SendClientMessage(playerid, -1, "TEST ATTACH");
		
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessage(playerid, -1, "TEST DETACH");
			
			new vehicleid = GetPlayerVehicleID(playerid);
			
			DestroyObject(test_objectid_attach);
			
			new mat[MATRIX];
			new pos[VECTOR];
			new rot[VECTOR];
			
			mat = GetVehicleMatrix(vehicleid);
			pos = Matrix_Multiply3x3(mat, test_attach_offset_pos);
			rot = Matrix_GetOffsetRotation(mat, test_attach_offset_rot, false);
			
			pos[VEC_X] += mat[MAT_POS][VEC_X];
			pos[VEC_Y] += mat[MAT_POS][VEC_Y];
			pos[VEC_Z] += mat[MAT_POS][VEC_Z];
			
			test_objectid_attach = CreateObject(19817, pos[VEC_X], pos[VEC_Y], pos[VEC_Z], rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
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
		
		if(vehicleid != test_vehicleid && test_timerid != -1)
		{
			test_vehicleid = vehicleid;
			
			AttachObjectToVehicle(test_objectid_attach, vehicleid, 0.0, 10.0, -0.75, 0.0, 0.0, 0.0);
			
			Attach3DTextLabelToVehicle(test_label_matrix, vehicleid, -3.0, 10.0, 2.0);
			Attach3DTextLabelToVehicle(test_label_attach, vehicleid, 0.0, 10.0, 2.0);
			Attach3DTextLabelToVehicle(test_label_quat, vehicleid, 3.0, 10.0, 2.0);
			
			test_attach_offset_pos = Vector(0.0, 10.0, -0.75);
			test_attach_offset_rot = Vector(0.0, 0.0, 0.0);
		}
		
		test_unoccupied[vehicleid] = 0;
	}
	
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	test_unoccupied[vehicleid] = 1;
	return 1;
}

forward test_timer();
public test_timer()
{
	if(test_vehicleid != INVALID_VEHICLE_ID)
	{
		new mat[MATRIX];
		new pos[VECTOR];
		new rot[VECTOR];
		
		mat = GetVehicleMatrix(test_vehicleid);
		pos = Matrix_Multiply3x3(mat, Vector(-3.0, 10.0, -0.75));
		rot = Matrix_GetRotation(mat);
		
		pos[VEC_X] += mat[MAT_POS][VEC_X];
		pos[VEC_Y] += mat[MAT_POS][VEC_Y];
		pos[VEC_Z] += mat[MAT_POS][VEC_Z];
		
		SetObjectPos(test_objectid_matrix, pos[VEC_X], pos[VEC_Y], pos[VEC_Z]);
		SetObjectRot(test_objectid_matrix, rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
		
		pos = Matrix_Multiply3x3(mat, Vector(3.0, 10.0, -0.75));
		
		pos[VEC_X] += mat[MAT_POS][VEC_X];
		pos[VEC_Y] += mat[MAT_POS][VEC_Y];
		pos[VEC_Z] += mat[MAT_POS][VEC_Z];
		
		new Float:qw, Float:qx, Float:qy, Float:qz, Float:rx, Float:ry, Float:rz;
		
		GetVehicleRotationQuat(test_vehicleid, qw, qx, qy, qz);
		GetQuaternionAngles(qw, qx, qy, qz, rx, ry, rz);
		
		SetObjectPos(test_objectid_quat, pos[VEC_X], pos[VEC_Y], pos[VEC_Z]);
		SetObjectRot(test_objectid_quat, rx, ry, rz);
	}
}

stock GetVehicleMatrix(vehicleid)
{
	new out[MATRIX];
	
	new netgame = ReadPhysMemoryCell(IsWindows() ? 0x004F5FB8 : 0x0081CB5BC); // addresses testing in SA-MP v0.3.7 R3
	
	if(netgame != 0)
	{
		new vehiclepool = ReadPhysMemoryCell(netgame + 0x0C);
		
		if(vehiclepool != 0)
		{
			new vehicle = ReadPhysMemoryCell(vehiclepool + vehicleid * 4 + 0x3F54);
			
			if(vehicle != 0)
			{
				ReadPhysMemory(vehicle + 0x0C, out[MAT_RIGHT], 3);
				ReadPhysMemory(vehicle + 0x1C, out[MAT_FRONT], 3);
				ReadPhysMemory(vehicle + 0x2C, out[MAT_UP], 3);
				ReadPhysMemory(vehicle + 0x3C, out[MAT_POS], 3);
				
				if(test_unoccupied[vehicleid]) // unoccupied sync, only send right and front vector
					Matrix_BuildUp(out);
			}
		}
	}
	
	return out;
}

