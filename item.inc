// ========================================
// ITEM DEFINITION SYSTEM
// ========================================

// ITEM ID

#define ITEM_WATER  1
#define ITEM_FOOD   2

// ITEM TYPE

#define ITEM_TYPE_GENERAL  0
#define ITEM_TYPE_DRINK   1
#define ITEM_TYPE_FOOD    2
#define ITEM_TYPE_MINERAL 3


// ========================================
// ITEM VALIDATION
// ========================================

stock bool:Item_IsValid(itemID)
{
    if(itemID == ITEM_WATER)
        return true;

    if(itemID == ITEM_FOOD)
        return true;

    return false;
}


// ========================================
// ITEM TYPE
// ========================================

stock Item_GetType(itemID)
{
    if(itemID == ITEM_WATER)
        return ITEM_TYPE_DRINK;

    if(itemID == ITEM_FOOD)
        return ITEM_TYPE_FOOD;

    return ITEM_TYPE_GENERAL;
}


// ========================================
// ITEM NAME
// ========================================

stock Item_GetName(itemID, name[], size)
{
    if(itemID == ITEM_WATER)
    {
        format(name, size, "น้ำ");
        return 1;
    }

    if(itemID == ITEM_FOOD)
    {
        format(name, size, "อาหาร");
        return 1;
    }

    format(name, size, "ไม่ทราบชื่อ");
    return 0;
}


// ========================================
// ITEM STACKABLE
// ========================================

stock bool:Item_IsStackable(itemID)
{
    if(itemID == ITEM_WATER)
        return true;

    if(itemID == ITEM_FOOD)
        return true;

    return false;
}


// ========================================
// ITEM WEIGHT
// ========================================

stock Float:Item_GetWeight(itemID)
{
    if(itemID == ITEM_WATER)
        return 1.0;

    if(itemID == ITEM_FOOD)
        return 1.0;

    return 0.0;
}


// ========================================
// ITEM EFFECT
// ========================================

stock Item_GetEffectAmount(itemID)
{
    if(itemID == ITEM_WATER)
        return 20;

    if(itemID == ITEM_FOOD)
        return 20;

    return 0;
}