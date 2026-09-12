// ========================================
// DIALOG DEFINITIONS
// ========================================

#define DIALOG_REGISTER          1
#define DIALOG_LOGIN             2
#define DIALOG_DEATH_RESPAWN     3
#define DIALOG_SPAWN_SELECT      4
#define DIALOG_CHARACTER_AGE     5
#define DIALOG_CHARACTER_GENDER  6
#define DIALOG_H_MENU            7
#define DIALOG_H_CHARACTER       8
#define DIALOG_H_STATUS          9
#define DIALOG_BIKE_MENU         10
#define DIALOG_7ELEVEN            11
#define DIALOG_INVENTORY         12
#define DIALOG_VEHICLE_MENU      16
#define DIALOG_VEHICLE_GARAGE    17


// ========================================
// INVENTORY
// ========================================

#define MAX_INVENTORY_SLOTS 20


// ========================================
// VEHICLE DEFINITION
// ========================================

// VEHICLE OWNER TYPE
#define VEHICLE_OWNER_PLAYER     1
#define VEHICLE_OWNER_FACTION    2
#define VEHICLE_OWNER_JOB        3

// VEHICLE STATUS
#define VEHICLE_STATUS_GARAGE    1
#define VEHICLE_STATUS_OUT       2
#define VEHICLE_STATUS_BROKEN    3


// ========================================
// ITEM DEFINITION
// ========================================

// ITEM ID
#define ITEM_WATER              1
#define ITEM_FOOD               2

// ITEM TYPE
#define ITEM_TYPE_GENERAL       0
#define ITEM_TYPE_FOOD          1
#define ITEM_TYPE_DRINK         2
#define ITEM_TYPE_MATERIAL      3

// ITEM EFFECT
#define ITEM_EFFECT_NONE        0
#define ITEM_EFFECT_HUNGER      1
#define ITEM_EFFECT_THIRST      2


stock bool:Item_IsValid(itemID)
{
    if(itemID == ITEM_WATER)
        return true;

    if(itemID == ITEM_FOOD)
        return true;

    return false;
}


stock Item_GetType(itemID)
{
    switch(itemID)
    {
        case ITEM_WATER: return ITEM_TYPE_DRINK;
        case ITEM_FOOD: return ITEM_TYPE_FOOD;
    }

    return ITEM_TYPE_GENERAL;
}


stock bool:Item_IsStackable(itemID)
{
    switch(itemID)
    {
        case ITEM_WATER: return true;
        case ITEM_FOOD: return true;
    }

    return false;
}


stock Float:Item_GetWeight(itemID)
{
    switch(itemID)
    {
        case ITEM_WATER: return 1.0;
        case ITEM_FOOD: return 1.0;
    }

    return 0.0;
}


stock Item_GetEffect(itemID)
{
    switch(itemID)
    {
        case ITEM_WATER: return ITEM_EFFECT_THIRST;
        case ITEM_FOOD: return ITEM_EFFECT_HUNGER;
    }

    return ITEM_EFFECT_NONE;
}


stock Item_GetEffectAmount(itemID)
{
    switch(itemID)
    {
        case ITEM_WATER: return 20;
        case ITEM_FOOD: return 20;
    }

    return 0;
}


stock Item_GetName(itemID, name[], size)
{
    switch(itemID)
    {
        case ITEM_WATER:
        {
            format(name, size, "น้ำ");
            return 1;
        }

        case ITEM_FOOD:
        {
            format(name, size, "อาหาร");
            return 1;
        }
    }

    format(name, size, "ไม่ทราบชื่อ");
    return 0;
}


// ========================================
// PLAYER INVENTORY DATA
// ========================================

new PlayerAccountID[MAX_PLAYERS];

new PlayerInventoryItem[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new PlayerInventoryAmount[MAX_PLAYERS][MAX_INVENTORY_SLOTS];


// ========================================
// PLAYER DATA
// ========================================

new bool:PlayerLoggedIn[MAX_PLAYERS];
new bool:PlayerDead[MAX_PLAYERS];

new PlayerDeathTime[MAX_PLAYERS];
new PlayerLoginAttempts[MAX_PLAYERS];

new PlayerLoginHash[MAX_PLAYERS][BCRYPT_HASH_LENGTH];

new PlayerMoney[MAX_PLAYERS];

new Float:PlayerHealth[MAX_PLAYERS];
new Float:PlayerArmour[MAX_PLAYERS];

new PlayerAdmin[MAX_PLAYERS];
new PlayerVIP[MAX_PLAYERS];

new PlayerOTime[MAX_PLAYERS];
new PlayerPaydayTime[MAX_PLAYERS];

new PlayerLevel[MAX_PLAYERS];
new PlayerEXP[MAX_PLAYERS];

new PlayerHunger[MAX_PLAYERS];
new PlayerThirst[MAX_PLAYERS];

new PlayerAge[MAX_PLAYERS];
new PlayerGender[MAX_PLAYERS];

new Float:PlayerDeathX[MAX_PLAYERS];
new Float:PlayerDeathY[MAX_PLAYERS];
new Float:PlayerDeathZ[MAX_PLAYERS];
new Float:PlayerDeathAngle[MAX_PLAYERS];

new bool:PlayerHasHouse[MAX_PLAYERS];
new PlayerFaction[MAX_PLAYERS];

new bool:PlayerSpawnSelected[MAX_PLAYERS];