// ========================================
// LEVEL SYSTEM
// ========================================

stock GetLevelRequiredEXP(level)
{
    new requiredEXP = 0;
    new step;
    new i;

    for(i = 1; i <= level; i++)
    {
        if(i <= 10)
        {
            step = 50;
        }
        else
        {
            step = ((i - 1) / 10) * 100;
        }

        requiredEXP += step;
    }

    return requiredEXP;
}

// ========================================
// UPDATE PLAYER SCORE
// ========================================

stock UpdatePlayerLevelScore(playerid)
{
    SetPlayerScore(playerid, PlayerLevel[playerid]);
    return 1;
}

// ========================================
// GIVE EXP
// ========================================

stock GivePlayerEXP(playerid, amount)
{
    new requiredEXP;
    new message[144];

    if(!PlayerLoggedIn[playerid])
    {
        return 0;
    }

    if(amount <= 0)
    {
        return 0;
    }

    PlayerEXP[playerid] += amount;

    while(PlayerEXP[playerid] >= GetLevelRequiredEXP(PlayerLevel[playerid]))
    {
        requiredEXP = GetLevelRequiredEXP(PlayerLevel[playerid]);
        PlayerEXP[playerid] -= requiredEXP;
        PlayerLevel[playerid]++;
        UpdatePlayerLevelScore(playerid);

        format(
            message,
            sizeof(message),
            "{D4AF37}[ SORRY ] {FFFFFF}ยินดีด้วย! คุณเลื่อนเป็น Level %d แล้ว",
            PlayerLevel[playerid]
        );

        SendClientMessage(playerid, -1, message);
    }

    return 1;
}