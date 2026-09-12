// ========================================
// DIALOG CALLBACKS
// ========================================

forward OnPasswordHash(playerid, hashid);
forward OnPasswordVerify(playerid, bool:success);
forward OnAccountRegistered(playerid);

#define DIALOG_INVENTORY_AMOUNT 13
#define DIALOG_INVENTORY_CATEGORY 14
#define DIALOG_7ELEVEN_AMOUNT 15

new PlayerBike[MAX_PLAYERS];
new PlayerBikeCooldown[MAX_PLAYERS];

new PlayerInventoryDialogSlot[MAX_PLAYERS][MAX_INVENTORY_SLOTS];
new PlayerInventoryUseSlot[MAX_PLAYERS];

new Player7ElevenBuyItem[MAX_PLAYERS];


// ========================================
// INVENTORY LIMIT
// ========================================

stock GetPlayerInventoryLimit(playerid)
{
    if(PlayerVIP[playerid] == 2)
        return 100;

    if(PlayerVIP[playerid] == 1)
        return 60;

    return 20;
}


// ========================================
// INVENTORY CATEGORY
// ========================================

stock ShowInventoryCategoryDialog(playerid)
{
    ShowPlayerDialog(
        playerid,
        DIALOG_INVENTORY_CATEGORY,
        DIALOG_STYLE_LIST,
        "{D4AF37}SORRY {FFFFFF}| กระเป๋าสัมภาระ",
        "อาหาร/น้ำดื่ม\nแร่ต่าง ๆ",
        "เลือก",
        "กลับ"
    );

    return 1;
}


// ========================================
// FOOD / DRINK INVENTORY
// ========================================

stock ShowFoodDrinkInventory(playerid)
{
    new dialogText[512];
    new line[128];
    new itemID;
    new amount;
    new waterAmount = 0;
    new foodAmount = 0;
    new waterSlot = -1;
    new foodSlot = -1;
    new limit;

    limit = GetPlayerInventoryLimit(playerid);

    for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
    {
        itemID = PlayerInventoryItem[playerid][slot];
        amount = PlayerInventoryAmount[playerid][slot];

        if(itemID == ITEM_WATER && amount > 0)
        {
            waterAmount = amount;
            waterSlot = slot;
        }
        else if(itemID == ITEM_FOOD && amount > 0)
        {
            foodAmount = amount;
            foodSlot = slot;
        }
    }

    PlayerInventoryDialogSlot[playerid][0] = -1;
    PlayerInventoryDialogSlot[playerid][1] = -1;

    // อาหาร
    PlayerInventoryDialogSlot[playerid][0] = foodSlot;

    format(
        line,
        sizeof(line),
        "อาหาร [%d/%d]",
        foodAmount,
        limit
    );

    strcat(dialogText, line);

    // น้ำดื่ม
    PlayerInventoryDialogSlot[playerid][1] = waterSlot;

    format(
        line,
        sizeof(line),
        "\nน้ำดื่ม [%d/%d]",
        waterAmount,
        limit
    );

    strcat(dialogText, line);

    ShowPlayerDialog(
        playerid,
        DIALOG_INVENTORY,
        DIALOG_STYLE_LIST,
        "{D4AF37}SORRY {FFFFFF}| อาหาร/น้ำดื่ม",
        dialogText,
        "เลือก",
        "กลับ"
    );

    return 1;
}


// ========================================
// MINERAL INVENTORY
// ========================================

stock ShowMineralInventory(playerid)
{
    new dialogText[512];
    new itemID;
    new amount;
    new itemCount;

    PlayerInventoryDialogSlot[playerid][0] = -1;

    for(new slot = 0; slot < MAX_INVENTORY_SLOTS; slot++)
    {
        itemID = PlayerInventoryItem[playerid][slot];
        amount = PlayerInventoryAmount[playerid][slot];

        if(itemID <= 0 || amount <= 0)
            continue;

        // ยังไม่มีแร่ในระบบ
        // จะเพิ่ม Item ID ของแร่ในขั้นต่อไป

        continue;
    }

    if(itemCount == 0)
    {
        format(
            dialogText,
            sizeof(dialogText),
            "ยังไม่มีแร่ในกระเป๋า"
        );
    }

    ShowPlayerDialog(
        playerid,
        DIALOG_INVENTORY,
        DIALOG_STYLE_LIST,
        "{D4AF37}SORRY {FFFFFF}| แร่ต่าง ๆ",
        dialogText,
        "เลือก",
        "กลับ"
    );

    return 1;
}


// ========================================
// DIALOG RESPONSE
// ========================================

public OnDialogResponse(
    playerid,
    dialogid,
    response,
    listitem,
    inputtext[]
)
{
    // ========================================
    // LOGIN
    // ========================================

    if(dialogid == DIALOG_LOGIN)
    {
        if(!response)
        {
            SendClientMessage(
                playerid,
                -1,
                "คุณยกเลิกการเข้าสู่ระบบ จึงถูกเตะออกจากเซิร์ฟเวอร์"
            );

            Kick(playerid);
            return 1;
        }

        if(strlen(inputtext) < 4)
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_LOGIN,
                DIALOG_STYLE_PASSWORD,
                "{D4AF37}SORRY {FFFFFF}| เข้าสู่ระบบ",
                "{FFFFFF}ยินดีต้อนรับกลับเข้าสู่เซิร์ฟเวอร์ครับ!\n\nกรุณากรอกรหัสผ่านของคุณเพื่อเข้าสู่เกม\n\nรหัสผ่านต้องมีอย่างน้อย 4 ตัวอักษร\n\n\nรหัสผ่านของคุณจะถูกเข้ารหัสอย่างปลอดภัย\n",
                "เข้าสู่ระบบ",
                "ยกเลิก"
            );

            return 1;
        }

        bcrypt_verify(
            playerid,
            "OnPasswordVerify",
            inputtext,
            PlayerLoginHash[playerid]
        );

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: กำลังตรวจสอบรหัสผ่าน..."
        );

        return 1;
    }


    // ========================================
    // REGISTER
    // ========================================

    if(dialogid == DIALOG_REGISTER)
    {
        if(!response)
        {
            SendClientMessage(
                playerid,
                -1,
                "คุณยกเลิกการสมัครสมาชิก จึงถูกเตะออกจากเซิร์ฟเวอร์"
            );

            Kick(playerid);
            return 1;
        }

        if(strlen(inputtext) < 4)
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_REGISTER,
                DIALOG_STYLE_PASSWORD,
                "{D4AF37}SORRY {FFFFFF}| สมัครสมาชิก",
                "{FFFFFF}ยินดีต้อนรับผู้เล่นใหม่ครับ!\n\nกรุณาตั้งรหัสผ่านสำหรับบัญชีของคุณ\n\nรหัสผ่านต้องมีอย่างน้อย 4 ตัวอักษร\n\nข้อควรรู้\nห้ามบอกรหัสผ่านให้ผู้อื่น\nกรุณาจำรหัสผ่านของคุณไว้ให้ดี\n\n\nรหัสผ่านจะถูกเข้ารหัสอย่างปลอดภัย\n",
                "สมัครสมาชิก",
                "ยกเลิก"
            );

            return 1;
        }

        bcrypt_hash(
            playerid,
            "OnPasswordHash",
            inputtext,
            BCRYPT_COST
        );

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: กำลังเข้ารหัสข้อมูลของคุณ..."
        );

        return 1;
    }


    // ========================================
    // DEATH RESPAWN
    // ========================================

    if(dialogid == DIALOG_DEATH_RESPAWN)
    {
        if(!response)
            return 1;

        Death_Respawn(playerid);

        return 1;
    }


    // ========================================
    // CHARACTER AGE
    // ========================================

    if(dialogid == DIALOG_CHARACTER_AGE)
    {
        if(!response)
        {
            Kick(playerid);
            return 1;
        }

        new age = strval(inputtext);

        if(age < 15 || age > 100)
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_CHARACTER_AGE,
                DIALOG_STYLE_INPUT,
                "{D4AF37}SORRY {FFFFFF}| ข้อมูลตัวละคร",
                "{FFFFFF}กรุณากรอกอายุของตัวละคร\n\nอายุที่สามารถเลือกได้: {D4AF37}15 - 100 ปี",
                "ยืนยัน",
                "ยกเลิก"
            );

            return 1;
        }

        PlayerAge[playerid] = age;

        ShowPlayerDialog(
            playerid,
            DIALOG_CHARACTER_GENDER,
            DIALOG_STYLE_LIST,
            "{D4AF37}SORRY {FFFFFF}| เลือกเพศ",
            "ชาย\nหญิง",
            "ยืนยัน",
            "ยกเลิก"
        );

        return 1;
    }


    // ========================================
    // CHARACTER GENDER
    // ========================================

    if(dialogid == DIALOG_CHARACTER_GENDER)
    {
        if(!response)
        {
            Kick(playerid);
            return 1;
        }

        PlayerGender[playerid] = listitem + 1;

        Database_SaveCharacter(playerid);

        PlayerLoggedIn[playerid] = false;

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: สร้างตัวละครสำเร็จ! กรุณาเข้าสู่ระบบ"
        );

        Database_CheckAccount(playerid);

        return 1;
    }


    // ========================================
    // BICYCLE MENU
    // ========================================

    if(dialogid == DIALOG_BIKE_MENU)
    {
        if(!response)
            return 1;

        if(listitem == 0)
        {
            new currentTime = gettime();
            new remainingTime;
            new minutes;
            new seconds;
            new message[128];

            if(PlayerBikeCooldown[playerid] > currentTime)
            {
                remainingTime =
                    PlayerBikeCooldown[playerid] - currentTime;

                minutes = remainingTime / 60;
                seconds = remainingTime % 60;

                format(
                    message,
                    sizeof(message),
                    "{FF0000}[Auto] {FFFFFF}: คุณสามารถเบิกจักรยานได้อีกครั้งใน %d นาที %d วินาที",
                    minutes,
                    seconds
                );

                SendClientMessage(
                    playerid,
                    -1,
                    message
                );

                return 1;
            }

            if(PlayerBike[playerid] != 0)
            {
                DestroyVehicle(
                    PlayerBike[playerid]
                );

                PlayerBike[playerid] = 0;
            }

            PlayerBike[playerid] = CreateVehicle(
                481,
                2034.0275,
                1544.2722,
                10.8203,
                268.0104,
                1,
                1,
                -1
            );

            if(PlayerBike[playerid] == 0)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Auto] {FFFFFF}: ไม่สามารถสร้างจักรยานได้"
                );

                return 1;
            }

            SystemVehicle[
                PlayerBike[playerid]
            ] = true;

            PutPlayerInVehicle(
                playerid,
                PlayerBike[playerid],
                0
            );

            PlayerBikeCooldown[playerid] =
                currentTime + 300;

            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Auto] {FFFFFF}: ได้รับจักรยานแล้ว กรุณารอ 5 นาทีก่อนเบิกคันใหม่"
            );

            return 1;
        }

        return 1;
    }


    // ========================================
    // 7-ELEVEN MENU
    // ========================================

    if(dialogid == DIALOG_7ELEVEN)
    {
        if(!response)
            return 1;

        if(!PlayerLoggedIn[playerid])
            return 1;

        if(PlayerDead[playerid])
            return 1;

        if(!PlayerInside7Eleven[playerid])
            return 1;

        if(!IsPlayerInRangeOfPoint(
            playerid,
            SHOP_7ELEVEN_BUY_RANGE,
            SHOP_7ELEVEN_BUY_X,
            SHOP_7ELEVEN_BUY_Y,
            SHOP_7ELEVEN_BUY_Z
        ))
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[7-Eleven] {FFFFFF}: คุณไม่ได้อยู่ที่จุดซื้อสินค้า"
            );

            return 1;
        }

        // น้ำดื่ม
        if(listitem == 0)
        {
            Player7ElevenBuyItem[playerid] =
                SHOP_ITEM_WATER;

            ShowPlayerDialog(
                playerid,
                DIALOG_7ELEVEN_AMOUNT,
                DIALOG_STYLE_INPUT,
                "{D4AF37}SORRY {FFFFFF}| ซื้อน้ำดื่ม",
                "{FFFFFF}ราคาน้ำดื่ม: {D4AF37}$50 {FFFFFF}ต่อขวด\n\nกรุณาระบุจำนวนที่ต้องการซื้อ",
                "ซื้อ",
                "กลับ"
            );

            return 1;
        }

        // อาหาร
        if(listitem == 1)
        {
            Player7ElevenBuyItem[playerid] =
                SHOP_ITEM_FOOD;

            ShowPlayerDialog(
                playerid,
                DIALOG_7ELEVEN_AMOUNT,
                DIALOG_STYLE_INPUT,
                "{D4AF37}SORRY {FFFFFF}| ซื้ออาหาร",
                "{FFFFFF}ราคาอาหาร: {D4AF37}$100 {FFFFFF}ต่อชิ้น\n\nกรุณาระบุจำนวนที่ต้องการซื้อ",
                "ซื้อ",
                "กลับ"
            );

            return 1;
        }

        return 1;
    }


    // ========================================
    // 7-ELEVEN BUY AMOUNT
    // ========================================

    if(dialogid == DIALOG_7ELEVEN_AMOUNT)
    {
        if(!response)
        {
            Shop_ShowMenu(playerid);
            return 1;
        }

        if(!PlayerLoggedIn[playerid])
            return 1;

        if(PlayerDead[playerid])
            return 1;

        if(!PlayerInside7Eleven[playerid])
            return 1;

        if(!IsPlayerInRangeOfPoint(
            playerid,
            SHOP_7ELEVEN_BUY_RANGE,
            SHOP_7ELEVEN_BUY_X,
            SHOP_7ELEVEN_BUY_Y,
            SHOP_7ELEVEN_BUY_Z
        ))
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[7-Eleven] {FFFFFF}: คุณไม่ได้อยู่ที่จุดซื้อสินค้า"
            );

            return 1;
        }

        new amount = strval(inputtext);

        if(amount <= 0)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[7-Eleven] {FFFFFF}: กรุณาระบุจำนวนที่ถูกต้อง"
            );

            return 1;
        }

        // ซื้ออาหาร
        if(Player7ElevenBuyItem[playerid] == SHOP_ITEM_FOOD)
        {
            Shop_BuyFood(
                playerid,
                amount
            );

            return 1;
        }

        // ซื้อน้ำ
        if(Player7ElevenBuyItem[playerid] == SHOP_ITEM_WATER)
        {
            Shop_BuyWater(
                playerid,
                amount
            );

            return 1;
        }

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[7-Eleven] {FFFFFF}: ไม่พบสินค้าที่ต้องการซื้อ"
        );

        return 1;
    }


    // ========================================
    // SPAWN SELECT
    // ========================================

    if(dialogid == DIALOG_SPAWN_SELECT)
    {
        if(!response)
        {
            ShowSpawnSelectDialog(playerid);
            return 1;
        }

        if(
            !PlayerHasHouse[playerid] &&
            PlayerFaction[playerid] == 0
        )
        {
            if(listitem == 0)
            {
                PlayerSpawnSelected[playerid] = true;

                TogglePlayerSpectating(
                    playerid,
                    false
                );

                SpawnPlayer(playerid);

                return 1;
            }
        }
        else if(
            PlayerHasHouse[playerid] &&
            PlayerFaction[playerid] == 0
        )
        {
            if(listitem == 0)
                return 1;

            if(listitem == 1)
            {
                PlayerSpawnSelected[playerid] = true;

                TogglePlayerSpectating(
                    playerid,
                    false
                );

                SpawnPlayer(playerid);

                return 1;
            }
        }
        else if(
            !PlayerHasHouse[playerid] &&
            PlayerFaction[playerid] > 0
        )
        {
            if(listitem == 0)
                return 1;

            if(listitem == 1)
            {
                PlayerSpawnSelected[playerid] = true;

                TogglePlayerSpectating(
                    playerid,
                    false
                );

                SpawnPlayer(playerid);

                return 1;
            }
        }
        else if(
            PlayerHasHouse[playerid] &&
            PlayerFaction[playerid] > 0
        )
        {
            if(listitem == 0)
                return 1;

            if(listitem == 1)
                return 1;

            if(listitem == 2)
            {
                PlayerSpawnSelected[playerid] = true;

                TogglePlayerSpectating(
                    playerid,
                    false
                );

                SpawnPlayer(playerid);

                return 1;
            }
        }

        return 1;
    }


    // ========================================
    // H MENU
    // ========================================

    if(dialogid == DIALOG_H_MENU)
    {
        if(!response)
            return 1;

        if(listitem == 0)
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_H_CHARACTER,
                DIALOG_STYLE_LIST,
                "{D4AF37}SORRY {FFFFFF}| จัดการตัวละคร",
                "สถานะตัวละคร\nกระเป๋าสัมภาระ",
                "เลือก",
                "กลับ"
            );

            return 1;
        }

        if(listitem == 1)
        {
            SendClientMessage(
                playerid,
                -1,
                "{D4AF37}[GPS] {FFFFFF}: ระบบ GPS กำลังพัฒนา"
            );

            return 1;
        }

        return 1;
    }


    // ========================================
    // H CHARACTER
    // ========================================

    if(dialogid == DIALOG_H_CHARACTER)
    {
        if(!response)
        {
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
        // CHARACTER STATUS
        // ========================================

        if(listitem == 0)
        {
            new playerName[MAX_PLAYER_NAME];
            new genderText[32];
            new factionText[32];
            new vipText[32];
            new message[1024];
            new requiredEXP;

            GetPlayerName(
                playerid,
                playerName,
                sizeof(playerName)
            );

            if(PlayerGender[playerid] == 1)
            {
                format(
                    genderText,
                    sizeof(genderText),
                    "ชาย"
                );
            }
            else if(PlayerGender[playerid] == 2)
            {
                format(
                    genderText,
                    sizeof(genderText),
                    "หญิง"
                );
            }
            else
            {
                format(
                    genderText,
                    sizeof(genderText),
                    "ไม่ระบุ"
                );
            }

            if(PlayerFaction[playerid] == 0)
            {
                format(
                    factionText,
                    sizeof(factionText),
                    "ไม่มี"
                );
            }
            else
            {
                format(
                    factionText,
                    sizeof(factionText),
                    "%d",
                    PlayerFaction[playerid]
                );
            }

            if(PlayerVIP[playerid] == 1)
            {
                format(
                    vipText,
                    sizeof(vipText),
                    "VIP"
                );
            }
            else if(PlayerVIP[playerid] == 2)
            {
                format(
                    vipText,
                    sizeof(vipText),
                    "PERMIUM"
                );
            }
            else
            {
                format(
                    vipText,
                    sizeof(vipText),
                    "ไม่มี"
                );
            }

            requiredEXP =
                GetLevelRequiredEXP(
                    PlayerLevel[playerid]
                );

            format(
                message,
                sizeof(message),
                "{FFFFFF}ข้อมูลส่วนตัว\n\n\
ชื่อ          %s\n\
อายุ          %d ปี\n\
เพศ           %s\n\n\
ความก้าวหน้า\n\n\
ระดับ         %d\n\
ประสบการณ์    %d / %d\n\n\
ข้อมูลการเล่น\n\n\
เงิน          $%d\n\
สังกัด        %s\n\
สิทธิพิเศษ    %s",
                playerName,
                PlayerAge[playerid],
                genderText,
                PlayerLevel[playerid],
                PlayerEXP[playerid],
                requiredEXP,
                GetPlayerMoney(playerid),
                factionText,
                vipText
            );

            ShowPlayerDialog(
                playerid,
                DIALOG_H_STATUS,
                DIALOG_STYLE_MSGBOX,
                "{D4AF37}SORRY {FFFFFF}| ข้อมูลตัวละคร",
                message,
                "ปิด",
                ""
            );

            return 1;
        }

        // ========================================
        // INVENTORY
        // ========================================

        if(listitem == 1)
        {
            ShowInventoryCategoryDialog(playerid);
            return 1;
        }

        return 1;
    }


    // ========================================
    // H STATUS
    // ========================================

    if(dialogid == DIALOG_H_STATUS)
        return 1;


    // ========================================
    // INVENTORY CATEGORY
    // ========================================

    if(dialogid == DIALOG_INVENTORY_CATEGORY)
    {
        if(!response)
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_H_CHARACTER,
                DIALOG_STYLE_LIST,
                "{D4AF37}SORRY {FFFFFF}| จัดการตัวละคร",
                "สถานะตัวละคร\nกระเป๋าสัมภาระ",
                "เลือก",
                "กลับ"
            );

            return 1;
        }

        if(listitem == 0)
        {
            ShowFoodDrinkInventory(playerid);
            return 1;
        }

        if(listitem == 1)
        {
            ShowMineralInventory(playerid);
            return 1;
        }

        return 1;
    }


    // ========================================
    // INVENTORY
    // ========================================

    if(dialogid == DIALOG_INVENTORY)
    {
        if(!response)
        {
            ShowInventoryCategoryDialog(playerid);
            return 1;
        }

        if(listitem < 0 || listitem >= MAX_INVENTORY_SLOTS)
            return 1;

        new slot =
            PlayerInventoryDialogSlot[playerid][listitem];

        if(slot < 0 || slot >= MAX_INVENTORY_SLOTS)
            return 1;

        if(PlayerInventoryItem[playerid][slot] <= 0)
            return 1;

        if(PlayerInventoryAmount[playerid][slot] <= 0)
            return 1;

        PlayerInventoryUseSlot[playerid] = slot;

        new itemID =
            PlayerInventoryItem[playerid][slot];

        new itemEffect =
            Item_GetEffect(itemID);

        // ========================================
        // CHECK CURRENT STATUS
        // ========================================

        if(itemEffect == ITEM_EFFECT_THIRST)
        {
            if(PlayerThirst[playerid] >= 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ค่ากระหายของคุณเต็ม 100 แล้ว"
                );

                ShowFoodDrinkInventory(playerid);
                return 1;
            }
        }
        else if(itemEffect == ITEM_EFFECT_HUNGER)
        {
            if(PlayerHunger[playerid] >= 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ค่าความหิวของคุณเต็ม 100 แล้ว"
                );

                ShowFoodDrinkInventory(playerid);
                return 1;
            }
        }
        else
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ไอเท็มนี้ยังไม่สามารถใช้งานได้"
            );

            return 1;
        }

        new inputMessage[256];
        new itemName[64];

        Item_GetName(
            itemID,
            itemName,
            sizeof(itemName)
        );

        format(
            inputMessage,
            sizeof(inputMessage),
            "{FFFFFF}กรุณาระบุจำนวน {00BFFF}%s {FFFFFF}ที่ต้องการใช้\n\nมีอยู่: {D4AF37}%d ชิ้น",
            itemName,
            PlayerInventoryAmount[playerid][slot]
        );

        ShowPlayerDialog(
            playerid,
            DIALOG_INVENTORY_AMOUNT,
            DIALOG_STYLE_INPUT,
            "{D4AF37}SORRY {FFFFFF}| ใช้ไอเท็ม",
            inputMessage,
            "ใช้",
            "กลับ"
        );

        return 1;
    }


    // ========================================
    // INVENTORY AMOUNT
    // ========================================

    if(dialogid == DIALOG_INVENTORY_AMOUNT)
    {
        new slot =
            PlayerInventoryUseSlot[playerid];

        if(slot < 0 || slot >= MAX_INVENTORY_SLOTS)
            return 1;

        if(!response)
        {
            ShowFoodDrinkInventory(playerid);
            return 1;
        }

        new amount = strval(inputtext);

        if(amount <= 0)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: กรุณาระบุจำนวนที่ถูกต้อง"
            );

            return 1;
        }

        new itemID =
            PlayerInventoryItem[playerid][slot];

        new itemAmount =
            PlayerInventoryAmount[playerid][slot];

        if(itemID <= 0 || itemAmount <= 0)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ไม่พบไอเท็มนี้ในกระเป๋า"
            );

            ShowFoodDrinkInventory(playerid);
            return 1;
        }

        // ========================================
        // CHECK ITEM AMOUNT
        // ========================================

        if(amount > itemAmount)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: จำนวนไอเท็มที่คุณมีไม่เพียงพอ"
            );

            return 1;
        }

        // ========================================
        // ITEM EFFECT
        // ========================================

        new itemEffect =
            Item_GetEffect(itemID);

        new effectAmount =
            Item_GetEffectAmount(itemID);

        if(itemEffect == ITEM_EFFECT_THIRST)
        {
            if(PlayerThirst[playerid] >= 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ค่ากระหายของคุณเต็ม 100 แล้ว"
                );

                ShowFoodDrinkInventory(playerid);
                return 1;
            }

            new maxUse =
                (100 - PlayerThirst[playerid] + effectAmount - 1)
                / effectAmount;

            if(amount > maxUse)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: จำนวนที่ใช้มากเกินไป ค่ากระหายจะเต็ม 100 ก่อน"
                );

                return 1;
            }

            AddPlayerThirst(
                playerid,
                amount * effectAmount
            );

            Inventory_RemoveItem(
                playerid,
                itemID,
                amount
            );

            Database_SaveHungerThirst(playerid);
            Inventory_Save(playerid);
            UpdateStatusHUD(playerid);

            new message[128];

            format(
                message,
                sizeof(message),
                "{00BFFF}[กระเป๋าสัมภาระ] {FFFFFF}: คุณใช้น้ำดื่มจำนวน %d ขวด | กระหาย %d/100",
                amount,
                PlayerThirst[playerid]
            );

            SendClientMessage(
                playerid,
                -1,
                message
            );

            ShowFoodDrinkInventory(playerid);

            return 1;
        }

        if(itemEffect == ITEM_EFFECT_HUNGER)
        {
            if(PlayerHunger[playerid] >= 100)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ค่าความหิวของคุณเต็ม 100 แล้ว"
                );

                ShowFoodDrinkInventory(playerid);
                return 1;
            }

            new maxUse =
                (100 - PlayerHunger[playerid] + effectAmount - 1)
                / effectAmount;

            if(amount > maxUse)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: จำนวนที่ใช้มากเกินไป ค่าความหิวจะเต็ม 100 ก่อน"
                );

                return 1;
            }

            AddPlayerHunger(
                playerid,
                amount * effectAmount
            );

            Inventory_RemoveItem(
                playerid,
                itemID,
                amount
            );

            Database_SaveHungerThirst(playerid);
            Inventory_Save(playerid);
            UpdateStatusHUD(playerid);

            new message[128];

            format(
                message,
                sizeof(message),
                "{00FF00}[กระเป๋าสัมภาระ] {FFFFFF}: คุณใช้อาหารจำนวน %d ชิ้น | หิว %d/100",
                amount,
                PlayerHunger[playerid]
            );

            SendClientMessage(
                playerid,
                -1,
                message
            );

            ShowFoodDrinkInventory(playerid);

            return 1;
        }

        // ========================================
        // UNKNOWN ITEM
        // ========================================

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[กระเป๋าสัมภาระ] {FFFFFF}: ไอเท็มนี้ยังไม่สามารถใช้งานได้"
        );

        ShowFoodDrinkInventory(playerid);

        return 1;
    }


    // ========================================
    // VEHICLE MANAGEMENT
    // ========================================

    if(dialogid == DIALOG_VEHICLE_MENU)
    {
        if(!response)
            return 1;

        if(!PlayerLoggedIn[playerid])
            return 1;

        if(PlayerDead[playerid])
            return 1;

        if(!IsPlayerInAnyVehicle(playerid))
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Vehicle] {FFFFFF}: คุณไม่ได้อยู่ในรถ"
            );

            return 1;
        }

        new vehicleid =
            GetPlayerVehicleID(playerid);

        if(!IsValidVehicle(vehicleid))
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Vehicle] {FFFFFF}: ไม่พบรถที่กำลังใช้งาน"
            );

            return 1;
        }

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

        // ========================================
        // LOCK
        // ========================================

        if(listitem == 0)
        {
            Vehicle_SetLock(
                vehicleid,
                true
            );

            SendClientMessage(
                playerid,
                -1,
                "{00FF00}[Vehicle] {FFFFFF}: ล็อกรถเรียบร้อยแล้ว"
            );

            return 1;
        }

        // ========================================
        // UNLOCK
        // ========================================

        if(listitem == 1)
        {
            Vehicle_SetLock(
                vehicleid,
                false
            );

            SendClientMessage(
                playerid,
                -1,
                "{00FF00}[Vehicle] {FFFFFF}: ปลดล็อกรถเรียบร้อยแล้ว"
            );

            return 1;
        }

        return 1;
    }


    // ========================================
    // VEHICLE GARAGE
    // ========================================

    if(dialogid == DIALOG_VEHICLE_GARAGE)
    {
        if(!response)
            return 1;

        if(!PlayerLoggedIn[playerid])
            return 1;

        if(PlayerDead[playerid])
            return 1;

        if(!IsPlayerInRangeOfPoint(
            playerid,
            3.0,
            2637.2996,
            2345.6279,
            10.6719
        ))
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Vehicle] {FFFFFF}: คุณไม่ได้อยู่ที่ Garage"
            );

            return 1;
        }

        new selected;
        new count;

        selected = -1;
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

            if(count == listitem)
            {
                selected = slot;
                break;
            }

            count++;
        }

        if(selected == -1)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Vehicle] {FFFFFF}: ไม่พบรถคันนี้"
            );

            return 1;
        }

        // ========================================
        // CALL VEHICLE
        // ========================================

        if(
            VehicleDBStatus[selected] == VEHICLE_STATUS_GARAGE ||
            VehicleDBStatus[selected] == VEHICLE_STATUS_BROKEN
        )
        {
            Vehicle_Call(
                playerid,
                VehicleDBID[selected]
            );

            return 1;
        }

        // ========================================
        // STORE VEHICLE
        // ========================================

        if(VehicleDBStatus[selected] == VEHICLE_STATUS_OUT)
        {
            new vehicleid;

            vehicleid = VehicleDBGameID[selected];

            if(vehicleid == INVALID_VEHICLE_ID)
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Vehicle] {FFFFFF}: ไม่พบรถในโลก"
                );

                return 1;
            }

            if(!IsValidVehicle(vehicleid))
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้ไม่สามารถใช้งานได้"
                );

                return 1;
            }

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

            new Float:x;
            new Float:y;
            new Float:z;

            GetVehiclePos(
                vehicleid,
                x,
                y,
                z
            );

            if(
                !IsPlayerInRangeOfPoint(
                    playerid,
                    5.0,
                    x,
                    y,
                    z
                )
            )
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Vehicle] {FFFFFF}: รถคันนี้ไม่ได้อยู่ใกล้คุณ"
                );

                return 1;
            }

            if(
                IsPlayerInAnyVehicle(playerid) &&
                GetPlayerVehicleID(playerid) != vehicleid
            )
            {
                SendClientMessage(
                    playerid,
                    -1,
                    "{FF0000}[Vehicle] {FFFFFF}: คุณไม่ได้อยู่ในรถคันนี้"
                );

                return 1;
            }

            Vehicle_Store(
                playerid,
                vehicleid
            );

            return 1;
        }

        return 1;
    }


    return 1;
}


// ========================================
// SPAWN SELECT DIALOG
// ========================================

stock ShowSpawnSelectDialog(playerid)
{
    new dialogText[256];

    if(
        PlayerHasHouse[playerid] &&
        PlayerFaction[playerid] > 0
    )
    {
        strcat(dialogText, "บ้าน\n");
        strcat(dialogText, "แฟคชั่น\n");
        strcat(dialogText, "จุดเกิดเด็กใหม่");
    }
    else if(PlayerHasHouse[playerid])
    {
        strcat(dialogText, "บ้าน\n");
        strcat(dialogText, "จุดเกิดเด็กใหม่");
    }
    else if(PlayerFaction[playerid] > 0)
    {
        strcat(dialogText, "แฟคชั่น\n");
        strcat(dialogText, "จุดเกิดเด็กใหม่");
    }
    else
    {
        strcat(dialogText, "จุดเกิดเด็กใหม่");
    }

    ShowPlayerDialog(
        playerid,
        DIALOG_SPAWN_SELECT,
        DIALOG_STYLE_LIST,
        "{D4AF37}SORRY {FFFFFF}| เลือกจุดเกิด",
        dialogText,
        "เลือก",
        ""
    );

    return 1;
}


// ========================================
// BCRYPT HASH
// ========================================

public OnPasswordHash(
    playerid,
    hashid
)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    new playerName[MAX_PLAYER_NAME];
    new hashedPassword[BCRYPT_HASH_LENGTH];
    new query[512];

    GetPlayerName(
        playerid,
        playerName,
        sizeof(playerName)
    );

    bcrypt_get_hash(
        hashedPassword,
        sizeof(hashedPassword)
    );

    mysql_format(
        g_SQL,
        query,
        sizeof(query),
        "INSERT INTO accounts (username, password, money, health, armour, admin, VIP, Level, EXP, Hunger, Thirst, PaydayTime, Dead, DeathTime, DeathX, DeathY, DeathZ, DeathAngle) VALUES ('%e', '%e', 0, 100.0, 0.0, 0, 0, 1, 0, 100, 100, 0, 0, 0, 0.0, 0.0, 0.0, 0.0)",
        playerName,
        hashedPassword
    );

    mysql_tquery(
        g_SQL,
        query,
        "OnAccountRegistered",
        "d",
        playerid
    );

    return 1;
}


// ========================================
// BCRYPT VERIFY
// ========================================

public OnPasswordVerify(
    playerid,
    bool:success
)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(PlayerLoggedIn[playerid])
        return 1;

    if(success)
    {
        PlayerLoggedIn[playerid] = true;

        // ========================================
        // LOAD INVENTORY
        // ========================================

        Inventory_Load(playerid);

        UpdatePlayerLevelScore(
            playerid
        );

        ResetPlayerMoney(
            playerid
        );

        GivePlayerMoney(
            playerid,
            PlayerMoney[playerid]
        );

        SetPlayerHealth(
            playerid,
            PlayerHealth[playerid]
        );

        SetPlayerArmour(
            playerid,
            PlayerArmour[playerid]
        );

        CreateStatusHUD(
            playerid
        );

        ShowStatusHUD(
            playerid
        );

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: เข้าสู่ระบบสำเร็จ!"
        );

        // ========================================
        // RESTORE DEATH STATE
        // ========================================

        if(PlayerDead[playerid])
        {
            PlayerSpawnSelected[playerid] = false;

            Death_Restore(playerid);

            return 1;
        }

        // ========================================
        // CHARACTER CREATION
        // ========================================

        if(PlayerGender[playerid] == 0)
        {
            ShowPlayerDialog(
                playerid,
                DIALOG_CHARACTER_AGE,
                DIALOG_STYLE_INPUT,
                "{D4AF37}SORRY {FFFFFF}| สร้างตัวละคร",
                "{FFFFFF}กรุณากรอกอายุของตัวละคร\n\nอายุที่สามารถเลือกได้: {D4AF37}15 - 100 ปี",
                "ยืนยัน",
                "ยกเลิก"
            );

            return 1;
        }

        // ========================================
        // NORMAL SPAWN SELECT
        // ========================================

        PlayerSpawnSelected[playerid] = false;

        TogglePlayerSpectating(
            playerid,
            true
        );

        SetSpawnSelectScene(
            playerid
        );

        ShowSpawnSelectDialog(playerid);
    }
    else
    {
        PlayerLoginAttempts[playerid]++;

        printf(
            "[DEBUG] Player %d login attempts: %d",
            playerid,
            PlayerLoginAttempts[playerid]
        );

        if(PlayerLoginAttempts[playerid] >= 5)
        {
            SendClientMessage(
                playerid,
                -1,
                "{FF0000}[Auto] {FFFFFF}: คุณกรอกรหัสผ่านผิดเกินจำนวนที่กำหนด จึงถูกเตะออกจากเซิร์ฟเวอร์"
            );

            Kick(playerid);

            return 1;
        }

        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: รหัสผ่านไม่ถูกต้อง กรุณาลองใหม่อีกครั้ง"
        );

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

    return 1;
}


// ========================================
// ACCOUNT REGISTERED
// ========================================

public OnAccountRegistered(
    playerid
)
{
    if(!IsPlayerConnected(playerid))
        return 1;

    if(cache_affected_rows() <= 0)
    {
        SendClientMessage(
            playerid,
            -1,
            "{FF0000}[Auto] {FFFFFF}: เกิดข้อผิดพลาดในการสมัครสมาชิก กรุณาลองใหม่อีกครั้ง"
        );

        ShowPlayerDialog(
            playerid,
            DIALOG_REGISTER,
            DIALOG_STYLE_PASSWORD,
            "{D4AF37}SORRY {FFFFFF}| สมัครสมาชิก",
            "{FFFFFF}ยินดีต้อนรับผู้เล่นใหม่ครับ!\n\nกรุณาตั้งรหัสผ่านสำหรับบัญชีของคุณ\n\nข้อกำหนดรหัสผ่าน\nอย่างน้อย 4 ตัวอักษร\nห้ามบอกรหัสผ่านให้ผู้อื่น\n\n\nรหัสผ่านจะถูกเข้ารหัสอย่างปลอดภัย\n",
            "สมัครสมาชิก",
            "ยกเลิก"
        );

        return 1;
    }

    SendClientMessage(
        playerid,
        -1,
        "{FF0000}[Auto] {FFFFFF}: สมัครสมาชิกสำเร็จ! กรุณาสร้างตัวละคร"
    );

    ShowPlayerDialog(
        playerid,
        DIALOG_CHARACTER_AGE,
        DIALOG_STYLE_INPUT,
        "{D4AF37}SORRY {FFFFFF}| สร้างตัวละคร",
        "{FFFFFF}กรุณากรอกอายุของตัวละคร\n\nอายุที่สามารถเลือกได้: {D4AF37}15 - 100 ปี",
        "ยืนยัน",
        "ยกเลิก"
    );

    return 1;
}