// ========================================
// VEHICLE CALLBACKS
// ========================================

public OnVehicleSpawn(vehicleid)
{
    return 1;
}


public OnVehicleDeath(vehicleid, killerid)
{
    // ========================================
    // PERSISTENT VEHICLE
    // ========================================

    Vehicle_HandleDeath(vehicleid);

    return 1;
}


public OnVehicleMod(playerid, vehicleid, componentid)
{
    return 1;
}


public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
    return 1;
}


public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
    return 1;
}