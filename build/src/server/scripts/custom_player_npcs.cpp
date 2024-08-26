#include "ScriptPCH.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "CombatAI.h"

#include <vector>


enum Spells
{
    SPELL_FROSTFIRE_BOLT = 44614,
    SPELL_FROST_NOVA = 122,
    SPELL_FIRE_BLAST = 2136,
    SPELL_BLINK = 65793,
    SPELL_SUMMON_WATER_ELEMENTAL = 31687,
    SPELL_ARCANE_BRILLIANCE = 1459,
    SPELL_MAGE_ARMOUR = 6117,
};


enum Events
{
    EVENT_FROST_NOVA = 1,
    EVENT_FIRE_BLAST = 2,
    EVENT_BLINK = 3,
    EVENT_ESCAPE_PLAYER = 4,
};

enum eTalks {
    SAY_AGGRO = 0,
    SAY_DEATH = 1,
};

enum eSummons {
    NPC_CONJURED_WATER_ELEMENTAL = 500001,
};

// https://github.com/alexkulya/pandaria_5.4.8/blob/master/src/server/game/AI/PlayerAI/PlayerAI.cpp


class npc_wandering_mage : public CreatureScript
{
public:
    npc_wandering_mage() : CreatureScript("npc_wandering_mage") { }

    struct npc_wandering_mageAI : public ScriptedAI
    {
        npc_wandering_mageAI(Creature* creature) : ScriptedAI(creature) { }


        struct PrioritySpell
        {
            uint32 SpellID;
            uint32 Cooldown;
            std::function<bool(npc_wandering_mageAI*)> Condition;
        };

        std::vector<PrioritySpell> _prioritySpells =
        {   
            { 2139, 24000, [](npc_wandering_mageAI* myAI) -> bool  // Counterspell
                {
                    return myAI->me->GetVictim()->HasUnitState(UNIT_STATE_CASTING);
                }
            },
            { 2136, 5000, [](npc_wandering_mageAI* myAI) -> bool  // Fire Blast
                {
                // no condition needed, cooldown already checked
                return true;
                }
            },
            { 11366, 0, [](npc_wandering_mageAI* myAI) -> bool  // Pyroblast
                {

                    return myAI->me->HasAura(48108); // Hot Streak
                }
            },
            { 44457, 0, [](npc_wandering_mageAI* myAI) -> bool  // Living Bomb
                {
                    return !myAI->me->GetVictim()->HasAura(44457, myAI->me->GetGUID()); // Check if target doesn't have living bomb casted by me
                }
            },
            { 31661, 20000, [](npc_wandering_mageAI* myAI) -> bool  // Dragon's Breath
                {
                    return myAI->me->GetVictim()->IsWithinMeleeRange(myAI->me) && myAI->me->GetVictim()->isInFront(myAI->me); // Check if target is in melee range and in front of me
                }
            },
            { 122, 20000, [](npc_wandering_mageAI* myAI) -> bool  // Frost Nova
                {
                    return myAI->me->GetVictim()->IsWithinMeleeRange(myAI->me); // Check if target is near me
                }
            },
            { 1953, 20000, [](npc_wandering_mageAI* myAI) -> bool  // Blink
                {
                    return myAI->me->GetVictim()->IsWithinMeleeRange(myAI->me); // Check if target is in melee range
                }
            },
            { 133, 0, [](npc_wandering_mageAI* myAI) -> bool  // Fireball - no Cooldown
                {
                    return true;
                }
            }
        };

        uint32 _remainingGcd = 0;
        std::unordered_map<uint32, uint32> _spellCooldowns;

        void UpdatePriority()
        {
            for (auto& spell : _prioritySpells)
            {
                if (!spell.Condition(this))
                    continue;

                if (Cast(me->GetVictim(), spell))
                    return;
            }
        }

        bool Cast(Unit* target, PrioritySpell const& spell)
        {
            if (_remainingGcd > 0)
                return false;

            auto it = _spellCooldowns.find(spell.SpellID);
            if (it != _spellCooldowns.end())
                return false;

            _remainingGcd = 1000;
            _spellCooldowns[spell.SpellID] = spell.Cooldown;
            DoCast(target, spell.SpellID);
            return true;
        }

        void EnterCombat(Unit* /*who*/) override
        {
            Talk(SAY_AGGRO);
        }

        void AttackStart(Unit* victim) override
        {
            if (victim && me->Attack(victim, true))
                me->GetMotionMaster()->MoveChase(victim, 30.0f);
        }

        void UpdateAI(uint32 diff) override
        {
            if (_remainingGcd >= diff)
                _remainingGcd -= diff;
            else
                _remainingGcd = 0;

            for (auto& cooldown : _spellCooldowns)
            {
                if (cooldown.second > diff)
                    cooldown.second -= diff;
                else
                    cooldown.second = 0;
            }

            // remove 0 cooldowns
            for (auto it = _spellCooldowns.begin(); it != _spellCooldowns.end();)
            {
                if (it->second == 0)
                    it = _spellCooldowns.erase(it); // erase returns the next iterator
                else
                    ++it;
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            UpdatePriority();

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_wandering_mageAI(creature);
    }
};

void AddSC_custom_player_npcs()
{
    new npc_wandering_mage();
}