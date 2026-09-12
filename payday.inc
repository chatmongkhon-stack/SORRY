// ========================================
// PAYDAY SYSTEM
// ========================================

forward PaydayTimer();
forward PaydayTimeTimer();

public PaydayTimeTimer()
{
    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid)) continue;
        if(!PlayerLoggedIn[playerid]) continue;

        PlayerPaydayTime[playerid]++;
    }

    return 1;
}

public PaydayTimer()
{
    new paydayMoney;
    new paydayEXP;
    new message[144];

    for(new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if(!IsPlayerConnected(playerid)) continue;
        if(!PlayerLoggedIn[playerid]) continue;

        // ต้องสะสมเวลาอย่างน้อย 60 นาที
        if(PlayerPaydayTime[playerid] >= 60)
        {
            // MEMBER
            if(PlayerVIP[playerid] == 0)
            {
                paydayMoney = 200;
                paydayEXP = 5;
            }
            // VIP
            else if(PlayerVIP[playerid] == 1)
            {
                paydayMoney = 400;
                paydayEXP = 8;
            }
            // PREMIUM
            else if(PlayerVIP[playerid] == 2)
            {
                paydayMoney = 600;
                paydayEXP = 10;
            }

            GivePlayerCash(playerid, paydayMoney);
            GivePlayerEXP(playerid, paydayEXP);
            Database_SaveLevelEXP(playerid);

            // รับ Payday แล้วจึง Reset เวลา
            PlayerPaydayTime[playerid] = 0;
            Database_SavePaydayTime(playerid);

            format(message, sizeof(message), "{D4AF37}[PAYDAY] {FFFFFF}คุณได้รับ $%d และ %d EXP", paydayMoney, paydayEXP);
            SendClientMessage(playerid, -1, message);
        }
        else
        {
            format(message, sizeof(message), "{D4AF37}[PAYDAY] {FFFFFF}คุณออนไม่ถึง 1 ชั่วโมง กรุณาออนไลน์ให้ครบเพื่อรอรับในรอบถัดไป");
            SendClientMessage(playerid, -1, message);

            // ยังไม่ถึง 60 นาที จะไม่ Reset เวลา
        }
    }

    return 1;
}