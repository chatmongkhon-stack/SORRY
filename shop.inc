// ========================================
// 7-ELEVEN SHOP SYSTEM
// ========================================

// ----------------------------------------
// OUTSIDE
// ----------------------------------------

#define SHOP_7ELEVEN_X          2194.9316
#define SHOP_7ELEVEN_Y          1990.9728
#define SHOP_7ELEVEN_Z          12.2969

#define SHOP_7ELEVEN_RANGE      0.7

// ----------------------------------------
// INSIDE - 24/7 INTERIOR ID 6
// ----------------------------------------

#define SHOP_7ELEVEN_INTERIOR   6

#define SHOP_7ELEVEN_INSIDE_X   -26.8339
#define SHOP_7ELEVEN_INSIDE_Y   -55.5846
#define SHOP_7ELEVEN_INSIDE_Z   1003.5469

#define SHOP_7ELEVEN_EXIT_X     -27.4263
#define SHOP_7ELEVEN_EXIT_Y     -58.2725
#define SHOP_7ELEVEN_EXIT_Z     1003.5469
#define SHOP_7ELEVEN_EXIT_ANGLE 181.6679

#define SHOP_7ELEVEN_EXIT_RANGE 0.7

// ----------------------------------------
// SHOP
// ----------------------------------------

#define SHOP_7ELEVEN_BUY_X      -23.4996
#define SHOP_7ELEVEN_BUY_Y      -55.6303
#define SHOP_7ELEVEN_BUY_Z      1003.5469

#define SHOP_7ELEVEN_BUY_RANGE  2.0

#define SHOP_ITEM_WATER         ITEM_WATER
#define SHOP_ITEM_FOOD          ITEM_FOOD

#define SHOP_WATER_PRICE        50
#define SHOP_FOOD_PRICE         100

// ----------------------------------------
// TRANSITION TIME
// ----------------------------------------

#define SHOP_7ELEVEN_TRANSITION_TIME 5000

// ----------------------------------------
// PLAYER STATE
// ----------------------------------------

new bool:PlayerNear7Eleven[MAX_PLAYERS];
new bool:PlayerEntering7Eleven[MAX_PLAYERS];
new bool:PlayerInside7Eleven[MAX_PLAYERS];


// ========================================
// ENTER 7-ELEVEN
// ========================================

stock Shop_Enter7Eleven(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(PlayerDead[playerid])
        return 0;

    if(PlayerEntering7Eleven[playerid])
        return 0;

    if(PlayerInside7Eleven[playerid])
        return 0;

    PlayerEntering7Eleven[playerid] = true;

    TogglePlayerControllable(playerid, false);

    SetPlayerInterior(
        playerid,
        SHOP_7ELEVEN_INTERIOR
    );

    SetPlayerVirtualWorld(playerid, 0);

    SetPlayerPos(
        playerid,
        SHOP_7ELEVEN_INSIDE_X,
        SHOP_7ELEVEN_INSIDE_Y,
        SHOP_7ELEVEN_INSIDE_Z
    );

    SetCameraBehindPlayer(playerid);

    PlayerInside7Eleven[playerid] = true;

    SetTimerEx(
        "Shop_FinishEnter",
        SHOP_7ELEVEN_TRANSITION_TIME,
        false,
        "i",
        playerid
    );

    return 1;
}

forward Shop_FinishEnter(playerid);

public Shop_FinishEnter(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(!PlayerLoggedIn[playerid])
        return 1;

    if(!PlayerEntering7Eleven[playerid])
        return 1;

    PlayerEntering7Eleven[playerid] = false;

    TogglePlayerControllable(playerid, true);

    SetCameraBehindPlayer(playerid);

    SendClientMessage(
        playerid,
        -1,
        "{FF0000}[ระบบ] {FFFFFF}: ยินดีต้อนรับสู่ 7-Eleven"
    );

    return 1;
}


// ========================================
// EXIT 7-ELEVEN
// ========================================

stock Shop_Exit7Eleven(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(PlayerDead[playerid])
        return 0;

    if(PlayerEntering7Eleven[playerid])
        return 0;

    if(!PlayerInside7Eleven[playerid])
        return 0;

    PlayerEntering7Eleven[playerid] = true;

    TogglePlayerControllable(playerid, false);

    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);

    SetPlayerPos(
        playerid,
        SHOP_7ELEVEN_X,
        SHOP_7ELEVEN_Y,
        SHOP_7ELEVEN_Z
    );

    SetPlayerFacingAngle(
        playerid,
        SHOP_7ELEVEN_EXIT_ANGLE
    );

    SetCameraBehindPlayer(playerid);

    PlayerInside7Eleven[playerid] = false;

    SetTimerEx(
        "Shop_FinishExit",
        SHOP_7ELEVEN_TRANSITION_TIME,
        false,
        "i",
        playerid
    );

    return 1;
}

forward Shop_FinishExit(playerid);

public Shop_FinishExit(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(!PlayerLoggedIn[playerid])
        return 1;

    if(!PlayerEntering7Eleven[playerid])
        return 1;

    PlayerEntering7Eleven[playerid] = false;

    PlayerNear7Eleven[playerid] = true;

    TogglePlayerControllable(playerid, true);

    SetCameraBehindPlayer(playerid);

    SendClientMessage(
        playerid,
        -1,
        "{FF0000}[ระบบ] {FFFFFF}: คุณออกจาก 7-Eleven แล้ว"
    );

    return 1;
}


// ========================================
// SHOP MENU
// ========================================

stock Shop_ShowMenu(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(PlayerDead[playerid])
        return 0;

    if(!PlayerInside7Eleven[playerid])
        return 0;

    if(!IsPlayerInRangeOfPoint(
        playerid,
        SHOP_7ELEVEN_BUY_RANGE,
        SHOP_7ELEVEN_BUY_X,
        SHOP_7ELEVEN_BUY_Y,
        SHOP_7ELEVEN_BUY_Z
    ))
        return 0;

    ShowPlayerDialog(
        playerid,
        DIALOG_7ELEVEN,
        DIALOG_STYLE_LIST,
        "{D4AF37}SORRY {FFFFFF}| 7-Eleven",
        "น้ำดื่ม - $50\nอาหาร - $100",
        "ซื้อ",
        "ปิด"
    );

    return 1;
}


// ========================================
// BUY WATER
// ========================================

stock Shop_BuyWater(playerid, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(PlayerDead[playerid])
        return 0;

    if(!PlayerInside7Eleven[playerid])
        return 0;

    if(!IsPlayerInRangeOfPoint(
        playerid,
        SHOP_7ELEVEN_BUY_RANGE,
        SHOP_7ELEVEN_BUY_X,
        SHOP_7ELEVEN_BUY_Y,
        SHOP_7ELEVEN_BUY_Z
    ))
        return 0;

    if(amount <= 0)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: กรุณาระบุจำนวนที่ถูกต้อง"
        );

        return 0;
    }

    new totalPrice = amount * SHOP_WATER_PRICE;

    if(GetPlayerMoney(playerid) < totalPrice)
    {
        new message[128];

        format(
            message,
            sizeof(message),
            "{FF0000}[7-Eleven] {FFFFFF}: เงินไม่พอ ต้องใช้ $%d สำหรับ %d ขวด",
            totalPrice,
            amount
        );

        SendClientMessage(
            playerid,
            -1,
            message
        );

        return 0;
    }

    if(!Inventory_AddItem(
        playerid,
        SHOP_ITEM_WATER,
        amount
    ))
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: จำนวนที่ต้องการซื้อเกินความจุของน้ำดื่มในกระเป๋า"
        );

        return 0;
    }

    if(!TakePlayerCash(
        playerid,
        totalPrice
    ))
    {
        Inventory_RemoveItem(
            playerid,
            SHOP_ITEM_WATER,
            amount
        );

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: ไม่สามารถชำระเงินได้"
        );

        return 0;
    }

    Inventory_Save(playerid);

    new message[128];

    format(
        message,
        sizeof(message),
        "{00FF00}[7-Eleven] {FFFFFF}: คุณซื้อน้ำดื่ม %d ขวด ราคา $%d",
        amount,
        totalPrice
    );

    SendClientMessage(
        playerid,
        -1,
        message
    );

    return 1;
}


// ========================================
// BUY FOOD
// ========================================

stock Shop_BuyFood(playerid, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!PlayerLoggedIn[playerid])
        return 0;

    if(PlayerDead[playerid])
        return 0;

    if(!PlayerInside7Eleven[playerid])
        return 0;

    if(!IsPlayerInRangeOfPoint(
        playerid,
        SHOP_7ELEVEN_BUY_RANGE,
        SHOP_7ELEVEN_BUY_X,
        SHOP_7ELEVEN_BUY_Y,
        SHOP_7ELEVEN_BUY_Z
    ))
        return 0;

    if(amount <= 0)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: กรุณาระบุจำนวนที่ถูกต้อง"
        );

        return 0;
    }

    new totalPrice = amount * SHOP_FOOD_PRICE;

    if(GetPlayerMoney(playerid) < totalPrice)
    {
        new message[128];

        format(
            message,
            sizeof(message),
            "{FF0000}[7-Eleven] {FFFFFF}: เงินไม่พอ ต้องใช้ $%d สำหรับ %d ชิ้น",
            totalPrice,
            amount
        );

        SendClientMessage(
            playerid,
            -1,
            message
        );

        return 0;
    }

    if(!Inventory_AddItem(
        playerid,
        SHOP_ITEM_FOOD,
        amount
    ))
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: จำนวนที่ต้องการซื้อเกินความจุของอาหารในกระเป๋า"
        );

        return 0;
    }

    if(!TakePlayerCash(
        playerid,
        totalPrice
    ))
    {
        Inventory_RemoveItem(
            playerid,
            SHOP_ITEM_FOOD,
            amount
        );

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: ไม่สามารถชำระเงินได้"
        );

        return 0;
    }

    Inventory_Save(playerid);

    new message[128];

    format(
        message,
        sizeof(message),
        "{00FF00}[7-Eleven] {FFFFFF}: คุณซื้ออาหาร %d ชิ้น ราคา $%d",
        amount,
        totalPrice
    );

    SendClientMessage(
        playerid,
        -1,
        message
    );

    return 1;
}