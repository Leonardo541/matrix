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

#if defined(MATRIX_EXAMPLE_INC)
	#endinput
#endif
#define MATRIX_EXAMPLE_INC

#define RPC_PLAYER_DATA			0x89
#define PACKET_UNOCCUPIED_SYNC	0xD1

#define FAKE_NPC_PLAYERID		1003
#define FAKE_NPC_PLAYERNAME		"FAKE_NPC"

enum VERSION
{
	VERSION_037_R2[OS],
	VERSION_037_R3[OS],
	VERSION_03DL_R1[OS],
};

enum NETGAME
{
	NETGAME_VEHICLEPOOL,
};

enum VEHICLEPOOL
{
	VEHICLEPOOL_VEHICLES,
};

enum VEHICLE
{
	VEHICLE_POSITION,
	VEHICLE_MATRIX_RIGHT,
	VEHICLE_MATRIX_FRONT,
	VEHICLE_MATRIX_UP,
	VEHICLE_MATRIX_POS,
};

new ADDRESS_NETGAME[VERSION] =
{
	{ 0x00000000, 0x081CA4BC, 0x004F5FB8 },
	{ 0x00000000, 0x081CB5BC, 0x004F5FB8 },
	{ 0x00000000, 0x081D7940, 0x004FC438 },
};

new OFFSET_NETGAME[NETGAME] =
{
	0xC,
};

new OFFSET_VEHICLEPOOL[VEHICLEPOOL] =
{
	0x3F54,
};

new OFFSET_VEHICLE[VEHICLE] =
{
	0x00,
	0x0C,
	0x1C,
	0x2C,
	0x3C,
};

new vehicles_unoccupied[MAX_VEHICLES] = { 0, ... };

stock GetNetGameAddress()
{
	static netgame_address = 0;
	
	if(netgame_address == 0)
	{
		new versionname[24];
		GetServerVarAsString("version", versionname, sizeof(versionname));
		
		if(strcmp(versionname, "0.3.7-R2") == 0)
		{
			netgame_address =  ADDRESS_NETGAME[VERSION_037_R2][GetOS()];
		}
		else if(strcmp(versionname, "0.3.7-R3") == 0)
		{
			netgame_address =  ADDRESS_NETGAME[VERSION_037_R3][GetOS()];
		}
		else if(strcmp(versionname, "0.3.DL-R1") == 0)
		{
			netgame_address =  ADDRESS_NETGAME[VERSION_03DL_R1][GetOS()];
		}
		
		if(netgame_address != 0)
			netgame_address = ReadPhysMemoryCell(netgame_address);
	}
	
	return netgame_address;
}

stock GetVehiclePoolAddress()
{
	new netgame_address = GetNetGameAddress();
	
	if(netgame_address == 0)
		return 0;
	
	return ReadPhysMemoryCell(netgame_address + OFFSET_NETGAME[NETGAME_VEHICLEPOOL]);
}

stock GetVehicleAddress(vehicleid)
{
	new vehiclepool_address = GetVehiclePoolAddress();
	
	if(vehiclepool_address == 0)
		return 0;
	
	return ReadPhysMemoryCell(vehiclepool_address + OFFSET_VEHICLEPOOL[VEHICLEPOOL_VEHICLES] + vehicleid * 4);
}

stock GetVehicleMatrix(vehicleid)
{
	new out[MATRIX];
	
	new vehicle_address = GetVehicleAddress(vehicleid);
	
	if(vehicle_address != 0)
	{
		ReadPhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_RIGHT], out[MAT_RIGHT], 3);
		ReadPhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_FRONT], out[MAT_FRONT], 3);
		ReadPhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_UP], out[MAT_UP], 3);
		ReadPhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_POS], out[MAT_POS], 3);
		
		if(out[MAT_RIGHT][VEC_X] == 0.0 && out[MAT_RIGHT][VEC_Y] == 0.0 && out[MAT_RIGHT][VEC_Z] == 0.0 && out[MAT_FRONT][VEC_X] == 0.0 && out[MAT_FRONT][VEC_Y] == 0.0 && out[MAT_FRONT][VEC_Z] == 0.0)
		{
			new Float:angle;
			GetVehicleZAngle(vehicleid, angle);
			
			Matrix_SetRotation(out, 0.0, 0.0, angle);
			GetVehiclePos(vehicleid, out[MAT_POS][VEC_X], out[MAT_POS][VEC_Y], out[MAT_POS][VEC_Z]);
		}
		else if(vehicles_unoccupied[vehicleid])
		{
			Matrix_BuildUp(out);
		}
	}
	
	return out;
}

stock SetVehicleMatrix(vehicleid, mat[MATRIX])
{
	new vehicle_address = GetVehicleAddress(vehicleid);

	if(vehicle_address != 0)
	{
		#if defined PAWNRAKNET_INC_
		new streamin = 0;
		new driverid = INVALID_PLAYER_ID;
		
		for(new i = 0; i < MAX_PLAYERS; i++)
		{
			if(IsVehicleStreamedIn(vehicleid, i))
			{
				streamin = 1;
				
				if(IsPlayerInVehicle(i, vehicleid) && GetPlayerVehicleSeat(i) == 0)
				{
					driverid = i;
					break;
				}
			}
		}
		
		if(streamin)
		{
			if(driverid != INVALID_PLAYER_ID)
				SetPlayerPos(driverid, mat[MAT_POS][VEC_X], mat[MAT_POS][VEC_Y], mat[MAT_POS][VEC_Z]);
			
			new Float:health;
			GetVehicleHealth(vehicleid, health);
			
			new unoccupied_sync[PR_UnoccupiedSync];
			
			unoccupied_sync[PR_vehicleId] = vehicleid;
			unoccupied_sync[PR_seatId] = 0;
			unoccupied_sync[PR_roll][0] = mat[MAT_RIGHT][VEC_X];
			unoccupied_sync[PR_roll][1] = mat[MAT_RIGHT][VEC_Y];
			unoccupied_sync[PR_roll][2] = mat[MAT_RIGHT][VEC_Z];
			unoccupied_sync[PR_direction][0] = mat[MAT_FRONT][VEC_X];
			unoccupied_sync[PR_direction][1] = mat[MAT_FRONT][VEC_Y];
			unoccupied_sync[PR_direction][2] = mat[MAT_FRONT][VEC_Z];
			unoccupied_sync[PR_position][0] = mat[MAT_POS][VEC_X];
			unoccupied_sync[PR_position][1] = mat[MAT_POS][VEC_Y];
			unoccupied_sync[PR_position][2] = mat[MAT_POS][VEC_Z] + 1.0;
			unoccupied_sync[PR_velocity][0] = 0.0;
			unoccupied_sync[PR_velocity][1] = 0.0;
			unoccupied_sync[PR_velocity][2] = 0.0;
			unoccupied_sync[PR_angularVelocity][0] = 0.0;
			unoccupied_sync[PR_angularVelocity][1] = 0.0;
			unoccupied_sync[PR_angularVelocity][2] = 0.0;
			unoccupied_sync[PR_vehicleHealth] = health;
			
			new BitStream:bs = BS_New();
			BS_WriteInt8(bs, PACKET_UNOCCUPIED_SYNC);
			BS_WriteInt16(bs, FAKE_NPC_PLAYERID);
			BS_WriteUnoccupiedSync(bs, unoccupied_sync);
			PR_SendPacketToVehicleStream(bs, vehicleid);
			BS_Delete(bs);
			
			if(driverid != INVALID_PLAYER_ID)
				PutPlayerInVehicle(driverid, vehicleid, 0);
		}
		#endif
		
		WritePhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_POSITION], mat[MAT_POS], 3);
		
		WritePhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_RIGHT], mat[MAT_RIGHT], 3);
		WritePhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_FRONT], mat[MAT_FRONT], 3);
		WritePhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_UP], mat[MAT_UP], 3);
		WritePhysMemory(vehicle_address + OFFSET_VEHICLE[VEHICLE_MATRIX_POS], mat[MAT_POS], 3);
	}
}

stock GetVehicleRot(vehicleid)
{
	new out[VECTOR];
	new mat[MATRIX];
	
	mat = GetVehicleMatrix(vehicleid);
	out = Matrix_GetRotation(mat);
	
	if(mat[MAT_UP][VEC_Z] >= 0.0)
	{
		out[VEC_X] = -(out[VEC_X] - 180.0);
		out[VEC_Y] = out[VEC_Y] - 180.0;
		out[VEC_Z] = out[VEC_Z] - 180.0;
	}
	
	return out;
}

stock SetVehicleRot(vehicleid, rot[VECTOR])
{
	new mat[MATRIX];
	
	Matrix_SetRotation(mat, rot[VEC_X], rot[VEC_Y], rot[VEC_Z]);
	GetVehiclePos(vehicleid, mat[MAT_POS][VEC_X], mat[MAT_POS][VEC_Y], mat[MAT_POS][VEC_Z]);
	
	SetVehicleMatrix(vehicleid, mat);
}

stock GetVehicleVelocityFrontVector(vehicleid)
{
	new out[VECTOR];
	new mat[MATRIX];
	
	GetVehicleVelocity(vehicleid, out[VEC_X], out[VEC_Y], out[VEC_Z]);
	
	mat = GetVehicleMatrix(vehicleid);
	mat = Matrix_Invert(mat);
	out = Matrix_Multiply3x3(mat, out);
	
	return out;
}

stock SetVehicleVelocityFrontVector(vehicleid, vec[VECTOR])
{
	new mat[MATRIX];
	
	mat = GetVehicleMatrix(vehicleid);
	vec = Matrix_Multiply3x3(mat, vec);
	
	SetVehicleVelocity(vehicleid, vec[VEC_X], vec[VEC_Y], vec[VEC_Z]);
}

#if defined _MAT_OnPlayerConnect
	forward _MAT_OnPlayerConnect(playerid);
#endif

#if defined _MAT_OnPlayerStateChange
	forward _MAT_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

#if defined _MAT_OnUnoccupiedVehicleUpdate
	forward _MAT_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
#endif

public OnPlayerConnect(playerid)
{
	#if defined PAWNRAKNET_INC_
	new BitStream:bs = BS_New();
	BS_WriteInt16(bs, FAKE_NPC_PLAYERID);
	BS_WriteInt32(bs, 0x00000000);
	BS_WriteInt8(bs, 1);
	BS_WriteInt8(bs, strlen(FAKE_NPC_PLAYERNAME));
	BS_WriteString(bs, FAKE_NPC_PLAYERNAME);
	PR_SendRPC(bs, playerid, RPC_PLAYER_DATA);
	BS_Delete(bs);
	#endif
	
	#if defined _MAT_OnPlayerConnect
		return _MAT_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER)
	{
		new vehicleid = GetPlayerVehicleID(playerid);

		if(vehicleid != 0)
		{
			vehicles_unoccupied[vehicleid] = 0;
		}
	}
	
	#if defined _MAT_OnPlayerStateChange
		return _MAT_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	vehicles_unoccupied[vehicleid] = 1;
	
	#if defined _MAT_OnPlayerStateChange
		return _MAT_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, new_x, new_y, new_z, vel_x, vel_y, vel_z);
	#else
		return 1;
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect _MAT_OnPlayerConnect

#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange _MAT_OnPlayerStateChange

#if defined _ALS_OnUnoccupiedVehicleUpdate
	#undef OnUnoccupiedVehicleUpdate
#else
	#define _ALS_OnUnoccupiedVehicleUpdate
#endif

#define OnUnoccupiedVehicleUpdate _MAT_OnUnoccupiedVehicleUpdate
