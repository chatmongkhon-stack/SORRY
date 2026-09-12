// ========================================
// DEATH SYSTEM
// ========================================

#define DEATH_TIME          300
#define DEATH_HOSPITAL_FEE 2000

forward DeathTimer();
forward Death_LeaveSpectating(playerid);
forward Death_ApplyAnimationDelayed(playerid);
forward Death_Restore(playerid);


// ========================================
// START DEATH STATE
// ========================================

stock Death_Start(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(PlayerDead[playerid])
        return 0;


    // ========================================
    // SET DEATH STATE
    // ========================================

    PlayerDead[playerid] = true;
    PlayerDeathTime[playerid] = DEATH_TIME;


    // ========================================
    // SAVE DEATH POSITION
    // ========================================

    GetPlayerPos(
        playerid,
        PlayerDeathX[playerid],
        PlayerDeathY[playerid],
        PlayerDeathZ[playerid]
    );

    GetPlayerFacingAngle(
        playerid,
        PlayerDeathAngle[playerid]
    );


    // ========================================
    // SAVE TO DATABASE
    //
    // บันทึกทันทีที่ตาย
    // ========================================

    Database_SavePlayer(
        playerid
    );


    // ========================================
    // SAVE SPAWN POSITION
    // ========================================

    SetSpawnInfo(
        playerid,
        NO_TEAM,
        26,
        PlayerDeathX[playerid],
        PlayerDeathY[playerid],
        PlayerDeathZ[playerid],
        PlayerDeathAngle[playerid],
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0
    );


    // ========================================
    // ENTER SPECTATING
    // ========================================

    TogglePlayerSpectating(
        playerid,
        true
    );


    // ========================================
    // LEAVE SPECTATING
    // ========================================

    SetTimerEx(
        "Death_LeaveSpectating",
        100,
        false,
        "i",
        playerid
    );


    return 1;
}


// ========================================
// LEAVE SPECTATING
// ========================================

public Death_LeaveSpectating(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(!PlayerLoggedIn[playerid])
        return 1;

    if(!PlayerDead[playerid])
        return 1;


    SetSpawnInfo(
        playerid,
        NO_TEAM,
        26,
        PlayerDeathX[playerid],
        PlayerDeathY[playerid],
        PlayerDeathZ[playerid],
        PlayerDeathAngle[playerid],
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0
    );


    TogglePlayerSpectating(
        playerid,
        false
    );


    return 1;
}


// ========================================
// RESTORE DEATH AFTER LOGIN
// ========================================

stock Death_Restore(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(!PlayerDead[playerid])
        return 0;


    // ========================================
    // DEATH TIME FINISHED
    // ========================================

    if(PlayerDeathTime[playerid] <= 0)
    {
        PlayerDeathTime[playerid] = -1;

        TogglePlayerSpectating(
            playerid,
            true
        );

        Death_ShowRespawnDialog(
            playerid
        );

        return 1;
    }


    // ========================================
    // SET DEATH SPAWN
    // ========================================

    SetSpawnInfo(
        playerid,
        NO_TEAM,
        26,
        PlayerDeathX[playerid],
        PlayerDeathY[playerid],
        PlayerDeathZ[playerid],
        PlayerDeathAngle[playerid],
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0
    );


    // ========================================
    // ENTER SPECTATING FIRST
    // ========================================

    TogglePlayerSpectating(
        playerid,
        true
    );


    // ========================================
    // LEAVE SPECTATING
    // ========================================

    SetTimerEx(
        "Death_LeaveSpectating",
        100,
        false,
        "i",
        playerid
    );


    return 1;
}


// ========================================
// APPLY DEATH ANIMATION
// ========================================

stock Death_ApplyAnimation(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerDead[playerid])
        return 0;


    // ========================================
    // RESTORE DEATH POSITION
    // ========================================

    SetPlayerPos(
        playerid,
        PlayerDeathX[playerid],
        PlayerDeathY[playerid],
        PlayerDeathZ[playerid]
    );

    SetPlayerFacingAngle(
        playerid,
        PlayerDeathAngle[playerid]
    );

    SetCameraBehindPlayer(
        playerid
    );


    // ========================================
    // LOCK PLAYER
    // ========================================

    TogglePlayerControllable(
        playerid,
        false
    );


    // ========================================
    // CLEAR OLD ANIMATION
    // ========================================

    ClearAnimations(
        playerid
    );


    // ========================================
    // DEATH ANIMATION
    // ========================================

    ApplyAnimation(
        playerid,
        "PED",
        "KO_shot_stom",
        4.1,
        false,
        true,
        true,
        true,
        0,
        SYNC_ALL
    );


    return 1;
}


// ========================================
// DELAYED DEATH ANIMATION
// ========================================

public Death_ApplyAnimationDelayed(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(!PlayerLoggedIn[playerid])
        return 1;

    if(!PlayerDead[playerid])
        return 1;


    Death_ApplyAnimation(
        playerid
    );


    return 1;
}


// ========================================
// CLEAR DEATH STATE
// ========================================

stock Death_Clear(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;


    PlayerDead[playerid] = false;
    PlayerDeathTime[playerid] = 0;


    PlayerDeathX[playerid] = 0.0;
    PlayerDeathY[playerid] = 0.0;
    PlayerDeathZ[playerid] = 0.0;
    PlayerDeathAngle[playerid] = 0.0;


    ClearAnimations(
        playerid
    );


    TogglePlayerControllable(
        playerid,
        true
    );


    // ========================================
    // CLEAR DATABASE DEATH STATE
    // ========================================

    Database_SavePlayer(
        playerid
    );


    return 1;
}


// ========================================
// SHOW RESPAWN DIALOG
// ========================================

stock Death_ShowRespawnDialog(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(!PlayerDead[playerid])
        return 0;


    ShowPlayerDialog(
        playerid,
        DIALOG_DEATH_RESPAWN,
        DIALOG_STYLE_MSGBOX,
        "{D4AF37}เกิดใหม่ที่โรงพยาบาล",
        "{FFFFFF}คุณสามารถเกิดใหม่ที่โรงพยาบาลได้\n\nค่ารักษาพยาบาล: $2,000\n\nหากมีเงินไม่ถึง $2,000 ระบบจะหักเงินที่มีทั้งหมด",
        "เกิดใหม่",
        ""
    );


    return 1;
}


// ========================================
// RESPAWN AT HOSPITAL
// ========================================

stock Death_Respawn(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(!PlayerDead[playerid])
        return 0;

    if(PlayerDeathTime[playerid] > 0)
        return 0;


    // ========================================
    // HOSPITAL FEE
    // ========================================

    new money = GetPlayerMoney(playerid);


    if(money >= DEATH_HOSPITAL_FEE)
    {
        GivePlayerMoney(
            playerid,
            -DEATH_HOSPITAL_FEE
        );
    }
    else
    {
        ResetPlayerMoney(
            playerid
        );
    }


    PlayerMoney[playerid] =
        GetPlayerMoney(playerid);


    // ========================================
    // CLEAR DEATH STATE
    // ========================================

    Death_Clear(
        playerid
    );


    PlayerSpawnSelected[playerid] = false;


    // ========================================
    // HOSPITAL SPAWN
    // ========================================

    SetPlayerInterior(
        playerid,
        0
    );

    SetPlayerVirtualWorld(
        playerid,
        0
    );


    SetSpawnInfo(
        playerid,
        NO_TEAM,
        26,
        1609.0,
        1816.0,
        13.0,
        360.0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0,
        WEAPON_FIST,
        0
    );


    SpawnPlayer(
        playerid
    );


    SetPlayerPos(
        playerid,
        1609.0,
        1816.0,
        13.0
    );


    SetPlayerFacingAngle(
        playerid,
        360.0
    );


    SetCameraBehindPlayer(
        playerid
    );


    SetPlayerHealth(
        playerid,
        100.0
    );


    SetPlayerArmour(
        playerid,
        0.0
    );


    PlayerHealth[playerid] = 100.0;
    PlayerArmour[playerid] = 0.0;


    // ========================================
    // SAVE PLAYER
    // ========================================

    Database_SavePlayer(
        playerid
    );


    SendClientMessage(
        playerid,
        -1,
        "{FF0000}[Auto] {FFFFFF}: คุณได้เกิดใหม่ที่โรงพยาบาลแล้ว"
    );


    return 1;
}


// ========================================
// DEATH TIMER
// ========================================

public DeathTimer()
{
    new message[64];
    new minutes;
    new seconds;


    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid))
            continue;

        if(!PlayerLoggedIn[playerid])
            continue;

        if(!PlayerDead[playerid])
            continue;


        // ========================================
        // DIALOG ALREADY SHOWN
        // ========================================

        if(PlayerDeathTime[playerid] == -1)
            continue;


        // ========================================
        // TIME FINISHED
        // ========================================

        if(PlayerDeathTime[playerid] <= 0)
        {
            PlayerDeathTime[playerid] = -1;


            // ========================================
            // SAVE FINISHED STATE
            // ========================================

            Database_SavePlayer(
                playerid
            );


            Death_ShowRespawnDialog(
                playerid
            );


            continue;
        }


        // ========================================
        // COUNTDOWN
        // ========================================

        PlayerDeathTime[playerid]--;


        minutes =
            PlayerDeathTime[playerid] / 60;

        seconds =
            PlayerDeathTime[playerid] % 60;


        format(
            message,
            sizeof(message),
            "YOU ARE DEAD~n~PLEASE WAIT %02d:%02d",
            minutes,
            seconds
        );


        GameTextForPlayer(
            playerid,
            message,
            1100,
            3
        );
    }


    return 1;
}