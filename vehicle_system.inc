// ========================================
// VEHICLE SYSTEM
// ========================================


// ========================================
// VEHICLE DATA
// ========================================

// GAME VEHICLE
new bool:SystemVehicle[MAX_VEHICLES];

new VehicleDatabaseID[MAX_VEHICLES];
new VehicleOwnerType[MAX_VEHICLES];
new VehicleOwnerID[MAX_VEHICLES];


// DATABASE VEHICLE SLOT
new VehicleDBID[MAX_VEHICLES];
new VehicleDBOwnerType[MAX_VEHICLES];
new VehicleDBOwnerID[MAX_VEHICLES];

new VehicleDBModel[MAX_VEHICLES];
new VehicleDBColor1[MAX_VEHICLES];
new VehicleDBColor2[MAX_VEHICLES];

new Float:VehicleDBFuel[MAX_VEHICLES];
new Float:VehicleDBHealth[MAX_VEHICLES];

new VehicleDBStatus[MAX_VEHICLES];
new VehicleDBGameID[MAX_VEHICLES];
new bool:VehicleDBSaving[MAX_VEHICLES];

new Float:VehicleDBPosX[MAX_VEHICLES];
new Float:VehicleDBPosY[MAX_VEHICLES];
new Float:VehicleDBPosZ[MAX_VEHICLES];
new Float:VehicleDBPosA[MAX_VEHICLES];

new VehicleDBInterior[MAX_VEHICLES];
new VehicleDBWorld[MAX_VEHICLES];


// ========================================
// GARAGE VEHICLE SPAWN
// ========================================

#define VEHICLE_GARAGE_SPAWN_X       2612.3354
#define VEHICLE_GARAGE_SPAWN_Y       2279.4326
#define VEHICLE_GARAGE_SPAWN_Z       10.5474
#define VEHICLE_GARAGE_SPAWN_A       89.6332


// ========================================
// CHECK VEHICLE OWNER
// ========================================

stock bool:Vehicle_IsPlayerOwner(playerid, vehicleid)
{
    if(!IsPlayerConnected(playerid))
        return false;

    if(!IsValidVehicle(vehicleid))
        return false;

    if(PlayerAccountID[playerid] <= 0)
        return false;

    if(VehicleOwnerType[vehicleid] != VEHICLE_OWNER_PLAYER)
        return false;

    if(VehicleOwnerID[vehicleid] != PlayerAccountID[playerid])
        return false;

    return true;
}


// ========================================
// FIND DATABASE VEHICLE SLOT
// ========================================

stock Vehicle_GetDatabaseSlot(databaseID)
{
    for(new slot = 0; slot < MAX_VEHICLES; slot++)
    {
        if(VehicleDBID[slot] == databaseID)
            return slot;
    }

    return -1;
}


// ========================================
// FIND FREE DATABASE VEHICLE SLOT
// ========================================

stock Vehicle_GetFreeDatabaseSlot()
{
    for(new slot = 0; slot < MAX_VEHICLES; slot++)
    {
        if(VehicleDBID[slot] == 0)
            return slot;
    }

    return -1;
}


// ========================================
// VEHICLE LOCK
// ========================================

stock Vehicle_SetLock(vehicleid, bool:locked)
{
    if(!IsValidVehicle(vehicleid))
        return 0;

    new bool:engine;
    new bool:lights;
    new bool:alarm;
    new bool:doors;
    new bool:bonnet;
    new bool:boot;
    new bool:objective;

    GetVehicleParamsEx(
        vehicleid,
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective
    );

    SetVehicleParamsEx(
        vehicleid,
        engine,
        lights,
        alarm,
        locked,
        bonnet,
        boot,
        objective
    );

    return 1;
}


// ========================================
// CHECK VEHICLE LOCK STATE
// ========================================

stock bool:Vehicle_IsLocked(vehicleid)
{
    if(!IsValidVehicle(vehicleid))
        return false;

    new bool:engine;
    new bool:lights;
    new bool:alarm;
    new bool:doors;
    new bool:bonnet;
    new bool:boot;
    new bool:objective;

    GetVehicleParamsEx(
        vehicleid,
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective
    );

    return doors;
}


// ========================================
// CREATE GAME VEHICLE
// ========================================

stock Vehicle_SpawnDatabaseVehicle(
    slot,
    bool:spawnAtGarage = false
)
{
    if(slot < 0 || slot >= MAX_VEHICLES)
        return INVALID_VEHICLE_ID;

    if(VehicleDBID[slot] <= 0)
        return INVALID_VEHICLE_ID;

    if(VehicleDBGameID[slot] != INVALID_VEHICLE_ID)
        return VehicleDBGameID[slot];

    new vehicleid;

    new Float:spawnX;
    new Float:spawnY;
    new Float:spawnZ;
    new Float:spawnA;

    if(spawnAtGarage)
    {
        spawnX = VEHICLE_GARAGE_SPAWN_X;
        spawnY = VEHICLE_GARAGE_SPAWN_Y;
        spawnZ = VEHICLE_GARAGE_SPAWN_Z;
        spawnA = VEHICLE_GARAGE_SPAWN_A;
    }
    else
    {
        spawnX = VehicleDBPosX[slot];
        spawnY = VehicleDBPosY[slot];
        spawnZ = VehicleDBPosZ[slot];
        spawnA = VehicleDBPosA[slot];
    }

    vehicleid = CreateVehicle(
        VehicleDBModel[slot],
        spawnX,
        spawnY,
        spawnZ,
        spawnA,
        VehicleDBColor1[slot],
        VehicleDBColor2[slot],
        -1
    );

    if(vehicleid == INVALID_VEHICLE_ID)
        return INVALID_VEHICLE_ID;

    VehicleDBGameID[slot] = vehicleid;

    VehicleDatabaseID[vehicleid] = VehicleDBID[slot];
    VehicleOwnerType[vehicleid] = VehicleDBOwnerType[slot];
    VehicleOwnerID[vehicleid] = VehicleDBOwnerID[slot];

    LinkVehicleToInterior(
        vehicleid,
        VehicleDBInterior[slot]
    );

    SetVehicleVirtualWorld(
        vehicleid,
        VehicleDBWorld[slot]
    );

    // ========================================
    // VEHICLE HEALTH
    // ========================================

    if(VehicleDBStatus[slot] == VEHICLE_STATUS_BROKEN)
    {
        SetVehicleHealth(
            vehicleid,
            250.0
        );

        // Broken vehicle starts with engine off.
        SetVehicleParamsEx(
            vehicleid,
            false,
            false,
            false,
            false,
            false,
            false,
            false
        );
    }
    else
    {
        SetVehicleHealth(
            vehicleid,
            VehicleDBHealth[slot]
        );
    }

    // ========================================
    // VEHICLE DEFAULT LOCK STATE
    // ========================================

    Vehicle_SetLock(
        vehicleid,
        false
    );

    return vehicleid;
}


// ========================================
// CALL VEHICLE
// ========================================

stock Vehicle_Call(playerid, databaseID)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(PlayerAccountID[playerid] <= 0)
        return 0;

    new slot;

    slot = Vehicle_GetDatabaseSlot(databaseID);

    if(slot == -1)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: ไม่พบข้อมูลรถคันนี้"
        );

        return 0;
    }

    if(
        VehicleDBOwnerType[slot] != VEHICLE_OWNER_PLAYER ||
        VehicleDBOwnerID[slot] != PlayerAccountID[playerid]
    )
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้ไม่ใช่รถของคุณ"
        );

        return 0;
    }

    if(
        VehicleDBStatus[slot] != VEHICLE_STATUS_GARAGE &&
        VehicleDBStatus[slot] != VEHICLE_STATUS_BROKEN
    )
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้ถูกเรียกออกมาแล้ว"
        );

        return 0;
    }

    if(VehicleDBSaving[slot])
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้กำลังบันทึกข้อมูล"
        );

        return 0;
    }

    new vehicleid;
    new bool:isBroken;

    isBroken =
        (VehicleDBStatus[slot] == VEHICLE_STATUS_BROKEN);

    // ========================================
    // SPAWN AT GARAGE
    // ========================================

    vehicleid = Vehicle_SpawnDatabaseVehicle(
        slot,
        true
    );

    if(vehicleid == INVALID_VEHICLE_ID)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: ไม่สามารถเรียกรถได้"
        );

        return 0;
    }

    VehicleDBStatus[slot] = VEHICLE_STATUS_OUT;

    // ========================================
    // PUT PLAYER INTO VEHICLE
    // ========================================

    PutPlayerInVehicle(
        playerid,
        vehicleid,
        0
    );

    if(isBroken)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FFFF00}[Vehicle] {FFFFFF}: เรียกรถออกมาแล้ว แต่รถคันนี้อยู่ในสภาพพัง"
        );
    }
    else
    {
        SendClientMessage(
            playerid,
            -1,
            "{00FF00}[Vehicle] {FFFFFF}: เรียกรถออกจาก Garage แล้ว"
        );
    }

    return vehicleid;
}


// ========================================
// STORE VEHICLE
// ========================================

stock Vehicle_Store(playerid, vehicleid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(PlayerAccountID[playerid] <= 0)
        return 0;

    if(!IsValidVehicle(vehicleid))
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: ไม่พบรถคันนี้"
        );

        return 0;
    }

    if(!Vehicle_IsPlayerOwner(playerid, vehicleid))
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: คุณไม่ใช่เจ้าของรถคันนี้"
        );

        return 0;
    }

    new databaseID;
    new slot;
    new status;

    databaseID = VehicleDatabaseID[vehicleid];

    if(databaseID <= 0)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้ไม่ใช่รถที่อยู่ในระบบ"
        );

        return 0;
    }

    slot = Vehicle_GetDatabaseSlot(databaseID);

    if(slot == -1)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: ไม่พบข้อมูลรถคันนี้"
        );

        return 0;
    }

    if(VehicleDBStatus[slot] != VEHICLE_STATUS_OUT)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้อยู่ใน Garage แล้ว"
        );

        return 0;
    }

    if(VehicleDBGameID[slot] != vehicleid)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: ข้อมูล Game Vehicle ไม่ตรงกัน"
        );

        return 0;
    }

    if(VehicleDBSaving[slot])
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้กำลังบันทึกข้อมูล"
        );

        return 0;
    }


    // ========================================
    // CHECK ORIGINAL VEHICLE STATE
    // ========================================

    status = VehicleDBStatus[slot];

    // A broken vehicle called out from BROKEN
    // must remain BROKEN after storing.
    if(VehicleDBHealth[slot] <= 0.0)
    {
        status = VEHICLE_STATUS_BROKEN;
    }
    else
    {
        status = VEHICLE_STATUS_GARAGE;

        GetVehicleHealth(
            vehicleid,
            VehicleDBHealth[slot]
        );
    }


    // ========================================
    // SAVE VEHICLE DATA
    // ========================================

    VehicleDBSaving[slot] = true;

    new query[256];

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE vehicles SET health = %.4f, status = %d WHERE id = %d",
        VehicleDBHealth[slot],
        status,
        VehicleDBID[slot]
    );

    mysql_tquery(
        g_SQL,
        query,
        "OnVehicleStore",
        "iiii",
        playerid,
        vehicleid,
        slot,
        status
    );

    SendClientMessage(
        playerid,
        -1,
        "{FFFF00}[Vehicle] {FFFFFF}: กำลังบันทึกข้อมูลรถ..."
    );

    return 1;
}


// ========================================
// STORE PLAYER VEHICLES
// ========================================

stock Vehicle_StorePlayerVehicles(playerid)
{
    if(PlayerAccountID[playerid] <= 0)
        return 0;

    for(new slot = 0; slot < MAX_VEHICLES; slot++)
    {
        if(VehicleDBID[slot] <= 0)
            continue;

        if(
            VehicleDBOwnerType[slot] != VEHICLE_OWNER_PLAYER ||
            VehicleDBOwnerID[slot] != PlayerAccountID[playerid]
        )
        {
            continue;
        }

        if(VehicleDBStatus[slot] != VEHICLE_STATUS_OUT)
            continue;

        if(VehicleDBGameID[slot] == INVALID_VEHICLE_ID)
            continue;

        if(!IsValidVehicle(VehicleDBGameID[slot]))
            continue;

        if(VehicleDBSaving[slot])
            continue;

        Vehicle_Store(
            playerid,
            VehicleDBGameID[slot]
        );
    }

    return 1;
}


// ========================================
// VEHICLE STORE CALLBACK
// ========================================

forward OnVehicleStore(playerid, vehicleid, slot, status);

public OnVehicleStore(
    playerid,
    vehicleid,
    slot,
    status
)
{
    if(slot < 0 || slot >= MAX_VEHICLES)
        return 0;

    VehicleDBSaving[slot] = false;

    if(VehicleDBID[slot] <= 0)
        return 0;

    if(VehicleDBGameID[slot] != vehicleid)
        return 0;

    if(!IsValidVehicle(vehicleid))
    {
        VehicleDBGameID[slot] = INVALID_VEHICLE_ID;
        VehicleDBStatus[slot] = status;

        return 1;
    }


    // ========================================
    // CHANGE VEHICLE STATUS
    // ========================================

    VehicleDBStatus[slot] = status;
    VehicleDBGameID[slot] = INVALID_VEHICLE_ID;


    // ========================================
    // DESTROY GAME VEHICLE
    // ========================================

    DestroyVehicle(vehicleid);

    VehicleDatabaseID[vehicleid] = 0;
    VehicleOwnerType[vehicleid] = 0;
    VehicleOwnerID[vehicleid] = 0;


    if(IsPlayerConnected(playerid))
    {
        if(status == VEHICLE_STATUS_BROKEN)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FFFF00}[Vehicle] {FFFFFF}: เก็บรถแล้ว รถยังอยู่ในสถานะพัง"
            );
        }
        else
        {
            SendClientMessage(
                playerid,
                -1,
                "{00FF00}[Vehicle] {FFFFFF}: บันทึกข้อมูลและเก็บรถเข้า Garage แล้ว"
            );
        }
    }

    return 1;
}


// ========================================
// HANDLE BROKEN VEHICLE
// ========================================

stock Vehicle_HandleDeath(vehicleid)
{
    if(vehicleid < 0 || vehicleid >= MAX_VEHICLES)
        return 0;

    if(VehicleDatabaseID[vehicleid] <= 0)
        return 0;

    new databaseID;
    new slot;

    databaseID = VehicleDatabaseID[vehicleid];

    slot = Vehicle_GetDatabaseSlot(databaseID);

    if(slot == -1)
        return 0;

    if(VehicleDBGameID[slot] != vehicleid)
        return 0;

    if(VehicleDBStatus[slot] != VEHICLE_STATUS_OUT)
        return 0;

    if(VehicleDBSaving[slot])
        return 0;


    // ========================================
    // SET BROKEN STATE
    // ========================================

    VehicleDBHealth[slot] = 0.0;
    VehicleDBStatus[slot] = VEHICLE_STATUS_BROKEN;
    VehicleDBGameID[slot] = INVALID_VEHICLE_ID;
    VehicleDBSaving[slot] = true;


    // ========================================
    // SAVE BROKEN VEHICLE
    // ========================================

    new query[256];

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE vehicles SET health = 0.0, status = %d WHERE id = %d",
        VEHICLE_STATUS_BROKEN,
        databaseID
    );

    mysql_tquery(
        g_SQL,
        query,
        "OnVehicleBroken",
        "ii",
        databaseID,
        slot
    );


    // ========================================
    // REMOVE GAME VEHICLE
    // ========================================

    DestroyVehicle(vehicleid);

    VehicleDatabaseID[vehicleid] = 0;
    VehicleOwnerType[vehicleid] = 0;
    VehicleOwnerID[vehicleid] = 0;

    printf(
        "[Vehicle] Vehicle #%d is broken.",
        databaseID
    );

    return 1;
}


// ========================================
// BROKEN VEHICLE CALLBACK
// ========================================

forward OnVehicleBroken(databaseID, slot);

public OnVehicleBroken(databaseID, slot)
{
    if(slot < 0 || slot >= MAX_VEHICLES)
        return 0;

    VehicleDBSaving[slot] = false;

    if(VehicleDBID[slot] != databaseID)
        return 0;

    if(VehicleDBStatus[slot] != VEHICLE_STATUS_BROKEN)
        return 0;

    printf(
        "[Vehicle] Vehicle #%d broken state saved.",
        databaseID
    );

    return 1;
}


// ========================================
// CREATE OWNED VEHICLE
// ========================================

stock Vehicle_CreateOwned(
    databaseID,
    ownerType,
    ownerID,
    model,
    color1,
    color2,
    Float:x,
    Float:y,
    Float:z,
    Float:a,
    interior = 0,
    world = 0
)
{
    new slot;
    new vehicleid;

    slot = Vehicle_GetDatabaseSlot(databaseID);

    if(slot == -1)
        slot = Vehicle_GetFreeDatabaseSlot();

    if(slot == -1)
        return INVALID_VEHICLE_ID;

    VehicleDBID[slot] = databaseID;
    VehicleDBOwnerType[slot] = ownerType;
    VehicleDBOwnerID[slot] = ownerID;

    VehicleDBModel[slot] = model;
    VehicleDBColor1[slot] = color1;
    VehicleDBColor2[slot] = color2;

    VehicleDBFuel[slot] = 100.0;
    VehicleDBHealth[slot] = 1000.0;

    VehicleDBStatus[slot] = VEHICLE_STATUS_OUT;
    VehicleDBGameID[slot] = INVALID_VEHICLE_ID;
    VehicleDBSaving[slot] = false;

    VehicleDBPosX[slot] = x;
    VehicleDBPosY[slot] = y;
    VehicleDBPosZ[slot] = z;
    VehicleDBPosA[slot] = a;

    VehicleDBInterior[slot] = interior;
    VehicleDBWorld[slot] = world;

    vehicleid = Vehicle_SpawnDatabaseVehicle(slot);

    return vehicleid;
}


// ========================================
// CLEAR DATABASE VEHICLE DATA
// ========================================

stock Vehicle_ClearDatabaseData()
{
    for(new slot = 0; slot < MAX_VEHICLES; slot++)
    {
        VehicleDBID[slot] = 0;
        VehicleDBOwnerType[slot] = 0;
        VehicleDBOwnerID[slot] = 0;

        VehicleDBModel[slot] = 0;
        VehicleDBColor1[slot] = 0;
        VehicleDBColor2[slot] = 0;

        VehicleDBFuel[slot] = 0.0;
        VehicleDBHealth[slot] = 0.0;

        VehicleDBStatus[slot] = 0;
        VehicleDBGameID[slot] = INVALID_VEHICLE_ID;
        VehicleDBSaving[slot] = false;

        VehicleDBPosX[slot] = 0.0;
        VehicleDBPosY[slot] = 0.0;
        VehicleDBPosZ[slot] = 0.0;
        VehicleDBPosA[slot] = 0.0;

        VehicleDBInterior[slot] = 0;
        VehicleDBWorld[slot] = 0;
    }

    return 1;
}


// ========================================
// CLEAR ALL GAME VEHICLES
// ========================================

stock Vehicle_ClearAll()
{
    for(new vehicleid = 1; vehicleid < MAX_VEHICLES; vehicleid++)
    {
        if(!IsValidVehicle(vehicleid))
            continue;

        if(
            !SystemVehicle[vehicleid] &&
            VehicleDatabaseID[vehicleid] <= 0
        )
        {
            continue;
        }

        DestroyVehicle(vehicleid);

        SystemVehicle[vehicleid] = false;
        VehicleDatabaseID[vehicleid] = 0;
        VehicleOwnerType[vehicleid] = 0;
        VehicleOwnerID[vehicleid] = 0;
    }

    Vehicle_ClearDatabaseData();

    return 1;
}


// ========================================
// LOAD ALL DATABASE VEHICLES
// ========================================

stock Vehicle_LoadAll()
{
    Vehicle_ClearAll();

    new query[512];

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "SELECT id, owner_type, owner_id, model, color1, color2, fuel, health, status, pos_x, pos_y, pos_z, pos_a, interior, world FROM vehicles ORDER BY id ASC"
    );

    mysql_tquery(
        g_SQL,
        query,
        "OnVehicleLoadAll"
    );

    return 1;
}


// ========================================
// VEHICLE LOAD CALLBACK
// ========================================

forward OnVehicleLoadAll();

public OnVehicleLoadAll()
{
    new rows;

    cache_get_row_count(rows);

    new loaded = 0;
    new spawned = 0;
    new garage = 0;
    new broken = 0;

    for(new row = 0; row < rows; row++)
    {
        new databaseID;
        new ownerType;
        new ownerID;
        new model;
        new color1;
        new color2;
        new status;
        new interior;
        new world;

        new Float:fuel;
        new Float:health;

        new Float:x;
        new Float:y;
        new Float:z;
        new Float:a;

        cache_get_value_name_int(
            row,
            "id",
            databaseID
        );

        cache_get_value_name_int(
            row,
            "owner_type",
            ownerType
        );

        cache_get_value_name_int(
            row,
            "owner_id",
            ownerID
        );

        cache_get_value_name_int(
            row,
            "model",
            model
        );

        cache_get_value_name_int(
            row,
            "color1",
            color1
        );

        cache_get_value_name_int(
            row,
            "color2",
            color2
        );

        cache_get_value_name_float(
            row,
            "fuel",
            fuel
        );

        cache_get_value_name_float(
            row,
            "health",
            health
        );

        cache_get_value_name_int(
            row,
            "status",
            status
        );

        cache_get_value_name_float(
            row,
            "pos_x",
            x
        );

        cache_get_value_name_float(
            row,
            "pos_y",
            y
        );

        cache_get_value_name_float(
            row,
            "pos_z",
            z
        );

        cache_get_value_name_float(
            row,
            "pos_a",
            a
        );

        cache_get_value_name_int(
            row,
            "interior",
            interior
        );

        cache_get_value_name_int(
            row,
            "world",
            world
        );


        // ========================================
        // VALIDATE OWNER TYPE
        // ========================================

        if(
            ownerType != VEHICLE_OWNER_PLAYER &&
            ownerType != VEHICLE_OWNER_FACTION &&
            ownerType != VEHICLE_OWNER_JOB
        )
        {
            continue;
        }


        // ========================================
        // VALIDATE OWNER ID
        // ========================================

        if(ownerID <= 0)
            continue;


        // ========================================
        // VALIDATE MODEL
        // ========================================

        if(model <= 0)
            continue;


        // ========================================
        // VALIDATE STATUS
        // ========================================

        if(
            status != VEHICLE_STATUS_GARAGE &&
            status != VEHICLE_STATUS_OUT &&
            status != VEHICLE_STATUS_BROKEN
        )
        {
            status = VEHICLE_STATUS_GARAGE;
        }


        // ========================================
        // GET DATABASE SLOT
        // ========================================

        new slot;

        slot = Vehicle_GetFreeDatabaseSlot();

        if(slot == -1)
            continue;


        // ========================================
        // STORE DATABASE DATA
        // ========================================

        VehicleDBID[slot] = databaseID;
        VehicleDBOwnerType[slot] = ownerType;
        VehicleDBOwnerID[slot] = ownerID;

        VehicleDBModel[slot] = model;
        VehicleDBColor1[slot] = color1;
        VehicleDBColor2[slot] = color2;

        VehicleDBFuel[slot] = fuel;
        VehicleDBHealth[slot] = health;

        VehicleDBStatus[slot] = status;
        VehicleDBGameID[slot] = INVALID_VEHICLE_ID;
        VehicleDBSaving[slot] = false;

        VehicleDBPosX[slot] = x;
        VehicleDBPosY[slot] = y;
        VehicleDBPosZ[slot] = z;
        VehicleDBPosA[slot] = a;

        VehicleDBInterior[slot] = interior;
        VehicleDBWorld[slot] = world;

        loaded++;


        // ========================================
        // GARAGE
        // ========================================

        if(status == VEHICLE_STATUS_GARAGE)
        {
            garage++;
            continue;
        }


        // ========================================
        // BROKEN
        // ========================================

        if(status == VEHICLE_STATUS_BROKEN)
        {
            broken++;
            continue;
        }


        // ========================================
        // OUT
        // ========================================

        if(status == VEHICLE_STATUS_OUT)
        {
            if(
                Vehicle_SpawnDatabaseVehicle(slot)
                != INVALID_VEHICLE_ID
            )
            {
                spawned++;
            }
        }
    }


    printf(
        "[Vehicle] Loaded %d vehicle rows. Garage: %d | Spawned: %d | Broken: %d",
        loaded,
        garage,
        spawned,
        broken
    );

    return 1;
}


// ========================================
// VEHICLE RESET WARNING
// ========================================

forward VehicleResetWarning();
forward VehicleReset();

public VehicleResetWarning()
{
    SendClientMessageToAll(
        -1,
        "{FF0000}[Auto] {FFFFFF}: อีก 10 วินาที เซิฟเวอร์จะเข้าสู่ระบบจัดเก็บยานพาหนะที่ไม่ได้ใช้งาน !"
    );

    SetTimer(
        "VehicleReset",
        10000,
        false
    );

    return 1;
}


// ========================================
// VEHICLE RESET
// ========================================

public VehicleReset()
{
    new bool:vehicleOccupied;

    for(new vehicleid = 1; vehicleid < MAX_VEHICLES; vehicleid++)
    {
        if(!SystemVehicle[vehicleid])
            continue;

        if(!IsValidVehicle(vehicleid))
            continue;

        vehicleOccupied = false;

        for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
        {
            if(!IsPlayerConnected(playerid))
                continue;

            if(IsPlayerInVehicle(playerid, vehicleid))
            {
                vehicleOccupied = true;
                break;
            }
        }

        if(vehicleOccupied)
            continue;

        DestroyVehicle(vehicleid);

        SystemVehicle[vehicleid] = false;
    }

    SendClientMessageToAll(
        -1,
        "{FF0000}[Auto] {FFFFFF}: ขณะนี้ยานพาหนะที่ไม่มีผู้ใช้งานได้ถูกจัดเก็บเรียบร้อยแล้ว"
    );

    return 1;
}