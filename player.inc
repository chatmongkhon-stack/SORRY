// ========================================
// PLAYER CALLBACKS
// ========================================

forward AdminRevivePlayer(
    playerid,
    Float:x,
    Float:y,
    Float:z,
    Float:angle
);


// ========================================
// VEHICLE MODEL NAME
// ========================================

stock Vehicle_GetDisplayName(modelid, name[], size)
{
    switch(modelid)
    {
        case 411: format(name, size, "Infernus");

        default:
        {
            format(
                name,
                size,
                "Model %d",
                modelid
            );
        }
    }

    return 1;
}


// ========================================
// PLAYER CONNECT
// ========================================

public OnPlayerConnect(playerid)
{
    PlayerLoggedIn[playerid] = false;
    PlayerSpawnSelected[playerid] = false;
    PlayerHasHouse[playerid] = false;
    PlayerFaction[playerid] = 0;
    PlayerNear7Eleven[playerid] = false;
    PlayerEntering7Eleven[playerid] = false;
    PlayerInside7Eleven[playerid] = false;

    PlayerDead[playerid] = false;
    PlayerDeathTime[playerid] = 0;

    PlayerLoginAttempts[playerid] = 0;
    PlayerOTime[playerid] = 0;
    PlayerPaydayTime[playerid] = 0;

    PlayerMoney[playerid] = 0;
    PlayerHealth[playerid] = 100.0;
    PlayerArmour[playerid] = 0.0;

    PlayerAdmin[playerid] = 0;
    PlayerVIP[playerid] = 0;

    PlayerLevel[playerid] = 1;
    PlayerEXP[playerid] = 0;

    PlayerHunger[playerid] = 100;
    PlayerThirst[playerid] = 100;

    PlayerAge[playerid] = 18;
    PlayerGender[playerid] = 0;

    PlayerDeathX[playerid] = 0.0;
    PlayerDeathY[playerid] = 0.0;
    PlayerDeathZ[playerid] = 0.0;
    PlayerDeathAngle[playerid] = 0.0;

    PlayerLoginHash[playerid][0] = '\0';

    SetPlayerSkin(playerid, 26);

    TogglePlayerSpectating(playerid, true);
    TogglePlayerControllable(playerid, false);

    SendClientMessage(
        playerid,
        -1,
        "ยินดีต้อนรับเข้าสู่เซิร์ฟเวอร์!"
    );

    Database_CheckAccount(playerid);

    return 1;
}


// ========================================
// PLAYER DISCONNECT
// ========================================

public OnPlayerDisconnect(playerid, reason)
{
    if(PlayerLoggedIn[playerid])
    {
        // ========================================
        // STORE PLAYER VEHICLES BEFORE LOGOUT
        // ========================================

        Vehicle_StorePlayerVehicles(playerid);

        // ========================================
        // SAVE PLAYER DATA
        // ========================================

        Database_SavePlayer(playerid);
        Database_SavePaydayTime(playerid);
    }

    PlayerLoggedIn[playerid] = false;
    PlayerSpawnSelected[playerid] = false;
    PlayerHasHouse[playerid] = false;
    PlayerFaction[playerid] = 0;
    PlayerNear7Eleven[playerid] = false;
    PlayerEntering7Eleven[playerid] = false;
    PlayerInside7Eleven[playerid] = false;

    PlayerDead[playerid] = false;
    PlayerDeathTime[playerid] = 0;

    PlayerDeathX[playerid] = 0.0;
    PlayerDeathY[playerid] = 0.0;
    PlayerDeathZ[playerid] = 0.0;
    PlayerDeathAngle[playerid] = 0.0;

    TogglePlayerControllable(playerid, true);

    SendClientMessageToAll(
        -1,
        "THE PLAYER HAS LEFT THE SERVER"
    );

    return 1;
}


// ========================================
// PLAYER SPAWN
// ========================================

public OnPlayerSpawn(playerid)
{
    if(PlayerDead[playerid])
    {
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

        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, false);

        SetTimerEx(
            "Death_ApplyAnimationDelayed",
            300,
            false,
            "i",
            playerid
        );

        return 1;
    }

    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);

    if(PlayerSpawnSelected[playerid])
    {
        SetPlayerSkin(playerid, 26);
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);

        SetPlayerPos(
            playerid,
            2002.1216,
            1544.4724,
            13.5859
        );

        SetPlayerFacingAngle(
            playerid,
            271.5414
        );

        SetPlayerHealth(
            playerid,
            PlayerHealth[playerid]
        );

        SetPlayerArmour(
            playerid,
            PlayerArmour[playerid]
        );

        SetCameraBehindPlayer(playerid);
    }

    return 1;
}


// ========================================
// PLAYER DEATH
// ========================================

public OnPlayerDeath(
    playerid,
    killerid,
    WEAPON:reason
)
{
    Death_Start(playerid);

    return 1;
}


// ========================================
// REQUEST CLASS
// ========================================

public OnPlayerRequestClass(playerid, classid)
{
    if(PlayerLoggedIn[playerid])
        return 0;

    SetPlayerSkin(playerid, 26);

    SetPlayerCameraPos(
        playerid,
        2002.1216,
        1544.4724,
        16.1
    );

    SetPlayerCameraLookAt(
        playerid,
        2002.1216,
        1544.4724,
        13.5859
    );

    return 1;
}


// ========================================
// LOGIN SCENE
// ========================================

stock SetLoginScene(playerid)
{
    SetPlayerCameraPos(
        playerid,
        -269.4498,
        -1614.9242,
        58.0998
    );

    SetPlayerCameraLookAt(
        playerid,
        -270.2507,
        -1614.3184,
        57.8497
    );

    InterpolateCameraPos(
        playerid,
        -269.4498, -1614.9242, 58.0998,
        -255.4192, -1487.0031, 50.0984,
        60000,
        CAMERA_MOVE
    );

    InterpolateCameraLookAt(
        playerid,
        -270.2507, -1614.9242, 57.8497,
        -256.3769, -1486.7010, 50.0233,
        60000,
        CAMERA_MOVE
    );

    return 1;
}


// ========================================
// SPAWN SELECT SCENE
// ========================================

stock SetSpawnSelectScene(playerid)
{
    SetPlayerCameraPos(
        playerid,
        -255.4192,
        -1487.0031,
        50.0984
    );

    SetPlayerCameraLookAt(
        playerid,
        -269.4498,
        -1614.9242,
        25.0
    );

    InterpolateCameraPos(
        playerid,
        -255.4192, -1487.0031, 50.0984,
        -220.0000, -1535.0000, 55.0000,
        30000,
        CAMERA_MOVE
    );

    InterpolateCameraLookAt(
        playerid,
        -269.4498, -1487.0031, 25.0,
        -285.0000, -1580.0000, 30.0,
        30000,
        CAMERA_MOVE
    );

    return 1;
}


// ========================================
// PLAYER TEXT
// ========================================

public OnPlayerText(playerid, text[])
{
    new Float:x;
    new Float:y;
    new Float:z;
    new message[144];
    new playerName[MAX_PLAYER_NAME];

    GetPlayerPos(playerid, x, y, z);
    GetPlayerName(playerid, playerName, sizeof(playerName));

    format(
        message,
        sizeof(message),
        "(%s) {AD1457}Say :. {FFFFFF}%s",
        playerName,
        text
    );

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i))
            continue;

        if(IsPlayerInRangeOfPoint(i, 15.0, x, y, z))
            SendClientMessage(i, -1, message);
    }

    return 0;
}


// ========================================
// PLAYER COMMAND TEXT
// ========================================

public OnPlayerCommandText(playerid, cmdtext[])
{
    if(!PlayerLoggedIn[playerid])
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: คุณต้องเข้าสู่ระบบก่อนจึงจะใช้คำสั่งได้"
        );

        return 1;
    }

    // ========================================
    // VEHICLE TEST - CALL
    // ========================================

    if(strfind(cmdtext, "/vcall ", true) == 0)
    {
        new databaseID;

        databaseID = strval(cmdtext[7]);

        if(databaseID <= 0)
        {
            SendClientMessage(
                playerid,
                -1,
                "ใช้: /vcall [Vehicle DB ID]"
            );

            return 1;
        }

        Vehicle_Call(
            playerid,
            databaseID
        );

        return 1;
    }

    // ========================================
    // VEHICLE TEST - STORE
    // ========================================

    if(!strcmp(cmdtext, "/vstore", true))
    {
        if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Vehicle] {FFFFFF}: คุณต้องเป็นคนขับรถก่อน"
            );

            return 1;
        }

        new vehicleid;

        vehicleid = GetPlayerVehicleID(playerid);

        Vehicle_Store(
            playerid,
            vehicleid
        );

        return 1;
    }

    // ========================================
    // OOC COMMAND
    // ========================================

    if(strfind(cmdtext, "/o ", true) == 0)
    {
        new message[144];
        new cooldownMessage[144];
        new playerName[MAX_PLAYER_NAME];
        new currentTime;
        new remainingTime;

        currentTime = GetTickCount();

        if(
            PlayerOTime[playerid] > 0 &&
            currentTime - PlayerOTime[playerid] < 60000
        )
        {
            remainingTime =
                (60000 - (currentTime - PlayerOTime[playerid])) / 1000;

            format(
                cooldownMessage,
                sizeof(cooldownMessage),
                "{FF0000}[Auto] {FFFFFF}: กรุณารออีก %d วินาทีก่อนใช้ /o อีกครั้ง",
                remainingTime + 1
            );

            SendClientMessage(
                playerid,
                -1,
                cooldownMessage
            );

            return 1;
        }

        GetPlayerName(
            playerid,
            playerName,
            sizeof(playerName)
        );

        if(PlayerVIP[playerid] == 0)
        {
            if(GetPlayerMoney(playerid) < 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Auto] {FFFFFF}: คุณมีเงินไม่พอ ต้องใช้ $100"
                );

                return 1;
            }

            TakePlayerCash(playerid, 100);

            format(
                message,
                sizeof(message),
                "{0A7E07}[MEMBER] {FFFFFF}[%s] [%d] Say :. %s",
                playerName,
                playerid,
                cmdtext[3]
            );
        }
        else if(PlayerVIP[playerid] == 1)
        {
            format(
                message,
                sizeof(message),
                "{880E4F}#[{CDDC39}VIP{880E4F}] {FFFFFF}[%s] [%d] Say :. {FFFFFF}%s",
                playerName,
                playerid,
                cmdtext[3]
            );
        }
        else if(PlayerVIP[playerid] == 2)
        {
            format(
                message,
                sizeof(message),
                "{B0120A}#[{F9A825}PREMIUM{B0120A}] {FFFFFF}[%s] [%d] Say :. {FFFFFF}%s",
                playerName,
                playerid,
                cmdtext[3]
            );
        }

        SendClientMessageToAll(-1, message);
        PlayerOTime[playerid] = currentTime;

        return 1;
    }

    // ========================================
    // ADMIN COMMAND
    // ========================================

    if(strfind(cmdtext, "/as ", true) == 0)
    {
        new message[144];

        if(PlayerAdmin[playerid] < 1)
        {
            SendClientMessage(
                playerid,
                -1,
                "คุณไม่มีสิทธิ์ใช้คำสั่งนี้"
            );

            return 1;
        }

        format(
            message,
            sizeof(message),
            "{9966FF}[Administrator] {000000}: {FFFFFF}%s",
            cmdtext[4]
        );

        SendClientMessageToAll(-1, message);

        return 1;
    }

    // ========================================
    // SETSTATS
    // ========================================

    if(strfind(cmdtext, "/setstats", true) == 0)
    {
        if(PlayerAdmin[playerid] < 1)
        {
            SendClientMessage(
                playerid,
                -1,
                "คุณไม่มีสิทธิ์ใช้คำสั่งนี้"
            );

            return 1;
        }

        if(!strcmp(cmdtext, "/setstats", true))
        {
            SendClientMessage(
                playerid,
                -1,
                "========== SETSTATS =========="
            );

            SendClientMessage(
                playerid,
                -1,
                "1.Level  2.VIP  3.Money  4.HP  5.Armor  6.Food  7.Water  8.Car  9.Revive"
            );

            SendClientMessage(
                playerid,
                -1,
                "ใช้: /setstats [ID] [เลขรายการ] [จำนวน]"
            );

            return 1;
        }

        new targetid;
        new option;
        new amount;
        new spacePos;

        spacePos = strfind(cmdtext, " ", true);

        if(spacePos == -1)
        {
            SendClientMessage(
                playerid,
                -1,
                "วิธีใช้: /setstats [ID] [เลขรายการ] [จำนวน]"
            );

            return 1;
        }

        targetid = strval(cmdtext[spacePos + 1]);

        spacePos = strfind(
            cmdtext,
            " ",
            true,
            spacePos + 1
        );

        if(spacePos == -1)
        {
            SendClientMessage(
                playerid,
                -1,
                "วิธีใช้: /setstats [ID] [เลขรายการ] [จำนวน]"
            );

            return 1;
        }

        option = strval(cmdtext[spacePos + 1]);

        spacePos = strfind(
            cmdtext,
            " ",
            true,
            spacePos + 1
        );

        if(spacePos == -1)
        {
            SendClientMessage(
                playerid,
                -1,
                "วิธีใช้: /setstats [ID] [เลขรายการ] [จำนวน]"
            );

            return 1;
        }

        amount = strval(cmdtext[spacePos + 1]);

        if(!IsPlayerConnected(targetid))
        {
            SendClientMessage(
                playerid,
                -1,
                "ไม่พบผู้เล่น ID นี้"
            );

            return 1;
        }

        if(!PlayerLoggedIn[targetid])
        {
            SendClientMessage(
                playerid,
                -1,
                "ผู้เล่นคนนี้ยังไม่ได้เข้าสู่ระบบ"
            );

            return 1;
        }

        // ========================================
        // LEVEL
        // ========================================

        if(option == 1)
        {
            if(PlayerAdmin[playerid] != 2)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "เฉพาะ Owner เท่านั้นที่สามารถตั้ง Level ได้"
                );

                return 1;
            }

            if(amount < 1)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "Level ต้องมีค่าตั้งแต่ 1 ขึ้นไป"
                );

                return 1;
            }

            PlayerLevel[targetid] = amount;
            SetPlayerScore(targetid, PlayerLevel[targetid]);
            Database_SaveLevelEXP(targetid);

            SendClientMessage(
                playerid,
                -1,
                "ตั้ง Level ให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "Owner ได้ตั้ง Level ของคุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // VIP
        // ========================================

        if(option == 2)
        {
            if(PlayerAdmin[playerid] != 2)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "เฉพาะ Owner เท่านั้นที่สามารถตั้ง VIP ได้"
                );

                return 1;
            }

            if(amount < 0 || amount > 2)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "VIP ต้องเป็น 0, 1 หรือ 2"
                );

                return 1;
            }

            PlayerVIP[targetid] = amount;
            Database_SaveVIP(targetid);

            if(amount == 0)
            {
                SendClientMessage(
                    targetid,
                    -1,
                    "VIP ของคุณถูกยกเลิกแล้ว"
                );
            }
            else if(amount == 1)
            {
                SendClientMessage(
                    targetid,
                    -1,
                    "คุณได้รับ VIP"
                );
            }
            else if(amount == 2)
            {
                SendClientMessage(
                    targetid,
                    -1,
                    "คุณได้รับ PREMIUM"
                );
            }

            SendClientMessage(
                playerid,
                -1,
                "ตั้ง VIP ให้ผู้เล่นเรียบร้อยแล้ว"
            );

            return 1;
        }

        // ========================================
        // MONEY
        // ========================================

        if(option == 3)
        {
            if(PlayerAdmin[playerid] != 2)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "เฉพาะ Owner เท่านั้นที่สามารถตั้ง Money ได้"
                );

                return 1;
            }

            if(amount < 0)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "จำนวนเงินต้องเป็น 0 ขึ้นไป"
                );

                return 1;
            }

            GivePlayerCash(targetid, amount);

            SendClientMessage(
                playerid,
                -1,
                "เพิ่มเงินให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "Owner ได้เพิ่มเงินให้คุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // HP
        // ========================================

        if(option == 4)
        {
            if(amount < 1 || amount > 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "HP ต้องอยู่ระหว่าง 1 - 100"
                );

                return 1;
            }

            SetPlayerHealth(targetid, float(amount));
            Database_SaveHealthArmour(targetid);

            SendClientMessage(
                playerid,
                -1,
                "ตั้ง HP ให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "Admin ได้ตั้ง HP ให้คุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // ARMOR
        // ========================================

        if(option == 5)
        {
            if(amount < 0 || amount > 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "Armor ต้องอยู่ระหว่าง 0 - 100"
                );

                return 1;
            }

            SetPlayerArmour(targetid, float(amount));
            Database_SaveHealthArmour(targetid);

            SendClientMessage(
                playerid,
                -1,
                "ตั้ง Armor ให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "Admin ได้ตั้ง Armor ให้คุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // FOOD
        // ========================================

        if(option == 6)
        {
            if(amount < 0 || amount > 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "Food ต้องอยู่ระหว่าง 0 - 100"
                );

                return 1;
            }

            SetPlayerHunger(targetid, amount);
            Database_SaveHungerThirst(targetid);
            UpdateStatusHUD(targetid);

            SendClientMessage(
                playerid,
                -1,
                "ตั้ง Food ให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "Admin ได้ตั้ง Food ให้คุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // WATER
        // ========================================

        if(option == 7)
        {
            if(amount < 0 || amount > 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "Water ต้องอยู่ระหว่าง 0 - 100"
                );

                return 1;
            }

            SetPlayerThirst(targetid, amount);
            Database_SaveHungerThirst(targetid);
            UpdateStatusHUD(targetid);

            SendClientMessage(
                playerid,
                -1,
                "ตั้ง Water ให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "Admin ได้ตั้ง Water ให้คุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // CAR
        // ========================================

        if(option == 8)
        {
            if(PlayerAdmin[playerid] < 1)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "คุณไม่มีสิทธิ์ใช้คำสั่งนี้"
                );

                return 1;
            }

            if(PlayerDead[targetid])
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Auto] {FFFFFF}: ผู้เล่นคนนี้เสียชีวิตอยู่ ไม่สามารถเสกรถได้"
                );

                return 1;
            }

            if(amount < 400 || amount > 611)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Auto] {FFFFFF}: Model ID ต้องอยู่ระหว่าง 400 - 611"
                );

                return 1;
            }

            new Float:x;
            new Float:y;
            new Float:z;
            new Float:angle;
            new Float:spawnX;
            new Float:spawnY;
            new vehicleid;

            GetPlayerPos(targetid, x, y, z);
            GetPlayerFacingAngle(targetid, angle);

            spawnX =
                x + (3.0 * floatsin(-angle, degrees));

            spawnY =
                y + (3.0 * floatcos(angle, degrees));

            vehicleid = CreateVehicle(
                amount,
                spawnX,
                spawnY,
                z,
                angle,
                -1,
                -1,
                -1
            );

            if(vehicleid == INVALID_VEHICLE_ID)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Auto] {FFFFFF}: ไม่สามารถเสกรถได้"
                );

                return 1;
            }

            SystemVehicle[vehicleid] = true;
            PutPlayerInVehicle(targetid, vehicleid, 0);

            SendClientMessage(
                playerid,
                -1,
                "{00FF00}[Auto] {FFFFFF}: เสกรถให้ผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "{00FF00}[Auto] {FFFFFF}: Admin ได้เสกรถให้คุณแล้ว"
            );

            return 1;
        }

        // ========================================
        // REVIVE
        // ========================================

        if(option == 9)
        {
            if(PlayerAdmin[playerid] < 1)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "คุณไม่มีสิทธิ์ใช้คำสั่งนี้"
                );

                return 1;
            }

            if(amount != 1)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "การชุบชีวิตให้ใช้ค่า 1"
                );

                return 1;
            }

            if(!PlayerDead[targetid])
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Auto] {FFFFFF}: ผู้เล่นคนนี้ไม่ได้เสียชีวิตอยู่"
                );

                return 1;
            }

            new Float:reviveX;
            new Float:reviveY;
            new Float:reviveZ;
            new Float:reviveAngle;

            reviveX = PlayerDeathX[targetid];
            reviveY = PlayerDeathY[targetid];
            reviveZ = PlayerDeathZ[targetid];
            reviveAngle = PlayerDeathAngle[targetid];

            PlayerDead[targetid] = false;
            PlayerDeathTime[targetid] = 0;
            PlayerSpawnSelected[targetid] = false;

            ClearAnimations(targetid);
            TogglePlayerControllable(targetid, true);

            SetSpawnInfo(
                targetid,
                NO_TEAM,
                26,
                reviveX,
                reviveY,
                reviveZ,
                reviveAngle,
                WEAPON_FIST,
                0,
                WEAPON_FIST,
                0,
                WEAPON_FIST,
                0
            );

            TogglePlayerSpectating(targetid, false);

            SetTimerEx(
                "AdminRevivePlayer",
                300,
                false,
                "iffff",
                targetid,
                reviveX,
                reviveY,
                reviveZ,
                reviveAngle
            );

            SendClientMessage(
                playerid,
                -1,
                "{00FF00}[Auto] {FFFFFF}: ชุบชีวิตผู้เล่นเรียบร้อยแล้ว"
            );

            SendClientMessage(
                targetid,
                -1,
                "{00FF00}[Auto] {FFFFFF}: Admin ได้ชุบชีวิตคุณแล้ว"
            );

            return 1;
        }

        SendClientMessage(
            playerid,
            -1,
            "ไม่พบรายการนี้ กรุณาใช้ /setstats เพื่อดูรายการ"
        );

        return 1;
    }

    return 0;
}


// ========================================
// ADMIN REVIVE
// ========================================

public AdminRevivePlayer(
    playerid,
    Float:x,
    Float:y,
    Float:z,
    Float:angle
)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(!PlayerLoggedIn[playerid])
        return 1;

    if(PlayerDead[playerid])
        return 1;

    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, angle);

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 0.0);

    PlayerHealth[playerid] = 100.0;
    PlayerArmour[playerid] = 0.0;

    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);

    Database_SavePlayer(playerid);

    return 1;
}


// ========================================
// ENTER VEHICLE
// ========================================

public OnPlayerEnterVehicle(
    playerid,
    vehicleid,
    ispassenger
)
{
    if(PlayerDead[playerid])
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: คุณเสียชีวิตอยู่ ไม่สามารถขึ้นรถได้"
        );

        return 1;
    }

    return 1;
}


// ========================================
// EXIT VEHICLE
// ========================================

public OnPlayerExitVehicle(playerid, vehicleid)
{
    if(PlayerDead[playerid])
        Death_ApplyAnimation(playerid);

    return 1;
}


// ========================================
// PLAYER STATE CHANGE
// ========================================

public OnPlayerStateChange(
    playerid,
    PLAYER_STATE:newstate,
    PLAYER_STATE:oldstate
)
{
    if(PlayerDead[playerid])
    {
        if(
            newstate == PLAYER_STATE_DRIVER ||
            newstate == PLAYER_STATE_PASSENGER
        )
        {
            RemovePlayerFromVehicle(playerid);
            Death_ApplyAnimation(playerid);
        }

        return 1;
    }

    return 1;
}


// ========================================
// REQUEST SPAWN
// ========================================

public OnPlayerRequestSpawn(playerid)
{
    if(!PlayerLoggedIn[playerid])
    {
        SendClientMessage(
            playerid,
            -1,
            "คุณต้องสมัครสมาชิกและเข้าสู่ระบบก่อนจึงจะเข้าเล่นได้"
        );

        Kick(playerid);

        return 0;
    }

    if(PlayerDead[playerid])
        return 0;

    return 1;
}


// ========================================
// CHECKPOINT
// ========================================

public OnPlayerEnterCheckpoint(playerid)
{
    return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
    return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
    return 1;
}


// ========================================
// OBJECT
// ========================================

public OnPlayerObjectMoved(playerid, objectid)
{
    return 1;
}


// ========================================
// PICKUP
// ========================================

public OnPlayerPickUpPickup(playerid, pickupid)
{
    return 1;
}


// ========================================
// MENU
// ========================================

public OnPlayerSelectedMenuRow(playerid, row)
{
    return 1;
}

public OnPlayerExitedMenu(playerid)
{
    return 1;
}


// ========================================
// INTERIOR
// ========================================

public OnPlayerInteriorChange(
    playerid,
    newinteriorid,
    oldinteriorid
)
{
    return 1;
}


// ========================================
// PLAYER KEY STATE
// ========================================

public OnPlayerKeyStateChange(
    playerid,
    KEY:newkeys,
    KEY:oldkeys
)
{
    // ========================================
    // Y KEY
    // ========================================

    if((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        if(!PlayerLoggedIn[playerid])
            return 1;

        if(
            GetPlayerWantedLevel(playerid) > 0 ||
            IsPlayerInAnyVehicle(playerid) ||
            PlayerDead[playerid]
        )
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Auto] {FFFFFF}: ไม่สามารถใช้งานเมนูนี้ได้ในขณะนี้"
            );

            return 1;
        }

        // ========================================
        // GARAGE
        // ========================================

        if(IsPlayerInRangeOfPoint(
            playerid,
            3.0,
            2637.2996,
            2345.6279,
            10.6719
        ))
        {
            new dialogText[1024];
            new line[128];
            new modelName[32];
            new count;

            dialogText[0] = '\0';
            count = 0;

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

                Vehicle_GetDisplayName(
                    VehicleDBModel[slot],
                    modelName,
                    sizeof(modelName)
                );

                if(VehicleDBStatus[slot] == VEHICLE_STATUS_GARAGE)
                {
                    format(
                        line,
                        sizeof(line),
                        "รถ #%d | %s | Garage\n",
                        VehicleDBID[slot],
                        modelName
                    );
                }
                else if(VehicleDBStatus[slot] == VEHICLE_STATUS_BROKEN)
                {
                    format(
                        line,
                        sizeof(line),
                        "รถ #%d | %s | พัง\n",
                        VehicleDBID[slot],
                        modelName
                    );
                }
                else
                {
                    format(
                        line,
                        sizeof(line),
                        "รถ #%d | %s | นอก Garage\n",
                        VehicleDBID[slot],
                        modelName
                    );
                }

                strcat(
                    dialogText,
                    line
                );

                count++;
            }

            if(count == 0)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Vehicle] {FFFFFF}: คุณยังไม่มีรถ"
                );

                return 1;
            }

            ShowPlayerDialog(
                playerid,
                DIALOG_VEHICLE_GARAGE,
                DIALOG_STYLE_LIST,
                "{D4AF37}SORRY {FFFFFF}| Garage",
                dialogText,
                "เลือก",
                "ปิด"
            );

            return 1;
        }

        // ========================================
        // BICYCLE
        // ========================================

        if(IsPlayerInRangeOfPoint(
            playerid,
            2.0,
            2026.0940,
            1540.2754,
            10.8203
        ))
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_BIKE_MENU,
                DIALOG_STYLE_LIST,
                "{D4AF37}SORRY {FFFFFF}| จักรยาน",
                "จักรยาน",
                "เลือก",
                "ปิด"
            );

            return 1;
        }

        return 1;
    }


    // ========================================
    // H KEY
    // ========================================

    if((newkeys & KEY_CTRL_BACK) && !(oldkeys & KEY_CTRL_BACK))
    {
        if(!PlayerLoggedIn[playerid])
            return 1;

        if(
            GetPlayerWantedLevel(playerid) > 0 ||
            IsPlayerInAnyVehicle(playerid) ||
            PlayerDead[playerid]
        )
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Auto] {FFFFFF}: ไม่สามารถใช้งานเมนูนี้ได้ในขณะนี้"
            );

            return 1;
        }

        ShowPlayerDialog(
            playerid,
            DIALOG_H_MENU,
            DIALOG_STYLE_LIST,
            "{D4AF37}SORRY {FFFFFF}| เมนูการใช้งาน",
            "จัดการตัวละคร\nGPS",
            "เลือก",
            "ปิด"
        );

        return 1;
    }


    // ========================================
    // N KEY - MAIN INTERACTION
    // ========================================

    if((newkeys & KEY_NO) && !(oldkeys & KEY_NO))
    {
        if(!PlayerLoggedIn[playerid])
            return 1;

        if(
            GetPlayerWantedLevel(playerid) > 0 ||
            PlayerDead[playerid]
        )
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Auto] {FFFFFF}: ไม่สามารถใช้งานเมนูนี้ได้ในขณะนี้"
            );

            return 1;
        }

        // ========================================
        // VEHICLE MANAGEMENT
        // ========================================

        if(IsPlayerInAnyVehicle(playerid))
        {
            new vehicleid;

            vehicleid = GetPlayerVehicleID(playerid);

            if(!IsValidVehicle(vehicleid))
                return 1;

            if(!Vehicle_IsPlayerOwner(
                playerid,
                vehicleid
            ))
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Vehicle] {FFFFFF}: คุณไม่ใช่เจ้าของรถคันนี้"
                );

                return 1;
            }

            ShowPlayerDialog(
                playerid,
                DIALOG_VEHICLE_MENU,
                DIALOG_STYLE_LIST,
                "{D4AF37}SORRY {FFFFFF}| จัดการรถ",
                "ล็อกรถ\nปลดล็อกรถ",
                "เลือก",
                "ปิด"
            );

            return 1;
        }


        // ========================================
        // 7-ELEVEN
        // ========================================

        if(PlayerInside7Eleven[playerid])
        {
            if(IsPlayerInRangeOfPoint(
                playerid,
                SHOP_7ELEVEN_BUY_RANGE,
                SHOP_7ELEVEN_BUY_X,
                SHOP_7ELEVEN_BUY_Y,
                SHOP_7ELEVEN_BUY_Z
            ))
            {
                Shop_ShowMenu(playerid);

                return 1;
            }
        }

        return 1;
    }

    return 1;
}


// ========================================
// PLAYER UPDATE
// ========================================

public OnPlayerUpdate(playerid)
{
    if(!PlayerLoggedIn[playerid])
        return 1;

    if(PlayerDead[playerid])
        return 1;

    if(IsPlayerInAnyVehicle(playerid))
        return 1;

    // ========================================
    // ENTER 7-ELEVEN
    // ========================================

    if(!PlayerInside7Eleven[playerid])
    {
        if(!PlayerEntering7Eleven[playerid])
        {
            if(IsPlayerInRangeOfPoint(
                playerid,
                SHOP_7ELEVEN_RANGE,
                SHOP_7ELEVEN_X,
                SHOP_7ELEVEN_Y,
                SHOP_7ELEVEN_Z
            ))
            {
                if(!PlayerNear7Eleven[playerid])
                {
                    PlayerNear7Eleven[playerid] = true;

                    SendClientMessage(
                        playerid,
                        -1,
                        "{FF0000}[ระบบ] {FFFFFF}: คุณอยู่หน้าประตู 7-Eleven"
                    );

                    Shop_Enter7Eleven(playerid);
                }
            }
            else
            {
                PlayerNear7Eleven[playerid] = false;
            }
        }
    }

    // ========================================
    // EXIT 7-ELEVEN
    // ========================================

    if(PlayerInside7Eleven[playerid])
    {
        if(!PlayerEntering7Eleven[playerid])
        {
            if(IsPlayerInRangeOfPoint(
                playerid,
                SHOP_7ELEVEN_EXIT_RANGE,
                SHOP_7ELEVEN_EXIT_X,
                SHOP_7ELEVEN_EXIT_Y,
                SHOP_7ELEVEN_EXIT_Z
            ))
            {
                Shop_Exit7Eleven(playerid);
            }
        }
    }

    return 1;
}


// ========================================
// STREAM
// ========================================

public OnPlayerStreamIn(playerid, forplayerid)
{
    return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
    return 1;
}


// ========================================
// CLICK PLAYER
// ========================================

public OnPlayerClickPlayer(
    playerid,
    clickedplayerid,
    CLICK_SOURCE:source
)
{
    return 1;
}