#include <a_mysql>

new MySQL:g_SQL;


// ========================================
// DATABASE CONNECTION
// ========================================

stock Database_Connect()
{
    g_SQL = mysql_connect("127.0.0.1", "root", "", "sorry");

    if(mysql_errno(g_SQL) != 0)
    {
        printf("[MySQL] Connection failed! Error: %d", mysql_errno(g_SQL));
        return 0;
    }

    printf("[MySQL] Successfully connected to database.");
    return 1;
}


stock Database_Disconnect()
{
    if(g_SQL != MySQL:0)
    {
        mysql_close(g_SQL);
        g_SQL = MySQL:0;
    }

    return 1;
}


// ========================================
// CHECK ACCOUNT
// ========================================

stock Database_CheckAccount(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[768];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "SELECT id, password, money, health, armour, admin, VIP, Level, EXP, Hunger, Thirst, PaydayTime, Age, Gender, Dead, DeathTime, DeathX, DeathY, DeathZ, DeathAngle FROM accounts WHERE username = '%e' LIMIT 1",
        playerName
    );

    mysql_tquery(g_SQL, query, "OnAccountCheck", "d", playerid);

    return 1;
}


forward OnAccountCheck(playerid);


public OnAccountCheck(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    new rows;
    cache_get_row_count(rows);

    if(rows > 0)
    {
        // ACCOUNT DATA

        cache_get_value_name_int(0, "id", PlayerAccountID[playerid]);

        cache_get_value_name(
            0,
            "password",
            PlayerLoginHash[playerid],
            BCRYPT_HASH_LENGTH
        );

        cache_get_value_name_int(0, "money", PlayerMoney[playerid]);
        cache_get_value_name_float(0, "health", PlayerHealth[playerid]);
        cache_get_value_name_float(0, "armour", PlayerArmour[playerid]);
        cache_get_value_name_int(0, "admin", PlayerAdmin[playerid]);
        cache_get_value_name_int(0, "VIP", PlayerVIP[playerid]);
        cache_get_value_name_int(0, "Level", PlayerLevel[playerid]);
        cache_get_value_name_int(0, "EXP", PlayerEXP[playerid]);
        cache_get_value_name_int(0, "Hunger", PlayerHunger[playerid]);
        cache_get_value_name_int(0, "Thirst", PlayerThirst[playerid]);
        cache_get_value_name_int(0, "PaydayTime", PlayerPaydayTime[playerid]);
        cache_get_value_name_int(0, "Age", PlayerAge[playerid]);
        cache_get_value_name_int(0, "Gender", PlayerGender[playerid]);

        // DEATH STATE

        cache_get_value_name_int(0, "Dead", PlayerDead[playerid]);
        cache_get_value_name_int(0, "DeathTime", PlayerDeathTime[playerid]);
        cache_get_value_name_float(0, "DeathX", PlayerDeathX[playerid]);
        cache_get_value_name_float(0, "DeathY", PlayerDeathY[playerid]);
        cache_get_value_name_float(0, "DeathZ", PlayerDeathZ[playerid]);
        cache_get_value_name_float(0, "DeathAngle", PlayerDeathAngle[playerid]);

        // LOGIN SCENE

        SetLoginScene(playerid);

        ShowPlayerDialog(
            playerid,
            DIALOG_LOGIN,
            DIALOG_STYLE_PASSWORD,
            "{D4AF37}SORRY {FFFFFF}| เข้าสู่ระบบ",
            "{FFFFFF}ยินดีต้อนรับกลับเข้าสู่เซิร์ฟเวอร์ครับ!\n\nกรุณากรอกรหัสผ่านของคุณเพื่อเข้าสู่เกม\n\n\nรหัสผ่านของคุณจะถูกเข้ารหัสอย่างปลอดภัย\n",
            "เข้าสู่ระบบ",
            "ยกเลิก"
        );
    }
    else
    {
        ShowPlayerDialog(
            playerid,
            DIALOG_REGISTER,
            DIALOG_STYLE_PASSWORD,
            "{D4AF37}SORRY {FFFFFF}| สมัครสมาชิก",
            "{FFFFFF}ยินดีต้อนรับผู้เล่นใหม่ครับ!\n\nกรุณาตั้งรหัสผ่านสำหรับบัญชีของคุณ\n\nข้อกำหนดรหัสผ่าน\nอย่างน้อย 4 ตัวอักษร\nห้ามบอกรหัสผ่านให้ผู้อื่น\n\n\nรหัสผ่านจะถูกเข้ารหัสอย่างปลอดภัย\n",
            "สมัครสมาชิก",
            "ยกเลิก"
        );
    }

    return 1;
}


// ========================================
// INVENTORY CORE
// ========================================

stock Inventory_Clear(playerid)
{
    for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
    {
        PlayerInventoryItem[playerid][slot] = 0;
        PlayerInventoryAmount[playerid][slot] = 0;
    }

    return 1;
}


stock Inventory_AddItem(playerid, itemID, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(!Item_IsValid(itemID))
        return 0;

    if(amount <= 0)
        return 0;

    // FOOD / DRINK CAPACITY

    new itemType = Item_GetType(itemID);

    if(itemType == ITEM_TYPE_FOOD || itemType == ITEM_TYPE_DRINK)
    {
        new limit = 20;
        new currentAmount = 0;

        if(PlayerVIP[playerid] == 1)
            limit = 60;
        else if(PlayerVIP[playerid] == 2)
            limit = 100;

        for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
        {
            if(PlayerInventoryItem[playerid][slot] == itemID)
            {
                currentAmount = PlayerInventoryAmount[playerid][slot];
                break;
            }
        }

        if(currentAmount + amount > limit)
            return 0;
    }

    // STACK ITEM

    if(Item_IsStackable(itemID))
    {
        for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
        {
            if(PlayerInventoryItem[playerid][slot] == itemID)
            {
                PlayerInventoryAmount[playerid][slot] += amount;
                return 1;
            }
        }
    }

    // FIND EMPTY SLOT

    for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
    {
        if(PlayerInventoryItem[playerid][slot] == 0)
        {
            PlayerInventoryItem[playerid][slot] = itemID;
            PlayerInventoryAmount[playerid][slot] = amount;
            return 1;
        }
    }

    return 0;
}


stock Inventory_RemoveItem(playerid, itemID, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(itemID <= 0 || amount <= 0)
        return 0;

    for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
    {
        if(PlayerInventoryItem[playerid][slot] != itemID)
            continue;

        if(PlayerInventoryAmount[playerid][slot] < amount)
            return 0;

        PlayerInventoryAmount[playerid][slot] -= amount;

        if(PlayerInventoryAmount[playerid][slot] <= 0)
        {
            PlayerInventoryItem[playerid][slot] = 0;
            PlayerInventoryAmount[playerid][slot] = 0;
        }

        return 1;
    }

    return 0;
}


stock Inventory_Load(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(PlayerAccountID[playerid] <= 0)
        return 0;

    Inventory_Clear(playerid);

    new query[256];

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "SELECT slot, item_id, amount FROM inventory WHERE account_id = %d ORDER BY slot ASC",
        PlayerAccountID[playerid]
    );

    mysql_tquery(g_SQL, query, "OnInventoryLoad", "d", playerid);

    return 1;
}


forward OnInventoryLoad(playerid);


public OnInventoryLoad(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    new rows;
    cache_get_row_count(rows);

    for(new row = 0; row < rows; row++)
    {
        new slot;
        new itemID;
        new amount;

        cache_get_value_name_int(row, "slot", slot);
        cache_get_value_name_int(row, "item_id", itemID);
        cache_get_value_name_int(row, "amount", amount);

        if(slot < 1 || slot > MAX_INVENTORY_SLOTS)
            continue;

        if(itemID <= 0 || amount <= 0)
            continue;

        // IGNORE INVALID ITEM FROM DATABASE

        if(!Item_IsValid(itemID))
            continue;

        PlayerInventoryItem[playerid][slot - 1] = itemID;
        PlayerInventoryAmount[playerid][slot - 1] = amount;
    }

    printf(
        "[Inventory] Player %d loaded %d item rows.",
        playerid,
        rows
    );

    return 1;
}


// ========================================
// SAVE INVENTORY
// ========================================

stock Inventory_Save(playerid)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    if(PlayerAccountID[playerid] <= 0)
        return 0;

    new query[256];

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "DELETE FROM inventory WHERE account_id = %d",
        PlayerAccountID[playerid]
    );

    mysql_tquery(g_SQL, query);

    for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
    {
        if(PlayerInventoryItem[playerid][slot] <= 0)
            continue;

        if(PlayerInventoryAmount[playerid][slot] <= 0)
            continue;

        mysql_format(
            g_SQL,
            query,
            sizeof(query),
            "INSERT INTO inventory (account_id, item_id, slot, amount) VALUES (%d, %d, %d, %d)",
            PlayerAccountID[playerid],
            PlayerInventoryItem[playerid][slot],
            slot + 1,
            PlayerInventoryAmount[playerid][slot]
        );

        mysql_tquery(g_SQL, query);
    }

    printf(
        "[Inventory] Player %d inventory saved.",
        playerid
    );

    return 1;
}


// ========================================
// CASH SYSTEM
// ========================================

stock GivePlayerCash(playerid, amount)
{
    if(!PlayerLoggedIn[playerid] || amount <= 0)
        return 0;

    GivePlayerMoney(playerid, amount);
    PlayerMoney[playerid] = GetPlayerMoney(playerid);

    Database_SaveMoney(playerid);

    return 1;
}


stock TakePlayerCash(playerid, amount)
{
    if(!PlayerLoggedIn[playerid] || amount <= 0)
        return 0;

    if(GetPlayerMoney(playerid) < amount)
        return 0;

    GivePlayerMoney(playerid, -amount);
    PlayerMoney[playerid] = GetPlayerMoney(playerid);

    Database_SaveMoney(playerid);

    return 1;
}


stock ChargePlayerCash(playerid, amount)
{
    new currentMoney;
    new chargeAmount;

    if(!PlayerLoggedIn[playerid] || amount <= 0)
        return 0;

    currentMoney = GetPlayerMoney(playerid);

    if(currentMoney <= 0)
    {
        PlayerMoney[playerid] = 0;
        Database_SaveMoney(playerid);
        return 1;
    }

    chargeAmount = currentMoney < amount ? currentMoney : amount;

    GivePlayerMoney(playerid, -chargeAmount);
    PlayerMoney[playerid] = GetPlayerMoney(playerid);

    Database_SaveMoney(playerid);

    return 1;
}


// ========================================
// SAVE MONEY
// ========================================

stock Database_SaveMoney(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];
    new money;

    GetPlayerName(playerid, playerName, sizeof(playerName));

    money = GetPlayerMoney(playerid);

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET money = %d WHERE username = '%e'",
        money,
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// SAVE HEALTH / ARMOUR
// ========================================

stock Database_SaveHealthArmour(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];
    new Float:health;
    new Float:armour;

    GetPlayerName(playerid, playerName, sizeof(playerName));

    GetPlayerHealth(playerid, health);
    GetPlayerArmour(playerid, armour);

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET health = %f, armour = %f WHERE username = '%e'",
        health,
        armour,
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// ADMIN
// ========================================

stock bool:IsAdmin(playerid)
{
    return PlayerAdmin[playerid] >= 1;
}


// ========================================
// SAVE VIP
// ========================================

stock Database_SaveVIP(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET VIP = %d WHERE username = '%e'",
        PlayerVIP[playerid],
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// SAVE PAYDAY TIME
// ========================================

stock Database_SavePaydayTime(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET PaydayTime = %d WHERE username = '%e'",
        PlayerPaydayTime[playerid],
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// RESET PAYDAY TIME
// ========================================

stock Database_ResetAllPaydayTime()
{
    new query[128];

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET PaydayTime = 0"
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// SAVE LEVEL / EXP
// ========================================

stock Database_SaveLevelEXP(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET Level = %d, EXP = %d WHERE username = '%e'",
        PlayerLevel[playerid],
        PlayerEXP[playerid],
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// SAVE HUNGER / THIRST
// ========================================

stock Database_SaveHungerThirst(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET Hunger = %d, Thirst = %d WHERE username = '%e'",
        PlayerHunger[playerid],
        PlayerThirst[playerid],
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// SAVE PLAYER
// ========================================

stock Database_SavePlayer(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[1024];
    new money;
    new Float:health;
    new Float:armour;

    GetPlayerName(playerid, playerName, sizeof(playerName));

    money = GetPlayerMoney(playerid);

    GetPlayerHealth(playerid, health);
    GetPlayerArmour(playerid, armour);

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET money = %d, health = %f, armour = %f, admin = %d, VIP = %d, Level = %d, EXP = %d, Hunger = %d, Thirst = %d, Age = %d, Gender = %d, Dead = %d, DeathTime = %d, DeathX = %f, DeathY = %f, DeathZ = %f, DeathAngle = %f WHERE username = '%e'",
        money,
        health,
        armour,
        PlayerAdmin[playerid],
        PlayerVIP[playerid],
        PlayerLevel[playerid],
        PlayerEXP[playerid],
        PlayerHunger[playerid],
        PlayerThirst[playerid],
        PlayerAge[playerid],
        PlayerGender[playerid],
        PlayerDead[playerid],
        PlayerDeathTime[playerid],
        PlayerDeathX[playerid],
        PlayerDeathY[playerid],
        PlayerDeathZ[playerid],
        PlayerDeathAngle[playerid],
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}


// ========================================
// DATABASE AUTO SAVE
// ========================================

forward Database_AutoSave();


public Database_AutoSave()
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid) || !PlayerLoggedIn[playerid])
            continue;

        Database_SavePlayer(playerid);
        Database_SavePaydayTime(playerid);
        Inventory_Save(playerid);

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: ได้ทำการบันทึกข้อมูลของคุณเรียบร้อยแล้ว"
        );
    }

    return 1;
}


// ========================================
// SAVE CHARACTER
// ========================================

stock Database_SaveCharacter(playerid)
{
    new playerName[MAX_PLAYER_NAME];
    new query[256];

    GetPlayerName(playerid, playerName, sizeof(playerName));

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "UPDATE accounts SET Age = %d, Gender = %d WHERE username = '%e'",
        PlayerAge[playerid],
        PlayerGender[playerid],
        playerName
    );

    mysql_tquery(g_SQL, query);

    return 1;
}