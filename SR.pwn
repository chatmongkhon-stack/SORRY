#include <open.mp>
#include <samp_bcrypt>
#include "config/definitions.inc"
#include "systems/database.inc"
#include "systems/level.inc"
#include "systems/status.inc"
#include "systems/payday.inc"
#include "systems/death.inc"
#include "systems/vehicle_system.inc"
#include "systems/shop.inc"
#include "callbacks/dialogs.inc"
#include "callbacks/player.inc"
#include "callbacks/vehicle.inc"
#include "callbacks/server.inc"

#if defined FILTERSCRIPT

public OnFilterScriptInit()
{
    print("\n--------------------------------------");
    print(" Blank Filterscript by your name here");
    print("--------------------------------------\n");
    return 1;
}

public OnFilterScriptExit() return 1;

#else

main()
{
    print("\n----------------------------------");
    print(" GTA SORRY ROLEPLAY THAILAND ");
    print("----------------------------------\n");
}

#endif

public OnGameModeInit()
{
    if(!Database_Connect())
    {
        print("[Auto] : Database connection failed. Server will shut down.");
        SendRconCommand("exit");
        return 0;
    }

    // ========================================
    // LOAD ALL DATABASE VEHICLES
    // ========================================

    Vehicle_LoadAll();

    SetTimer("PaydayTimer", 3600000, true);
    SetTimer("DeathTimer", 1000, true);
    SetTimer("PaydayTimeTimer", 60000, true);
    SetTimer("StatusTimer", 60000, true);
    SetTimer("Database_AutoSave", 300000, true);
    SetTimer("VehicleResetWarning", 600000, true);

    SetGameModeText("eSupport");

    CreatePickup(954,1,2026.0940,1540.2754,10.8203,-1); // pickup จุดเบิกรถเด็กใหม่
    Create3DTextLabel("{00FF00}[ กด Y ]\n{FFFFFF}เบิกรถ",-1,2026.0940,1540.2754,10.8203,20.0,-1,false);
    CreatePickup(19197,1,2194.9404,1990.9764,12.2969,-1); // pickup จุดเข้า 7-11
    CreatePickup(19197,1,-27.4263,-58.2725,1003.5469,-1); // pickup จุดออก 7-11
    Create3DTextLabel("{00FF00}[ กด N ]\n{FFFFFF}ซื้อสินค้า",-1,-23.4996,-55.6303,1003.5469,20.0,-1,false);
    CreatePickup(19134,1,2637.2996,2345.6279,10.6719,-1); // pickup การาด
    Create3DTextLabel("{00FF00}[ กด N ]\n{FFFFFF}เรียกรถ/เก็บรถ",-1,2637.2996,2345.6279,10.6719,20.0,-1,false);

    AddPlayerClass(
        26,
        2002.1216,
        1544.4724,
        13.5859,
        271.5414,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0
    );

    return 1;
}

public OnGameModeExit()
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid)) continue;
        if(!PlayerLoggedIn[playerid]) continue;

        Database_SavePlayer(playerid);
        Database_SavePaydayTime(playerid);
    }

    return 1;
}