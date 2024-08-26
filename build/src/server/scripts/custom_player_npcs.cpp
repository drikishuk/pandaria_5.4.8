#include "ScriptPCH.h"
#include "Vehicle.h"
#include "AchievementMgr.h"
#include "CombatAI.h"


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


class npc_wandering_mage : public CreatureScript {
public: npc_wandering_mage() : CreatureScript("npc_wandering_mage") {}

      struct npc_wandering_mageAI : public ScriptedAI {

          npc_wandering_mageAI(Creature* creature) : ScriptedAI(creature) {
              me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
              me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
              me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
              SetEquipmentSlots(false, 47524, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

              // Setup Spell Cooldowns
              std::map<uint32, uint32> _spellCooldowns;
              _spellCooldowns[SPELL_FROSTFIRE_BOLT] = 2000;


          }

          uint32 GlobalCooldown = 0;
          uint32 BuffTimer = 0;

          uint32 BlinkCooldown = 0;
          uint32 FrostNovaCooldown = 0;
          uint32 FrostShockTimer = 0;
         

          void Reset() override {
              GlobalCooldown = 0;
              BuffTimer = 0;
          }

          void TriggerCooldown(uint32 cooldownTimer, ) {
              GlobalCooldown = 1500;
              cooldownTimer = 
          }

          void UpdateCooldownTimers() {

          }

          void EnterCombat(Unit* who) override {
              //events.ScheduleEvent(EVENT_FIRE_BLAST, 5000);
              //events.ScheduleEvent(EVENT_ESCAPE_PLAYER, 10000);
          }

          void AttackStart(Unit* who) {
              if (!who)
                  return;

              if (me->Attack(who, true)) {
                  DoStartMovement(who, 20.0f); // Catch up to 20yds to be able to attack
                  SetCombatMovement(true);
              }
          }

          void JustDied(Unit* killer) override {
              Talk(SAY_AGGRO);
              Talk(SAY_DEATH);
          }

          void UpdateAI(uint32 diff) override {
              // Decrease Global Cooldown
              if (GlobalCooldown > diff)
                  GlobalCooldown -= diff;
              else GlobalCooldown = 0;

              // Handle Self Buffs
              if (!me->IsInCombat() && me->IsAlive()) {
                  if (BuffTimer <= diff) {
                      if (!GlobalCooldown) {
                          DoCast(SPELL_ARCANE_BRILLIANCE);
                          DoCast(SPELL_MAGE_ARMOUR);

                          GlobalCooldown = 1500;
                          BuffTimer = 600000;

                      }
                      else { // Try again in 30 secs
                          BuffTimer = 30000;
                      }
                  }
                  else {
                      BuffTimer -= diff;
                  }
              }

              //Return since we have no target
              if (!UpdateVictim())
                  return;

              //If we are within melee range to the target
              if (me->IsWithinMeleeRange(me->GetVictim())) {
                  DoCast(SPELL_BLINK);
              }
              // If we are out of melee range
              else {

              }


          }

      };

      CreatureAI* GetAI(Creature* creature) const override {
          return new npc_wandering_mageAI(creature);
      }
};

void AddSC_custom_player_npcs()
{
    new npc_wandering_mage();
}