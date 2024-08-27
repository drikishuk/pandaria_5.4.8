#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "Unit.h"
#include "Player.h"
#include "SpellAuras.h"


class CharacterDamageNerf : public PlayerScript
{
public:
    CharacterDamageNerf() : PlayerScript("CharacterDamageNerf") { }

    void OnLogin(Player* player) override
    {
        ApplyNerfAura(player);
    }

    void OnFreeTalentPointsChanged(Player* player, uint32 /*points*/) override
    {
        ApplyNerfAura(player);
    }

    void OnTalentsReset(Player* player, bool /*noCost*/) override
    {
        ApplyNerfAura(player);
    }

    void OnActivateSpec(Player* player, uint8 /*spec*/) override
    {
        ApplyNerfAura(player);
    }

private:
    uint32 GetActiveNerfAura(Player* player)
    {
        Unit::AuraMap& ownedAuras = player->GetOwnedAuras();
        for (uint32 nerfSpellId = FRACTURED_NERF_SPELLS_BEGIN; nerfSpellId < FRACTURED_NERF_SPELLS_END; ++nerfSpellId)
        {
            //if (ownedAuras.contains(nerfSpellId))
                //return nerfSpellId;
            if (ownedAuras.find(nerfSpellId) != ownedAuras.end())
                return nerfSpellId;
        }
        return 0;
    }

    void ApplyNerfAura(Player* player)
    {
        uint32 fracturedNerfSpell = FRACTURED_NERF_GENERIC;
        uint32 activeNerfSpell = GetActiveNerfAura(player);

        switch (player->GetSpecialization())
        {
        case SPEC_WARRIOR_ARMS:
            fracturedNerfSpell = FRACTURED_NERF_WARRIOR_ARMS;
            break;
        case SPEC_WARRIOR_FURY:
            fracturedNerfSpell = FRACTURED_NERF_WARRIOR_FURY;
            break;
        case SPEC_WARRIOR_PROTECTION:
            fracturedNerfSpell = FRACTURED_NERF_WARRIOR_PROTECTION;
            break;
        case SPEC_PALADIN_HOLY:
            fracturedNerfSpell = FRACTURED_NERF_PALADIN_HOLY;
            break;
        case SPEC_PALADIN_PROTECTION:
            fracturedNerfSpell = FRACTURED_NERF_PALADIN_PROTECTION;
            break;
        case SPEC_PALADIN_RETRIBUTION:
            fracturedNerfSpell = FRACTURED_NERF_PALADIN_RETRIBUTION;
            break;
        case SPEC_HUNTER_BEAST_MASTERY:
            fracturedNerfSpell = FRACTURED_NERF_HUNTER_BEAST_MASTERY;
            break;
        case SPEC_HUNTER_MARKSMANSHIP:
            fracturedNerfSpell = FRACTURED_NERF_HUNTER_MARKSMANSHIP;
            break;
        case SPEC_HUNTER_SURVIVAL:
            fracturedNerfSpell = FRACTURED_NERF_HUNTER_SURVIVAL;
            break;
        case SPEC_ROGUE_ASSASSINATION:
            fracturedNerfSpell = FRACTURED_NERF_ROGUE_ASSASINATION;
            break;
        case SPEC_ROGUE_COMBAT:
            fracturedNerfSpell = FRACTURED_NERF_ROGUE_COMBAT;
            break;
        case SPEC_ROGUE_SUBTLETY:
            fracturedNerfSpell = FRACTURED_NERF_ROGUE_SUBTLETY;
            break;
        case SPEC_PRIEST_DISCIPLINE:
            fracturedNerfSpell = FRACTURED_NERF_PRIEST_DISCIPLINE;
            break;
        case SPEC_PRIEST_HOLY:
            fracturedNerfSpell = FRACTURED_NERF_PRIEST_HOLY;
            break;
        case SPEC_PRIEST_SHADOW:
            fracturedNerfSpell = FRACTURED_NERF_PRIEST_SHADOW;
            break;
        case SPEC_DEATH_KNIGHT_BLOOD:
            fracturedNerfSpell = FRACTURED_NERF_DK_BLOOD;
            break;
        case SPEC_DEATH_KNIGHT_FROST:
            fracturedNerfSpell = FRACTURED_NERF_DK_FROST;
            break;
        case SPEC_DEATH_KNIGHT_UNHOLY:
            fracturedNerfSpell = FRACTURED_NERF_DK_UNHOLY;
            break;
        case SPEC_SHAMAN_ELEMENTAL:
            fracturedNerfSpell = FRACTURED_NERF_SHAMAN_ELEMENTAL;
            break;
        case SPEC_SHAMAN_ENHANCEMENT:
            fracturedNerfSpell = FRACTURED_NERF_SHAMAN_ENHANCEMENT;
            break;
        case SPEC_SHAMAN_RESTORATION:
            fracturedNerfSpell = FRACTURED_NERF_SHAMAN_RESTORATION;
            break;
        case SPEC_MAGE_ARCANE:
            fracturedNerfSpell = FRACTURED_NERF_WARRIOR_PROTECTION;
            break;
        case SPEC_MAGE_FIRE:
            fracturedNerfSpell = FRACTURED_NERF_MAGE_FIRE;
            break;
        case SPEC_MAGE_FROST:
            fracturedNerfSpell = FRACTURED_NERF_MAGE_FROST;
            break;
        case SPEC_WARLOCK_AFFLICTION:
            fracturedNerfSpell = FRACTURED_NERF_WARLOCK_AFFLICTION;
            break;
        case SPEC_WARLOCK_DEMONOLOGY:
            fracturedNerfSpell = FRACTURED_NERF_WARLOCK_DEMONOLOGY;
            break;
        case SPEC_WARLOCK_DESTRUCTION:
            fracturedNerfSpell = FRACTURED_NERF_WARLOCK_DESTRUCTION;
            break;
        case SPEC_DRUID_BALANCE:
            fracturedNerfSpell = FRACTURED_NERF_DRUID_BALANCE;
            break;
        case SPEC_DRUID_FERAL:
            fracturedNerfSpell = FRACTURED_NERF_DRUID_FERAL;
            break;
        case SPEC_DRUID_RESTORATION:
            fracturedNerfSpell = FRACTURED_NERF_DRUID_RESTORATION;
            break;
        case SPEC_DRUID_GUARDIAN:
            fracturedNerfSpell = FRACTURED_NERF_DRUID_GUARDIAN;
            break;
        case SPEC_MONK_WINDWALKER:
            fracturedNerfSpell = FRACTURED_NERF_MONK_WINDWALKER;
            break;
        case SPEC_MONK_BREWMASTER:
            fracturedNerfSpell = FRACTURED_NERF_MONK_BREWMASTER;
            break;
        case SPEC_MONK_MISTWEAVER:
            fracturedNerfSpell = FRACTURED_NERF_MONK_MISTWEAVER;
            break;

        default:
            break;
        }

        // The nerf aura already active on player is same as one being applied; do nothing.
        if (activeNerfSpell == fracturedNerfSpell)
            return;

        // Remove active nerf spell and learn new nerf spell.
        player->RemoveSpell(activeNerfSpell, false, false);
        player->LearnSpell(fracturedNerfSpell, false);
    }
};

void AddSC_Player_Scripts()
{
    new CharacterDamageNerf();
}