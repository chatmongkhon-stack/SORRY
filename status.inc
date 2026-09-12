// ========================================
// STATUS SYSTEM
// ========================================

// ========================================
// HUNGER / THIRST SYSTEM
// ========================================

stock SetPlayerHunger(playerid, value)
{
    if(value < 0) value = 0;
    if(value > 100) value = 100;

    PlayerHunger[playerid] = value;
    return 1;
}

stock SetPlayerThirst(playerid, value)
{
    if(value < 0) value = 0;
    if(value > 100) value = 100;

    PlayerThirst[playerid] = value;
    return 1;
}

// ========================================
// ADD HUNGER / THIRST
// ========================================

stock AddPlayerHunger(playerid, amount)
{
    SetPlayerHunger(playerid, PlayerHunger[playerid] + amount);
    return 1;
}

stock AddPlayerThirst(playerid, amount)
{
    SetPlayerThirst(playerid, PlayerThirst[playerid] + amount);
    return 1;
}

// ========================================
// STATUS TIMER
// ========================================

forward StatusTimer();

public StatusTimer()
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid)) continue;
        if(!PlayerLoggedIn[playerid]) continue;
        if(PlayerDead[playerid]) continue;

        // ====================================
        // ลดอาหารและน้ำทุก 1 นาที
        // ====================================

        AddPlayerHunger(playerid, -1);
        AddPlayerThirst(playerid, -2);

        // ====================================
        // HP DAMAGE จากอาหาร / น้ำ
        // ====================================

        new Float:currentHP;
        new Float:damage = 0.0;

        GetPlayerHealth(playerid, currentHP);

        // หมดทั้งอาหารและน้ำ = -4 HP
        if(PlayerHunger[playerid] <= 0 && PlayerThirst[playerid] <= 0)
        {
            damage = 4.0;
        }
        // หมดอย่างใดอย่างหนึ่ง = -2 HP
        else if(PlayerHunger[playerid] <= 0 || PlayerThirst[playerid] <= 0)
        {
            damage = 2.0;
        }

        // ====================================
        // ลด HP
        // ====================================

        if(damage > 0.0)
        {
            currentHP -= damage;

            if(currentHP < 0.0)
            {
                currentHP = 0.0;
            }

            SetPlayerHealth(playerid, currentHP);
            PlayerHealth[playerid] = currentHP;
        }

        // ====================================
        // UPDATE HUD
        // ====================================

        UpdateStatusHUD(playerid);
    }

    return 1;
}

// ========================================
// STATUS HUD
//
// 0 = FOOD TEXT
// 1 = FOOD BAR BACKGROUND
// 2 = FOOD BAR
// 3 = WATER TEXT
// 4 = WATER BAR BACKGROUND
// 5 = WATER BAR
// ========================================

new PlayerText:StatusHUD[MAX_PLAYERS][6];

// ========================================
// CREATE STATUS HUD
// ========================================

stock CreateStatusHUD(playerid)
{
    // ====================================
    // FOOD - F
    // ====================================

    StatusHUD[playerid][0] = CreatePlayerTextDraw(playerid, 10.5, 422.0, "F");
    PlayerTextDrawFont(playerid, StatusHUD[playerid][0], TEXT_DRAW_FONT:2);
    PlayerTextDrawLetterSize(playerid, StatusHUD[playerid][0], 0.20, 0.9);
    PlayerTextDrawColour(playerid, StatusHUD[playerid][0], 0xFFB300FF);
    PlayerTextDrawSetOutline(playerid, StatusHUD[playerid][0], 1);
    PlayerTextDrawSetProportional(playerid, StatusHUD[playerid][0], bool:1);
    PlayerTextDrawAlignment(playerid, StatusHUD[playerid][0], TEXT_DRAW_ALIGN:2);

    // ====================================
    // FOOD BAR BACKGROUND
    // ====================================

    StatusHUD[playerid][1] = CreatePlayerTextDraw(playerid, 7.0, 432.0, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, StatusHUD[playerid][1], TEXT_DRAW_FONT:4);
    PlayerTextDrawTextSize(playerid, StatusHUD[playerid][1], 7.0, 10.0);
    PlayerTextDrawColour(playerid, StatusHUD[playerid][1], 0x222222CC);

    // ====================================
    // FOOD BAR
    // ====================================

    StatusHUD[playerid][2] = CreatePlayerTextDraw(playerid, 7.0, 432.0, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, StatusHUD[playerid][2], TEXT_DRAW_FONT:4);
    PlayerTextDrawTextSize(playerid, StatusHUD[playerid][2], 7.0, 10.0);
    PlayerTextDrawColour(playerid, StatusHUD[playerid][2], 0xFFB300FF);

    // ====================================
    // WATER - W
    // ====================================

    StatusHUD[playerid][3] = CreatePlayerTextDraw(playerid, 20.5, 422.0, "W");
    PlayerTextDrawFont(playerid, StatusHUD[playerid][3], TEXT_DRAW_FONT:2);
    PlayerTextDrawLetterSize(playerid, StatusHUD[playerid][3], 0.20, 0.9);
    PlayerTextDrawColour(playerid, StatusHUD[playerid][3], 0x29B6F6FF);
    PlayerTextDrawSetOutline(playerid, StatusHUD[playerid][3], 1);
    PlayerTextDrawSetProportional(playerid, StatusHUD[playerid][3], bool:1);
    PlayerTextDrawAlignment(playerid, StatusHUD[playerid][3], TEXT_DRAW_ALIGN:2);

    // ====================================
    // WATER BAR BACKGROUND
    // ====================================

    StatusHUD[playerid][4] = CreatePlayerTextDraw(playerid, 17.0, 432.0, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, StatusHUD[playerid][4], TEXT_DRAW_FONT:4);
    PlayerTextDrawTextSize(playerid, StatusHUD[playerid][4], 7.0, 10.0);
    PlayerTextDrawColour(playerid, StatusHUD[playerid][4], 0x222222CC);

    // ====================================
    // WATER BAR
    // ====================================

    StatusHUD[playerid][5] = CreatePlayerTextDraw(playerid, 17.0, 432.0, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, StatusHUD[playerid][5], TEXT_DRAW_FONT:4);
    PlayerTextDrawTextSize(playerid, StatusHUD[playerid][5], 7.0, 10.0);
    PlayerTextDrawColour(playerid, StatusHUD[playerid][5], 0x29B6F6FF);

    return 1;
}

// ========================================
// SHOW HUD
// ========================================

stock ShowStatusHUD(playerid)
{
    for(new i = 0; i < 6; i++)
    {
        PlayerTextDrawShow(playerid, StatusHUD[playerid][i]);
    }

    UpdateStatusHUD(playerid);
    return 1;
}

// ========================================
// UPDATE HUD
// ========================================

stock UpdateStatusHUD(playerid)
{
    new Float:foodHeight;
    new Float:waterHeight;
    new Float:foodY;
    new Float:waterY;

    // ====================================
    // คำนวณความสูงตามค่า 0-100
    // ====================================

    foodHeight = 10.0 * PlayerHunger[playerid] / 100.0;
    waterHeight = 10.0 * PlayerThirst[playerid] / 100.0;

    // ====================================
    // FOOD BAR
    //
    // ยึดด้านล่างไว้ที่ Y = 442
    // สีจะหายจากบนลงล่าง
    // ====================================

    PlayerTextDrawHide(playerid, StatusHUD[playerid][2]);

    if(foodHeight > 0.0)
    {
        foodY = 442.0 - foodHeight;

        PlayerTextDrawSetPos(
            playerid,
            StatusHUD[playerid][2],
            7.0,
            foodY
        );

        PlayerTextDrawTextSize(
            playerid,
            StatusHUD[playerid][2],
            7.0,
            foodHeight
        );

        PlayerTextDrawShow(playerid, StatusHUD[playerid][2]);
    }

    // ====================================
    // WATER BAR
    //
    // ยึดด้านล่างไว้ที่ Y = 442
    // สีจะหายจากบนลงล่าง
    // ====================================

    PlayerTextDrawHide(playerid, StatusHUD[playerid][5]);

    if(waterHeight > 0.0)
    {
        waterY = 442.0 - waterHeight;

        PlayerTextDrawSetPos(
            playerid,
            StatusHUD[playerid][5],
            17.0,
            waterY
        );

        PlayerTextDrawTextSize(
            playerid,
            StatusHUD[playerid][5],
            7.0,
            waterHeight
        );

        PlayerTextDrawShow(playerid, StatusHUD[playerid][5]);
    }

    return 1;
}